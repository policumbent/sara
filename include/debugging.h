#pragma once

const bool WIFI_DEBUG         = true;
const bool SD_DEBUG           = false;
const bool SPIFFS_DEBUG       = true;
const bool MAGNETOMETER_DEBUG = true;
const bool BME_DEBUG          = true;
const bool RTC_DEBUG          = true;
const bool ANEMOMETER_DEBUG   = true;
const bool EPAPER_DEBUG       = false;
const bool WEBSERVER_DEBUG    = true;

template<const bool c>
bool check(){return c;}