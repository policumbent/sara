#ifndef SARA_DEBUGGING_H


const bool WIFI_DEBUG         = true;
const bool SD_DEBUG           = true;
const bool MAGNETOMETER_DEBUG = true;
const bool BME_DEBUG          = true;
const bool ADC_DEBUG          = true;
const bool RTC_DEBUG          = true;
const bool ANEMOMETER_DEBUG   = true;

template<const bool c>
bool check(){return c;}


#define SARA_DEBUGGING_H

#endif //SARA_DEBUGGING_H
