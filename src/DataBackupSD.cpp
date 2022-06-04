#include "DataBackupSD.h"
#include "../lib/utils/utils.h"

SPIClass spi = SPIClass(VSPI);


SDHandler::SDHandler(Data &data) {
    if (check<SD_DEBUG>()){
        setup_sd(data);
    }
}

void SDHandler::setLog(Data &data, Sensors<RTC_DS1307> rtc) {

    rtc.get_data(data);
    data.log_file =  "/" +
                     String(data.timestamp.year(), DEC) + "_" +
                     String(data.timestamp.month(), DEC) + "_" +
                     String(data.timestamp.day(), DEC) + "__";
    data.log_file += String(data.timestamp.hour(), DEC) + "_" +
                     String(data.timestamp.minute(), DEC) + "_" +
                     String(data.timestamp.second(), DEC) + ".csv";

    log_set = true;
}


void SDHandler::write_sd(Data &data, Sensors<RTC_DS1307> rtc) {

    if(not log_set){
        setLog(data, rtc);
    }

    // printing on file
    data_log = SD.open(data.log_file, FILE_APPEND);
    String data_to_print; // ---> this string should contain all the information
    // and write just once on the file --> this should
    // reduce errors

    data_to_print += String(data.timestamp.year(), DEC) + "/" +
                     String(data.timestamp.month(), DEC) + "/" +
                     String(data.timestamp.day(), DEC) + " ";
    data_to_print += String(data.timestamp.hour(), DEC) + ":" +
                     String(data.timestamp.minute(), DEC) + ":" +
                     String(data.timestamp.second(), DEC) + ", ";

    // Convert the value to a char array
    char buffer[8];
    get_str(data.temperature, buffer);
    data_to_print += String(buffer) + ", ";

    // Convert the value to a char array
    get_str(data.humidity, buffer);
    data_to_print += String(buffer) + ", ";

    // Convert the value to a char array
    get_str(data.pressure, buffer);
    data_to_print += String(buffer) + ", ";

    // Convert the value to a char array
    get_str(data.windSpeed, buffer);
    data_to_print += String(buffer) + ", ";

    // Convert the value to a char array
    get_str(data.windDirection, buffer);
    data_to_print += String(buffer);

    data_log.println(data_to_print);

    data_log.close();
}

void SDHandler::setup_sd(Data &data) {

    auto open_mode = FILE_WRITE;

    log_set = false;

    if(!check<RTC_DEBUG>()){
        // in case the rtc is not implemented it prints everything in the same file(we
        // can then use another program to separate the CSV files)
        data.log_file += "data.csv";
    }

    clientConnect();

    if (!SD.begin(2, SPI)) {
        Serial.println("Impossible to connect SD reader");

        if(!check<WIFI_DEBUG>()) {
            clientPublish("SENSORE SD CARD: ", "NOT WORKING");
        }

    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        loop_infinite();
    }

    Serial.println("CORRECTLY INITIALIZED: SD CARD");

    if (!SD.exists(data.log_file)) {
        Serial.println("The file ./data.csv doesn't exist --- CREATING...");
    } else {
        open_mode = FILE_APPEND;
    }
    data_log = SD.open(data.log_file, open_mode);

    if (!data_log) {
        Serial.print("Error opening the file: ");
        Serial.print(data_log);
        data_log.close();
        loop_infinite();
    }

    data_log.print("timeStamp, Temperature, Pressure, Humidity, Wind_Speed, "
                   "Wind_Direction \n");
    data_log.close();
}
