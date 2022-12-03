# pragma once

#include <RTClib.h>

class Data {
public:
    float temperature, humidity, pressure;
    double wind_speed, wind_direction;
    bool set;
    DateTime timestamp;
    String log_file;

public:
    Data();
    void set_log(bool use_timestamp);
};