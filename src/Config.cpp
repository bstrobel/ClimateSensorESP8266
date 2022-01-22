#include "Config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

Config::Config(const std::string &config_file_name)
{
}

void Config::parseStringLineToSettings(const std::string &strLine)
{
    std::string param_name;
    std::string param_value;
    char cmt{'#'};
    const char * const delimiters = " \t\n";
    const char * const eolchars = "\r\n";
    // Eat leading whitespace
    size_t i = strLine.find_first_not_of(delimiters);
    if (i == std::string::npos)
        return;
    if (strLine.substr(i)[0] == cmt)
        return;
    size_t j = strLine.find_first_of(delimiters, i);
    if (j == std::string::npos)
        return;
    size_t k = strLine.find_first_not_of(delimiters, j);
    if (k == std::string::npos)
        return;
    size_t l = strLine.find_first_of(eolchars, k);
    if (l == std::string::npos)
        l = strLine.length();
    param_name = strLine.substr(i,j-i);
    param_value = strLine.substr(k,l-k);
    
    Config::settings[param_name] = param_value;

}

const std::string Config::toString()
{
    std::stringstream os;
    //for (auto it = Config::settings.cbegin(); it!=Config::settings.end(); it++)
    for (auto setting : Config::settings)
    {
        os << "\"" << setting.first << "\" => \"" << setting.second << "\"" << std::endl;
    }
    return os.str();
}

const std::string &Config::getSetting(const std::string &key)
{
    return Config::settings[key];
}

long Config::getSettingAsLongInt(const std::string &key)
{
    return strtol(Config::getSetting(key).c_str(),nullptr,10);
}

float Config::getSettingAsFloat(const std::string &key)
{
    return strtof(Config::getSetting(key).c_str(),nullptr);
}

bool Config::hasSettingValue(const std::string &key, const std::string &val)
{
    std::string actual_val = getSetting(key);
    return actual_val == val;
};