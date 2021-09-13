#include <SPI.h>
#include <SD.h>


File data_log;

void setup() {
  
  // put your setup code here, to run once:
  if(!SD.begin()){
    Serial.println("Impossible to connect SD reader");  
    while(1);
  }
  
  Serial.println("Successful initialization sd card");

  if(!SD.exists("logs/data.log")){
    Serial.println("Errors in opening the file.");
    Serial.println("The file logs/data.log doesn't exist.");
    while(1);
  }else{
    data_log = SD.open("logs/data.log", FILE_READ);

    while(data_log.available()){
       Serial.write(data_log.read());
    }

    data_log.close();
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
