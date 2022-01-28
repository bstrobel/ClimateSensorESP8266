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
#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <unistd.h>


long sleepSeconds{SLEEP_SECONDS_FAIL};

void setup()
{

	Serial.begin(115200);
	Serial.println();
	Serial.println(F("Start of Climate Sensor V1.00"));
	MCDisplay d;
	try
	{
		Serial.print(F("### Settings parsed from "));
		Serial.print(SETTINGS_FILE);
		Serial.println(F(" ###"));
		Config_ESP8266 cfg(SETTINGS_FILE);

		Serial.print(cfg.toString().c_str());
		Serial.println(F("### Done parsing settings ###"));
		sleepSeconds = cfg.getSettingAsLongInt(CFG_KEY_SLEEP_TIME_SECONDS);

		#ifdef CHECK_BATTERY_VOLTAGE
		Serial.print(F("Checking battery voltage..."));
		float voltage = analogRead(A0) * cfg.getSettingAsFloat(CFG_KEY_VOLTAGE_CALIBRATION_FACTOR) / 1024;
		Serial.print(voltage, 2); // print with 2 decimal places
		Serial.println("V");
		if (voltage < cfg.getSettingAsFloat(CFG_KEY_LOW_VOLTAGE))
		{
			sleepSeconds = 0;
			Serial.print(F("Battery voltage below threshold of "));
			Serial.print(cfg.getSetting(CFG_KEY_LOW_VOLTAGE).c_str());
			Serial.println(F("V. Sleeping forever"));
			return;
		}
		Serial.println(F("Voltage good."));
		#endif //CHECK_BATTERY_VOLTAGE

		Serial.print(F("Starting measurements..."));
		MeasureClimate mc(cfg.getSettingAsFloat(CFG_KEY_ELEVATION));

		Serial.println(F("done."));
		Serial.print(mc.toString().c_str());
		d.displayWeatherData(mc);

		Serial.print(F("Opening the WiFi connection..."));
		MCWifiClient wificlient(cfg.getSetting(CFG_KEY_SENSOR_HOSTNAME));
		wificlient.connect(cfg.getSetting(CFG_KEY_SSID),cfg.getSetting(CFG_KEY_PASS));
		Serial.println(F("connected!"));

		MCMQTTClient mqtt(
			cfg.getSetting(CFG_KEY_SENSOR_HOSTNAME),
			cfg.getSetting(CFG_KEY_SENSOR_HOSTNAME),
			cfg.getSetting(CFG_KEY_MQTT_TOPIC_STUB),
			cfg.getSetting(CFG_KEY_MQTT_STAT_TOPIC_STUB)
			);
		Serial.print(F("Sending values to MQTT..."));
		mqtt.connect(cfg.getSetting(CFG_KEY_MQTT_SERVER));
		mqtt.sendMeasurements(mc);
		#ifdef CHECK_BATTERY_VOLTAGE
		mqtt.sendVoltage(voltage);
		#endif //CHECK_BATTERY_VOLTAGE
		Serial.println(F("finished."));
	}
	catch (ConfigException &ex)
	{
		const char* who = "Configuration error";
		Serial.println(who);
		Serial.println(ex.what());
		d.displayError(who,ex.what());
		sleepSeconds = 0; // nonrecoverable problem -> sleep forever
	}
	catch (MeasurementException &ex)
	{
		const char* who = "Measurement error";
		Serial.println(who);
		Serial.println(ex.what());
		d.displayError(who,ex.what());
		sleepSeconds = 0; // nonrecoverable problem -> sleep forever
	}
	catch (MCMQTTClientException &ex)
	{
		const char* who = "MQTT error";
		Serial.println(who);
		Serial.println(ex.what());
		d.displayError(who,ex.what());
	}
	catch (MCWifiClientException &ex)
	{
		const char* who = "Wifi error";
		Serial.println(who);
		Serial.println(ex.what());
		d.displayError(who,ex.what());
	}
	catch (std::exception &ex)
	{
		const char* who = "General error";
		Serial.println(who);
		Serial.println(ex.what());
		d.displayError(who,ex.what());
		sleepSeconds = 0; // nonrecoverable problem -> sleep forever
	}
	catch (...)
	{
		const char* who = "General error";
		Serial.println(who);
		d.displayError(who,"Error unknown");
		sleepSeconds = 0; // nonrecoverable problem -> sleep forever
	}
} // end of void setup()

//loop is used to shut down the controller and sleep
void loop()
{
	Serial.println("Preparing to sleep...");
	Serial.print("Sleeping for ");
	Serial.print(sleepSeconds);
	Serial.println(" seconds.");
	ESP.deepSleep(sleepSeconds * 1000000); // convert to microseconds
}
