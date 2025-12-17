#pragma once
#include <Arduino.h>

extern const byte PinTrig;
extern const byte PinEcho;

void initUltrasonic();
long citesteDistantaCM();
