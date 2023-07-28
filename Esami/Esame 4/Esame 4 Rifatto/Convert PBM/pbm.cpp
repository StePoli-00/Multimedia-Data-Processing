#include "pbm.h"
#include  <math.h>


BinaryImage::BinaryImage() {

}
BinaryImage::BinaryImage(int rows, int cols) {

    this->H = rows;
    this->W = cols;

}
void  BinaryImage::resize(int rows, int cols) {
    this->H = rows;
    this->W = cols;
    this->ImageData.resize(rows * cols);
}
int BinaryImage::rows() {
    return this->H;
}
int BinaryImage::cols() {
    return this->W;
}
std::vector<uint8_t>& BinaryImage::data() {

    return this->ImageData;
}
uint8_t BinaryImage::operator()(int r, int c) {

    assert(r >= 0 && r < this->rows() && c >= 0 && this->cols());
    return this->ImageData[r * this->cols() + c];
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
    int C = std::ceil(this->cols() / 8) + 1;

    for (int r = 0; r < this->rows(); r++)
    {
        for (int c = 0; c < C; c++)
        {

            this->data().push_back(is.get());
        }
    }

    return true;
}


void transform(Image& img) {

    for (auto& p : img.ImageData)
    {
        if (p == 1)
        {
            p =0;
        }
        else
        {
            p = 255;
        }
    }
    return;
}

Image BinaryImageToImage(const BinaryImage& bimg) {


    Image im;
    im.H = bimg.H;
    im.W = bimg.W;
    uint8_t val;
    int j = 0;
    im.ImageData.resize(im.H * im.W * 8);
    for (int i = 0; i < bimg.ImageData.size(); ++i)
    {
        val=bimg.ImageData[i];

        im.ImageData[j] = ((val & 0b1000'0000) >> 7);
        ++j;
        im.ImageData[j] = ((val & 0b0100'0000) >> 6);
        ++j;
        im.ImageData[j] = ((val & 0b0010'0000) >> 5);
        ++j;
        im.ImageData[j] = ((val & 0b0001'0000) >> 4);
        ++j;
        im.ImageData[j] = ((val & 0b0000'1000) >> 3);
        ++j;
        im.ImageData[j] = ((val & 0b0000'0100) >> 2);
        ++j;
        im.ImageData[j] = ((val & 0b0000'0010) >> 1);
        ++j;
        im.ImageData[j] = ((val & 0b0100'0001) >> 0);
        ++j;
    }


  /*  im.ImageData.resize(im.H * im.W);

    for (int i = 0; i < bimg.ImageData.size();++i)
    {
        im.ImageData[i] = bimg.ImageData[i];
    }*/
    transform(im);
    return im;

}

void save_pbm(Image& im, std::string& fileout)
{

    std::ofstream os(fileout, std::ios::binary);
    if (!os)
    {
        return;
    }

    os << "P4\n";
    os << std::to_string(im.W) + " " + std::to_string(im.H) + "\n";


    for (int i = 0; i < im.ImageData.size(); ++i)
    {
        os.put(im.ImageData[i]);
    }

    return;
}