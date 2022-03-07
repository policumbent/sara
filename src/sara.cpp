#include "../include/sara.h"


using namespace sara;


long now, lastMsg;

SensorHandler& getHandler(){
    static SensorHandler handler;
    try {
        handler = SensorHandler();
    }
    catch(const std::exception& ex){
        Serial.println(ex.what());
    }
    return handler;
}


void setup() {
  Serial.begin(115200);

  Serial.println("BEGIN");
  Serial.println(secured::mqtt_password);
  Serial.println(secured::mqtt_server);
  Serial.println(secured::ssid);
  Serial.println(secured::password);   

  //client = PubSubClient(espClient);

  // per ora salto la verifica del certificato
  espClient.setInsecure();

  getHandler();

  client.setServer(secured::mqtt_server.c_str(), 8883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

void loop() {

    clientConnect();

    now = millis();
    if (now - lastMsg > 1000) {
      lastMsg = now;

      #if BME_DEBUG
        getHandler().getBME280Data();
      #else
          handler.temperature = 0.0;
          handler.pressure = 0.0;
          handler.humidity = 0.0;
      #endif
      #if ANEMOMETER_DEBUG
        getHandler().getWindSpeedData();
      #else
        handler.windSpeed = 0.0;
      #endif

      #if MAGNETOMETER_DEBUG
        getHandler().getWindDirectionData();
      #else
        handler.windDirection = 0.0;
      #endif

      #if RTC_DEBUG
        getHandler().getDateTime();
      #endif

      publishMQTT(getHandler().temperature, getHandler().pressure, getHandler().humidity, getHandler().windSpeed, (int) getHandler().windDirection, getHandler().timestamp);
        getHandler().write_sd();
  }
}
