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
#ifndef BERND_CONFIG_H
#define BERND_CONFIG_H
#include <map>
#include <stdexcept>
#include <string>

using namespace std;

class ConfigException final: public runtime_error
{
public:
    ConfigException(const std::string &msg): runtime_error(msg.c_str()){};
};

class Config
{
protected:
    void parseStringLineToSettings(const std::string &strLine);
private:
    std::map<std::string,std::string> settings;
public:
    const std::string toString();
    const std::string &getSetting(const std::string &key);
    long getSettingAsLongInt(const std::string &key);
    float getSettingAsFloat(const std::string &key);
	bool hasSettingValue(const std::string &key, const std::string &val);
    Config(const std::string &configFileName);
    virtual ~Config(){};
};

#endif