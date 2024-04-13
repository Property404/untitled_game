#include "Sprite.hpp"
#include <fstream>
#include <vector>
#include <filesystem>
#include <iostream>
#include <sstream>

const std::vector<Color>& Sprite::pixels () const {
    return _pixmap.at(_index);
}

Sprite Sprite::fromBitmap(std::filesystem::path&& path) {
    constexpr uint64_t MAGIC = 0x4249'544d'4150'3332ull;
    std::ifstream fp;
    fp.open(path.native(), std::ios::in | std::ios::binary);
    if (fp.fail() || !fp.is_open()) {
        std::cerr << "No such file" << std::endl;
        throw std::runtime_error(std::string{"Could not open file: "} + path.native());
    }

    uint64_t magic = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    fp.read ((char*)&magic, sizeof(magic));
    fp.read ((char*)&width, sizeof(width));
    fp.read ((char*)&height, sizeof(height));

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
    assert(width > 0);
    assert(height > 0);
    assert(pixels.size() > 0);
    assert(pixels.size() == width * height);
    return Sprite(width, height, pixels);
}
