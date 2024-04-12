#include "Game.hpp"

class GameImpl {
    size_t width{};
    size_t height{};
    public:
    GameImpl(size_t width, size_t height) :
        width(width), height(height)
    {
    }
};

Game::Game(size_t width, size_t height) {
    this->impl = std::make_unique<GameImpl>(width, height);
}
Game::~Game() = default;

