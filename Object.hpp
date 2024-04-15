#pragma once
#include "Sprite.hpp"
#include <cstdint>
#include <memory>

/// General representation of a game object
class Object {
    std::shared_ptr<Sprite> _sprite;
    bool _flipped = false;
    bool _static = false;
    int _z_index = 0;

    public:
    int32_t x = 0;
    int32_t y = 0;

    Object(std::shared_ptr<Sprite> sprite, int32_t x, int32_t y) : _sprite(sprite), x(x), y(y) {}

    static Object createStatic(std::shared_ptr<Sprite> sprite, int32_t x, int32_t y);

    /// Flip horizontally
    inline void setFlipped(bool flipped) { _flipped = flipped; }

    /// Check if flipped
    inline bool isFlipped() const { return _flipped; }

    /// Returns absolute position given board X
    inline int32_t realX(int32_t board_x) const { return _static ? x + board_x : x; }

    inline int32_t realY(int32_t board_y) const { return _static ? y + board_y : y; }

    /// Check if static
    inline bool isStatic() const { return _static; }

    /// Check if static
    inline void setIndex(size_t index) { _sprite->setIndex(index); }

    inline bool operator<(const Object& other) const { return this->compareDepth(other, 0) < 0; }

    int compareDepth(const Object& other, int32_t board_y) const;

    const std::shared_ptr<Sprite> sprite() const;
};
