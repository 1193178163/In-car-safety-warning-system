#ifndef _DS18B20_H_
#define _DS18B20_H_

#include <main.h>
sbit DQ=P3^7;

float ReadTemp(void);

#endif