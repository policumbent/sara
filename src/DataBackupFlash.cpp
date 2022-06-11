#include "DataBackupFlash.h"
#include "utils.h"


FlashHandler::FlashHandler(Data &data) {
    auto open_mode = FILE_WRITE;
    log_set = false;
    data.log_file = "/data.csv";

    if(!SPIFFS.begin(true)){
        Serial.println("Impossible to access flash");
        while(1);
    }

    if (!SPIFFS.exists(data.log_file)) {
        Serial.println("The file ./data.csv doesn't exist --- CREATING...");
    } else {
        open_mode = FILE_APPEND;
    }
    data_log = SPIFFS.open(data.log_file, open_mode);

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

void FlashHandler::setLog(Data &data, Sensors<RTC_DS1307> &rtc) {

    rtc.get_data(data);
    data.log_file =  "/" +
                     String(data.timestamp.year(), DEC) + "_" +
                     String(data.timestamp.month(), DEC) + "_" +
                     String(data.timestamp.day(), DEC) + "__";
    data.log_file += String(data.timestamp.hour(), DEC) + "_" +
                     String(data.timestamp.minute(), DEC) + "_" +
                     String(data.timestamp.second(), DEC) + ".csv";

    // the insertion of the header is done here in order not to access the RTC at setup time
    data_log = SPIFFS.open(data.log_file, FILE_WRITE);

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


void FlashHandler::write_flash(Data &data, Sensors<RTC_DS1307>& rtc) {

    if(not log_set){
        setLog(data, rtc);
    }

    // printing on file
    data_log = SPIFFS.open(data.log_file, FILE_APPEND);
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
