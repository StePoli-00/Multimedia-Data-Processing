#include "pbm.h"
#include <sstream>
#include <cmath>
#include <fstream>

Image BinaryImageToImage(const BinaryImage& bimg) {

    if (bimg.ImageData.empty())
    {
        return {};
    }

    Image im;
    im.H = bimg.H;
    im.W = bimg.W;
    //im.resize(im.rows(), im.cols());
    

    if ((im.cols() % 8) == 0)
    {
        int pad = 8 - (im.cols() % 8);

    }
    else {

        uint8_t val=0;
     
        for (int i = 0; i < bimg.ImageData.size(); ++i)
        {
            val = bimg.ImageData[i];

            im.ImageData.push_back((val & 0b1000'0000) >>7);
            im.ImageData.push_back((val & 0b0100'0000) >> 6);
            im.ImageData.push_back((val & 0b0010'0000) >> 5);
            im.ImageData.push_back((val & 0b0001'0000) >> 4);
            im.ImageData.push_back((val & 0b0000'1000) >> 3);
            im.ImageData.push_back((val & 0b0000'0100) >> 2);
            im.ImageData.push_back((val & 0b0000'0010) >> 1);
            im.ImageData.push_back((val & 0b0000'0001) >> 0);
        }
    }


    for (auto &p : im.ImageData)
    {
        if (p == 1)
        {
            p = 0;
        }
        else
        {
            p = 255;
        }
    }

    return im;
}


void save_pbm(Image &im,std::string &fileout)
{

    std::ofstream os(fileout, std::ios::binary);
    if (!os)
    {
        return;
    }
    
    os << "P4\n";
    os << std::to_string(im.cols()) + " " + std::to_string(im.rows()) + "\n";
   

    for (int i = 0; i < im.ImageData.size(); ++i)
    {
        os.put(im.ImageData[i]);
    }

    return;
}











