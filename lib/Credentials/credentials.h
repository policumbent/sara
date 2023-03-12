# pragma once
#include <SPIFFS.h>

class Info {
public:
    String ssid;
    String password;
    String mqtt_server;
    String mqtt_usr;
    String mqtt_pass;
    uint32_t mqtt_ip;
    long mqtt_port;

public:
    Info();
};