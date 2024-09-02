#pragma once
#include <iostream>
#include <cstdint>

class Color
{
public:
    Color() : r(0), g(0), b(0) {}

    Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}

    uint8_t getRed() const
    {
        return r;
    }

    uint8_t getGreen() const
    {
        return g;
    }

    uint8_t getBlue() const
    {
        return b;
    }

    // Operador de suma5
    Color operator+(const Color &other) const
    {
        uint8_t red = static_cast<uint8_t>(r + other.r);
        uint8_t green = static_cast<uint8_t>(g + other.g);
        uint8_t blue = static_cast<uint8_t>(b + other.b);
        return Color(red, green, blue);
    }

    // Operador de multiplicación por float
    Color operator*(float factor) const
    {
        uint8_t red = static_cast<uint8_t>(r * factor);
        uint8_t green = static_cast<uint8_t>(g * factor);
        uint8_t blue = static_cast<uint8_t>(b * factor);
        return Color(red, green, blue);
    }

private:
    uint8_t r; // Red
    uint8_t g; // Green
    uint8_t b; // Blue
};