#include "EpaperDisplay.h"

unsigned char image[5760];

Epd display;
Paint paint(image, 256, 180);
char buffer[8];


void init_display(){

    if(display.Init(lut_full_update) != 0){
        Serial.println("E-PAPER INIT FAILED");
        while(1);
    }else {
        Serial.println("E-PAPER CORRECTLY INITIALIZED");
    }

    /**
     *  there are 2 memory areas embedded in the e-paper display
     *  and once the display is refreshed, the memory area will be auto-toggled,
     *  i.e. the next action of SetFrameMemory will set the other memory area
     *  therefore you have to clear the frame memory twice.
     */
    display.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    display.DisplayFrame();
    display.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    display.DisplayFrame();

    paint.SetRotate(ROTATE_0);
    paint.Clear(COLORED);
    paint.DrawStringAt(20, 4, "Team Policumbent", &Font16, UNCOLORED);
    paint.DrawStringAt(30, 44, "Temperature: 0.0", &Font16, UNCOLORED);
    paint.DrawStringAt(30, 74, "Pressure: 0.0", &Font16, UNCOLORED);
    paint.DrawStringAt(30, 104, "Humidity: 0.0", &Font16, UNCOLORED);
    paint.DrawStringAt(30, 134, "Wind Speed: 0.0", &Font16, UNCOLORED);
    paint.DrawStringAt(30, 164, "Wind Direction: 0.0", &Font16, UNCOLORED);

    display.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());
    display.DisplayFrame();
}

void display_data(Data &data){

    display.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    display.DisplayFrame();
    display.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    display.DisplayFrame();

    paint.Clear(COLORED);
    paint.DrawStringAt(20, 4, "Team Policumbent", &Font16, UNCOLORED);

    paint.DrawStringAt(30, 44, "Temperature: ", &Font16, UNCOLORED);
    dtostrf(data.temperature, 1, 2, buffer);
    paint.DrawStringAt(70, 44, buffer, &Font16, UNCOLORED);

    paint.DrawStringAt(30, 74, "Pressure: ", &Font16, UNCOLORED);
    dtostrf(data.pressure, 1, 2, buffer);
    paint.DrawStringAt(70, 44, buffer, &Font16, UNCOLORED);

    paint.DrawStringAt(30, 104, "Humidity: ", &Font16, UNCOLORED);
    dtostrf(data.humidity, 1, 2, buffer);
    paint.DrawStringAt(70, 44, buffer, &Font16, UNCOLORED);

    paint.DrawStringAt(30, 134, "Wind Speed: ", &Font16, UNCOLORED);
    dtostrf(data.wind_speed, 1, 2, buffer);
    paint.DrawStringAt(70, 44, buffer, &Font16, UNCOLORED);

    paint.DrawStringAt(30, 164, "Wind Direction: ", &Font16, UNCOLORED);
    dtostrf(data.wind_direction, 1, 2, buffer);
    paint.DrawStringAt(70, 44, buffer, &Font16, UNCOLORED);

    display.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());
    display.DisplayFrame();
}