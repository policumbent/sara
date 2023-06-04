#include "ServerMQTT.h"

void init_wifi(const char* ssid, const char* password) {
  Serial.println();
  Serial.print("Connecting to ");

  WiFi.begin(ssid, password);

  while (WiFiClass::status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void init_client(const char *mqtt_server, int mqtt_port, void (*callback) (const char *, uint8_t *, unsigned int), PubSubClient &client) {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void init_client(IPAddress mqtt_server, int mqtt_port, void (*callback) (const char *, uint8_t *, unsigned int), PubSubClient &client) {
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

bool attempt_reconnection(const char* id, const char *mqtt_user, const char *mqtt_password, PubSubClient &client) {

    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(id, mqtt_user, mqtt_password)) {
        Serial.println("connected");
        return true;
    } else {
        Serial.println("failed, rc=");
        Serial.print(client.state());
        return false;
    }
}

void reconnect(const char* id, const char *mqtt_user, const char *mqtt_password, PubSubClient &client) {
    // Loop until we're reconnected
    while (!client.connected()) {
        if(!attempt_reconnection(id, mqtt_user, mqtt_password, client)){
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void client_publish(const char *topic, const char *payload, PubSubClient &client) {
  client.publish(topic, payload);
}

void client_read(const char *topic, PubSubClient &client){
    client.subscribe(topic);
}

void client_connect(const char* id, const char *mqtt_user, const char *mqtt_password, PubSubClient &client, bool keep_trying) {
    if (!client.connected()) {
        if(keep_trying) {
            reconnect(id, mqtt_user, mqtt_password, client);
        }else{
            attempt_reconnection(id, mqtt_user, mqtt_password, client);
        }
    }
    client.loop();
}
