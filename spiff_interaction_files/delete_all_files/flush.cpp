# include "SPIFFS.h"

void setup(){

    Serial.begin(115200);

    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        while(1);
    }

    File root = SPIFFS.open("/");

    File file = root.openNextFile("r");

    String txt;

    while(file){

        Serial.print("FILE: ");
        Serial.println(file.name());

        SPIFFS.remove(file.name());

        file = root.openNextFile("r");
    }



}

void loop(){

};