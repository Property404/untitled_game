#include "Game.hpp"
#include "Color.hpp"
#include <optional>
#include <iostream>

class GameImpl final {
    size_t _width{};
    size_t _height{};

    std::optional<KeyPress> key_press;
    // This needs to be abstracted out
    std::vector<Color> _pixels{};
    
    public:
    GameImpl(size_t width, size_t height) :
        _width(width), _height(height)
    {
        _pixels = std::vector<Color>(width * height, Color(1,2,100));
    }

    friend class Game;
};

size_t Game::width() const {
    return this->impl->_width;
}

size_t Game::height() const {
    return this->impl->_height;
}

void Game::onKeyDown(KeyPress key_press) {
    this->impl->key_press = key_press;
}

void Game::onKeyUp(KeyPress key_press) {
    if (this->impl->key_press == key_press) {
        this->impl->key_press = std::optional<KeyPress>{};
    }
}

const std::vector<Color>& Game::pixels() const {
    return this->impl->_pixels;
}

void Game::step() {
    for (auto& pixel : this->impl->_pixels) {
        pixel.set_rgb(100,10,30);
    }
}

Game::Game(size_t width, size_t height) {
    this->impl = std::make_unique<GameImpl>(width, height);
}

Game::~Game() = default;
