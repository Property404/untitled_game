#pragma once
#include <optional>

class KeyPress {
    public:
    enum Value {
        Up,
        Down,
        Left,
        Right,
    };

    constexpr KeyPress(Value value) : _value(value) {}

    explicit operator bool() const = delete;

    constexpr bool operator==(KeyPress kp) const {
        return this->_value == kp._value;
    }

    constexpr bool operator!=(KeyPress kp) const {
        return this->_value != kp._value;
    }

    private:
    Value _value;
};
