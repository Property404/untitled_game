#include "KeyPress.hpp"
#include <optional>

std::optional<KeyPress> KeyPress::from_event(const EmscriptenKeyboardEvent* event) {
    if (!std::strcmp(event->key, "ArrowLeft")) {
        return KeyPress::Left;
    } else if (!std::strcmp(event->key, "ArrowRight")) {
        return KeyPress::Right;
    } else if (!std::strcmp(event->key, "ArrowUp")) {
        return KeyPress::Up;
    } else if (!std::strcmp(event->key, "ArrowDown")) {
        return KeyPress::Down;
    }
    return std::optional<KeyPress>{};
}
