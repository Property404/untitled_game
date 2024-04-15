#pragma once
#include "Color.hpp"
#include "Object.hpp"
#include "Sprite.hpp"
#include <cstdint>
#include <vector>

class Board {
    size_t _width{};
    size_t _height{};
    int32_t _offset_x = 0;
    int32_t _offset_y = 0;
    std::vector<Color> _pixels{};

    public:
    Board(size_t width, size_t height) : _width(width), _height(height) {
        _pixels = std::vector<Color>(width * height, Color(100, 0, 150));
    }

    void clear(Color color);

    void shift(int32_t delta_x, int32_t y);

    void drawSprite(const Sprite&, int32_t x, int32_t y, bool flip = false);

    void drawObject(const Object&);

    inline int32_t offsetX() const { return _offset_x; }

    inline int32_t offsetY() const { return _offset_y; }

    const std::vector<Color>& pixels() const;

    std::vector<Color>& pixels();
};
