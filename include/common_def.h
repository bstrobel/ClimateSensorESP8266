/*----------------------------------------------------------------------------------------------------
  Project Name : Climate Sensor V1.00
  Authors: Bernd Strobel
*/
#ifndef BERND_COMMON_DEF_H
#define BERND_COMMON_DEF_H

#define SETTINGS_FILE "settings.txt"

// number of seconds to sleep if reading the config fails and we can't read the corresponding setting
#define SLEEP_SECONDS_FAIL 1200

#define CFG_KEY_SSID "ssid"
#define CFG_KEY_PASS "pass"
#define CFG_KEY_MQTT_SERVER "mqtt_server"
#define CFG_KEY_SENSOR_HOSTNAME "sensor_hostname"
#define CFG_KEY_MQTT_TOPIC_STUB "mqtt_topic_stub"
#define CFG_KEY_MQTT_STAT_TOPIC_STUB "mqtt_stat_topic_stub"
#define CFG_KEY_LOW_VOLTAGE "low_voltage"
#define CFG_KEY_VOLTAGE_CALIBRATION_FACTOR "voltage_calibration_factor"
#define CFG_KEY_ELEVATION "elevation"
#define CFG_KEY_SLEEP_TIME_SECONDS "sleep_time_seconds"

#define WIFI_NUM_RETRIES (20) // Number of retries when connecting to WiFI
#define WIFI_RETRY_DELAY_MSECS (500) // msecs to wait between retries while connecting to WiFI

#endif