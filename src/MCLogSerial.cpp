#include <Arduino.h>
#include <string>
#include "common_def.h"
#include "MCLogSerial.h"

MCLogSerial::MCLogSerial()
{
    Serial.begin(MCLOGSERIAL_BAUD);
    Serial.println();
}

MCLogSerial::~MCLogSerial(){}

void MCLogSerial::logColdBoot()
{
    Serial.println(F("Start of Climate Sensor V1.00"));
    Serial.print(F("### Settings parsed from "));
    Serial.print(SETTINGS_FILE);
    Serial.println(F(" ###"));
}

void MCLogSerial::logWarmBoot()
{
    Serial.println(F("Resuming from sleep"));
}

void MCLogSerial::logError(const std::string &who, const std::string &what)
{
    Serial.println(who.c_str());
    Serial.println(what.c_str());
}

void MCLogSerial::logMQTTsent()
{
    Serial.println(F("Done sending measurements to MQTT"));
}

void MCLogSerial::logVoltage(float &voltage)
{
    Serial.print(F("Battery voltage: "));
    Serial.print(voltage, 2); // print with 2 decimal places
    Serial.println("V");
}

void MCLogSerial::logVoltageTooLow(const std::string voltage_threshold)
{
    Serial.print(F("Battery voltage below threshold of "));
    Serial.print(voltage_threshold.c_str());
    Serial.println(F("V. Sleeping forever"));

}

void MCLogSerial::logConfig(Config &cfg)
{
    Serial.println(F("### Configuration ###"));
    Serial.print(cfg.toString().c_str());
    Serial.println(F("#####################"));
}

void MCLogSerial::logWifiConnected()
{
    Serial.println(F("Wifi connected"));
}

void MCLogSerial::logSleep(long sleepSeconds)
{
	Serial.println(F("Preparing to sleep..."));
	Serial.print(F("Sleeping for "));
	Serial.print(sleepSeconds);
	Serial.println(F(" seconds."));
}

void MCLogSerial::logWeatherData(MeasureClimate &mc)
{
    Serial.println(mc.toString().c_str());
}