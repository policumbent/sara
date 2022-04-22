#pragma once

#ifndef SARA_UTILS_H
#define SARA_UTILS_H

#include <iostream>
#include <Arduino.h>

template<typename T>
void get_str(T val, char buff[8]){
    dtostrf(val, 1, 2, buff);
}

void loop_infinite();


template<> void get_str<int>(int val, char buff[8]){
    String(val).toCharArray(buff, 8);
}


#endif //SARA_UTILS_H
