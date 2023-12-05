#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__
#include <REG52.H>

void init_temperature();
void show_temperature();
void clear_temperature();

void temperature_VAL_ADD();
void temperature_VAL_SUB();
void show_EXCEED_VAL();
bit TEM_exceed();
unsigned char get_temperature_VAL_TOP();
unsigned char get_temperature_VAL_LOW();
void temperature_VAL_LOW_ADD();
void temperature_VAL_LOW_SUB();
void clearLCD_EXCEED_VAL();
void show_set_temperature();

#endif
