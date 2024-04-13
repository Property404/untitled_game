#pragma once
#include "Color.hpp"
#include "Sprite.hpp"
#include <cstdint>
#include <vector>

/// General representation of a game object
class Object {
    Sprite _sprite;
    bool _flipped = false;
    bool _static = false;
public:
    int32_t x = 0;
    int32_t y = 0;

    Object(Sprite sprite, int32_t x, int32_t y) :
        _sprite(sprite), x(x), y(y) {}

    /// Flip horizontally
    inline void flip() { _flipped = true;};

    /// Check if flipped
    inline bool isFlipped() const {return _flipped;};

    /// Check if static
    inline bool isStatic() const {return _static;};

    /// Check if static
    inline void setIndex(size_t index) {_sprite.setIndex(index);};

    const Sprite& sprite() const;
};
