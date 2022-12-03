# include "credentials.h"

Info::Info(){

    this->ssid = WIFI_HOTSPOT_SSID;
    this->password = WIFI_HOTSPOT_PASSWORD;
    this->mqtt_server = MQTT_SERVER;
    this->mqtt_usr = MQTT_USER;
    this->mqtt_pass = MQTT_PASSWORD;
    this->mqtt_port = MQTT_PORT;

}