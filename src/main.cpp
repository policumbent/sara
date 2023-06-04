#include "Led.h"
#include "Sensors.h"
#include "DataBackupSD.h"
#include "DataBackupFlash.h"
#include "EpaperDisplay.h"
#include "WebServer.h"
#include "Connections.h"

#define TIME_PUBLISH_SERVER 3000
#define TIME_SD_BACKUP 10000
#define TIME_SPIFFS_LOG 1000
#define TIME_SCREEN_UPDATE 10000

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
unsigned long last_spiff_stage;
unsigned long last_screen_refresh;
unsigned long last_publish;
unsigned long last_log;

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

Sensors<Adafruit_HMC5883_Unified> &getCompass() {
    try {
        static Sensors<Adafruit_HMC5883_Unified> compass = Sensors<Adafruit_HMC5883_Unified>();
        return compass;
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

FlashHandler &getFlashHandler(bool staged = false){
    try {
        static FlashHandler flash_handler = FlashHandler(getData(), staged);
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

    //digitalWrite(cs_sd, LOW);
    //digitalWrite(cs_mag, HIGH);

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
        connect();
        if(check<WEBSERVER_DEBUG>()){
            getWebServer();
        }
    }

    delay(10);
    // must necessarily be declared after the Wifi
    if(check<NTP_DEBUG>()){
        getNTP();
        getNTP().get_data(getData()); // doesn't have a debug clause
        getData().timestamp = getData().ntp_timestamp;
        getData().set_log(true);
    }else{
        getData().set_log(false);
    }

    // the SD must be initialized before the MAGNETIC ENCODER
    // they both communicate using the SPI protocol BUT SD is configured on SPI_0 while the encoder is on SPI_1
    if(check<SD_DEBUG>()){
        getSdHandler();
        getFlashHandler(true);
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
        getEpaper().print_on_display((char *) "SETUP HAS COMPLETED AND EPD INITIALIZED", draw_print);
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
    last_publish = last_log = last_spiff_stage = last_screen_refresh = now; // initializing time variables

    getData().update_timestamp();
}

void loop() {

    if (check<BME_DEBUG>()){
      getBme().get_data(getData());
    }
    if(check<COMPASS_DEBUG>()){
        getCompass().get_data(getData());
    }

    if(check<GPS_DEBUG>()){
        getGPS().get_data(getData());
    }

    if (check<ANEMOMETER_DEBUG>()){
      getAds().get_data(getData());
    }else {
      getData().wind_speed = 0.0;
    }

    if (check<MAGNETOMETER_DEBUG>()){
        digitalWrite(cs_sd, HIGH);
        digitalWrite(cs_mag, LOW);

        getAngleSensor().get_data(getData());

        digitalWrite(cs_sd, HIGH);
        digitalWrite(cs_mag, HIGH);
    }

    if(check<WIFI_DEBUG>()) {
        connect(false); // if the connection is not present we don't want to waste time
    }

    if (check<NTP_DEBUG>()){
      getNTP().get_data(getData());
    }

    // we log into the SPIFFS buffer file frequently, but we backup to SD less frequently
    if(now - last_spiff_stage >= TIME_SPIFFS_LOG) {
        // condition met both when SD buffering is active or SPIFFS log
      if (check<SPIFFS_DEBUG>() || check<SD_DEBUG>()) {
          getFlashHandler(true).write_flash(getData());
      }
      last_spiff_stage = now;
    }

    // Backup into SD
    now = millis();
    if(now - last_log >= TIME_SD_BACKUP) {

      digitalWrite(cs_sd, LOW);
      digitalWrite(cs_mag, HIGH);

      delay(10);

      if (check<SD_DEBUG>()) {
          String txt;
          getFlashHandler(true).read_flash(getData(), txt);
          getSdHandler().write_sd(txt);
      }
      digitalWrite(cs_sd, HIGH);
      digitalWrite(cs_mag, HIGH);

      last_log = now;
    }

    now = millis();
    if(now - last_publish >= TIME_PUBLISH_SERVER){
        if(check<WIFI_DEBUG>()) {
            publishMQTT(getData());
        }

        Serial.print(getData().to_text());

        last_publish = now;
    }

    #ifdef DEBUG
    wind_speed = getData().windSpeed;
    p.Plot();
    #endif

    now = millis();
    if(now - last_screen_refresh >= TIME_SCREEN_UPDATE){
      if(check<EPAPER_DEBUG>()){
          getEpaper().display_data(getData(), draw_print);
      }
      last_screen_refresh = now;
    }
}
