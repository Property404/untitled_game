#include <cassert>
#include <iostream>
#include "Board.hpp"

void Board::clear(Color color) {
    for(auto& pixel : _pixels) {
        pixel = color;
    }
}

void Board::drawBox(size_t x, size_t y, size_t width, size_t height, Color color) {
    assert(x + width <= this->_width);
    assert(y + height <= this->_height);
    for (size_t row = y; row < y + height; row++) {
        for (size_t col = x; col < x + width; col++) {
            // This is hot so we might want to optimize this with []
            this->_pixels.at(row*_width + col) = color;
        }
    }
}

void Board::drawSprite(const Sprite& sprite, size_t x, size_t y, bool flip) {
    for (size_t row = y; row < y + sprite.height(); row++) {
        for (size_t col = x; col < x + sprite.width(); col++) {
            // Can optimize here
            const auto col_local = flip ? (sprite.width() + x - col - 1) : col - x;
            const auto spix = sprite.pixels().at((row-y) * sprite.width() + (col_local));
            if (spix.alpha > 0) {
                this->_pixels.at(row*_width + col).set_rgb(spix.red, spix.green, spix.blue);
            }
        }
    }
}

void Board::drawObject(const Object& object) {
    auto x = object.x;
    auto y = object.y;

    if (!object.isStatic()) {
        x -= _offset_x;
        y -= _offset_y;
    }

    drawSprite(object.sprite(), x, y, object.isFlipped());
}

const std::vector<Color>& Board::pixels() const {
    return this->_pixels;
}

std::vector<Color>& Board::pixels() {
    return this->_pixels;
}

void Board::shift(int32_t delta_x, int32_t delta_y) {
    _offset_x += delta_x;
    _offset_y += delta_y;
}
