#include "Config_ESP8266.h"
#include "LittleFS.h"
#include <string>

Config_ESP8266::Config_ESP8266(const std::string &configFileName): Config{configFileName}
{
    // https://links2004.github.io/Arduino/d1/d51/class_stream.html
    // https://links2004.github.io/Arduino/index.html
    // https://www.arduino.cc/reference/en/language/functions/communication/stream/streamreadstringuntil/
    LittleFS.begin();
    File cf = LittleFS.open(configFileName.c_str(), "r");
    if (!cf.isFile())
    {
        throw ConfigException("Could not open config file " + configFileName);
    }
    // ifstream cf{config_file_name};
    // if (!cf)
    // {
    //     errorState = true;
    //     return;
    // }
    while (cf.position() < cf.size() - 1)
    {
        // getline(cf, strLine);
        Config::parseStringLineToSettings(cf.readStringUntil('\n').c_str());
    }
    cf.close();
    LittleFS.end();
};
