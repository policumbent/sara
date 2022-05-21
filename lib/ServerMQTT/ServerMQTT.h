#pragma once

# ifndef SERVERMQTT_H
# define SERVERMQTT_H


#include <Arduino.h>
#include <PubSubClient.h>
#include <RTClib.h>
#include <SPI.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <cstdio>
#include <iostream>

#include "Sensors.h"

void clientPublish(const char *topic, const char *payload);
void clientConnect();

void publishMQTT(Data* data);
void init_wifi();
void init_client();
void printMqttInfo();
void printWifiInfo();

void callback(char *topic, byte *message, unsigned int length);
void reconnect();

# endif