#include "Game.hpp"
#include "Color.hpp"
#include "Board.hpp"
#include "Sprite.hpp"
#include <optional>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <cmath>

class GameImpl final {
    size_t _width{};
    size_t _height{};
    uint64_t _steps;

    std::optional<KeyPress> key_press;
    std::vector<Sprite> _sprites;

    Sprite _player;

    Board _board;
    
    public:
    GameImpl(size_t width, size_t height) :
        _width(width), _height(height), 
        _player(std::filesystem::path("assets/may.rgba"), 16, 32),
        _board(width, height)
    {
    }

    void set_background() {
        auto color = Color(
                    abs(cos(_steps/200.0))*255,
                    abs(cos(_steps/2000.0))*255,
                    abs(sin(_steps/200.0))*255
                    );
        _board.clear(color);
        color.red = 255-color.red;
        color.blue = color.blue;
        color.green = color.green;
    }

    void step() {
        if (key_press == KeyPress::Up) {
            _player.setIndex(1);
        } else if (key_press == KeyPress::Down) {
            _player.setIndex(0);
        } else if (key_press == KeyPress::Left) {
            _player.setIndex(2);
        } else if (key_press == KeyPress::Right) {
            _player.setIndex(2);
        }
        _board.drawSprite(_player, _width/2,_height/2);
        _steps++;
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
    return this->impl->_board.pixels();
}

void Game::step() {
    this->impl->step();
}

Game::Game(size_t width, size_t height) {
    this->impl = std::make_unique<GameImpl>(width, height);
}

Game::~Game() = default;
