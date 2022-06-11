# include "credentials.h"

Info::Info(){

    if(!SPIFFS.begin()){
        Serial.println("Impossible to read flash");
    }


    File credentials = SPIFFS.open("/credentials.txt", "r");

    if(!credentials){
        Serial.println("There is not a credential.txt file in memory. Create on");
        while(1);
    }

    this->ssid = credentials.readStringUntil('\n');
    this->password = credentials.readStringUntil('\n');
    this->mqtt_server = credentials.readStringUntil('\n');
    this->mqtt_usr = credentials.readStringUntil('\n');
    this->mqtt_pass = credentials.readStringUntil('\n');

}