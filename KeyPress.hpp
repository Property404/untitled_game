#pragma once
#include <emscripten/html5.h>
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
    // Construct from event
    static std::optional<KeyPress> from_event(const EmscriptenKeyboardEvent* event);

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
