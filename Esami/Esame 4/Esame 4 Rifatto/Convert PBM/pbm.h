#pragma once
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <cassert>
struct BinaryImage {
    int W;
    int H;
    std::vector<uint8_t> ImageData;

    int rows();
    int cols();
    BinaryImage();
    BinaryImage(int rows, int cols);
    void resize(int rows, int cols);
    std::vector<uint8_t>& data();
    uint8_t operator()(int r, int c);
    bool ReadFromPBM(const std::string& filename);
};


struct Image {
    int W;
    int H;
    std::vector<uint8_t> ImageData;
};

Image BinaryImageToImage(const BinaryImage& bimg);