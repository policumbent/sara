//
// Created by hp on 04/03/2022.
//

#ifndef SARA_WEATHER_STATION_LIBRARIES_H
#define SARA_WEATHER_STATION_LIBRARIES_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <cstdio>
#include <Wire.h>
#include "FS.h"
#include <SPI.h>
#include <SD.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADS1X15.h>
#include <PubSubClient.h>
#include <RTClib.h>
#include <iostream>

// these two libraries must be included from the files
#include "../lib/AS5048A/AS5048A.h"

void clientConnect();
void callback(char* topic, byte* message, unsigned int length);
void reconnect();
void publishMQTT(float temperature, float pressure, float humidity, float windSpeed, int windDirection, DateTime timestamp);


#endif //SARA_WEATHER_STATION_LIBRARIES_H
