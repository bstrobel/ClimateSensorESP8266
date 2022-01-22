/*
 * ----------------------------------------------------------------------------------------------------
 * Project Name : Climate Sensor V1.00
 * MCWifiClient.h - Initializes Wifi and connects to AP
 * ----------------------------------------------------------------------------------------------------
 */

#ifndef MCWIFICLIENT_H
#define MCWIFICLIENT_H
#include <exception>
#include <stdexcept>
#include <string>


class MCWifiClientException final: public std::runtime_error
{
public:
    MCWifiClientException(const std::string &msg): runtime_error(msg.c_str()){};
};

class MCWifiClient final
{
public:
    MCWifiClient(
        const std::string &sensor_hostname
    );
    ~MCWifiClient();
    void connect(const std::string &ssid, const std::string &pass);
    void disconnect();
    bool connected();
    const std::string toString();
private:
    std::string sensor_hostname;
    std::string ssid;
    bool flConnected;
};
#endif // MCWIFICLIENT_H
