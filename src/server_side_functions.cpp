#include "../include/sara.h"

using namespace sara;


void clientConnect(){
    #if WIFI_DEBUG
      if (!client.connected()) {
        reconnect();
      }
      client.loop();
    #endif
}

void callback(char* topic, byte* message, unsigned int length) {
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

//   // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
//   // Changes the output state according to the message
//   if (String(topic) == "test/led") {
//     Serial.print("Changing output to ");
//     if(messageTemp == "on"){
//       Serial.println("on");
//       digitalWrite(ledPin, HIGH);
//     }
//     else if(messageTemp == "off"){
//       Serial.println("off");
//       digitalWrite(ledPin, LOW);
//     }
//   }
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP8266Client", secured::username.c_str(), secured::mqtt_password.c_str())){
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

void publishMQTT(float temperature, float pressure, float humidity, float windSpeed, int windDirection, DateTime timestamp){

    digitalWrite(ledPin, HIGH);

    Serial.println("\n- - - - - MQTT DATA - - - - -\n");

    //TIME
    Serial.print(timestamp.year(), DEC);
    Serial.print("/");
    Serial.print(timestamp.month(), DEC);
    Serial.print("/");
    Serial.print(timestamp.day(), DEC);
    Serial.print("  ");
    Serial.print(timestamp.hour(), DEC);
    Serial.print(":");
    Serial.print(timestamp.minute(), DEC);
    Serial.print(":");
    Serial.println(timestamp.second(), DEC);


    // Convert the value to a char array
    char buffer[8];
    dtostrf(temperature, 1, 2, buffer);
    Serial.print("Temperature: ");
    Serial.println(buffer);
#if WIFI_DEBUG
    client.publish("weather_stations/ws1/temperature", buffer);
#endif

    // Convert the value to a char array
    dtostrf(humidity, 1, 2, buffer);
    Serial.print("Humidity: ");
    Serial.println(buffer);

#if WIFI_DEBUG
    client.publish("weather_stations/ws1/humidity", buffer);
#endif

    // Convert the value to a char array
    dtostrf(pressure, 1, 2, buffer);
    Serial.print("Pressure: ");
    Serial.println(buffer);

#if WIFI_DEBUG
    client.publish("weather_stations/ws1/pressure", buffer);
#endif

    // Convert the value to a char array
    dtostrf(windSpeed, 1, 2, buffer);
    Serial.print("Wind Speed: ");
    Serial.println(buffer);

#if WIFI_DEBUG
    client.publish("weather_stations/ws1/wind_speed", buffer);
#endif

    // Convert the value to a char array
    String(windDirection).toCharArray(buffer,8);
    Serial.print("Wind Direction: ");
    Serial.println(buffer);

#if WIFI_DEBUG
    client.publish("weather_stations/ws1/wind_direction", buffer);
#endif

    digitalWrite(ledPin, LOW);

    Serial.println("\n- - - - - - - - - - - - - - -\n");
}
