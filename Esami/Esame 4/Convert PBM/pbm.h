#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <cassert>

struct BinaryImage {
    int W;
    int H;
    std::vector<uint8_t> ImageData;

    BinaryImage()
    {
        H = 0;
        W = 0;
    }
    BinaryImage(int h, int w)
    {
        resize(h, w);
    }

    void resize(int h, int w)
    {
        H = h;
        W = w;
        ImageData.resize((W * H));
    }


    char* raw_data() {
        return reinterpret_cast<char*>(&ImageData[0]);
    }

    bool ReadFromPBM(const std::string& filename) {

        if (filename.empty())
        {
            return false;
        }
        std::ifstream is(filename, std::ios::binary);
        if (!is)
        {
            return false;
        }

        std::string magicnumber, line;
        char ch;
        int h = 0, w = 0;


        is >> magicnumber;
        if (magicnumber != "P4")
        {
            return false;
        }
        ch = is.get();
        ch = is.peek();
        if (ch == '#')
        {
            is >> line;
            is >> ch;
        }
        is >> w;
        ch = is.get();
        is >> h;
        ch = is.get();
        char num;
        this->H = h;
        this->W = w;
        while (is.read(&num,sizeof(char)))
        {
            this->ImageData.push_back(num);
        }

        return true;
    }


    int rows() { return H; }
    int cols() { return W; }
    size_t size() { return ImageData.size(); }
};


struct Image {
    int W;
    int H;
    std::vector<uint8_t> ImageData;

    Image()
    {
        H = 0;
        W = 0;
    }
    Image(int h, int w)
    {
        resize(h, w);
    }

    void resize(int h, int w)
    {
        H = h;
        W = w;
        ImageData.resize(W * H);
    }

    int rows() { return H; }
    int cols() { return W; }
    size_t size() { return ImageData.size(); }

    uint8_t& operator()(int r, int c)
    {
        assert(r >= 0 && r < H && c >= 0 && c < W);
        return ImageData[r * W + c];
    }

    const uint8_t& operator()(int r, int c) const
    {
        assert(r >= 0 && r < H && c >= 0 && c < W);
        return ImageData[r * W + c];
    }

    char* raw_data() {
        return reinterpret_cast<char*>(&ImageData[0]);
    }

};

Image BinaryImageToImage(const BinaryImage& bimg);