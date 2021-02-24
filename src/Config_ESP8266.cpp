#include "Config_ESP8266.h"
#include "LittleFS.h"
#include <string>

Config_ESP8266::Config_ESP8266(const std::string &configFileName): Config{configFileName}
{
    LittleFS.begin();
    File cf = LittleFS.open(configFileName.c_str(), "r");
    if (!cf)
    {
        throw ConfigException("Could not open config file " + configFileName);
    }
    while (cf.position() < cf.size() - 1)
    {
        // getline(cf, strLine);
        Config::parseStringLineToSettings(cf.readStringUntil('\n').c_str());
    }
    cf.close();
    LittleFS.end();
};
