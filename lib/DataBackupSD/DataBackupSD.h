#pragma once

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include "utils.h"
# include "Data.h"

class SDHandler{
private:
    String filename;
    File data_log;
    int cs;
public:
    explicit SDHandler(Data &, int cs);
    void write_sd(String &txt, const char * mode = FILE_APPEND);
    void read_sd(Data &, String &);
    void flush();
private:
    void setup_sd(Data &, int cs);
    bool prepare();
    void deselect();
};