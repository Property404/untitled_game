#include "Board.hpp"
#include <cassert>

void Board::clear(Color color) {
    for (auto& pixel : _pixels) {
        pixel = color;
    }
}

void Board::drawSprite(const Sprite& sprite, int32_t x, int32_t y, bool flip) {
    const int32_t sprite_width = sprite.width();
    const int32_t sprite_height = sprite.height();
    for (int32_t row = y; row < y + sprite_height; row++) {
        if (row < 0 || row >= static_cast<int32_t>(_height)) {
            continue;
        }
        for (int32_t col = x; col < x + sprite_width; col++) {
            const auto col_local = flip ? (sprite_width + x - col - 1) : col - x;
            const auto spix = sprite.pixels().at((row - y) * sprite_width + (col_local));
            if (col < 0 || col >= static_cast<int32_t>(_width)) {
                continue;
            }
            // Can optimize here
            if (spix.alpha == 0) {
                this->_pixels.at(row * _width + col).set_rgb(0, 0, 0);
                continue;
            }
            const auto index = row * _width + col;
            assert(index < _width * _height);
            assert(index < _pixels.size());
            assert(_width*_height == _pixels.size());
            this->_pixels.at(index).set_rgb(spix.red, spix.green, spix.blue);
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
    const auto sprite = object.sprite();
    const auto sprite_width = static_cast<int32_t>(sprite->width());
    const auto sprite_height = static_cast<int32_t>(sprite->height());

    // Don't draw if we're out of view, because we loop over every pixels
    if (x + sprite_width >= 0 && x < static_cast<int32_t>(_width)) {
        if (y + sprite_height >= 0 && y < static_cast<int32_t>(_height)) {
            drawSprite(*sprite, x, y, object.isFlipped());
        }
    }
}

const std::vector<Color>& Board::pixels() const { return this->_pixels; }

std::vector<Color>& Board::pixels() { return this->_pixels; }

void Board::shift(int32_t delta_x, int32_t delta_y) {
    _offset_x += delta_x;
    _offset_y += delta_y;
}
