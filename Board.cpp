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

void Board::drawSprite(const Sprite& sprite, size_t x, size_t y) {
    for (size_t row = y; row < y + sprite.height(); row++) {
        for (size_t col = x; col < x + sprite.width(); col++) {
            // Can optimize here
            const auto spix = sprite.pixels().at((row-y) * sprite.width() + (col-x));
            if (spix.alpha > 0) {
                this->_pixels.at(row*_width + col).set_rgb(spix.red, spix.green, spix.blue);
            }
        }
    }
}

const std::vector<Color>& Board::pixels() const {
    return this->_pixels;
}

std::vector<Color>& Board::pixels() {
    return this->_pixels;
}
