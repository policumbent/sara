#include "Led.h"

const int LED = 2;

void led_on() { digitalWrite(LED, HIGH); }

void led_off() { digitalWrite(LED, LOW); }

void blink_led(int n){
    led_off();
    for(int i=0; i<n; i++){
        delay(10);
        led_on();
        delay(10);
        led_off();
    }
}