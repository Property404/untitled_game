#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <cmath>
#include <emscripten.h>
#include <emscripten/html5.h>
#include "Pixel.hpp"
#include "GameData.hpp"
#include "KeyPress.hpp"
#include "Clamped.hpp"

const size_t WIDTH=160;
const size_t HEIGHT=240;
const size_t FRAME_RATE=60;
const size_t BLOCK_SIZE=5;
const size_t PADDLE_HEIGHT = BLOCK_SIZE;
const size_t PADDLE_WIDTH = BLOCK_SIZE*5;

struct Point {
    Clamped<long> x;
    Clamped<long> y;
    long x_clamp;
    long y_clamp;

    Point() = delete;

    Point(long xx,long yy,long width, long height):x(xx,0l,width),y(yy,0l,height) {
        x_clamp = width;
        y_clamp = height;
    }

    bool hit_clamp() const {
        return x == x_clamp || y == y_clamp || x == 0 || y == 0;
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
            //pix.blue = (255-sat < pix.blue)?255:(pix.blue+sat);
            pix.blue = 0;
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

    static double ball_angle = .9;

    static Point ball(
            game_data->width()/2,
            game_data->height()/2,
            game_data->width() - BLOCK_SIZE,
            game_data->height()
    );

    static Point paddle(
            0,
            game_data->height(),
            game_data->width() - PADDLE_WIDTH,
            game_data->height()
    );

    const auto keypress = game_data->get_keypress();
    if (keypress == KeyPress::Left) {
        paddle.x--;
    } else if (keypress == KeyPress::Right) {
        paddle.x++;
    }

    ball.x += cos(ball_angle);
    ball.y += sin(ball_angle);

    if (ball.hit_clamp()) {
        ball_angle = -ball_angle;
    }

    for (size_t i=0; i < game_data->area(); i++) {
        game_data->pixels()[i].set_rgb(
                ((ball.x+paddle.x.inner()) % 255).inner(),
                ((paddle.y*255/HEIGHT) + -255).inner(),
                ((paddle.x*255/WIDTH) + -255).inner()
                );
    }

    draw_block(
            game_data,
            paddle.x.inner(),
            (paddle.y - PADDLE_HEIGHT).inner(),
            PADDLE_WIDTH,
            PADDLE_HEIGHT
    );

    draw_block(
            game_data,
            ball.x.inner(),
            ball.y.inner(),
            BLOCK_SIZE,
            BLOCK_SIZE
    );

    draw(game_data);
}

int main() {
    GameData game_data(WIDTH, HEIGHT);
    emscripten_set_keydown_callback("#canvas", &game_data, 1, keydown);
    emscripten_set_keyup_callback("#canvas", &game_data, 1, keyup);
    emscripten_set_main_loop_arg(main_loop, &game_data, FRAME_RATE, 1);
    return 0;
}

