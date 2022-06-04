#pragma once

#include <Arduino.h>

#include "FS.h"
#include <Adafruit_ADS1X15.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <SD.h>
#include "AS5048A.h"
#include "utils.h"
#include "debugging.h"
#include "data.h"
#include "ServerMQTT.h"


const double SEALEVELPRESSURE_HPA = 1013.25;

const double VOLTAGE_MIN = 0.41;
const double VOLTAGE_MAX = 2.0;
const double MIN_SPEED = 0.0;
const double MAX_SPEED = 32.4;

// LED pin for status info
const int LED = 2;

template<typename sensor_T>
class Sensors {
private:
    sensor_T *s;
public:
    Sensors();
    Sensors(uint8_t, bool);
    void setup();
    void setup(uint8_t, bool);
    void get_data(Data& data);
};

template<typename T> Sensors<T>::Sensors(){
    s = NULL;
    setup();
}

template<typename T> Sensors<T>::Sensors(uint8_t arg_cs, bool debug){
    s = NULL;
    setup(arg_cs, debug);
}


// setters
template<>
inline
void Sensors<RTC_DS1307>::setup() {

    s = new RTC_DS1307();

    clientConnect();

    if (!s->begin()) {
        Serial.println("Problems setting up RTC module");
        if(check<WIFI_DEBUG>()){
            clientPublish("SENSORE RTC: ", "NOT WORKING");
        }
        loop_infinite();
    }

    Serial.println("CORRECTLY INITIALIZED: RTC");

    if (!s->isrunning()) {
        s->adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}



template<>
inline
void Sensors<AS5048A>::setup(uint8_t arg_cs, bool debug) {
    s = new AS5048A(arg_cs, debug);
    s->begin();
    Serial.println("CORRECTLY INITIALIZED: MAGNETIC ENCODER");
}

template<>
inline
void Sensors<Adafruit_ADS1115>::setup() {

    s = new Adafruit_ADS1115();

    clientConnect();

    if (!s->begin(0b1001000)) {
        Serial.println("Failed to initialize ADC.");

        if(check<WIFI_DEBUG>()) {
            clientPublish("SENSORE ADC: ", "NOT WORKING");
        }

        loop_infinite();
    }

    Serial.println("CORRECTLY INITIALIZED: ADC");
}

template<>
inline
void Sensors<Adafruit_BME280>::setup() {

    int status;

    s = new Adafruit_BME280();

    clientConnect();

    status = s->begin(0x76); // inizializzazione sensore temperatura
    for (int i = 0x00; !status && i < 0xFF; i++) {
        status = s->begin(0x76);
    }
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, "
                       "address, sensor ID!");
        Serial.print("SensorID was: 0x");
        Serial.println(s->sensorID(), 16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 "
                     "or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");

        clientPublish("SENSORE BME: ", "NOT WORKING");

        loop_infinite();
    }

    Serial.println("CORRECTLY INITIALIZED: BME280");
}

// getters

template<>
inline
void Sensors<Adafruit_BME280>::get_data(Data& data) {
    data.temperature = s->readTemperature();
    data.humidity = s->readHumidity();
    data.pressure = s->readPressure() / 100.0F;
}

template<>
inline
void Sensors<Adafruit_ADS1115>::get_data(Data& data) {
    int16_t val = s->readADC_SingleEnded(0);
    float voltage = 0.0;

    // voltage = (val * analog_to_volt_conv);
    voltage = s->computeVolts(val);

    Serial.println(val);
    Serial.println(voltage);

    if(check<WIFI_DEBUG>()) {
        char buffer[8];
        dtostrf(voltage, 1, 2, buffer);
        clientPublish("weather_stations/ws1/humidity", buffer);
    }

    if (voltage <= VOLTAGE_MIN) {
        data.windSpeed = 0.0;
    } else if (voltage >= VOLTAGE_MAX) {
        data.windSpeed = MAX_SPEED;
    } else {
        data.windSpeed = ((voltage - VOLTAGE_MIN) * (MAX_SPEED - MIN_SPEED) /
                      (VOLTAGE_MAX - VOLTAGE_MIN)) *
                     3.6;
    }
}

template<>
inline
void Sensors<AS5048A>::get_data(Data& data) {
    data.windDirection = (int) s->getRotationInDegrees();
}

template<>
inline
void Sensors<RTC_DS1307>::get_data(Data& data){
    data.timestamp = s->now();
}









