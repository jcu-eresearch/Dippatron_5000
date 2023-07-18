#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "Arduino.h"

void setMenuStream(HardwareSerial *stream);


int printfln(const char *format, ...);
int printf(const char *format, ...);
size_t read_line(HardwareSerial &stream, char *val, size_t length);
bool read_double_t(HardwareSerial &stream, double *val);
bool read_uint16_t(HardwareSerial &stream, uint16_t *val);
bool read_int32_t(HardwareSerial &stream, int32_t *val);
bool read_int(HardwareSerial &stream, int *val);
bool read_bool_t(HardwareSerial &stream, bool *val, bool *valid);
void clearBuf();

#endif