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
    for (const auto& object : objects) {
        if (y + height >= object.y && y <= (object.y + object.height)) {
            if (delta_x > 0 && object.x >= x + width) {
                delta_x = std::min(delta_x, object.x - x - width);
            } else if (delta_x < 0 && object.x + object.width <= x) {
                delta_x = std::max(delta_x, object.x + object.width - x);
            }
        }

        if (x + width >= object.x && x <= (object.x + object.width)) {
            if (delta_y > 0 && object.y >= y + height) {
                delta_y = std::min(delta_y, object.y - y - height);
            } else if (delta_y < 0 && object.y + object.height <= y) {
                delta_y = std::max(delta_y, object.y + object.height - y);
            }
        }

        if (delta_x == 0 && delta_y == 0) {
            break;
        }
    }
    return std::tuple<int32_t, int32_t>(delta_x, delta_y);
}
