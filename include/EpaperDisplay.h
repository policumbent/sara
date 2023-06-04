#pragma once

#include <Wstring.h>
#include "Data.h"
#include <GxEPD.h>
#include <GxGDEH0154D67/GxGDEH0154D67.h>  // 1.54" b/w

#include <Fonts/FreeMono9pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#define COLORED     0
#define UNCOLORED   1

class EpaperDisplay{
private:
    String to_print;
    GxIO_Class *io;

public:
    GxEPD_Class *display;

public:
    EpaperDisplay();
    void display_data(Data &, void (*)(void));
    void print_on_display(char *text, void (*draw_callback)(void));

    String get_to_print();

private:
    void construct_text(Data &data);
};