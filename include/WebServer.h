# pragma once

#include <SPIFFS.h>
#include "Data.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

void init_webserver(Data *);