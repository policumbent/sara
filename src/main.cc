#include <Arduino.h>
#include <PubSubClient.h>
#include <RTClib.h>
#include <SPI.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <cstdio>
#include <iostream>
#include "ServerMQTT.h"
#include "DataBackupSD.h"
#include "Sensors.h"

// todo: try to understand how the library system in platformio works and make it function

unsigned long now;
unsigned long lastMsg;


Data *getData() {
    try {
        static Data data = Data();
        return &data;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<Adafruit_BME280> &getBme() {
    try {
        static Sensors<Adafruit_BME280> bme = Sensors<Adafruit_BME280>();
        return bme;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<AS5048A> &getAngleSensor() {
    try {
        static Sensors<AS5048A> angleSensor = Sensors<AS5048A>();
        return angleSensor;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<Adafruit_ADS1115> &getAds() {
    try {
        static Sensors<Adafruit_ADS1115> ads = Sensors<Adafruit_ADS1115>();
        return ads;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<RTC_DS1307> &getRtc() {
    try {
        static Sensors<RTC_DS1307> rtc = Sensors<RTC_DS1307>();
        return rtc;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

SDHandler &getSdHandler(){
    try {
        static SDHandler sd_handler = SDHandler(*getData());
        return sd_handler;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

void setup() {
  Serial.begin(115200);

  Serial.println("BEGIN");

  printMqttInfo();
  printWifiInfo();

  // client = PubSubClient(espClient);

  init_wifi();
  init_client();

  pinMode(LED, OUTPUT);
}

void loop() {

  clientConnect();

  now = millis();
  if (now - lastMsg > 1000) {
      lastMsg = now;

      if (check<BME_DEBUG>()){
          getBme().get_data(*getData());
      }else {
          getData()->temperature = 0.0;
          getData()->pressure = 0.0;
          getData()->humidity = 0.0;
      }
      if (check<ANEMOMETER_DEBUG>()){
          getAds().get_data(*getData());
      }else {
          getData()->windSpeed = 0.0;
      }

      if (check<MAGNETOMETER_DEBUG>()){
          getAngleSensor().get_data(*getData());
      }else {
          getData()->windDirection = 0.0;
      }

      if (check<RTC_DEBUG>()){
          getRtc().get_data(*getData());
      }else {
          getData()->windDirection = 0.0;
      }

      publishMQTT(getData());
      if(check<SD_DEBUG>()){
        getSdHandler().write_sd(*getData(), getRtc());
      }
  }
}
