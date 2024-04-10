#include <stdio.h>
#include <stdint.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include "Pixel.hpp"
#include "GameData.hpp"
#include "KeyPress.hpp"

const size_t WIDTH=160;
const size_t HEIGHT=240;
const size_t FRAME_RATE=60;
const size_t BLOCK_SIZE=5;
const size_t PADDLE_HEIGHT = BLOCK_SIZE;
const size_t PADDLE_WIDTH = BLOCK_SIZE*5;

struct Clamped {
    size_t _x=0;
    size_t _y=0;
    size_t _width=0;
    size_t _height=0;

    public:
    Clamped(size_t x, size_t y, size_t width, size_t height):
        _x(x),_y(y),_width(width),_height(height){
            if(x >= width || y >= height) {
                throw std::runtime_error("Clamped incorrect parameters");
            }

            if(width == 0 || height == 0) {
                throw std::runtime_error("Clamped incorrect parameters");
            }
    }

    size_t x() const {
        return this->_x;
    }

    void set_x(ssize_t val) {
        if (val < 0) {
            this->_x = 0;
        } else if (static_cast<size_t>(val) >= this->_width)  {
            this->_x = this->_width - 1;
        } else {
            this->_x = val;
        }
    }

    void inc_x(ssize_t val) {
        this->set_x(_x + val);
    }

    size_t y() const {
        return this->_y;
    }

    void set_y(ssize_t val) {
        if (val < 0) {
            this->_y = 0;
        } else if (static_cast<size_t>(val) >= this->_height)  {
            this->_y = this->_height - 1;
        } else {
            this->_y = val;
        }
    }

    void inc_y(ssize_t val) {
        this->set_y(_y + val);
    }
};

void draw(GameData* game_data) {
    const size_t width = game_data->width();
    const size_t height = game_data->height();
    const Pixel* data = game_data->pixels().data();

    EM_ASM_({
      const data = Module.HEAPU8.slice($0, $0 + $1 * $2 * 4);
      const context = Module['canvas'].getContext('2d');
      const image_data = context.getImageData(0, 0, $1, $2);
      image_data.data.set(data);
      context.putImageData(image_data, 0, 0);
    }, data, width, height);
}

void draw_block(GameData* game_data, size_t x, size_t y, size_t width, size_t height) {
    for (size_t row = y; row < y + height; row++){
        for (size_t col = x; col < x + width; col++){
            const size_t index = row * game_data->width() + col % game_data->height();
            auto& pix = game_data->pixels()[index];
            auto sat = 150;
            pix.blue = (255-sat < pix.blue)?255:(pix.blue+sat);
            pix.red = (255-sat < pix.red)?255:(pix.red+sat);
            pix.green = (255-sat < pix.green)?255:(pix.green+sat);
        }
    }
}

static int keydown(int, const EmscriptenKeyboardEvent* event, void* ptr) {
    GameData* game_data = static_cast<GameData*>(ptr);
    const auto keypress = KeyPress::from_event(event);
    if (keypress.has_value()) {
        game_data->set_keypress(keypress.value());
    }
    return 0;
}

static int keyup(int, const EmscriptenKeyboardEvent* event, void* ptr) {
    GameData* game_data = static_cast<GameData*>(ptr);
    const auto keypress = KeyPress::from_event(event);

    if (keypress == game_data->get_keypress()) {
        game_data->take_keypress();
    }
    return 0;
}


void main_loop(void* ptr) {
    GameData* game_data = static_cast<GameData*>(ptr);

    static int x = game_data->width()/2 - PADDLE_WIDTH/2;
    static int y = game_data->height() - 1;
    static Clamped paddle(x, y, game_data->width(), game_data->height());

    const auto keypress = game_data->get_keypress();
    if (keypress == KeyPress::Left) {
        paddle.inc_x(-1);
    } else if (keypress == KeyPress::Right) {
        paddle.inc_x(1);
    }

    for (size_t i=0; i < game_data->area(); i++) {
        game_data->pixels()[i].set_rgb((x*255/WIDTH), 255-(y*255/HEIGHT),255-(x*255/WIDTH));
    }

    draw_block(game_data, x, y % game_data->height() - PADDLE_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT);

    draw(game_data);
}

int main() {
    GameData game_data(WIDTH, HEIGHT);
    emscripten_set_keydown_callback("#canvas", &game_data, 1, keydown);
    emscripten_set_keyup_callback("#canvas", &game_data, 1, keyup);
    emscripten_set_main_loop_arg(main_loop, &game_data, FRAME_RATE, 1);
    return 0;
}

