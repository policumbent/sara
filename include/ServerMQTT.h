#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <RTClib.h>
#include <SPI.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <cstdio>
#include <iostream>

#include <debugging.h>
#include "data.h"
#include "credentials.h"


void clientPublish(const char *topic, const char *payload);
void clientConnect();

void publishMQTT(Data &data);


void init_wifi();
void init_client();
void printMqttInfo();
void printWifiInfo();

void callback(char *topic, byte *message, unsigned int length);
void reconnect();
