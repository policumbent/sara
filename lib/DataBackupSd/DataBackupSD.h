#pragma once

#include <Arduino.h>
#include "SD.h"
#include "debugging.h"
#include <iostream>
#include "../Server/Server.h"
#include "../utils/utils.h"
#include "../Sensor/Sensor.h"

class SDHandler{
private:
    File data_log;
public:
    SDHandler(Data &data);
    void write_sd(Data data, Sensor<RTC_DS1307> rtc);

    static void setLog(Data &data, Sensor<RTC_DS1307> rtc);

private:
    void setup_sd(Data &data);
};