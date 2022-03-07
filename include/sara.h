//
// Created by hp on 02/03/2022.
//

#ifndef SARA_SARA_H
#define SARA_SARA_H

#include "libraries.h"
#include "../lib/Protected/protected.h"
#include "debugging.h"


#define SEALEVELPRESSURE_HPA (1013.25)


namespace sara{
    // client communication
    static WiFiClientSecure espClient;
    static PubSubClient client = PubSubClient(espClient);

    static const float vmin = 0.41, vmax = 2.0;
    static const float min_speed = 0.0, max_speed = 32.4;

    // LED pin
    static const int ledPin = 2;
}

class SensorHandler{

private:
    // sensors
    Adafruit_BME280 bme; // I2C
    AS5048A angleSensor = AS5048A(SS, true);
    Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
    RTC_DS1307 rtc; /* REAL TIME CLOCK */
    File data_log;
    String LOG_FILE = "/";

public:

    SensorHandler();

    float temperature{}, humidity{}, pressure{};
    float windSpeed{}, windDirection{};
    DateTime timestamp;

    long lastMsg = 0;
    char msg[50]{};

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
    void setup_wifi();
    void setup_sensorTempUm();
};

#endif //SARA_SARA_H
