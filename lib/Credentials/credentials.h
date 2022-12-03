# pragma once
#include <SPIFFS.h>

class Info {
public:
    String ssid;
    String password;
    String mqtt_server;
    String mqtt_usr;
    String mqtt_pass;
    long mqtt_port;

public:
    Info();
};