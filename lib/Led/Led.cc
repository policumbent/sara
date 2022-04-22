#include "Led.h"

const int LED = 2;

void led_on() { digitalWrite(LED, HIGH); }

void led_off() { digitalWrite(LED, LOW); }
