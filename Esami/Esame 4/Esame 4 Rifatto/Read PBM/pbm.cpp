#include "pbm.h"
#include  <math.h>


BinaryImage::BinaryImage(){

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
std::vector<uint8_t> & BinaryImage::data() {
	
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
    this->H=h;
    this->W = w;
    int C = std::ceil(this->cols() / 8)+1;

    for (int  r = 0; r < this->rows(); r++)
    {
        for (int  c = 0; c < C; c++)
        {
           
            this->data().push_back(is.get());
        }
    }
   
    return true;
}