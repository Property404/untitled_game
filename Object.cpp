#include "Object.hpp"

const std::shared_ptr<Sprite> Object::sprite() const {
    return this->_sprite;
}

Object Object::createStatic(std::shared_ptr<Sprite> sprite, int32_t x, int32_t y) {
    auto object = Object(sprite, x, y);
    object._static = true;
    return object;
}
