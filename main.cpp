#include "Color.hpp"
#include "Game.hpp"
#include "KeyPress.hpp"
#include <cstdint>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include <optional>

const size_t WIDTH = 240;
const size_t HEIGHT = 160;
const size_t FRAME_RATE = 60;

std::optional<KeyPress> keypress_from_event(const EmscriptenKeyboardEvent* event) {
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

void draw(const Game* game) {
    const auto height = game->height();
    const auto width = game->width();
    const Color* data = game->pixels().data();

    EM_ASM_(
        {
            const data = Module.HEAPU8.slice($0, $0 + $1 * $2 * 4);
            const context = Module['canvas'].getContext('2d');
            const image_data = context.getImageData(0, 0, $1, $2);
            image_data.data.set(data);
            context.putImageData(image_data, 0, 0);
        },
        data, width, height);
}

static int keydown(int, const EmscriptenKeyboardEvent* event, void* ptr) {
    Game* game_data = static_cast<Game*>(ptr);
    const auto keypress = keypress_from_event(event);
    if (keypress.has_value()) {
        game_data->onKeyDown(keypress.value());
    }
    return 0;
}

static int keyup(int, const EmscriptenKeyboardEvent* event, void* ptr) {
    Game* game_data = static_cast<Game*>(ptr);
    const auto keypress = keypress_from_event(event);

    if (keypress.has_value()) {
        game_data->onKeyUp(keypress.value());
    }
    return 0;
}

void main_loop(void* ptr) {
    Game* game = static_cast<Game*>(ptr);
    game->step();
    draw(game);
}

int main() {
    static auto game_data = Game(WIDTH, HEIGHT);

    EM_ASM_(
        {
            Module.canvas = document.querySelector("#canvas");
            Module.canvas.focus();
        });

    emscripten_set_keydown_callback("#canvas", &game_data, 1, keydown);
    emscripten_set_keyup_callback("#canvas", &game_data, 1, keyup);
    emscripten_set_main_loop_arg(main_loop, &game_data, FRAME_RATE, 1);
    return 0;
}
