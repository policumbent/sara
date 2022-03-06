#include "../include/data_manipulation.h"

SensorHandler::SensorHandler(){

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
    setup_sensorTempUm();
#endif

#if ADC_DEBUG
    setup_adc();
#endif

}


void SensorHandler::setup_rtc(){

    clientConnect();

    if(!rtc.begin()){
        Serial.println("Problems setting up RTC module");
#if WIFI_DEBUG
        sara::client.publish("SENSORE RTC: ", "NOT WORKING");
#endif
        while(1) delay(100);
    }

    Serial.println("CORRECTLY INITIALIZED: RTC");

    //the file has the timestamp as name
    timestamp = getDateTime();
    LOG_FILE += String(timestamp.year(), DEC) + "_" + String(timestamp.month(), DEC) + "_" + String(timestamp.day(), DEC) + "__";
    LOG_FILE += String(timestamp.hour(), DEC) + "_" + String(timestamp.minute(), DEC) + "_" + String(timestamp.second(), DEC) + ".csv";


    if(!rtc.isrunning()){
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}
void SensorHandler::setup_sd(){

    auto open_mode = FILE_WRITE;

#if !RTC_DEBUG
    //in case the rtc is not implemented it prints everything in the same file(we can then use another program to separate the CSV files)
    LOG_FILE += "data.csv";
#endif

    clientConnect();

    if(!SD.begin(2, SPI)){
        Serial.println("Impossible to connect SD reader");

#if WIFI_DEBUG
        sara::client.publish("SENSORE SD CARD: ", "NOT WORKING");
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
void SensorHandler::setup_magnetometer(){
    angleSensor.begin();
    Serial.println("CORRECTLY INITIALIZED: MAGNETIC ENCODER");
}
void SensorHandler::setup_adc(){

    clientConnect();

    if (!ads.begin(0b1001000)) {
        Serial.println("Failed to initialize ADC.");

#if WIFI_DEBUG
        sara::client.publish("SENSORE ADC: ", "NOT WORKING");
#endif

        while (1) delay(100);
    }

    Serial.println("CORRECTLY INITIALIZED: ADC");
}
void SensorHandler::setup_wifi() {

    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(secured::ssid);

    WiFi.begin(secured::ssid.c_str(), secured::password.c_str());

    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void SensorHandler::setup_sensorTempUm(){

    int status;

    clientConnect();


    status = bme.begin(0x76);//inizializzazione sensore temperatura
    for(int i=0x00; !status && i< 0xFF; i++){
        status = bme.begin(0x76);
    }
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");

        sara::client.publish("SENSORE BME: ", "NOT WORKING");
        while(1) delay(10);
    }

    Serial.println("CORRECTLY INITIALIZED: BME280");

}


void        SensorHandler::getBME280Data(){
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure() / 100.0F;
}
double       SensorHandler::getWindSpeedData(){
    double wind_speed = 0.0;
    int16_t val = ads.readADC_SingleEnded(0);
    float voltage = 0.0;

    //voltage = (val * analog_to_volt_conv);
    voltage = ads.computeVolts(val);

    Serial.println(val);
    Serial.println(voltage);

#if WIFI_DEBUG
    char buffer[8];
    dtostrf(voltage, 1, 2, buffer);
    sara::client.publish("weather_stations/ws1/humidity", buffer);
#endif

    if(voltage <= sara::vmin){
        wind_speed = 0.0;
    }
    else if(voltage >= sara::vmax){
        wind_speed = sara::max_speed;
    }else{
        wind_speed = ((voltage-sara::vmin) * (sara::max_speed - sara::min_speed)/(sara::vmax - sara::vmin))*3.6;
    }

    return wind_speed;
}
int         SensorHandler::getWindDirectionData(){
    return (int) angleSensor.getRotationInDegrees();
}
DateTime    SensorHandler::getDateTime(){
    return rtc.now();
}
void        SensorHandler::write_sd(){

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
