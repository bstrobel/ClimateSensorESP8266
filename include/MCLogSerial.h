/*
 * ----------------------------------------------------------------------------------------------------
 * Project Name : Climate Sensor V1.00
 * MCLogSerial.h - Logs to the serial output
 * ----------------------------------------------------------------------------------------------------
 */

#ifndef MCLOGSERIAL_H
#define MCLOGSERIAL_H
#include <exception>
#include <stdexcept>
#include <string>
#include "MeasureClimate.h"
#include "Config.h"

#define MCLOGSERIAL_BAUD 115200

class MCLogSerial final
{
public:
    MCLogSerial();
    ~MCLogSerial();
    void logColdBoot();
    void logWarmBoot();
    void logWifiConnected();
    void logMQTTsent();
    void logVoltage(float &voltage);
    void logVoltageTooLow(const std::string voltage_threshold);
    void logError(const std::string &who, const std::string &what);
    void logWeatherData(MeasureClimate &mc);
    void logConfig(Config &cfg);
    void logSleep(long sleepSeconds);
};
#endif // MCLOGSERIAL_H