#pragma once
#include <cstdint>
#include <memory>
#include "Terminal.hpp"

// Object used for PIMPL idiom
class GameImpl;

class Game final {
    std::unique_ptr<GameImpl> impl;

    public:
    Game(size_t width, size_t height);
    ~Game();
};
