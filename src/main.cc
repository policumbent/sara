#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>
#include "Led.h"
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

    pinMode(cs_sd, GPIO_MODE_OUTPUT);
    pinMode(cs_mag, GPIO_MODE_OUTPUT);

    digitalWrite(cs_sd, LOW);
    digitalWrite(cs_mag, HIGH);

    led_off();

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

    delay(10);

    //digitalWrite(cs_mag, LOW);
    //digitalWrite(cs_sd, HIGH);

    // the SD must be initialized before the MAGNETIC ENCODER
    if(check<SD_DEBUG>()){
        getSdHandler();
    }else if(check<SPIFFS_DEBUG>()){
        getFlashHandler();
    }

    digitalWrite(cs_sd, LOW);
    digitalWrite(cs_mag, LOW);
    //digitalWrite(cs_mag, HIGH);
    //digitalWrite(cs_sd, LOW);

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

    now = millis();
}

void loop() {

  clientConnect();

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
      getData().wind_speed = 0.0;
  }

  if (check<RTC_DEBUG>()){
      getRtc().get_data(getData());
  }else {
      getData().wind_direction = 0.0;
  }

  digitalWrite(cs_sd, LOW);
  digitalWrite(cs_mag, HIGH);

  if(check<SD_DEBUG>()){
      getSdHandler().write_sd(getData(), getRtc());
  }else if(check<SPIFFS_DEBUG>()){
      getFlashHandler().write_flash(getData(), getRtc());
  }

  digitalWrite(cs_sd, LOW);
  digitalWrite(cs_mag, LOW);

  if (check<MAGNETOMETER_DEBUG>()){
      getAngleSensor().get_data(getData());
  }else {
      getData().wind_direction = 0.0;
  }

  publishMQTT(getData());

  if(check<EPAPER_DEBUG>()){
      display_data(getData());
  }

#ifdef DEBUG
  wind_speed = getData().windSpeed;
  p.Plot();
#endif

  now = millis();

  if(now - lastMsg < 1000){
    delay(1000 - (now - lastMsg));
  }

}
