#ifndef Potertiometer_h
#define Potentiometer_h

#include "Arduino.h"

class Potentiometer
{
public:
    Potentiometer(unit8_t pin);
    int read();
    float readNormalized();
    float readNormalizedRange(int bottom_limit, int upper_limit);

    privaet : unit8_t _pin;
}