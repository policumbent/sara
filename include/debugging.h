#pragma once

const bool WIFI_DEBUG         = true;
const bool SD_DEBUG           = true;
const bool SPIFFS_DEBUG       = true;
const bool MAGNETOMETER_DEBUG = true;
const bool BME_DEBUG          = true;
const bool NTP_DEBUG          = true;
const bool ANEMOMETER_DEBUG   = true;
const bool WEBSERVER_DEBUG    = true;
const bool EPAPER_DEBUG       = true;
const bool GPS_DEBUG          = true;
const bool COMPASS_DEBUG      = true;

template<const bool c>
bool check(){return c;}