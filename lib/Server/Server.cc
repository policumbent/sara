#include "Server.h"
#include "../Led/Led.h"
#include "../Sensor/Sensor.h"

// Wifi and Mqtt credentials
#include "credentials.h"

// client communication
WiFiClientSecure espClient;
PubSubClient client = PubSubClient(espClient);

void init_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  //  Serial.println(SSID);
  //
  WiFi.begin(SSID, PASSWORD);

  while (WiFiClass::status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void init_client() {
  // TODO: aggiungere la verifica del certificato
  espClient.setInsecure();

  client.setServer(MQTT_SERVER, 8883);
  client.setCallback(callback);
}

void printMqttInfo() {
  Serial.println(MQTT_PASSWORD);
  Serial.println(MQTT_SERVER);
}

void printWifiInfo() {
  Serial.println(SSID);
  Serial.println(PASSWORD);
}

void clientPublish(const char *topic, const char *payload) {
  client.publish(topic, payload);
}

void clientConnect() {
#if WIFI_DEBUG
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
#endif
}

void callback(char *topic, byte *message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  //   // Feel free to add more if statements to control more GPIOs with MQTT

  //   // If a message is received on the topic esp32/output, you check if the
  //   message is either "on" or "off".
  //   // Changes the output state according to the message
  //   if (String(topic) == "test/led") {
  //     Serial.print("Changing output to ");
  //     if(messageTemp == "on"){
  //       Serial.println("on");
  //       digitalWrite(LED, HIGH);
  //     }
  //     else if(messageTemp == "off"){
  //       Serial.println("off");
  //       digitalWrite(LED, LOW);
  //     }
  //   }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", MQTT_USERNAME, MQTT_PASSWORD)) {
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

void publishMQTT(Data data) {

    led_on();

    Serial.println("\n- - - - - MQTT DATA - - - - -\n");

    // TIME
    Serial.print(data.timestamp.year(), DEC);
    Serial.print("/");
    Serial.print(data.timestamp.month(), DEC);
    Serial.print("/");
    Serial.print(data.timestamp.day(), DEC);
    Serial.print("  ");
    Serial.print(data.timestamp.hour(), DEC);
    Serial.print(":");
    Serial.print(data.timestamp.minute(), DEC);
    Serial.print(":");
    Serial.println(data.timestamp.second(), DEC);

    // Convert the value to a char array
    char buffer[8];
    dtostrf(data.temperature, 1, 2, buffer);
    Serial.print("Temperature: ");
    Serial.println(buffer);
    if (check<WIFI_DEBUG>()){
        client.publish("weather_stations/ws1/temperature", buffer);
    }

  // Convert the value to a char array
  dtostrf(data.humidity, 1, 2, buffer);
  Serial.print("Humidity: ");
  Serial.println(buffer);

  if (check<WIFI_DEBUG>()){
      client.publish("weather_stations/ws1/humidity", buffer);
  }

  // Convert the value to a char array
  dtostrf(data.pressure, 1, 2, buffer);
  Serial.print("Pressure: ");
  Serial.println(buffer);

  if (check<WIFI_DEBUG>()) {
      client.publish("weather_stations/ws1/pressure", buffer);
  }

  // Convert the value to a char array
  dtostrf(data.windSpeed, 1, 2, buffer);
  Serial.print("Wind Speed: ");
  Serial.println(buffer);

  if (check<WIFI_DEBUG>()){
      client.publish("weather_stations/ws1/wind_speed", buffer);
  }

  // Convert the value to a char array
  String(data.windDirection).toCharArray(buffer, 8);
  Serial.print("Wind Direction: ");
  Serial.println(buffer);

  if (check<WIFI_DEBUG>()){
      client.publish("weather_stations/ws1/wind_direction", buffer);
  }

  led_off();

  Serial.println("\n- - - - - - - - - - - - - - -\n");
}
