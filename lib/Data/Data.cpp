#include "Data.h"


Data::Data() {
    this->temperature = 0.0;
    this->humidity = 0.0;
    this->pressure = 0.0;
    this->wind_speed = -1.0;
    this->wind_direction = -1.0;
    this->latitude = 0.0;
    this->longitude = 0.0;
    this->log_file = "/";
    this->set = false;
    this->last_tick = millis();
}

Data::~Data()= default;


void Data::set_log(bool use_timestamp) {
    if(use_timestamp) {
        this->log_file =                 (String("/") +
                                         String(this->timestamp.year(), DEC) + String("_") +
                                         String(this->timestamp.month(), DEC) + String("_") +
                                         String(this->timestamp.day(), DEC) + String("__") +
                                         String(this->timestamp.hour(), DEC) + String("_") +
                                         String(this->timestamp.minute(), DEC) + String("_") +
                                         String(this->timestamp.second(), DEC) + String(".csv"));
    }else{
        this->log_file = "/data.csv";
    }
    this->set = true;
}

void Data::update_timestamp(){
    unsigned long now = millis();
    unsigned long delta = now - last_tick;
    last_tick = now;
    TimeSpan seconds_delays = TimeSpan(delta/1000);
    timestamp = timestamp+seconds_delays;
}

String Data::to_text() {

    String buffer = "";

    buffer = timestamp.timestamp() + "\n"
            + "Temperature: " + String(temperature) + " °C\n";
            + "Humidity: " + String(humidity) + " %\n";
            + "Pressure: " + String(pressure) + " hPa\n";
            + "Wind Speed: " + String(wind_speed) + " m/s\n";
            + "Wind Direction: " + String(wind_direction) + " °\n";
            + "Latitude: " + String(latitude) + "\n";
            + "longitude: " + String(longitude) + "\n";
            + "Altitude: " + String(longitude) + "\n";
            + "Pointing degrees" + String(pointingDegrees)+"\n";

    return buffer;
}