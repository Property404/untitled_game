#pragma once
#include "Color.hpp"
#include "Sprite.hpp"
#include <vector>
#include <cstdint>

class Board {
    size_t _width{};
    size_t _height{};
    std::vector<Color> _pixels{};

    public:
    Board(size_t width, size_t height) : _width(width), _height(height) {
        _pixels = std::vector<Color>(width*height,Color(100, 0, 150));
    }

    void clear(Color color);

    void drawBox(size_t x, size_t y, size_t width, size_t height, Color color);

    void drawSprite(const Sprite&, size_t x, size_t y);

    const std::vector<Color>& pixels() const;

    std::vector<Color>& pixels();
};
