#include "Object.hpp"
#include <iostream>

const std::shared_ptr<Sprite> Object::sprite() const { return this->_sprite; }

int Object::compareDepth(const Object& other) const {
    if (_z_index == other._z_index) {
        const auto base_y = y + static_cast<int32_t>(_sprite->height());
        const auto base_other_y = other.y + static_cast<int32_t>(other.sprite()->height());
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

std::tuple<int32_t, int32_t> Object::detectCollision(const std::vector<Object>& objects,
                                                     int32_t delta_x, int32_t delta_y) const {
    const auto self_low = y + _elevation;
    const auto self_high = y + _height;
    for (const auto& object : objects) {
        const auto obj_low = object.y + object._elevation;
        const auto obj_high = object.y + object._height;

        if (self_high > obj_low && self_low < obj_high) {
            if (delta_x > 0 && object.x >= x + _width) {
                delta_x = std::min(delta_x, object.x - x - _width);
            } else if (delta_x < 0 && object.x + object._width <= x) {
                delta_x = std::max(delta_x, object.x + object._width - x);
            }
        }

        if (x + _width > object.x && x < (object.x + object._width)) {
            if (delta_y > 0 && object.y + object._elevation >= y + _height) {
                delta_y = std::min(delta_y, object.y + object._elevation - y - _height);
            } else if (delta_y < 0 && object.y + object._height <= y + _elevation) {
                delta_y = std::max(delta_y, object.y + object._height - y - _elevation);
            }
        }

        if (delta_x == 0 && delta_y == 0) {
            break;
        }
    }
    return std::tuple<int32_t, int32_t>(delta_x, delta_y);
}
