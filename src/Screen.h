#ifndef Screen_h
#define Screen_h

#include <Arduino.h>
#include "Potentiometer.h"

class Screen
{
private:
    int x_pin;
    int y_pin;
    int z_pin;

    int size;
    int width;
    int height;

    int z;

    void _DrawHorizontalLine(double y, double x1, double x2);
    void _DrawVerticalLine(double x, double y1, double y2);
public:
    Screen(int x_pin, int y_pin, int z_pin, int width, int height);
    ~Screen();

    void ShowCursor();
    void HideCursor();
    void MoveCursor(double x, double y);

    double MaxWidth();
    double MaxHeight();

    void DrawVerticalLine(double x, double y1, double y2);
    void DrawRectangle(double x1, double y1, double x2, double y2);
    void DrawCircle(double cx, double cy, double r);
};

#endif
