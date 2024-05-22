#include "Potentiometer.h"

Potentiometer::Potentiometer(int vsource_pin, int vin_pin)
{
    this->vin_pin = vin_pin;

    pinMode(vin_pin, INPUT);
}

Potentiometer::~Potentiometer()
{
}

void Potentiometer::Calibrate()
{
    int start = millis();

    while (millis() - start < 5000)
    {
        int val = analogRead(vin_pin);

        vmax = max(val, vmax);
        vmin = min(val, vmin);
    }
}

double Potentiometer::Read()
{
    int val = analogRead(vin_pin);
    return (double)(val - vmin) / (vmax - vmin);
}
