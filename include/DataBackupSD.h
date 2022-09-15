#pragma once

#include <Arduino.h>
#include <iostream>
#include <SD.h>
#include "Sensors.h"
#include "utils.h"
#include "debugging.h"
# include "Data.h"

const unsigned int cs_sd = 2;

class SDHandler{
private:
    File data_log;
    bool log_set;
public:
    explicit SDHandler(Data &, int cs);
    void write_sd(Data&, Sensors<RTC_DS1307> &);
    void setLog(Data&, Sensors<RTC_DS1307> &);

private:
    void setup_sd(Data &, int cs);
};