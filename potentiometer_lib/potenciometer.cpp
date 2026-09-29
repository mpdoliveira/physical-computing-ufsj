#include <Arduino.h>
#include "potentiometer.h"

Potentiometer::Potentiometer(uint8_t pin)
{
    this->pin = pin;

    pinMode(pin, INPUT);
}

int Potentiometer::read()
{
    return analogRead(pin);
}

float Potentiometer::readTension(float max_tension)
{
    return analogRead(pin) * max_tension / MAX_READ;
}

float Potentiometer::readInRange(float bottom_range, float upper_range)
{

    return {
        (analogRead(pin) * (upper_range - bottom_range) / MAX_READ)
        + bottom_range;
    }
}