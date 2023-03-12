#include "EpaperDisplay.h"

void EpaperDisplay::construct_text(Data &data){
    this->to_print = "";
    char timestamp[80];

    sprintf(timestamp, "%02d/%02d/%02d %02d:%02d:%02d",
            data.timestamp.year(),data.timestamp.month(), data.timestamp.day(),
            data.timestamp.hour(), data.timestamp.minute(), data.timestamp.second());

    this->to_print =
               "Temp: " + String(data.temperature, 0) + " °C" +
               "\nHumidity: " + String(data.humidity, 0) + " %" +
               "\nPressure: " + String(data.pressure, 0) + " hPa" +
               "\nW-speed: " + String(data.wind_speed, 2) + " m/s" +
               "\nW-dir: " + String(data.wind_direction, 0) + " °";

}

EpaperDisplay::EpaperDisplay() {
    this->io = new GxIO_Class(SPI, /*CS=5*/ 26, /*DC=*/ 25, /*RST=*/ 33);
    this->display = new GxEPD_Class(*this->io, /*RST=*/ 33, /*BUSY=*/ 27);

    this->display->init(); // enable diagnostic output on Serial
}

void EpaperDisplay::display_data(Data &data, void (*draw_callback)(void)) {
    this->construct_text(data);
    this->display->drawPaged(draw_callback);
}

void EpaperDisplay::print_on_display(String to_print, void (*draw_callback)(void)) {
    this->display->eraseDisplay();
    this->to_print = to_print;
    this->display->drawPaged(draw_callback);
}

String EpaperDisplay::get_to_print() {
    return this->to_print;
}

