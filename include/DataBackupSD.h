#pragma once

#include <Arduino.h>
#include <iostream>
#include <SD.h>
#include "Sensors.h"
#include "utils.h"
#include "debugging.h"
# include "data.h"

class SDHandler{
private:
    File data_log;
public:
    explicit SDHandler(Data &);
    void write_sd(Data&, Sensors<RTC_DS1307>);
    static void setLog(Data&, Sensors<RTC_DS1307>);

private:
    void setup_sd(Data &);
};