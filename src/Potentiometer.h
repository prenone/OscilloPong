#ifndef Potentiometer_h
#define Potentiometer_h

#include <Arduino.h>

class Potentiometer
{
private:
    int vin_pin;

    int vmax = 0;
    int vmin = 4096;

public:
    Potentiometer(int, int);
    ~Potentiometer();

    double Read();
    void Calibrate();
};

#endif
