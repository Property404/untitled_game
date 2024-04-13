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
    Sprite(std::filesystem::path&& path, size_t width, size_t height);

    inline size_t width() const {
        return _width;
    }

    inline size_t height() const {
        return _height;
    }

    void setIndex(size_t);

    const std::vector<Color>& pixels () const;
};
