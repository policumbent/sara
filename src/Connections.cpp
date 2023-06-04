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

    if(USE_MQTT_IP){
        init_client(IPAddress(cred.mqtt_ip), cred.mqtt_port, callback, client);
    }else {
        init_client(cred.mqtt_server.c_str(), cred.mqtt_port, callback, client);
    }


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

void connect(bool keep_trying){
    client_connect((const char *) "ESP-WROOM-32", cred.mqtt_usr.c_str(), cred.mqtt_pass.c_str(), client, keep_trying);
}


void publishMQTT(Data &data) {

    led_on();
    char buffer[32];

    sprintf(buffer, "%02d/%02d/%02d %02d:%02d:%02d\n",
            data.timestamp.year(),data.timestamp.month(), data.timestamp.day(),
            data.timestamp.hour(), data.timestamp.minute(), data.timestamp.second());
    publish("Policumbent/weather/ws1/timestamp", buffer);

    dtostrf(data.temperature, 1, 2, buffer);
    publish("Policumbent/weather/ws1/temperature", buffer);

    dtostrf(data.humidity, 1, 2, buffer);
    publish("Policumbent/weather/ws1/humidity", buffer);

    dtostrf(data.pressure, 1, 2, buffer);
    publish("Policumbent/weather/ws1/pressure", buffer);

    dtostrf(data.wind_speed, 1, 2, buffer);
    publish("Policumbent/weather/ws1/speed", buffer);

    String(data.wind_direction).toCharArray(buffer, 8);
    publish("Policumbent/weather/ws1/direction", buffer);

    String(data.latitude).toCharArray(buffer, 8);
    publish("Policumbent/weather/ws1/latitude", buffer);

    String(data.longitude).toCharArray(buffer, 8);
    publish("Policumbent/weather/ws1/longitude", buffer);

    String(data.altitude).toCharArray(buffer, 8);
    publish("Policumbent/weather/ws1/altitude", buffer);

    led_off();
}