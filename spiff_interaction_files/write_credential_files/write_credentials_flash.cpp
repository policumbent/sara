# include "SPIFFS.h"

void setup(){

    Serial.begin(115200);

    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        while(1);
    }

    String ssid = "";
    String password = "";
    String mqtt_server = "";
    String mqtt_usr = "";
    String mqtt_pass = "";


    Serial.println("SSID: "+ssid);
    Serial.println("PASSWORD: "+password);
    Serial.println("MQTT SERVER: "+mqtt_server);
    Serial.println("MQTT USERNAME: "+mqtt_usr);
    Serial.println("MQTT PASSWORD: "+mqtt_pass);

    File cred = SPIFFS.open("/credentials.txt", "w");

    cred.println(ssid);
    cred.println(password);
    cred.println(mqtt_server);
    cred.println(mqtt_usr);
    cred.println(mqtt_pass);

    cred.close();

}

void loop(){

};