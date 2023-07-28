//#include "pbm.h"
//#include <sstream>
//#include <cmath>
//#include <fstream>
//
//
//    BinaryImage::BinaryImage()
//    {
//        H = 0;
//        W = 0;
//    }
//    BinaryImage::BinaryImage(int h, int w)
//    {
//        resize(h, w);
//    }
//
//    void BinaryImage::resize(int h, int w)
//    {
//        H = h;
//        W = w;
//        ImageData.resize(W * H);
//    }
//
//    uint8_t& BinaryImage::operator()(int r, int c)
//    {
//        assert(r >= 0 && r < H && c >= 0 && c < W);
//        return ImageData[r * W + c];
//    }
//
//
//     const uint8_t& BinaryImage::operator()(int r, int c) const
//    {
//        assert(r >= 0 && r < H && c >= 0 && c < W);
//        return ImageData[r * W + c];
//    }
//
//
//
//    char* BinaryImage::raw_data() {
//        return reinterpret_cast<char*>(&ImageData[0]);
//    }
//
//    bool BinaryImage::ReadFromPBM(const std::string& filename) {
//
//        if (filename.empty())
//        {
//            return false;
//        }
//        std::ifstream is(filename, std::ios::binary);
//        if (!is)
//        {
//            return false;
//        }
//
//        std::string magicnumber, line;
//        char ch;
//        int h = 0, w = 0;
//
//
//        is >> magicnumber;
//        if (magicnumber != "P4")
//        {
//            return false;
//        }
//        ch = is.get();
//        ch = is.peek();
//        if (ch == '#')
//        {
//            is >> line;
//            is >> ch;
//        }
//        is >> w;
//        ch = is.get();
//        is >> h;
//        ch = is.get();
//        char num;           
//        int  cnt = 0;
//        this->resize(h, w);    
//        if ((w % 8) != 0)
//        {
//            int C = std::round((double)w / 8); 
//            
//            for (int r = 0; r < this->rows(); r++)
//            {
//                for (int c = 0; c < this->cols(); c++)
//                {
//                    if(c == this->cols() - 1){
//                        for(int i = 0; i < (C*8) - this->cols(); ++i){
//                            this->ImageData[c + i] = 0; 
//
//                            //ImageData
//                        }
//                    }
//
//                }
//               
//
//
//            }
//        }
//        else
//        {
//           
//            for (int r = 0; r < this->rows(); r++)
//            {
//                for (int c = 0; c < this->cols(); c++)
//                {
//
//                    
//                    this->operator()(r, c) = is.get();
//
//                }
//
//
//            }
//        }
//
//
//        return true;
//    }
//
//     int BinaryImage::rows()
//    {
//        return H;
//    }
//     int BinaryImage::cols()
//    {
//        return W;
//    }
//
//     size_t BinaryImage::size()
//    {
//        size_t num = ImageData.size();
//        return num;
//    }
//
//
//
//
//
//
//
//
//
//
//
