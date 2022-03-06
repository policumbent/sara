#include "../include/sara.h"


using namespace sara;

SensorHandler handler;
long now, lastMsg;

void setup() {
  Serial.begin(115200);

  Serial.println(secured::mqtt_password);
  Serial.println(secured::mqtt_server);
  Serial.println(secured::ssid);
  Serial.println(secured::password);   

  //client = PubSubClient(espClient);

  // per ora salto la verifica del certificato
  espClient.setInsecure(); 

  client.setServer(secured::mqtt_server.c_str(), 8883);
  client.setCallback(callback);

  handler = SensorHandler();

  pinMode(ledPin, OUTPUT);
}

void loop() {

    clientConnect();

    now = millis();
    if (now - lastMsg > 1000) {
      lastMsg = now;

      #if BME_DEBUG
      handler.getBME280Data();
      #else
          handler.temperature = 0.0;
          handler.pressure = 0.0;
          handler.humidity = 0.0;
      #endif
      #if ANEMOMETER_DEBUG
        handler.getWindSpeedData();
      #else
        handler.windSpeed = 0.0;
      #endif

      #if MAGNETOMETER_DEBUG
        handler.getWindDirectionData();
      #else
        handler.windDirection = 0.0;
      #endif

      #if RTC_DEBUG
        handler.getDateTime();
      #endif

      publishMQTT(handler.temperature, handler.pressure, handler.humidity, handler.windSpeed, handler.windDirection, handler.timestamp);
      handler.write_sd();
  }
}
