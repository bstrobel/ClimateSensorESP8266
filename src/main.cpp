/*----------------------------------------------------------------------------------------------------
	Project Name : Climate Sensor V1.00
	Based on: Solar Powered WiFi Weather Station V2.34 from Keith Hungerford, Debasish Dutta and Marc Stähli
						https://github.com/3KUdelta/Solar_WiFi_Weather_Station
	Features: temperature, dewpoint, humidity, absolute pressure, relative pressure, battery status
	Authors: Bernd Strobel
 
	Main microcontroller (ESP8266) and BME280 both sleep between measurements
	BME280 is used in single shot mode ("forced mode")
	Original CODE: https://github.com/3KUdelta/Solar_WiFi_Weather_Station
	INSTRUCTIONS & HARDWARE: https://www.instructables.com/id/Solar-Powered-WiFi-Weather-Station-V20/
	
	CREDITS:
	Inspiration and code fragments of Dewpoint and Heatindex calculations are taken from:  
	https://arduinotronics.blogspot.com/2013/12/temp-humidity-w-dew-point-calcualtions.html
	
	Needed libraries:
	<Adafruit_Sensor.h>    --> Adafruit unified sensor
	<Adafruit_BME280.h>    --> Adafrout BME280 sensor
	<ESPWiFi.h>
	
	CREDITS for Adafruit libraries:
	This is a library for the BME280 humidity, temperature & pressure sensor
	Designed specifically to work with the Adafruit BME280 Breakout
	----> http://www.adafruit.com/products/2650
	These sensors use I2C or SPI to communicate, 2 or 4 pins are required
	to interface. The device's I2C address is either 0x76 or 0x77.
	Adafruit invests time and resources providing this open source code,
	please support Adafruit andopen-source hardware by purchasing products
	from Adafruit!
	Written by Limor Fried & Kevin Townsend for Adafruit Industries.
	BSD license, all text above must be included in any redistribution
	
----------------------------------------------------------------------------------------------------*/

#include <Arduino.h>
#include "common_def.h"
#include "Config_ESP8266.h"
#include "MeasureClimate.h"
#include "MCWifiClient.h"
#include "MCMQTTClient.h"
#include "MCDisplay.h"
#include "MCLogSerial.h"
#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <unistd.h>
#include <rtc_memory.h>

// https://github.com/fabianoriccardi/rtcmemory
typedef struct {
	uint8_t warmBoot;
} RTCState;

RTCState* rtcState;

RtcMemory rtcMemory;
bool isWarmBoot=false;

long sleepSeconds{SLEEP_SECONDS_FAIL};

MCLogSerial l;

void setup()
{
	MCDisplay d; // this cannot be in the global space, doesn"t work
	isWarmBoot = rtcMemory.begin();
	rtcState = rtcMemory.getData<RTCState>();
	if (isWarmBoot)
		l.logWarmBoot();
	else
		l.logColdBoot();
	try
	{
		Config_ESP8266 cfg(SETTINGS_FILE);
		l.logConfig(cfg);

		sleepSeconds = cfg.getSettingAsLongInt(CFG_KEY_SLEEP_TIME_SECONDS);

		#ifdef CHECK_BATTERY_VOLTAGE
		float voltage = analogRead(A0) * cfg.getSettingAsFloat(CFG_KEY_VOLTAGE_CALIBRATION_FACTOR) / 1024;
		if (voltage < cfg.getSettingAsFloat(CFG_KEY_LOW_VOLTAGE))
		{
			l.logVoltageTooLow(cfg.getSetting(CFG_KEY_LOW_VOLTAGE));
			sleepSeconds = 0;
			return;
		}
		#endif //CHECK_BATTERY_VOLTAGE

		MeasureClimate mc(cfg.getSettingAsFloat(CFG_KEY_ELEVATION));
		Serial.print(mc.toString().c_str());
		l.logWeatherData(mc);
		d.displayWeatherData(mc);

		MCWifiClient wificlient(cfg.getSetting(CFG_KEY_SENSOR_HOSTNAME));
		wificlient.connect(cfg.getSetting(CFG_KEY_SSID),cfg.getSetting(CFG_KEY_PASS));
		l.logWifiConnected();

		MCMQTTClient mqtt(
			cfg.getSetting(CFG_KEY_SENSOR_HOSTNAME),
			cfg.getSetting(CFG_KEY_SENSOR_HOSTNAME),
			cfg.getSetting(CFG_KEY_MQTT_TOPIC_STUB),
			cfg.getSetting(CFG_KEY_MQTT_STAT_TOPIC_STUB)
			);
		mqtt.connect(cfg.getSetting(CFG_KEY_MQTT_SERVER));
		mqtt.sendMeasurements(mc);
		#ifdef CHECK_BATTERY_VOLTAGE
		mqtt.sendVoltage(voltage);
		#endif //CHECK_BATTERY_VOLTAGE
		l.logMQTTsent();
	}
	catch (ConfigException &ex)
	{
		const static char* PROGMEM who = "Configuration error";
		l.logError(who,ex.what());
		d.displayError(who,ex.what());
		sleepSeconds = 0; // nonrecoverable problem -> sleep forever
	}
	catch (MeasurementException &ex)
	{
		const static char* PROGMEM who = "Measurement error";
		l.logError(who,ex.what());
		d.displayError(who,ex.what());
		sleepSeconds = 0; // nonrecoverable problem -> sleep forever
	}
	catch (MCMQTTClientException &ex)
	{
		const static char* PROGMEM who = "MQTT error";
		l.logError(who,ex.what());
		d.displayError(who,ex.what());
	}
	catch (MCWifiClientException &ex)
	{
		const static char* who PROGMEM = "Wifi error";
		l.logError(who,ex.what());
		d.displayError(who,ex.what());
	}
	catch (std::exception &ex)
	{
		const static char* who PROGMEM = "General error";
		l.logError(who,ex.what());
		d.displayError(who,ex.what());
		sleepSeconds = 0; // nonrecoverable problem -> sleep forever
	}
	catch (...)
	{
		const char* who = "General error";
		l.logError(who,"");
		sleepSeconds = 0; // nonrecoverable problem -> sleep forever
	}
} // end of void setup()

//loop is used to shut down the controller and sleep
void loop()
{
	rtcState->warmBoot = true;
	rtcMemory.save();
	l.logSleep(sleepSeconds);
	ESP.deepSleep(sleepSeconds * 1000000); // convert to microseconds
}
