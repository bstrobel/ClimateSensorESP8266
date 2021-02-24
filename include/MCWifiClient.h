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

using namespace std;

class MCWifiClientException final: public std::runtime_error
{
public:
    MCWifiClientException(const std::string &msg): runtime_error(msg.c_str()){};
};

class MCWifiClient final
{
public:
    MCWifiClient(
        const string &sensor_hostname
    );
    ~MCWifiClient();
    void connect(const string &ssid, const string &pass);
    void disconnect();
    bool connected();
    const string toString();
private:
    string sensor_hostname;
    string ssid;
    bool flConnected;
};
#endif // MCWIFICLIENT_H
