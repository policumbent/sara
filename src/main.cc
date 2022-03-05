#include <WiFiClientSecure.h>
#include <stdio.h>
#include <Wire.h>
#include "FS.h"
#include <SPI.h>
#include <SD.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_Sensor.h>
#include <RTClib.h>
#include <PubSubClient.h>

#include "AS5048A.h"


#define SEALEVELPRESSURE_HPA (1013.25)

// USA QUESTI PER DEBUGGARE NON COMMENTARE
#define WIFI_DEBUG         0
#define SD_DEBUG           0
#define MAGNETOMETER_DEBUG 0
#define BME_DEBUG          0
#define ADC_DEBUG          0
#define RTC_DEBUG          0
#define ANEMOMETER_DEBUG   0


Adafruit_BME280 bme; // I2C
AS5048A angleSensor(SS, true);
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
RTC_DS1307 rtc; /* REAL TIME CLOCK */


// Replace the next variables with your SSID/Password combination for the wifi
const char* ssid = "toolbox";
const char* password = "Toolbox.Torino";

// username e password server mqtt
const char* username = "stefano";
const char* mqtt_password = "martafaschifo!";

// MQTT server nel lab del poli:
const char* mqtt_server = "51.38.227.9";

const int led = 2;
int status = 0;
unsigned long delayTime;

File data_log;
String LOG_FILE = "/";
int val = 0;
float voltage = 0.0;
//float analog_to_volt_conv = 2.0/10720; //converte il valore che legge il pin analogico in un voltaggio.

float vmin = 0.41, vmax = 2.0;
float min_speed = 0.0, max_speed = 32.4;
float temperature, humidity, pressure;
float windSpeed, windDirection;
DateTime timestamp_;

WiFiClientSecure espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

// LED Pin
const int ledPin = 2;

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", username, mqtt_password)){
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

DateTime getDateTime(){
  return rtc.now();
}

