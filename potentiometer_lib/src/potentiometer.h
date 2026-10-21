/*
    Potentiometer 
    Made by mpdoliveira and kakabahia, September 29th, 2026
    Inspired by Milene, September 22nd, 2026
*/


#ifndef Potertiometer_h
#define Potentiometer_h

#include "Arduino.h"

#define MAX_READ 1023

class Potentiometer
{
public:
    /**
     * Potentiometer constructor
     * @param pin , analogic pin to read potentiometer input
     */
    Potentiometer(uint8_t pin);

    /**
     * Measure tension read the pin in 10bits (0 to 1023).
     * @returns The tension in 10bits (0 to 1023).
    */ 
    int read();

    /**
     * Measures tension in Volts
     * @param max_tension , maximum tension in the circuit, 5V is the default
     * @returns the tension from 0 to max_tension.
     */
    float readTension(float max_tension = 5);

    /**
     * Measures tension in specific range
     * @param bottom_limit , minimum value in range, 0 is the default
     * @param upper_limit , maximum value in range, 255 is the default (8 bits)
     * @returns the tension from bottom_limit to upper_limit
     */
    float readInRange(float bottom_limit = 0, float upper_limit = 255);
    
    /**
     * Get the tension read as a paramether and then prints the result
     * @param Stream &serialPort = Serial , Gets the value of the tension and prints it, it can be any value in 0 to 5.
     */
    void readSerial();

private:
    uint8_t pin;
};

#endif //