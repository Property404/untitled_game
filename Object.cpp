#include "Object.hpp"

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