void setup_rtc(){

#if WIFI_DEBUG
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
#endif
  if(!rtc.begin()){
    Serial.println("Problems setting up RTC module");
#if WIFI_DEBUG
    client.publish("SENSORE RTC: ", "NOT WORKING");
#endif
    while(1) delay(100);
  }

  Serial.println("CORRECTLY INITIALIZED: RTC");

  //the file has the timestamp as name
  timestamp_ = getDateTime();
  LOG_FILE += String(timestamp_.year(), DEC)+"_"+String(timestamp_.month(), DEC)+"_"+String(timestamp_.day(), DEC)+"__";
  LOG_FILE += String(timestamp_.hour(), DEC)+"_"+String(timestamp_.minute(), DEC)+"_"+String(timestamp_.second(), DEC) + ".csv";


  if(!rtc.isrunning()){
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void setup_sd(){

  auto open_mode = FILE_WRITE;

#if !RTC_DEBUG
  //in case the rtc is not implemented it prints everything in the same file(we can then use another program to separate the CSV files)
  LOG_FILE += "data.csv";
#endif

#if WIFI_DEBUG
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
#endif

  if(!SD.begin(2, SPI)){
    Serial.println("Impossible to connect SD reader");

#if WIFI_DEBUG
    client.publish("SENSORE SD CARD: ", "NOT WORKING");
#endif

    while(1) delay(100);
  }

  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    while(1);
  }

  Serial.println("CORRECTLY INITIALIZED: SD CARD");


  if(!SD.exists(LOG_FILE)){
    Serial.println("The file ./data.csv doesn't exist --- CREATING...");
  }else{
    open_mode = FILE_APPEND;
  }
  data_log = SD.open(LOG_FILE, open_mode);

  if(!data_log){
    Serial.print("Error opening the file: ");
    Serial.print(data_log);
    data_log.close();
    while(1);
  }

  data_log.print("timeStamp, Temperature, Pressure, Humidity, Wind_Speed, Wind_Direction \n");
  data_log.close();
}

void setup_magnetometer(){
  angleSensor.begin();
  Serial.println("CORRECTLY INITIALIZED: MAGNETIC ENCODER");
}

void setup_adc(){

#if WIFI_DEBUG
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
#endif

  if (!ads.begin(0b1001000)) {
    Serial.println("Failed to initialize ADC.");

#if WIFI_DEBUG
    client.publish("SENSORE ADC: ", "NOT WORKING");
#endif

    while (1) delay(100);
  }

  Serial.println("CORRECTLY INITIALIZED: ADC");
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_sensoreTempUm(){

  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  status = bme.begin(0x76);//inizializzazione sensore temperatura
  for(int i=0x00; !status && i< 0xFF; i++){
    status = bme.begin(0x76);
  }
  delayTime = 1000;
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");

    client.publish("SENSORE BME: ", "NOT WORKING");
    while(1) delay(10);
  }

  Serial.println("CORRECTLY INITIALIZED: BME280");

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

void getBME280Data(float *temp, float *pres, float *hum){
  *temp = bme.readTemperature();
  *hum = bme.readHumidity();
  *pres = bme.readPressure() / 100.0F;
}

float getWindSpeedData(){
  float wind_speed = 0.0; // converte il voltaggio in velocità del vento
  int16_t val = ads.readADC_SingleEnded(0);

  //voltage = (val * analog_to_volt_conv);
  voltage = ads.computeVolts(val);

  Serial.println(val);
  Serial.println(voltage);

#if WIFI_DEBUG
  char buffer[8];
  dtostrf(voltage, 1, 2, buffer);
  client.publish("weather_stations/ws1/humidity", buffer);
#endif

  if(voltage <= vmin){
    return 0.0;
  }
  else if(voltage >= vmax){
    return max_speed;
  }else{
    wind_speed = ((voltage-vmin) * (max_speed - min_speed)/(vmax - vmin))*3.6;
    return wind_speed;
  }

}

int getWindDirectiondData(){
  return angleSensor.getRotationInDegrees();
}

void write_sd(float temperature, float pressure, float humidity, float windSpeed, int windDirection, DateTime timestamp){

  // printing on file
  data_log = SD.open(LOG_FILE, FILE_APPEND);
  String data_to_print;   // ---> this string should contain all the information and write just once on the file --> this should reduce errors

  data_to_print += String(timestamp.year(), DEC)+"/"+String(timestamp.month(), DEC)+"/"+String(timestamp.day(), DEC)+" ";
  data_to_print += String(timestamp.hour(), DEC)+":"+String(timestamp.minute(), DEC)+":"+String(timestamp.second(), DEC) + ", ";

  // Convert the value to a char array
  char buffer[8];
  dtostrf(temperature, 1, 2, buffer);
  data_to_print += String(buffer) + ", ";

  // Convert the value to a char array
  dtostrf(humidity, 1, 2, buffer);
  data_to_print += String(buffer) + ", ";

  // Convert the value to a char array
  dtostrf(pressure, 1, 2, buffer);

  data_to_print += String(buffer) + ", ";

  // Convert the value to a char array
  dtostrf(windSpeed, 1, 2, buffer);
  data_to_print += String(buffer) + ", ";

  // Convert the value to a char array
  String(windDirection).toCharArray(buffer,8);
  data_to_print += String(buffer);

  data_log.println(data_to_print);

  data_log.close();
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

void setup() {
  Serial.begin(115200);

  // per ora salto la verifica del certificato
  espClient.setInsecure();

  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);

#if WIFI_DEBUG
  setup_wifi();
#endif

#if RTC_DEBUG
  setup_rtc();
#endif

#if SD_DEBUG
  setup_sd();
#endif

#if MAGNETOMETER_DEBUG
  setup_magnetometer();
#endif

#if BME_DEBUG
  setup_sensoreTempUm();
#endif

#if ADC_DEBUG
  setup_adc();
#endif

  pinMode(ledPin, OUTPUT);
}

void loop() {

#if WIFI_DEBUG
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
#endif

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;

#if BME_DEBUG
    getBME280Data(&temperature, &pressure, &humidity);
#else
    temperature = 0.0;
    pressure = 0.0;
    humidity = 0.0;
#endif
#if ANEMOMETER_DEBUG
    windSpeed = getWindSpeedData();
#else
    windSpeed = 0.0;
#endif

#if MAGNETOMETER_DEBUG
    windDirection = getWindDirectiondData();
#else
    windDirection = 0.0;
#endif

#if RTC_DEBUG
    timestamp_ = getDateTime();
#endif

    publishMQTT(temperature, pressure, humidity, windSpeed, windDirection, timestamp_);
    write_sd(temperature, pressure, humidity, windSpeed, windDirection, timestamp_);

  }
}
