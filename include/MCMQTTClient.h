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
#ifndef MCMQTTCLIENT_H
#define MCMQTTCLIENT_H
#include <string>
#include <stdexcept>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "MeasureClimate.h"

#define MQTT_MSG_RETAINED false

#define MQTT_TEMP "temperature"
#define MQTT_DEWPOINT "dewpoint_temp"
#define MQTT_PRESSURE_ABS "pressure_abs"
#define MQTT_PRESSURE_REL "pressure_rel"
#define MQTT_HUMIDITY "humidity"
#define MQTT_BATT_VOLTAGE "battvoltage"

#define MQTT_NUM_RETRIES (5) // Number of retries when connecting to MQTT broker
#define MQTT_RETRY_DELAY_MSECS (500) // msecs to wait between retries while connecting to MQTT broker


using namespace std;

class MCMQTTClientException final: public std::runtime_error
{
public:
    MCMQTTClientException(const std::string &msg): runtime_error(msg.c_str()){};
};

class MCMQTTClient final
{
public:
    MCMQTTClient(
        const string &sensor_hostname, 
        const string &mqtt_client_id, 
        const string &topic_stub, 
        const string &stat_topic_stub);
    ~MCMQTTClient();
    void sendMeasurements(const MeasureClimate &m);
    void sendVoltage(float v);
    void sendStatus(const string &s);
    void connect(const string &mqtt_server);
    void disconnect();
    bool connected();
private:
    void publishFloat(const char* topic, float val);
    string sensor_hostname;
    string mqtt_client_id;
    string topic_prefix;
    string stat_topic;

    WiFiClient wifiClientEsp;
    PubSubClient mqttClient;
};
#endif