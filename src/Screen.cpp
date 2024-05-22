#include "Screen.h"

Screen::Screen(int x_pin, int y_pin, int z_pin, int width, int height)
{
    this->width = width;
    this->height = height;
    this->size = max(width, height);

    this->x_pin = x_pin;
    this->y_pin = y_pin;
    this->z_pin = z_pin;

    pinMode(x_pin, OUTPUT);
    pinMode(y_pin, OUTPUT);
    pinMode(z_pin, OUTPUT);

    this->HideCursor();
}

Screen::~Screen()
{
    // Destructor code if needed
}

void Screen::ShowCursor()
{
    digitalWrite(z_pin, HIGH);
    z = 1;
}

void Screen::HideCursor()
{
    digitalWrite(z_pin, LOW);
    z = 0;
}

void Screen::MoveCursor(double x, double y)
{
    analogWrite(x_pin, x * size);
    analogWrite(y_pin, y * size);

    // Serial.println(String(x * size) + "," + String(y * size) + "," + String(z));
}

double Screen::MaxWidth() { return width / size; }
double Screen::MaxHeight() { return height / size; }

void Screen::_DrawHorizontalLine(double y, double x1, double x2)
{
    int step = (x2 - x1) / 10;
    MoveCursor(x1, y);

    for (int i = 0; i < 10; i++)
    {
        MoveCursor(x1 + step * i, y);
    }

    MoveCursor(x2, y);
}

void Screen::_DrawVerticalLine(double x, double y1, double y2)
{
    int step = (y2 - y1) / 10;
    MoveCursor(x, y1);

    for (int i = 0; i < 10; i++)
    {
        MoveCursor(x, y1 + step * i);
    }


    MoveCursor(x, y2);
}


void Screen::DrawRectangle(double x1, double y1, double x2, double y2)
{
    HideCursor();
    MoveCursor(x1, y1);
    ShowCursor();

    _DrawHorizontalLine(y1, x1, x2);
    _DrawVerticalLine(x2, y1, y2);
    _DrawHorizontalLine(y2, x2, x1);
    _DrawVerticalLine(x1, y2, y1);

    HideCursor();
}

void Screen::DrawCircle(double cx, double cy, double r)
{
    HideCursor();
    double angle_step = 2 * M_PI / 64;

    const double cos_step = cos(angle_step);
    const double sin_step = sin(angle_step);

    double x = r;
    double y = 0.0;

    MoveCursor(cx + r, cy);
    ShowCursor();

    for (int i = 0; i < 64; i++)
    {
        double new_x = x * cos_step - y * sin_step;
        double new_y = x * sin_step + y * cos_step;
        x = new_x;
        y = new_y;

        MoveCursor(cx + x, cy + y);
    }

    MoveCursor(cx + r, cy);
    HideCursor();
}

void Screen::DrawVerticalLine(double x, double y1, double y2)
{
    HideCursor();
    MoveCursor(x, y1);
    ShowCursor();
    MoveCursor(x, y1);
    MoveCursor(x, y2);
    HideCursor();
}