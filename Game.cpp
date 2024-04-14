#include "Game.hpp"
#include "Color.hpp"
#include "Board.hpp"
#include "Sprite.hpp"
#include "Object.hpp"
#include <optional>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <cmath>

class GameImpl final {
    size_t _width{};
    size_t _height{};
    uint64_t _steps = 0;

    std::optional<KeyPress> key_press{};
    std::vector<Object> _objects{};

    Object _player;

    Board _board;
    
    public:
    GameImpl(size_t width, size_t height) :
        _width(width), _height(height), 
        _player(Object::createStatic(std::make_shared<Sprite>(std::filesystem::path("assets/may.rgba"), 16, 32), _width/2, _height/2)),
        _board(width, height)
    {
        std::shared_ptr<Sprite> berry = std::make_shared<Sprite>(std::filesystem::path("assets/berry.rgba"), 48, 48);
        _objects.emplace_back(berry, 100, 100);
        for (auto i = -50; i < 5000; i++) {
            _objects.emplace_back(berry, i*50, 50);
        }
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
            _board.shift(0, -1);
        } else if (key_press == KeyPress::Down) {
            _player.setIndex(0);
            _board.shift(0, 1);
        } else if (key_press == KeyPress::Left) {
            _player.setIndex(2);
            _player.setFlipped(false);
            _board.shift(-1, 0);
        } else if (key_press == KeyPress::Right) {
            _player.setIndex(2);
            _player.setFlipped(true);
            _board.shift(1, 0);
        }
        set_background();

        for(auto& object : _objects) {
            _board.drawObject(object);
        }

        _board.drawObject(_player);
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
