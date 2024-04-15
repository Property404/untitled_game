#include "Object.hpp"

const std::shared_ptr<Sprite> Object::sprite() const { return this->_sprite; }

Object Object::createStatic(std::shared_ptr<Sprite> sprite, int32_t x, int32_t y) {
    auto object = Object(sprite, x, y);
    object._static = true;
    return object;
}

int Object::compareDepth(const Object& other, int32_t board_y) const {
    if (_z_index == other._z_index) {
        const auto base_y = realY(board_y) + static_cast<int32_t>(_sprite->height());
        const auto base_other_y =
            other.realY(board_y) + static_cast<int32_t>(other.sprite()->height());
        if (base_y > base_other_y) {
            return 1;
        } else if (base_y < base_other_y) {
            return -1;
        } else {
            return 0;
        }
    } else if (_z_index < other._z_index) {
        return -1;
    } else {
        return 1;
    }
}
