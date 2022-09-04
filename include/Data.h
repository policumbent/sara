# pragma once

#include <RTClib.h>

class Data {
public:
    float temperature, humidity, pressure;
    double windSpeed, windDirection;
    DateTime timestamp;
    String log_file;
    long lastMsg = 0;
    char msg[50] = {};



public:
    Data();
};