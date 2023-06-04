# pragma once

#include <RTClib.h>

class Data {
public:
    float temperature,      //  °C
          humidity,         //  relative
          pressure;        //  hPa
    double wind_speed,      //  m/s
           wind_direction;  //  degrees
    double latitude,        //  degrees
           longitude,       //  degrees
           altitude,        //  m
           pointingDegrees; // respect to Nord
    int gps_char;   // a character which must be passed to the gps library from serial
    bool set;

    unsigned long last_tick;

    DateTime timestamp;
    DateTime gps_timestamp;
    DateTime ntp_timestamp;
    String log_file;
public:
    Data();
    ~Data();
    void set_log(bool use_timestamp);
    void update_timestamp();
    String to_text();
};