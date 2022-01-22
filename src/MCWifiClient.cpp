#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <string>
#include <sstream>
#include "common_def.h"
#include "MCWifiClient.h"


MCWifiClient::MCWifiClient(const std::string &_sensor_hostname):
    sensor_hostname{_sensor_hostname},
    flConnected{false}
{
			WiFi.mode(WIFI_STA);
			WiFi.hostname(sensor_hostname.c_str());
}

MCWifiClient::~MCWifiClient()
{
    disconnect();
}

void MCWifiClient::connect(const std::string &ssid, const std::string &pass)
{
    if (!flConnected)
    {
        WiFi.begin(ssid.c_str(), pass.c_str());
        int i = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(WIFI_RETRY_DELAY_MSECS);
            i++;
            if (i > WIFI_NUM_RETRIES)
            {
                throw MCWifiClientException("Could not connect to WiFi!");
            }
        }
        flConnected = true;
    }
}

void MCWifiClient::disconnect()
{
    if (!flConnected)
    {
        WiFi.disconnect(true); // wifiOff=true
        flConnected = false;
    }
}

bool MCWifiClient::connected() {return flConnected;}

const std::string MCWifiClient::toString()
{
    std::stringstream strm;
    strm << "Hostname: " << sensor_hostname << std::endl;
    if (flConnected)
    {
        strm << "SSID: " << ssid << std::endl;
        strm << "Status: Connected" << std::endl;
    }
    else
    {
        strm << "Status: Not connected" << std::endl;
    }
    return strm.str();
}