#include <Arduino.h>
#include <gtest/gtest.h>

void setup(){
    Serial.begin(115200);
    delay(2000);

    testing::InitGoogleTest();

    if(!RUN_ALL_TESTS()){
    }

    pinMode(2, OUTPUT);
}


void loop(){

    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(1000);


}