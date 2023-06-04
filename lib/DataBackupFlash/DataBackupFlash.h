#pragma once

#include <Arduino.h>
# include <SPIFFS.h>
#include "utils.h"
# include "Data.h"

class FlashHandler{
private:
    File data_log;
    bool staging_mode;
    bool log_set;
    char buffer[256];
public:
    explicit FlashHandler(Data &data, bool staged = false);
    void write_flash(Data&, const char* mode = FILE_APPEND);
    void read_flash(Data&, String &txt);
    void flush();
private:
    void setup_spiffs(Data &);
};