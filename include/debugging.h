#pragma once


const bool WIFI_DEBUG         = false;
const bool SD_DEBUG           = false;
const bool MAGNETOMETER_DEBUG = true;
const bool BME_DEBUG          = true;
const bool ADC_DEBUG          = true;
const bool RTC_DEBUG          = true;
const bool ANEMOMETER_DEBUG   = true;

template<const bool c>
bool check(){return c;}