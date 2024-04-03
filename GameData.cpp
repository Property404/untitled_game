#include "GameData.h"
#include <memory>
#include <vector>

GameData::GameData(size_t width, size_t height) {
    this->_width = width;
    this->_height = height;
    this->_pixels = std::vector<Pixel>(width*height);
}


