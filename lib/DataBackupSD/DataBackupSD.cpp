#include "DataBackupSD.h"

SDHandler::SDHandler(Data &data, int cs) {
    setup_sd(data, cs);
}

void SDHandler::write_sd(Data &data) {

    // printing on file
    this->data_log = SD.open(data.log_file, FILE_APPEND);

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

void SDHandler::setup_sd(Data &data, int cs) {

    uint8_t card_type;

    while(!SD.begin(cs, SPI, 3000000)) {
        Serial.println("Impossible to connect SD reader");
        delay(200);
    }

    card_type = SD.cardType();

    if (card_type == CARD_NONE) {
        Serial.println("No SD card attached");
        loop_infinite();
    }

    Serial.println("CORRECTLY INITIALIZED: SD CARD");

    if(!data.set){
        Serial.println("The path of the Log file was not set in Data.");
        loop_infinite();
    }

    // the insertion of the header is done here in order not to access the RTC at setup time
    this->data_log = SD.open(data.log_file, FILE_WRITE, true);

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
