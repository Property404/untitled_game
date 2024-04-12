#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <optional>
#include <emscripten.h>
#include <emscripten/html5.h>
#include "Pixel.hpp"
#include "Terminal.hpp"
#include "KeyPress.hpp"
#include "Clamped.hpp"
#include "Game.hpp"

const size_t WIDTH=160;
const size_t HEIGHT=240;
const size_t FRAME_RATE=60;

class WebTerminal: public Terminal {
    private:
        size_t _width{};
        size_t _height{};
        std::optional<KeyPress> _key_press{};
    public:
        WebTerminal(size_t width, size_t height) : _width(width), _height(height) {
        }

        void setKeyPress(KeyPress value) {
            this->_key_press = value;
        }

        std::optional<KeyPress> getKeyPress() const {
            return this->_key_press;
        }

        std::optional<KeyPress> takeKeyPress() {
            std::optional<KeyPress> swapped{};
            swapped.swap(_key_press);
            return swapped;
        }

        /// Get width
        virtual size_t width() override {
            return _width;
        }

        /// Get height
        virtual size_t height() override {
            return _height;
        }

        virtual void draw(const std::vector<Pixel>& pixels) override {
            const Pixel* data = pixels.data();

            EM_ASM_({
              const data = Module.HEAPU8.slice($0, $0 + $1 * $2 * 4);
              const context = Module['canvas'].getContext('2d');
              const image_data = context.getImageData(0, 0, $1, $2);
              image_data.data.set(data);
              context.putImageData(image_data, 0, 0);
            }, data, _width, _height);
        }
};

static int keydown(int, const EmscriptenKeyboardEvent* event, void* ptr) {
    WebTerminal* game_data = static_cast<WebTerminal*>(ptr);
    const auto keypress = KeyPress::from_event(event);
    if (keypress.has_value()) {
        game_data->setKeyPress(keypress.value());
    }
    return 0;
}

static int keyup(int, const EmscriptenKeyboardEvent* event, void* ptr) {
    WebTerminal* game_data = static_cast<WebTerminal*>(ptr);
    const auto keypress = KeyPress::from_event(event);

    if (keypress == game_data->getKeyPress()) {
        game_data->takeKeyPress();
    }
    return 0;
}


void main_loop(void* ptr) {
}

int main() {
    WebTerminal game_data(WIDTH, HEIGHT);
    emscripten_set_keydown_callback("#canvas", &game_data, 1, keydown);
    emscripten_set_keyup_callback("#canvas", &game_data, 1, keyup);
    emscripten_set_main_loop_arg(main_loop, &game_data, FRAME_RATE, 1);
    return 0;
}

