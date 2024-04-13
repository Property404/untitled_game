#pragma once
#include "Color.hpp"
#include "Sprite.hpp"
#include "Object.hpp"
#include <vector>
#include <cstdint>

class Board {
    size_t _width{};
    size_t _height{};
    int32_t _offset_x = 0;
    int32_t _offset_y = 0;
    std::vector<Color> _pixels{};

    public:
    Board(size_t width, size_t height) : _width(width), _height(height) {
        _pixels = std::vector<Color>(width*height,Color(100, 0, 150));
    }

    void clear(Color color);

    void shift(int32_t delta_x, int32_t y);

    void drawBox(size_t x, size_t y, size_t width, size_t height, Color color);

    void drawSprite(const Sprite&, size_t x, size_t y, bool flip = false);

    void drawObject(const Object&);

    const std::vector<Color>& pixels() const;

    std::vector<Color>& pixels();
};
