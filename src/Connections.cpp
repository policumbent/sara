#include "Connections.h"
#include "Led.h"

Info cred = Info();

// client communication
WiFiClient espClient;
PubSubClient client = PubSubClient(espClient);

void printMqttInfo() {
    Serial.println(cred.mqtt_pass);
    Serial.println(cred.mqtt_server);
}

void printWifiInfo() {
    Serial.println(cred.ssid);
    Serial.println(cred.password);
}

void setupMQTT(){
    Serial.println("MQTT");
    printMqttInfo();
    Serial.println("WiFi");
    printWifiInfo();

    init_wifi(cred.ssid.c_str(), cred.password.c_str());
    init_client(cred.mqtt_server.c_str(), cred.mqtt_port, callback, client);
}

void callback(const char *topic, byte *message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();
}

void publish(const char* topic, const char * payload){
    client_publish(topic, payload, client);
}

void connect(){
    client_connect((const char *) "ESP-WROOM-32", cred.mqtt_usr.c_str(), cred.mqtt_pass.c_str(), client);
}

void publishMQTT(Data &data) {

    led_on();
    char buffer[32];

    sprintf(buffer, "%02d/%02d/%02d %02d:%02d:%02d\n",
            data.timestamp.year(),data.timestamp.month(), data.timestamp.day(),
            data.timestamp.hour(), data.timestamp.minute(), data.timestamp.second());

#ifndef DEBUG
    Serial.println("\n- - - - - MQTT DATA - - - - -\n");
    // TIME
    Serial.print(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        publish("weather/ws1/timestamp", buffer);
    }
    // Convert the value to a char array
    dtostrf(data.temperature, 1, 2, buffer);

#ifndef DEBUG
    Serial.print("Temperature: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        publish("weather/ws1/temperature", buffer);
    }

    // Convert the value to a char array
    dtostrf(data.humidity, 1, 2, buffer);

#ifndef DEBUG
    Serial.print("Humidity: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        publish("weather/ws1/humidity", buffer);
    }

    // Convert the value to a char array
    dtostrf(data.pressure, 1, 2, buffer);
#ifndef DEBUG
    Serial.print("Pressure: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()) {
        publish("weather/ws1/pressure", buffer);
    }

    // Convert the value to a char array
    dtostrf(data.wind_speed, 1, 2, buffer);
#ifndef DEBUG
    Serial.print("Wind Speed: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        publish("weather/ws1/speed", buffer);
    }

    // Convert the value to a char array
    String(data.wind_direction).toCharArray(buffer, 8);
#ifndef DEBUG
    Serial.print("Wind Direction: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        publish("weather/ws1/direction", buffer);
    }

    // Convert the value to a char array
    String(data.latitude).toCharArray(buffer, 8);
#ifndef DEBUG
    Serial.print("Latitude: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        publish("weather/ws1/latitude", buffer);
    }

    // Convert the value to a char array
    String(data.longitude).toCharArray(buffer, 8);
#ifndef DEBUG
    Serial.print("Longitude: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        publish("weather/ws1/longitude", buffer);
    }

    // Convert the value to a char array
    String(data.altitude).toCharArray(buffer, 8);
#ifndef DEBUG
    Serial.print("Altitude: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        publish("weather/ws1/altitude", buffer);
    }

    led_off();
#ifndef DEBUG
    Serial.println("\n- - - - - - - - - - - - - - -\n");
#endif
}