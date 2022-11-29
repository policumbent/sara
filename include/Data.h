# pragma once

#include <RTClib.h>

class Data {
public:
    float temperature, humidity, pressure;
    double wind_speed, wind_direction;
    DateTime timestamp;
    String log_file;
    long lastMsg = 0;
    char msg[50] = {};



public:
    Data();
};