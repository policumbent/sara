#pragma once

#include <Arduino.h>

void get_str(double val, char buff[8]);
void get_str(int val, char buff[8]);
void uint_to_bytes(uint8_t *payload, unsigned int var, unsigned int len);
void bytes_to_uint(uint8_t *payload, unsigned int &var, unsigned int len);
void bytes_to_string(uint8_t *payload, char *message, unsigned int len);

void loop_infinite();