#pragma once

#include <Arduino.h>
#include <SD.h>
#include "utils.h"
# include "Data.h"

class SDHandler{
private:
    File data_log;
    char buffer[256];
public:
    explicit SDHandler(Data &, int cs);
    void write_sd(Data &, const char * mode = FILE_APPEND);
    void read_sd(Data &, String &);
    void flush();
private:
    void setup_sd(Data &, int cs);
};