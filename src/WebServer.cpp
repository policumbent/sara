#include "WebServer.h"
#include <WiFi.h>
#include <WiFiAP.h>

AsyncWebServer server = AsyncWebServer(80);

void init_webserver(Data *data){
    WiFi.softAP(AP_SSID, AP_PASSWORD);

    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "API Not Found");
    });

    // API REQUESTS
    server.on("/api/refresh", HTTP_GET, [data](AsyncWebServerRequest *request) {
        auto response = new AsyncJsonResponse();
        auto json = response->getRoot();

        json["temperature"] = data->temperature;
        json["humidity"] = data->humidity;
        json["pressure"] = data->pressure;
        json["wind_speed"] = data->wind_speed;
        json["wind_direction"] = data->wind_direction;

        response->setLength();
        response->setCode(200);

        request->send(response);
    });

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.begin();
}