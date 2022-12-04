#include "Data.h"


Data::Data() {
    this->temperature = 0.0;
    this->humidity = 0.0;
    this->pressure = 0.0;
    this->wind_speed = -1.0;
    this->wind_direction = 0;
    this->latitude = 0.0;
    this->longitude = 0.0;
    this->log_file = "/";
    this->set = false;
}

Data::~Data()= default;


void Data::set_log(bool use_timestamp) {
    if(use_timestamp) {
        this->log_file = "/" +
                         String(this->timestamp.year(), DEC) + "_" +
                         String(this->timestamp.month(), DEC) + "_" +
                         String(this->timestamp.day(), DEC) + "__" +
                         String(this->timestamp.hour(), DEC) + "_" +
                         String(this->timestamp.minute(), DEC) + "_" +
                         String(this->timestamp.second(), DEC) + ".csv";
    }else{
        this->log_file = "/data.csv";
    }
    this->set = true;
}