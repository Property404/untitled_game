#include "Object.hpp"

const Sprite& Object::sprite() const {
    return this->_sprite;
}

Object Object::createStatic(Sprite sprite, int32_t x, int32_t y) {
    auto object = Object(sprite, x, y);
    object._static = true;
    return object;
}
