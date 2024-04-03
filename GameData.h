#pragma once
#include <stddef.h>
#include <memory>
#include <vector>
#include "Pixel.h"

class GameData {
    size_t _width = 0;
    size_t _height = 0;
    std::vector<Pixel> _pixels;

public:
    GameData(size_t width, size_t height);

    inline size_t width() const {
        return this->_width;
    }

    inline size_t height() const {
        return this->_height;
    }

    inline const std::vector<Pixel>& pixels() const {
        return this->_pixels;
    }

    inline std::vector<Pixel>& pixels() {
        return this->_pixels;
    }
};
