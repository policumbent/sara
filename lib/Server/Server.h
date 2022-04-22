#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <RTClib.h>
#include <SPI.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <cstdio>
#include <iostream>

void init_wifi();
void init_client();
void printMqttInfo();
void printWifiInfo();

void clientPublish(const char *topic, const char *payload);

void clientConnect();
void callback(char *topic, byte *message, unsigned int length);
void reconnect();
void publishMQTT(float temperature, float pressure, float humidity,
                 float windSpeed, int windDirection, DateTime timestamp);
