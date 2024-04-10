#include "GameData.hpp"
#include <memory>
#include <vector>
#include <optional>

GameData::GameData(size_t width, size_t height) {
    this->_width = width;
    this->_height = height;
    this->_pixels = std::vector<Pixel>(width*height);
}

std::optional<KeyPress> GameData::get_keypress() const {
    return this->_keypress;
}

void GameData::set_keypress(KeyPress keypress) {
    this->_keypress = keypress;
}

std::optional<KeyPress> GameData::take_keypress() {
    std::optional<KeyPress> swapped{};
    std::swap(swapped, this->_keypress);
    return swapped;
}


