#pragma once
#include "Color.hpp"
#include "KeyPress.hpp"
#include <cstdint>
#include <memory>
#include <vector>

// Object used for PIMPL idiom
class GameImpl;

class Game final {
    std::unique_ptr<GameImpl> impl;

    public:
    Game(size_t width, size_t height);

    /// Get width
    size_t width() const;

    /// Get height
    size_t height() const;

    /// On keypress
    void onKeyDown(KeyPress keypress);

    /// On keypress
    void onKeyUp(KeyPress keypress);

    const std::vector<Color>& pixels() const;

    /// main loop
    void step();

    ~Game();
};
