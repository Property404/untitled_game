#pragma once
#include <stdint.h>

struct Pixel final {
    uint32_t red:8 = 0;
    uint32_t green:8 = 0;
    uint32_t blue:8 = 0;
    uint32_t alpha:8 = 255;

    inline Pixel() {}

    inline Pixel(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {
    }

    inline void set_rgb(uint8_t red, uint8_t green, uint8_t blue) {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    inline void set_grayscale(uint8_t val) {
        this->red = val;
        this->green = val;
        this->blue = val;
    }
};
