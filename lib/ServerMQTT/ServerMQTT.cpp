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

void reconnect(char* id, const char *mqtt_user, const char *mqtt_password, PubSubClient &client) {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(id, mqtt_user, mqtt_password)) {
            Serial.println("connected");
            // Subscribe
            // client.subscribe("test/led");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void clientPublish(const char *topic, const char *payload, PubSubClient &client) {
  client.publish(topic, payload);
}

void clientConnect(char* id, const char *mqtt_user, const char *mqtt_password, PubSubClient &client) {
    if (!client.connected()) {
        reconnect(id, mqtt_user, mqtt_password, client);
    }
    client.loop();
}
