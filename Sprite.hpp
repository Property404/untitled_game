#pragma once
#include "Color.hpp"
#include <cassert>
#include <vector>
#include <filesystem>

class Sprite {
    size_t _width{};
    size_t _height{};
    size_t _index = 0;
    std::vector<std::vector<Color>> _pixmap{};
public:
    Sprite(size_t width, size_t height, std::vector<Color> pixels) :
        _width(width),
        _height(height)
    {
        _pixmap.push_back(pixels);
    }

    /// Construct sprite from bitmap
    static Sprite fromBitmap(std::filesystem::path&& path);

    inline size_t width() const {
        return _width;
    }

    inline size_t height() const {
        return _height;
    }

    const std::vector<Color>& pixels () const;
};
