#ifndef BERND_CONFIG_ESP8266_H
#define BERND_CONFIG_ESP8266_H
#include "Config.h"

class Config_ESP8266 : public Config
{
public:
    Config_ESP8266(const std::string &configFileName);
};
#endif