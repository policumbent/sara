#include "Led.h"
#include "Sensors.h"
#include "DataBackupSD.h"
#include "DataBackupFlash.h"
#include "EpaperDisplay.h"
#include "WebServer.h"
#include "Connections.h"

#define CICLE_MSEC 1000
#define SCREEN_UPDATE_MSEC 30000

#ifdef DEBUG
#include "Plotter.h"
Plotter p;
float wind_speed = 0.0;
#endif

// LED pin for status info
const unsigned int LED = 2;
const unsigned int cs_mag = 5;
const unsigned int cs_sd = 2;

unsigned long now;
unsigned long last_msg;
unsigned long last_seen;

/*
 * getters and setters avoid the possibility
 * to have unhandled exceptions which could lead to
 * reboots
 * */
Data &getData() {
    try {
        static Data data = Data();
        return data;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<Adafruit_BME280> &getBme() {
    try {
        static Sensors<Adafruit_BME280> bme = Sensors<Adafruit_BME280>();
        return bme;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<AS5048A> &getAngleSensor() {
    try {
        static Sensors<AS5048A> angleSensor = Sensors<AS5048A>(cs_mag, false);
        return angleSensor;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<Adafruit_ADS1115> &getAds() {
    try {
        static Sensors<Adafruit_ADS1115> ads = Sensors<Adafruit_ADS1115>();
        return ads;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<TinyGPSPlus> &getGPS() {
    try {
        static Sensors<TinyGPSPlus> gps = Sensors<TinyGPSPlus>();
        return gps;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

Sensors<NTPClient> &getNTP(){
    try{
        static Sensors<NTPClient> ntp = Sensors<NTPClient>();
        return ntp;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

SDHandler &getSdHandler(){
    try {
        static SDHandler sd_handler = SDHandler(getData(), cs_sd);
        return sd_handler;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

FlashHandler &getFlashHandler(){
    try {
        static FlashHandler flash_handler = FlashHandler(getData());
        return flash_handler;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

EpaperDisplay &getEpaper(){
    try {
        static EpaperDisplay epd = EpaperDisplay();
        return epd;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

WebServer &getWebServer(){
    try {
        static WebServer web_server = WebServer(&getData());
        return web_server;
    }catch (const std::exception &ex) {
        Serial.println("EXCEPTION");
        Serial.println(ex.what());
        exit(EXIT_FAILURE);
    }
}

void draw_print(){

    getEpaper().display->setRotation(3);
    getEpaper().display->setFont(&FreeMono9pt7b);
    getEpaper().display->setTextColor(GxEPD_BLACK);
    int16_t tbx, tby; uint16_t tbw, tbh;
    getEpaper().display->getTextBounds(getEpaper().get_to_print(), 0, 0, &tbx, &tby, &tbw, &tbh);
    // center bounding box by transposition of origin:
    uint16_t x = ((getEpaper().display->width() - tbw) / 2) - tbx;
    uint16_t y = ((getEpaper().display->height() - tbh) / 2) - tby;
    //display.fillScreen(GxEPD_WHITE);
    getEpaper().display->setCursor(x, y);
    getEpaper().display->print(getEpaper().get_to_print());
}



void setup() {
    Serial.begin(115200);

    Serial.println("BEGIN");

    pinMode(cs_sd, GPIO_MODE_OUTPUT);
    pinMode(cs_mag, GPIO_MODE_OUTPUT);

    digitalWrite(cs_sd, LOW);
    digitalWrite(cs_mag, HIGH);

    led_off();

    delay(10);

    getData(); // initializes Data first

    delay(10);

    if(check<BME_DEBUG>()){
        getBme();
    }
    delay(10);

    if(check<ANEMOMETER_DEBUG>()){
        getAds();
    }

    delay(10);

    if(check<WIFI_DEBUG>()){
        setupMQTT();
        if(check<WEBSERVER_DEBUG>()){
            getWebServer();
        }
    }

    delay(10);
    // must necessarily be declared after the Wifi
    if(check<NTP_DEBUG>()){
        getNTP();
        getNTP().get_data(getData()); // doesn't have a debug clause
        getData().set_log(true);
    }else{
        getData().set_log(false);
    }

    // the SD must be initialized before the MAGNETIC ENCODER
    // they both communicate using the SPI protocol BUT SD is configured on SPI_0 while the encoder is on SPI_1
    if(check<SD_DEBUG>()){
        getSdHandler();
    }else if(check<SPIFFS_DEBUG>()){
        getFlashHandler();
    }

    // some problems with the SD card lead to this solution, TODO: find better solution
    //digitalWrite(cs_sd, HIGH);
    //digitalWrite(cs_mag, HIGH);

    delay(10);

    if(check<MAGNETOMETER_DEBUG>()){
        getAngleSensor();
    }

    if(check<EPAPER_DEBUG>()){
        getEpaper();
        getEpaper().print_on_display(String("SETUP HAS COMPLETED AND EPD INITIALIZED"), draw_print);
    }

#ifdef DEBUG
    p.Begin();
    p.AddTimeGraph("Wind Speed", 1500, "speed", wind_speed);
#endif

    pinMode(LED, OUTPUT);
#ifndef DEBUG
    Serial.println("SETUP COMPLETED");
#endif

    now = millis();
    last_seen = now;
}

void loop() {

  last_msg = now;

  if(check<WIFI_DEBUG>()) {
      connect();
  }
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
      getData().wind_speed = 0.0;
  }

  if (check<NTP_DEBUG>()){
      getNTP().get_data(getData());
  }

  digitalWrite(cs_sd, LOW);
  digitalWrite(cs_mag, HIGH);

  if(check<SD_DEBUG>()){
      getSdHandler().write_sd(getData());
  }else if(check<SPIFFS_DEBUG>()){
      getFlashHandler().write_flash(getData());
  }


  if (check<MAGNETOMETER_DEBUG>()){
      getAngleSensor().get_data(getData());
  }else {
      getData().wind_direction = 0.0;
  }

  publishMQTT(getData());

  if(check<GPS_DEBUG>()){
      getGPS().get_data(getData());
  }

#ifdef DEBUG
  wind_speed = getData().windSpeed;
  p.Plot();
#endif

  now = millis();

  if(now - last_seen > SCREEN_UPDATE_MSEC){
      if(check<EPAPER_DEBUG>()){
          getEpaper().display_data(getData(), draw_print);
      }
      last_seen = now;
  }

  now = millis();

  if(now - last_msg < CICLE_MSEC){
    delay(CICLE_MSEC - (now - last_msg));
  }
}
