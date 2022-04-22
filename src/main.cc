#include "../lib/Sensor/Sensor.h"
#include "../lib/Server/Server.h"
#include "../lib/DataBackupSd/DataBackupSD.h"

long now;
long lastMsg;


Data &getData() {
    try {
        static Data data = Data();
        return data;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}
Sensor<Adafruit_BME280> &getBme() {
    try {
        static Sensor<Adafruit_BME280> bme = Sensor<Adafruit_BME280>();
        return bme;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}
Sensor<AS5048A> &getAngleSensor() {
    try {
        static Sensor<AS5048A> angleSensor = Sensor<AS5048A>();
        return angleSensor;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}
Sensor<Adafruit_ADS1115> &getAds() {
    try {
        static Sensor<Adafruit_ADS1115> ads = Sensor<Adafruit_ADS1115>();
        return ads;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}
Sensor<RTC_DS1307> &getRtc() {
    try {
        static Sensor<RTC_DS1307> rtc = Sensor<RTC_DS1307>();
        return rtc;
    }catch (const std::exception &ex) {
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

SDHandler &getSdHandler(){
    try {
        static SDHandler sd_handler = SDHandler(getData());
        return sd_handler;
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

      if (check<BME_DEBUG>()){
          getBme().get_data(getData());
      }else {
          getData().temperature = 0.0;
          getData().pressure = 0.0;
          getData().humidity = 0.0;
      }
      if (check<ANEMOMETER_DEBUG>()){
          getAds().get_data(getData());
      }else {
          getData().windSpeed = 0.0;
      }

      if (check<MAGNETOMETER_DEBUG>()){
          getAngleSensor().get_data(getData());
      }else {
          getData().windDirection = 0.0;
      }

      if (check<RTC_DEBUG>()){
          getRtc().get_data(getData());
      }else {
          getData().windDirection = 0.0;
      }

      publishMQTT(getData());
      getSdHandler().write_sd(getData(), getRtc());
  }
}
