#pragma once


# ifndef DATABACKUP_H
# define DATABACKUP_H

#include <Arduino.h>
#include <iostream>
#include "SD.h"
#include "debugging.h"
#include "utils.h"
#include "ServerMQTT.h"
#include "Sensors.h"

class SDHandler{
private:
    File data_log;
public:
    SDHandler(Data &data);
    void write_sd(Data data, Sensors<RTC_DS1307> rtc);

    static void setLog(Data &data, Sensors<RTC_DS1307> rtc);

private:
    void setup_sd(Data &data);
};

# endif