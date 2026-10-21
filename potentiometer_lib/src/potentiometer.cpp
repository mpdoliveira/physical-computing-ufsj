/*
    Potentiometer
    Made by mpdoliveira and kakabahia, September 29th, 2026
    Inspired by Milene, September 22nd, 2026
*/

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
    // delta_range = upper_range - bottom_range
    // normalized_range = analogRead(pin) * delta_range / MAX_READ
    //
    return
    {
        (analogRead(pin) * (upper_range - bottom_range) / MAX_READ) + bottom_range;
    }
}

void Potentiometer::readSerial(Stream &serialPort = Serial)
{
    serialPort.print("Tensão do pino: ");
    serialPort.println(this->readTension());
}