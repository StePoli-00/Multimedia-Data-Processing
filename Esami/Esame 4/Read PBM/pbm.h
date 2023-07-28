#pragma once
#include <vector>
#include <cassert>
#include <string>

struct BinaryImage {
    int W;
    int H;
    std::vector<uint8_t> ImageData;
    BinaryImage();
    BinaryImage(int h, int w);
    void resize(int h, int w);
    char* raw_data();
    bool ReadFromPBM(const std::string& filename);
    //int  rows();
    //int  cols();
    //size_t   size();
};

    
    
 
