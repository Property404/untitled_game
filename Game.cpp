#include "Game.hpp"
#include "Board.hpp"
#include "Color.hpp"
#include "Object.hpp"
#include "Sprite.hpp"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <optional>

class GameImpl final {
    size_t _width{};
    size_t _height{};
    uint64_t _steps = 0;

    std::optional<KeyPress> key_press{};
    std::vector<Object> _objects{};

    Object _player;

    Board _board;

    public:
    GameImpl(size_t width, size_t height)
        : _width(width), _height(height),
          _player(Object(std::make_shared<Sprite>(std::filesystem::path("assets/may.rgba"), 16, 24),
                         _width / 2, _height / 2)),
          _board(width, height) {
        std::shared_ptr<Sprite> tamato =
            std::make_shared<Sprite>(std::filesystem::path("assets/tamato.rgba"), 16, 26);
        std::shared_ptr<Sprite> persim =
            std::make_shared<Sprite>(std::filesystem::path("assets/persim.rgba"), 16, 26);
        tamato->setIndex(5);
        persim->setIndex(5);
        std::srand(std::time(nullptr));
        for (auto i = 0; i < 500; i++) {
            {
                Object object(tamato, i * 20, 0);
                object.setElevation(20);
                object.setShrinkness(4);
                _objects.push_back(object);
            }
            {
                Object object(tamato, i * 20, 180);
                object.setElevation(20);
                object.setShrinkness(4);
                _objects.push_back(object);
            }
            {
                Object object(tamato, 0, i * 20);
                object.setElevation(20);
                object.setShrinkness(4);
                _objects.push_back(object);
            }
        }
        std::sort(_objects.begin(), _objects.end());
        _player.setElevation(16);
        _player.setShrinkness(4);
    }

    void set_background() {
        auto color = Color(std::abs(std::cos(_steps / 200.0)) * 255,
                           std::abs(std::cos(_steps / 2000.0)) * 255,
                           std::abs(std::sin(_steps / 200.0)) * 255);
        _board.clear(color);
        color.red = 255 - color.red;
        color.blue = color.blue;
        color.green = color.green;
    }

    void step() {
        auto delta_x = 0, delta_y = 0;
        if (key_press == KeyPress::Up) {
            _player.setIndex((_steps / 10) % 2 ? 5 : 6);
            delta_y = -1;
        } else if (key_press == KeyPress::Down) {
            _player.setIndex((_steps / 10) % 2 ? 3 : 4);
            delta_y = 1;
        } else if (key_press == KeyPress::Left) {
            _player.setIndex((_steps / 10) % 2 ? 2 : 7);
            _player.setFlipped(false);
            delta_x = -1;
        } else if (key_press == KeyPress::Right) {
            _player.setIndex((_steps / 10) % 2 ? 2 : 8);
            _player.setFlipped(true);
            delta_x = 1;
        }
        if (delta_x || delta_y) {
            const auto shift = _player.detectCollision(_objects, delta_x, delta_y);
            _board.shift(std::get<0>(shift), std::get<1>(shift));
            _player.shift(std::get<0>(shift), std::get<1>(shift));
        }
        set_background();

        bool drew_player = false;
        for (const auto& object : _objects) {
            if (!drew_player && _player.compareDepth(object) < 0) {
                _board.drawObject(_player);
                drew_player = true;
            }
            _board.drawObject(object);
        }

        if (!drew_player) {
            _board.drawObject(_player);
        }

        _steps++;
    }

    friend class Game;
};

size_t Game::width() const { return this->impl->_width; }

size_t Game::height() const { return this->impl->_height; }

void Game::onKeyDown(KeyPress key_press) { this->impl->key_press = key_press; }

void Game::onKeyUp(KeyPress key_press) {
    if (this->impl->key_press == key_press) {
        this->impl->key_press = std::optional<KeyPress>{};
    }
}

const std::vector<Color>& Game::pixels() const { return this->impl->_board.pixels(); }

void Game::step() { this->impl->step(); }

Game::Game(size_t width, size_t height) { this->impl = std::make_unique<GameImpl>(width, height); }

Game::~Game() = default;
