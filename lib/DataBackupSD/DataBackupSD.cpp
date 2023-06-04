#include "DataBackupSD.h"

SDHandler::SDHandler(Data &data, int cs) {
    setup_sd(data, cs);
}

void SDHandler::flush() {

    File root = SD.open("/");
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

        SD.remove(file.name());

        file = root.openNextFile("r");
    }

}


void SDHandler::write_sd(String txt,  const char *mode) {

    // The SD card is a device that must be mounted AND UNMOUNTED at every writing stage!
    // every other behaviour may damage the SD card and should be avoided
    prepare();

    // printing on file
    this->data_log = SD.open(filename, mode);

    this->data_log.print(txt);

    this->data_log.close();

    deselect();
}

void SDHandler::read_sd(Data &data, String &txt) {
    this->data_log = SD.open(data.log_file, FILE_READ);

    while(this->data_log.available()){
        txt += this->data_log.readStringUntil('\n');
    }

    this->data_log.close();
}

bool SDHandler::prepare(){

    bool initialized = false;
    int count = 5;
    while(!initialized && count > 0) {
        if(!(initialized = SD.begin(cs, SPI, 3000000))){
            Serial.println("Impossible to connect SD reader");
        }else{
            Serial.println("SD connected");
        }
        delay(200);
    }

    return initialized;
}

void SDHandler::deselect(){
    SD.end();
}


void SDHandler::setup_sd(Data &data, int chip_s) {

    uint8_t card_type;

    // TODO: test this solution
    //SPI.setDataMode(SPI_MODE1); // the magnetic encoder communicates with SPI1: CPOL=0 CPHA=1 so we force also the SD card
    //SPI.setBitOrder(MSBFIRST); // the magnetic encoder communicates in MSBFIRST


    cs = chip_s;

    // The SD card is a device that must be mounted AND UNMOUNTED at every writing stage!
    // every other behaviour may damage the SD card and should be avoided
    if(!prepare()){
        loop_infinite();
    }

    card_type = SD.cardType();

    if (card_type == CARD_NONE) {
        Serial.println("No SD card attached");
        loop_infinite();
    }else{
        Serial.println("The card is of type: " + String(card_type));
    }

    Serial.println("CORRECTLY INITIALIZED: SD CARD");

    if(!data.set){
        Serial.println("The path of the Log file was not set in Data.");
        loop_infinite();
    }

    filename = data.log_file;

    // the insertion of the header is done here in order not to access the RTC at setup time
    this->data_log = SD.open(filename, FILE_WRITE, true);

    if (!this->data_log) {
        Serial.print("Error opening the file: ");
        Serial.print(this->data_log);
        this->data_log.close();
        loop_infinite();
    }

    this->data_log.print("timeStamp, Temperature(°C), Pressure(hPa), Humidity(%), Wind_Speed(m/s), "
                         "Wind_Direction(deg), Longitude(deg), Latitude(deg), Altitude(m)\n");
    this->data_log.close();

    deselect();

}
