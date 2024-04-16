#pragma once
#include "Sprite.hpp"
#include <cassert>
#include <cstdint>
#include <memory>
#include <tuple>

/// General representation of a game object
class Object {
    std::shared_ptr<Sprite> _sprite;
    bool _flipped = false;
    int _z_index = 0;

    int32_t _width = 0;
    int32_t _height = 0;
    int32_t _elevation = 0;

    public:
    int32_t x = 0;
    int32_t y = 0;

    Object(std::shared_ptr<Sprite> sprite, int32_t x, int32_t y) : _sprite(sprite), x(x), y(y) {
        _width = sprite->width();
        _height = sprite->height();
    }

    /// Set elevation
    inline void setElevation(int32_t elev) {
        assert(elev >= 0);
        assert(elev <= _height);
        _elevation = elev;
    }

    /// Flip horizontally
    inline void setFlipped(bool flipped) { _flipped = flipped; }

    inline void shift(int32_t delta_x, int32_t delta_y) {
        x += delta_x;
        y += delta_y;
    }

    std::tuple<int32_t, int32_t> detectCollision(const std::vector<Object>&, int32_t delta_x,
                                                 int32_t delta_y) const;

    /// Check if flipped
    inline bool isFlipped() const { return _flipped; }

    inline void setIndex(size_t index) { _sprite->setIndex(index); }

    inline bool operator<(const Object& other) const { return this->compareDepth(other) < 0; }

    int compareDepth(const Object& other) const;

    const std::shared_ptr<Sprite> sprite() const;
};
