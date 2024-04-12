#pragma once
#include "Pixel.hpp"
#include<vector>

/// Abstract interface that represents an end point
class Terminal {
    public:
        /// Get width
        virtual size_t width() = 0;

        /// Get height
        virtual size_t height() = 0;

        /// Draw to screen
        virtual void draw(const std::vector<Pixel>& pixels) = 0;
};
