#include <stdio.h>
#include <stdint.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include "Pixel.h"
#include "GameData.h"

const size_t WIDTH=160;
const size_t HEIGHT=240;
const size_t FRAME_RATE=60;
const size_t BLOCK_SIZE=5;

extern void weenus();
struct Pixel* allocate_pixels() {
    return new Pixel[WIDTH*HEIGHT];
}

void draw(struct Pixel* ptr, int width, int height) {
  EM_ASM_({
      const data = Module.HEAPU8.slice($0, $0 + $1 * $2 * 4);
      const context = Module['canvas'].getContext('2d');
      const image_data = context.getImageData(0, 0, $1, $2);
      image_data.data.set(data);
      context.putImageData(image_data, 0, 0);
      console.log("Put");
    }, ptr, width, height);
}

void draw_block(struct Pixel* ptr, size_t x, size_t y, size_t width, size_t height) {
    for (size_t row = y; row < y + height; row++){
        for (size_t col = x; col < x + width; col++){
            const size_t index = row * WIDTH + col % HEIGHT;
            ptr[index].set_grayscale(255);
        }
    }
}

static int keydown(int, const struct EmscriptenKeyboardEvent*, void*) {
  EM_ASM_({
      console.log("Down");
    });
  return 0;
}


void main_loop(void* pptr) {
    static int iteration = 0;
    iteration++;
    struct Pixel* pixels = (struct Pixel*)pptr;
    for (size_t i=0; i < WIDTH * HEIGHT; i++) {
        pixels[i].set_grayscale((i+iteration) % 255);
        pixels[i].alpha = 255;
    }
    draw_block(pixels, 50,0 + ((iteration)%HEIGHT), BLOCK_SIZE, BLOCK_SIZE);
    draw_block(pixels, 50,5 + ((iteration)%HEIGHT), BLOCK_SIZE, BLOCK_SIZE);

    draw(pixels, WIDTH, HEIGHT);
}

int main() {
    struct Pixel* pixels = allocate_pixels();
    emscripten_set_keydown_callback("#canvas", NULL, 1, keydown );
    emscripten_set_main_loop_arg(main_loop, pixels, FRAME_RATE, 1);
    return 0;
}

