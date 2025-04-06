#ifndef __TSL2591_H__
#define __TSL2591_H__

#include "Adafruit_TSL2591.h"

void configureSensor(Adafruit_TSL2591& tsl);
void displaySensorDetails(Adafruit_TSL2591& tsl);
float advancedRead(Adafruit_TSL2591& tsl);
#endif
