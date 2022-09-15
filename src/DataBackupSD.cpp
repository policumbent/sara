#include "DataBackupSD.h"
#include "../lib/utils/utils.h"

SPIClass vspi = SPIClass(VSPI);


SDHandler::SDHandler(Data &data, int cs) {
    if (check<SD_DEBUG>()){
        setup_sd(data, cs);
    }
}

void SDHandler::setLog(Data &data, Sensors<RTC_DS1307> &rtc) {

    rtc.get_data(data);
    data.log_file =  "/" +
                     String(data.timestamp.year(), DEC) + "_" +
                     String(data.timestamp.month(), DEC) + "_" +
                     String(data.timestamp.day(), DEC) + "__";
    data.log_file += String(data.timestamp.hour(), DEC) + "_" +
                     String(data.timestamp.minute(), DEC) + "_" +
                     String(data.timestamp.second(), DEC) + ".csv";


    // the insertion of the header is done here in order not to access the RTC at setup time
    data_log = SD.open(data.log_file, FILE_WRITE);

    if (!data_log) {
        Serial.print("Error opening the file: ");
        Serial.print(data_log);
        data_log.close();
        loop_infinite();
    }

    data_log.print("timeStamp, Temperature, Pressure, Humidity, Wind_Speed, "
                   "Wind_Direction \n");
    data_log.close();

    log_set = true;
}


void SDHandler::write_sd(Data &data, Sensors<RTC_DS1307> &rtc) {

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

void SDHandler::setup_sd(Data &data, int cs) {

    auto open_mode = FILE_WRITE;
    log_set = false;

    data.log_file = "/data.csv";

    if(!check<WIFI_DEBUG>()) {
        clientConnect();
    }

    // setup pins
    //pinMode(cs, OUTPUT);

    while(!SD.begin(cs, SPI, 3000000)) {
        Serial.println("Impossible to connect SD reader");

        if(!check<WIFI_DEBUG>()) {
            clientPublish("SENSORE SD CARD: ", "NOT WORKING");
        }
        delay(200);
        //counter--;
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
