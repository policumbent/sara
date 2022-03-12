#include "Sensor.h"
#include "Server.h"

long now;
long lastMsg;

SensorHandler &getHandler() {
    try {
        static SensorHandler handler = SensorHandler();
        return handler;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

void setup() {
  Serial.begin(115200);

  Serial.println("BEGIN");

  printMqttInfo();
  printWifiInfo();

  // client = PubSubClient(espClient);

  init_wifi();
  init_client();

  pinMode(LED, OUTPUT);
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

   publishMQTT(getHandler().temperature, getHandler().pressure,
                getHandler().humidity, getHandler().windSpeed,
                (int)getHandler().windDirection, getHandler().timestamp);

    getHandler().write_sd();
  }
}
