#include "pbm.h"
#include <sstream>
#include <cmath>
#include <fstream>


void get_pixels(std::vector<uint8_t>& vec, uint8_t pixel, int n, int cnt = 0)
{
    uint8_t mask, val;
    while (n-- > cnt)
    {
        mask = 1 << n;
        val = (pixel & mask) >> n;
        vec.push_back(val);
    }
}

Image BinaryImageToImage(const BinaryImage& bimg) {

    if (bimg.ImageData.empty())
    {
        return {};
    }

    Image im;
    im.H = bimg.H;
    im.W = bimg.W;

    int cols = bimg.W / 8;
    int resto = bimg.W % 8;

    if (resto != 0)
    {
        cols++;
    }
 
  
    for (int  r = 0; r < bimg.H; r++)
    {
        for (int  c = 0; c < cols; c++)
        {

            uint8_t pixel = bimg.ImageData.at(r * cols + c);
            if (c == cols - 1)
            {
                get_pixels(im.ImageData, pixel, 8, 8 - resto);
            }
            else
            {
                get_pixels(im.ImageData, pixel, 8);
            }
            
          
        }
 
    }

   /* if ((im.cols() % 8) == 0)
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
    }*/

    //conversione 0->255
    for (auto &p : im.ImageData)
    {
        if (p == 0)
        {
            p = 255;
        }
        else
        {
            p = 0;
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
   

    for (int i = 0; i < (int)im.ImageData.size(); ++i)
    {
        os.put(im.ImageData[i]);
    }

    return;
}











