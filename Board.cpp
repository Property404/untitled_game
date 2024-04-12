#include <cassert>
#include "Board.hpp"

void Board::clear(Color color) {
    for(auto& pixel : _pixels) {
        pixel = color;
    }
}

void Board::drawBox(size_t x, size_t y, size_t width, size_t height, Color color) {
    assert(x + width <= this->_width);
    assert(y + height <= this->_height);
    for (size_t row = y; row < y + width; row++) {
        for (size_t col = x; col < x + width; col++) {
            // This is hot so we might want to optimize this with []
            this->_pixels.at(row*_width + col) = color;
        }
    }
}

const std::vector<Color>& Board::pixels() const {
    return this->_pixels;
}

std::vector<Color>& Board::pixels() {
    return this->_pixels;
}
