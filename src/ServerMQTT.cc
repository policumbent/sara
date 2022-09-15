#include "ServerMQTT.h"
#include "Led.h"

// client communication
WiFiClient espClient;
PubSubClient client = PubSubClient(espClient);
Info cred = Info();
time_t timestamp;
struct tm *tm_timestamp;

void printMqttInfo() {
    Serial.println(cred.mqtt_pass);
    Serial.println(cred.mqtt_server);
}

void printWifiInfo() {
    Serial.println(cred.ssid);
    Serial.println(cred.password);
}

void init_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  //  Serial.println(ssid);
  //
  WiFi.begin(cred.ssid.c_str(), cred.password.c_str());

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
  //espClient.setInsecure();

  client.setServer(cred.mqtt_server.c_str(), cred.mqtt_port);
  client.setCallback(callback);
}

void clientPublish(const char *topic, const char *payload) {
  client.publish(topic, payload);
}

void clientConnect() {
    if (check<WIFI_DEBUG>()) {
        if (!client.connected()) {
            reconnect();
        }
        client.loop();
    }
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
    /*Serial.print(data.timestamp.year(), DEC);
    Serial.print("/");
    Serial.print(data.timestamp.month(), DEC);
    Serial.print("/");
    Serial.print(data.timestamp.day(), DEC);
    Serial.print("  ");
    Serial.print(data.timestamp.hour(), DEC);
    Serial.print(":");
    Serial.print(data.timestamp.minute(), DEC);
    Serial.print(":");
    Serial.println(data.timestamp.second(), DEC);*/

     #endif
    if (check<WIFI_DEBUG>()){
        client.publish("weather/ws1/timestamp", buffer);
    }
    // Convert the value to a char array
    dtostrf(data.temperature, 1, 2, buffer);

#ifndef DEBUG
    Serial.print("Temperature: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        client.publish("weather/ws1/temperature", buffer);
    }

    // Convert the value to a char array
    dtostrf(data.humidity, 1, 2, buffer);

#ifndef DEBUG
    Serial.print("Humidity: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        client.publish("weather/ws1/humidity", buffer);
    }

    // Convert the value to a char array
    dtostrf(data.pressure, 1, 2, buffer);
#ifndef DEBUG
    Serial.print("Pressure: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()) {
        client.publish("weather/ws1/pressure", buffer);
    }

    // Convert the value to a char array
    dtostrf(data.windSpeed, 1, 2, buffer);
#ifndef DEBUG
    Serial.print("Wind Speed: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        client.publish("weather/ws1/speed", buffer);
    }

    // Convert the value to a char array
    String(data.windDirection).toCharArray(buffer, 8);
#ifndef DEBUG
    Serial.print("Wind Direction: ");
    Serial.println(buffer);
#endif
    if (check<WIFI_DEBUG>()){
        client.publish("weather/ws1/direction", buffer);
    }



    led_off();
#ifndef DEBUG
    Serial.println("\n- - - - - - - - - - - - - - -\n");
#endif
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", cred.mqtt_usr.c_str(), cred.mqtt_pass.c_str())) {
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
