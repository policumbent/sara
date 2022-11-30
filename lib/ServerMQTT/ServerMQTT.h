#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

void clientPublish(const char *topic, const char *payload, PubSubClient &client);
void clientConnect(char* id, const char *mqtt_user, const char *mqtt_password, PubSubClient &client);

void init_wifi(const char* ssid, const char* password);
void init_client(const char *mqtt_server, int mqtt_port, void (*callback) (const char *, uint8_t *, unsigned int), PubSubClient &client);

