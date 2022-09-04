#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>
#include "Sensors.h"
#include "DataBackupSD.h"
#include "DataBackupFlash.h"
#include "EpaperDisplay.h"
#include "WebServer.h"

#ifdef DEBUG
#include "Plotter.h"
Plotter p;
float wind_speed = 0.0;
#endif


unsigned long now;
unsigned long lastMsg;

unsigned int cs_sd = 2;
unsigned int cs_mag = 5;

Data &getData() {
    try {
        static Data data = Data();
        return data;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<Adafruit_BME280> &getBme() {
    try {
        static Sensors<Adafruit_BME280> bme = Sensors<Adafruit_BME280>();
        return bme;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<AS5048A> &getAngleSensor() {
    try {
        static Sensors<AS5048A> angleSensor = Sensors<AS5048A>(cs_mag, false);
        return angleSensor;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<Adafruit_ADS1115> &getAds() {
    try {
        static Sensors<Adafruit_ADS1115> ads = Sensors<Adafruit_ADS1115>();
        return ads;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<RTC_DS1307> &getRtc() {
    try {
        static Sensors<RTC_DS1307> rtc = Sensors<RTC_DS1307>();
        return rtc;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

SDHandler &getSdHandler(){
    try {
        static SDHandler sd_handler = SDHandler(getData(), cs_sd);
        return sd_handler;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

FlashHandler &getFlashHandler(){
    try {
        static FlashHandler flash_handler = FlashHandler(getData());
        return flash_handler;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

void setup() {
    Serial.begin(115200);

    Serial.println("BEGIN");

    pinMode(cs_sd, OUTPUT);
    pinMode(cs_mag, OUTPUT);

    digitalWrite(cs_sd, LOW);
    digitalWrite(cs_mag, LOW);

    delay(10);

    if(check<RTC_DEBUG>()){
        getRtc();
    }

    delay(10);

    if(check<BME_DEBUG>()){
        getBme();
    }

    delay(10);

    if(check<ANEMOMETER_DEBUG>()){
        getAds();
    }
    digitalWrite(cs_mag, LOW);

    delay(10);

    // the SD must be initialized before the MAGNETIC ENCODER
    if(check<SD_DEBUG>()){
        getSdHandler();
    }else if(check<SPIFFS_DEBUG>()){
        getFlashHandler();
    }
    digitalWrite(cs_sd, LOW);

    delay(10);

    if(check<MAGNETOMETER_DEBUG>()){
      getAngleSensor();
    }

    if(check<EPAPER_DEBUG>()){
        init_display();
    }

    if(check<WIFI_DEBUG>()){
        init_wifi();
        init_client();
        if(check<WEBSERVER_DEBUG>()){
            init_webserver(&getData());
        }
    }

#ifdef DEBUG
    p.Begin();
    p.AddTimeGraph("Wind Speed", 1500, "speed", wind_speed);
#endif

    pinMode(LED, OUTPUT);
#ifndef DEBUG
    Serial.println("SETUP COMPLETED");
#endif
}

void loop() {

  clientConnect();

  now = millis();
  if (now - lastMsg > 1000) {
      lastMsg = now;

      if (check<BME_DEBUG>()){
          getBme().get_data(getData());
      }else {
          getData().temperature = 0.0;
          getData().pressure = 0.0;
          getData().humidity = 0.0;
      }

      if (check<ANEMOMETER_DEBUG>()){
          getAds().get_data(getData());
      }else {
          getData().windSpeed = 0.0;
      }

      if (check<RTC_DEBUG>()){
          getRtc().get_data(getData());
      }else {
          getData().windDirection = 0.0;
      }

      if (check<MAGNETOMETER_DEBUG>()){
          getAngleSensor().get_data(getData());
      }else {
          getData().windDirection = 0.0;
      }

      delay(100);

      publishMQTT(getData());

      if(check<SD_DEBUG>()){
        getSdHandler().write_sd(getData(), getRtc());
      }else if(check<SPIFFS_DEBUG>()){
          getFlashHandler().write_flash(getData(), getRtc());
      }

      if(check<EPAPER_DEBUG>()){
          display_data(getData());
      }

  }

#ifdef DEBUG
  wind_speed = getData().windSpeed;
  p.Plot();
#endif
  delay(100);
}
