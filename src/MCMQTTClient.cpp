#include <Arduino.h>
#include <string>
#include <sstream>
#include "MCMQTTClient.h"


MCMQTTClient::MCMQTTClient(
        const std::string &_sensor_hostname, 
        const std::string &_mqtt_client_id, 
        const std::string &_topic_stub, 
        const std::string &_stat_topic_stub):
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

void MCMQTTClient::connect(const std::string &mqtt_server)
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
                 std::stringstream msg;
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

void MCMQTTClient::sendStatus(const std::string &s)
{
    mqttClient.publish(stat_topic.c_str(), s.c_str());
}

void MCMQTTClient::publishFloat(const char* topic, float val)
{
    std::stringstream valss;
    valss << val;
    if (mqttClient.state())
    {
        std::stringstream s;
        s << "Could not send msg  to topic " << std::string(topic) << " because MQTT ClientState=" << mqttClient.state();
        throw MCMQTTClientException(s.str().c_str());
    }
    if (!mqttClient.publish(topic,valss.str().c_str(), MQTT_MSG_RETAINED))
    {
        throw MCMQTTClientException("Could not send value " + valss.str() + " to topic " + std::string(topic));
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