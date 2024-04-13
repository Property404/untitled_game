#include "Sprite.hpp"
#include <fstream>
#include <vector>
#include <filesystem>
#include <iostream>
#include <sstream>

const std::vector<Color>& Sprite::pixels () const {
    return _pixmap.at(_index);
}

void Sprite::setIndex(size_t index) {
    _index = index;
}

Sprite::Sprite(std::filesystem::path&& path, const size_t width, const size_t height) {
    assert(width > 0);
    assert(height > 0);

    constexpr uint64_t MAGIC = 0x4249'544d'4150'3332ull;
    std::ifstream fp;
    fp.open(path.native(), std::ios::in | std::ios::binary);
    if (fp.fail() || !fp.is_open()) {
        std::cerr << "No such file" << std::endl;
        throw std::runtime_error(std::string{"Could not open file: "} + path.native());
    }

    uint64_t magic = 0;
    uint32_t hdr_width = 0;
    uint32_t hdr_height = 0;
    fp.read ((char*)&magic, sizeof(magic));
    fp.read ((char*)&hdr_width, sizeof(hdr_width));
    fp.read ((char*)&hdr_height, sizeof(hdr_height));
    assert(hdr_height > 0);
    assert(hdr_width > 0);
    assert(hdr_width % width == 0);
    assert(hdr_height % height == 0);

    if (magic != MAGIC) {
        std::stringstream ss;
        ss << path << " is not a RGBA bitmap. Expected magic 0x"<<std::hex<<MAGIC
            <<", but found 0x" << std::hex << magic << "(" << std::dec << magic << ")";
        throw std::runtime_error(ss.str());
    }

    std::vector<Color> pixels{};
    while (fp.good() && !fp.eof()) {
        Color color;
        fp.read ((char*)&color, sizeof(Color));
        if (!fp.eof()) {
            pixels.push_back(color);
        }
    }

    assert(pixels.size() > 0);
    assert(pixels.size() == hdr_width * hdr_height);

    std::vector<std::vector<Color>> pixmap{};
    for(unsigned row_start=0; row_start < hdr_height; row_start+=height) {
        for(unsigned col_start=0; col_start < hdr_width; col_start+=width) {
            std::vector<Color> subpixels{};
            for (unsigned row = row_start ; row < row_start+height; row+=1) {
                for (unsigned col = col_start ; col < col_start+width; col+=1) {
                    subpixels.push_back(pixels.at(row * hdr_width + col));
                }
            }
            pixmap.push_back(subpixels);
        }
    }

    _width = width;
    _height = height;
    _pixmap = pixmap;
}
