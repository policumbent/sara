#pragma once

#include <Arduino.h>

#include "FS.h"
#include <Adafruit_ADS1X15.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <RTClib.h>
#include <SD.h>
#include "AS5048A.h"

#include "debugging.h"

const double SEALEVELPRESSURE_HPA = 1013.25;

const double VOLTAGE_MIN = 0.41;
const double VOLTAGE_MAX = 2.0;
const double MIN_SPEED = 0.0;
const double MAX_SPEED = 32.4;

// LED pin for status info
const int LED = 2;

class SensorHandler {

private:
  // sensors
  Adafruit_BME280 bme; // I2C
  AS5048A angleSensor = AS5048A(SS, true);
  Adafruit_ADS1115 ads; /* Use this for the 16-bit version */
  RTC_DS1307 rtc;       /* REAL TIME CLOCK */
  File data_log;
  String LOG_FILE = "/";

public:
  SensorHandler();

  float temperature, humidity, pressure;
  float windSpeed, windDirection;
  DateTime timestamp;

  long lastMsg = 0;
  char msg[50] = {};

  // getters
  void getBME280Data();
  double getWindSpeedData();
  int getWindDirectionData();
  DateTime getDateTime();
  void write_sd();

private:
  void setup_rtc();
  void setup_sd();
  void setup_magnetometer();
  void setup_adc();
  void setup_sensorTempUm();
};
