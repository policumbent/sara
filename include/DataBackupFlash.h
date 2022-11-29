#pragma once

#include <Arduino.h>
# include <SPIFFS.h>
#include "Sensors.h"
#include "utils.h"
#include "debugging.h"
# include "Data.h"

class FlashHandler{
private:
    File data_log;
    bool log_set;
public:
    explicit FlashHandler(Data &data);
    void write_flash(Data&, Sensors<RTC_DS1307>&);
    void set_log(Data&, Sensors<RTC_DS1307>&);
};