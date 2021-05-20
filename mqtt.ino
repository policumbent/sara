#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <stdio.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "PubSubClient.h"

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C

// Replace the next variables with your SSID/Password combination
const char* ssid = "Home Cornaglia"; //Redmi
const char* password = "homecornaglia_toaldo2019";// riky2001

// username e password server mqtt
const char* username = "stefano";
const char* mqtt_password = "martafaschifo!";

// MQTT server nel lab del poli:
const char* mqtt_server = "130.192.38.75";

//const int D34 = 10, D23 = 36, D22 = 39, D21 = 42, D19 = 38, D18 = 35;
const int led = 2;
int status = 0;
unsigned long delayTime;

WiFiClientSecure espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

// LED Pin
const int ledPin = 2;

void setup() {
   /*
  pinMode(D34, INPUT);
  pinMode(D23, INPUT);
  pinMode(D22, INPUT);
  pinMode(D21, INPUT); //SDA
  pinMode(D19, INPUT);
  pinMode(D18, INPUT); //SCL
*/
  Serial.begin(115200);
  
  // per ora salto la verifica del certificato
  espClient.setInsecure(); 

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  //status = bme.begin(); 
  
  setup_wifi();
  setup_sensoreTempUm();
  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);

    
  
  pinMode(ledPin, OUTPUT);
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
  status = bme.begin(0x76);//inizializzazione sensore temperatura
  delayTime = 1000;
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
      Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
      while(1) delay(10);
  }else{
   Serial.println("The sensor is connected");
  }

 Serial.println("SETUP: DONE");
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
    if (client.connect("ESP8266Client")) {
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

void getBME280Data(float *temp, float *pres, float *hum){
  *temp = bme.readTemperature();
  *hum = bme.readHumidity();
  *pres = bme.readPressure() / 100.0F;
}


float getWindSpeedData(){
  return float(random(10,40))/10;
}


int getWindDirectiondData(){
  return int(random(0,359));
}

void publishMQTT(float temperature, float pressure, float humidity, float windSpeed, int windDirection){
    digitalWrite(ledPin, HIGH);
    // Convert the value to a char array
    char buffer[8];
    dtostrf(temperature, 1, 2, buffer);
    Serial.print("Temperature: ");
    Serial.println(buffer);
    client.publish("weather_stations/ws1/temperature", buffer);

    // Convert the value to a char array
    dtostrf(humidity, 1, 2, buffer);
    Serial.print("Humidity: ");
    Serial.println(buffer);
    client.publish("weather_stations/ws1/humidity", buffer);

    // Convert the value to a char array
    dtostrf(pressure, 1, 2, buffer);
    Serial.print("Pressure: ");
    Serial.println(buffer);
    client.publish("weather_stations/ws1/pressure", buffer);

    // Convert the value to a char array
    dtostrf(windSpeed, 1, 2, buffer);
    Serial.print("Wind Speed: ");
    Serial.println(buffer);
    client.publish("weather_stations/ws1/wind_speed", buffer);

    // Convert the value to a char array
    String(windDirection).toCharArray(buffer,8);
    Serial.print("Wind Direction: ");
    Serial.println(buffer);
    client.publish("weather_stations/ws1/wind_direction", buffer);
    digitalWrite(ledPin, LOW);

    Serial.println("\n- - - - - - - - - - - - - - -\n");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    float temperature, humidity, pressure;
    getBME280Data(&temperature, &pressure, &humidity);

    float windSpeed = getWindSpeedData();
    int windDirection = getWindDirectiondData();
    publishMQTT(temperature, pressure, humidity, windSpeed, windDirection);

  }
}
