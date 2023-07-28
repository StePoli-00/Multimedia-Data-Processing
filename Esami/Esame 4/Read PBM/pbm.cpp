#include "pbm.h"
#include <sstream>
#include <cmath>
#include <fstream>
#include "pbm.h"


BinaryImage::BinaryImage()
{
    H = 0;
    W = 0;
}
BinaryImage::BinaryImage(int h, int w)
{
    resize(h, w);
}

void BinaryImage::resize(int h, int w)
{
    H = h;
    W = w;
    ImageData.resize(W * H);
}


char* BinaryImage::raw_data() {
    return reinterpret_cast<char*>(&ImageData[0]);
}

bool BinaryImage::ReadFromPBM(const std::string& filename) {

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
    while (1)
    {
        num = is.get();
        if(!is)
        {
            break;
        }
        this->ImageData.push_back(num);
    }
  
    return true;
}


