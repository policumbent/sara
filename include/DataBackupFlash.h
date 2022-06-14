#pragma once

#include <Arduino.h>
#include <iostream>
# include <SPIFFS.h>
#include "Sensors.h"
#include "utils.h"
#include "debugging.h"
# include "data.h"

class FlashHandler{
private:
    File data_log;
    bool log_set;
public:
    explicit FlashHandler(Data &data);
    void write_flash(Data&, Sensors<RTC_DS1307>&);
    void setLog(Data&, Sensors<RTC_DS1307>&);
};