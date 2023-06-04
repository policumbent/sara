#pragma once

#include <Arduino.h>

#include "FS.h"
#include <Adafruit_ADS1X15.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <NTPClient.h>
#include <SD.h>
#include "AS5048A.h"
#include "utils.h"
#include "debugging.h"
#include "Data.h"
#include "Connections.h"

const double SEALEVELPRESSURE_HPA = 1013.25;

const double VOLTAGE_MIN = 0.41;
const double VOLTAGE_MAX = 2.0;
const double MIN_SPEED = 0.0;
const double MAX_SPEED = 32.4;

HardwareSerial SerialGPS(2);
WiFiUDP *wiudp;

template<typename sensor_T>
class Sensors {
private:
    sensor_T *s;
public:
    Sensors();
    Sensors(uint8_t, bool);

    ~ Sensors();

    void setup();
    void setup(uint8_t, bool);
    void get_data(Data& data);
};

template<typename T> Sensors<T>::Sensors(){
    this->s = NULL;
    setup();
}

template<typename T> Sensors<T>::Sensors(uint8_t arg_cs, bool debug){
    this->s = NULL;
    setup(arg_cs, debug);
}

template<typename T> Sensors<T>::~ Sensors(){
    delete this->s;
}

// setters
template<>
inline
void Sensors<NTPClient>::setup(){
    wiudp = new WiFiUDP();
    this->s = new NTPClient(*wiudp);

    if(!this->s){
        Serial.println("Problems setting up NTP Client");
        if(check<WIFI_DEBUG>()){
            publish("CLIENT NTP: ", "NOT WORKING");
        }
        loop_infinite();
    }
    this->s->setPoolServerName(POOLING_SERVER);
    this->s->begin();

    Serial.println("CORRECTLY INITIALIZED: CLIENT NTP");
}

template<>
inline
void Sensors<Adafruit_HMC5883_Unified>::setup(){
    this->s = new Adafruit_HMC5883_Unified();

    sensor_t sens;

    if(!this->s->begin()){
        Serial.println("Problems setting up compass");
        if(check<WIFI_DEBUG>()){
            publish("COMPASS MODULE: ", "NOT WORKING");
        }
        loop_infinite();
    }

    this->s->getSensor(&sens);
    //TODO: Implement sensor info

    Serial.println("CORRECTLY INITIALIZED: COMPASS");
}

template<>
inline
void Sensors<TinyGPSPlus>::setup(){
    this->s = new TinyGPSPlus();
    SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
    if(!this->s){
        Serial.println("Problems setting up GPS module");
        if(check<WIFI_DEBUG>()){
            publish("SENSORE GPS: ", "NOT WORKING");
        }
        loop_infinite();
    }
    Serial.println("CORRECTLY INITIALIZED: GPS");
}



template<>
inline
void Sensors<RTC_DS1307>::setup() {

    this->s = new RTC_DS1307();

    if (!this->s->begin()) {
        Serial.println("Problems setting up RTC module");
        if(check<WIFI_DEBUG>()){
            publish("SENSORE RTC: ", "NOT WORKING");
        }
        loop_infinite();
    }

    this->s->adjust(DateTime(F(__DATE__), F(__TIME__)));

    Serial.println("CORRECTLY INITIALIZED: RTC");
}



template<>
inline
void Sensors<AS5048A>::setup(uint8_t arg_cs, bool debug) {
    this->s = new AS5048A(arg_cs, debug);
    this->s->begin();
    Serial.println("CORRECTLY INITIALIZED: MAGNETIC ENCODER");

}

template<>
inline
void Sensors<Adafruit_ADS1115>::setup() {

    this->s = new Adafruit_ADS1115();


    if (!this->s->begin(0b1001000)) {
        Serial.println("Failed to initialize ADC.");

        if(check<WIFI_DEBUG>()) {
            publish("SENSORE ADC: ", "NOT WORKING");
        }

        loop_infinite();
    }

    Serial.println("CORRECTLY INITIALIZED: ADC");
}

template<>
inline
void Sensors<Adafruit_BME280>::setup() {

    int status;

    this->s = new Adafruit_BME280();

    if (!this->s->begin(0x76)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, "
                       "address, sensor ID!");
        Serial.print("SensorID was: 0x");
        Serial.println(s->sensorID(), 16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 "
                     "or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");

        publish("SENSORE BME: ", "NOT WORKING");

        loop_infinite();
    }

    Serial.println("CORRECTLY INITIALIZED: BME280");
}

// getters
template<>
inline
void Sensors<TinyGPSPlus>::get_data(Data &data) {
    // the message must be transmitted completely before it can be read
    char c;
    while(SerialGPS.available()){
        c = (char) SerialGPS.read();
        if(this->s->encode(c)){

            if(this->s->location.isValid()){
            data.longitude = this->s->location.lng();
            data.latitude = this->s->location.lat();
            }
            if(this->s->altitude.isValid()){
                data.altitude = this->s->altitude.meters();
            }
            if(this->s->date.isValid() && this->s->time.isValid()){
                data.gps_timestamp = DateTime(
                        this->s->date.year(),
                        this->s->date.month(),
                        this->s->date.day(),
                        this->s->time.hour(),
                        this->s->time.minute(),
                        this->s->time.second()
                        );
            }
        }
    }
}

template<>
inline
void Sensors<Adafruit_BME280>::get_data(Data& data) {
    data.temperature = this->s->readTemperature();
    data.humidity = this->s->readHumidity();
    data.pressure = this->s->readPressure() / 100.0F;
}

template<>
inline
void Sensors<Adafruit_HMC5883_Unified>::get_data(Data &data){

    sensors_event_t event;
    this->s->getEvent(&event);

    double heading = atan2(event.magnetic.y, event.magnetic.x);
    double declinationAngle = 0;
    if(check<BATTLE_MOUNTAIN>()){
        declinationAngle = 0.219; // This is calibrated to BM VALUES
    }else{
        declinationAngle = 0.067; // This is calibrated to TURIN VALUES
    }

    heading += declinationAngle;

    // Correct for when signs are reversed.
    if(heading < 0)
        heading += 2*PI;

    // Check for wrap due to addition of declination.
    if(heading > 2*PI)
        heading -= 2*PI;

    // Convert radians to degrees for readability.
    double headingDegrees = heading * 180/M_PI;

    data.pointingDegrees = headingDegrees;
}


template<>
inline
void Sensors<Adafruit_ADS1115>::get_data(Data& data) {
    int16_t val = this->s->readADC_SingleEnded(0);
    float voltage = 0.0f;

    // voltage = (val * analog_to_volt_conv);
    voltage = this->s->computeVolts(val);

    if (voltage <= VOLTAGE_MIN) {
        data.wind_speed = 0.0;
    } else if (voltage >= VOLTAGE_MAX) {
        data.wind_speed = MAX_SPEED;
    } else {
        data.wind_speed = ((voltage - VOLTAGE_MIN) * (MAX_SPEED - MIN_SPEED) /
                      (VOLTAGE_MAX - VOLTAGE_MIN));
    }
}

template<>
inline
void Sensors<AS5048A>::get_data(Data& data) {
    data.wind_direction = (double) this->s->getRotationInDegrees();
}

template<>
inline
void Sensors<RTC_DS1307>::get_data(Data& data){
    data.timestamp = this->s->now();
}

template<>
inline
void Sensors<NTPClient>::get_data(Data& data){
    this->s->update();
    data.timestamp = DateTime(this->s->getEpochTime());
}