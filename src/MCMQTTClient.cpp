#include <Arduino.h>
#include <string>
#include <sstream>
#include "MCMQTTClient.h"

using namespace std;

MCMQTTClient::MCMQTTClient(
        const string &_sensor_hostname, 
        const string &_mqtt_client_id, 
        const string &_topic_stub, 
        const string &_stat_topic_stub):
        sensor_hostname{_sensor_hostname},
        mqtt_client_id{_mqtt_client_id}
{
    mqttClient.setClient(wifiClientEsp);

    topic_prefix = _topic_stub + _sensor_hostname +"/";
    stat_topic = _stat_topic_stub + _sensor_hostname;
}

MCMQTTClient::~MCMQTTClient()
{
    if (mqttClient.connected())
    {
        sendStatus("CS_FINISH");
        disconnect();
    }
}

void MCMQTTClient::connect(const string &mqtt_server)
{
     mqttClient.setServer(mqtt_server.c_str(), 1883);
     // Loop until we're reconnected
     int i = 0;
     while (!mqttClient.connected())
     {
         if (mqttClient.connect(mqtt_client_id.c_str()))
         {
             mqttClient.publish(stat_topic.c_str(), "CS_START");
             delay(50);
         }
         else
         {
             i++;
             if (i > MQTT_NUM_RETRIES)
             {
                 stringstream msg;
                 msg << "Max retries (" << MQTT_NUM_RETRIES << ") reached! Last rc=" << mqttClient.state();
                 throw MCMQTTClientException(msg.str());
             }
             delay(MQTT_RETRY_DELAY_MSECS);
         }
     }
}

void MCMQTTClient::disconnect()
{
    if (mqttClient.connected())
    {
        mqttClient.disconnect();
    }
}

bool MCMQTTClient::connected() {return mqttClient.connected();}

void MCMQTTClient::sendStatus(const string &s)
{
    mqttClient.publish(stat_topic.c_str(), s.c_str());
}

void MCMQTTClient::publishFloat(const char* topic, float val)
{
    stringstream valss;
    valss << val;
    if (mqttClient.state())
    {
        stringstream s;
        s << "Could not send msg  to topic " << string(topic) << " because MQTT ClientState=" << mqttClient.state();
        throw MCMQTTClientException(s.str().c_str());
    }
    if (!mqttClient.publish(topic,valss.str().c_str(), MQTT_MSG_RETAINED))
    {
        throw MCMQTTClientException("Could not send value " + valss.str() + " to topic " + string(topic));
    }
}

void MCMQTTClient::sendMeasurements(const MeasureClimate &m)
{
    publishFloat((topic_prefix + MQTT_TEMP).c_str(),m.temperature);
    publishFloat((topic_prefix + MQTT_DEWPOINT).c_str(),m.dewpoint_temp);
    publishFloat((topic_prefix + MQTT_HUMIDITY).c_str(),m.humidity);
    publishFloat((topic_prefix + MQTT_PRESSURE_ABS).c_str(),m.pressure_abs);
    publishFloat((topic_prefix + MQTT_PRESSURE_REL).c_str(),m.pressure_rel);
}

void MCMQTTClient::sendVoltage(float v)
{
    publishFloat((topic_prefix + MQTT_BATT_VOLTAGE).c_str(),v);
}