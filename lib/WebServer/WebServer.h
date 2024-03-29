# pragma once

#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Data.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"

class WebServer {

    AsyncWebServer *server;

public:
    WebServer(Data *);
    ~ WebServer();

private:
    void init_webserver(Data *);

};


