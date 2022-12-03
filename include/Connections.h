#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <debugging.h>
#include "Data.h"
#include "credentials.h"
#include "ServerMQTT.h"

void printMqttInfo();
void printWifiInfo();

void setupMQTT();
void publishMQTT(Data &data);
void publish(const char* topic, const char *payload);
void connect();

void callback(const char *topic, byte *message, unsigned int length);