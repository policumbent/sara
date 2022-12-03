#pragma once

#include <Arduino.h>
#include <SD.h>
#include "Sensors.h"
#include "utils.h"
#include "debugging.h"
# include "Data.h"

class SDHandler{
private:
    File data_log;
    bool log_set;
public:
    explicit SDHandler(Data &, int cs);
    void write_sd(Data&, Sensors<RTC_DS1307> &);
    void set_log(Data&, Sensors<RTC_DS1307> &);

private:
    void setup_sd(Data &, int cs);
};