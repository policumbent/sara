#include "DataBackupFlash.h"
#include "utils.h"


FlashHandler::FlashHandler(Data &data, bool staged) {

    staging_mode = staged;
    this->setup_spiffs(data);
}


void FlashHandler::write_flash(Data &data, const char *mode) {

    // printing on file

    this->data_log = SPIFFS.open(data.log_file, mode);

    // and write just once on the file --> this should
    // reduce errors

    data.update_timestamp(); // updated timestamp just before print
    sprintf(this->buffer, "%02d/%02d/%02d %02d:%02d:%02d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f",
            data.timestamp.year(),data.timestamp.month(), data.timestamp.day(),
            data.timestamp.hour(), data.timestamp.minute(), data.timestamp.second(),

            data.temperature, data.humidity, data.pressure, data.wind_speed, data.wind_direction,
            data.longitude, data.latitude, data.altitude);

    this->data_log.println(this->buffer);

    this->data_log.close();
}

void FlashHandler::read_flash(Data &data, String &txt) {
    if(!staging_mode) {
        this->data_log = SPIFFS.open(staging_filename, FILE_READ);
    }else{
        this->data_log = SPIFFS.open(data.log_file, FILE_READ);
    }
    while(this->data_log.available()){
        txt += this->data_log.readString();
    }
    this->data_log.close();

    // when I read the file I want the contnent to be reset so I delete it
    if(staging_mode){
        this->data_log = SPIFFS.open(data.log_file, FILE_WRITE, true);
        this->data_log.close();
    }
}

void FlashHandler::flush() {

    File root = SPIFFS.open("/");
    File file = root.openNextFile("r");
    String txt;

    while(file){

        Serial.print("PRINTING FILE CONTENT: ");
        Serial.println(file.name());
        while(file.available()){
            txt = file.readStringUntil('\n');
            Serial.println(txt);
        }

        Serial.print("DELETING FILE: ");
        Serial.print(file.name());
        Serial.println(" ...");

        SPIFFS.remove(file.name());

        file = root.openNextFile("r");
    }

}

void FlashHandler::setup_spiffs(Data &data) {

    if(!SPIFFS.begin(true)){
        Serial.println("Impossible to access flash");
        loop_infinite();
    }

    Serial.println("CORRECTLY INITIALIZED: SPIFFS");

    if(!data.set && !staging_mode){
        Serial.println("The path of the Log file was not set in Data.");
        loop_infinite();
    }

    if(!staging_mode) {
        this->data_log = SPIFFS.open(data.log_file.c_str(), FILE_WRITE, true);
    }else{
        this->data_log = SPIFFS.open(staging_filename, FILE_WRITE, true);
    }
    if (!this->data_log) {
        Serial.print("Error opening the file: ");
        Serial.print(this->data_log);
        this->data_log.close();
        loop_infinite();
    }

    if(!staging_mode) {
        this->data_log.print("timeStamp, Temperature(°C), Pressure(hPa), Humidity(%), Wind_Speed(m/s), "
                             "Wind_Direction(deg), Longitude(deg), Latitude(deg), Altitude(m)\n");
    }
    this->data_log.close();

}