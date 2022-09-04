#pragma once

#include "Data.h"
#include <SPI.h>
#include <Arduino.h>
#include "epd1in54.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

void init_display();
void display_data(Data &);