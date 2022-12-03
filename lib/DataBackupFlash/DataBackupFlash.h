#pragma once

#include <Arduino.h>
# include <SPIFFS.h>
#include "utils.h"
# include "Data.h"

class FlashHandler{
private:
    File data_log;
    bool log_set;
    char buffer[256];
public:
    explicit FlashHandler(Data &data);
    void write_flash(Data&);
private:
    void setup_spiffs(Data &);
};