#include "DataBackupFlash.h"
#include "utils.h"


FlashHandler::FlashHandler(Data &data) {
    this->setup_spiffs(data);
}


void FlashHandler::write_flash(Data &data) {

    // printing on file
    this->data_log = SPIFFS.open(data.log_file, FILE_APPEND);

    // and write just once on the file --> this should
    // reduce errors

    sprintf(this->buffer, "%02d/%02d/%02d %02d:%02d:%02d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f",
            data.timestamp.year(),data.timestamp.month(), data.timestamp.day(),
            data.timestamp.hour(), data.timestamp.minute(), data.timestamp.second(),

            data.temperature, data.humidity, data.pressure, data.wind_speed, data.wind_direction,
            data.longitude, data.latitude, data.altitude);

    this->data_log.println(this->buffer);

    this->data_log.close();
}

void FlashHandler::setup_spiffs(Data &data) {

    uint8_t card_type;

    if(!SPIFFS.begin(true)){
        Serial.println("Impossible to access flash");
        loop_infinite();
    }

    Serial.println("CORRECTLY INITIALIZED: SPIFFS");

    if(!data.set){
        Serial.println("The path of the Log file was not set in Data.");
        loop_infinite();
    }

    this->data_log = SPIFFS.open(data.log_file.c_str(), FILE_WRITE, true);

    if (!this->data_log) {
        Serial.print("Error opening the file: ");
        Serial.print(this->data_log);
        this->data_log.close();
        loop_infinite();
    }

    this->data_log.print("timeStamp, Temperature(°C), Pressure(hPa), Humidity(%), Wind_Speed(m/s), "
                         "Wind_Direction(deg), Longitude(deg), Latitude(deg), Altitude(m)\n");
    this->data_log.close();

}