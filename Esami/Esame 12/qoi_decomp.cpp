//#include <cstdint>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <array>
//#include <string>
//#include <unordered_map>
//
//
//
//uint32_t byteswap32(uint32_t x)
//{
//    uint32_t y = (x >> 24) & 0xff;
//    y |= (x >> 8) & 0xff00;
//    y |= (x << 8) & 0xff0000;
//    y |= (x << 24) & 0xff00000u;
//
//    return y;
//}
//
//using rgba = std::array<uint8_t, 4>; // Potete modificare questa definizione!
//using rgb = std::array<uint8_t, 3>;
//
//
//template<typename T>
//struct mat {
//    size_t rows_, cols_;
//    std::vector<T> data_;
//
//    mat(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(rows*cols) {}
//
//    const T& operator[](size_t i) const { return data_[i]; }
//    T& operator[](size_t i) { return data_[i]; }
//
//    size_t size() const { return rows_ * cols_; }
//    size_t rows() const { return rows_; }
//    size_t cols() const { return cols_; }
//
//    const char* rawdata() const {
//        return reinterpret_cast<const char*>(data_.data());
//    }
//    size_t rawsize() const { return size() * sizeof(T); }
//};
//
//struct  Header
//{
//    uint32_t width, height, channels, colorspace;
//};
//bool read_header_qoi(std::istream &is ,struct Header &header )
//{
//    
//    char magic_number[5];
//    uint32_t width, height, channels, colorspace;
//   
//    is.read(&magic_number[0], 4);
//    magic_number[4] = '\0';
//    
//    if (strcmp(magic_number, "qoif")!=0)
//    { return false; }
//
//    
//    if (!(is.read(reinterpret_cast<char *>(&width),4)))
//    {
//        return false;
//        
//    }
//    header.width = byteswap32(width);
//    if (!(is.read(reinterpret_cast<char*>(&height), 4)))
//    {
//        return false;
//       
//    }
//    header.height = byteswap32(height);
//    
//    channels = is.get();
//    if (channels != 3 && channels != 4)
//    {
//        return false;
//    }
//    header.channels = channels;
//    colorspace = is.get();
//    if (colorspace != 0 && colorspace != 1)
//    {
//        return false;
//    }
//
//    header.colorspace= colorspace;
//    return true;
//
//}
//
//
//
//bool save_pam(std::ostream& os, mat<rgba>& img)
//{
//    os << "P7\nWIDTH " << img.cols() << "\nHEIGHT " << img.rows() <<
//        "\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
//    os.write(img.rawdata(), img.rawsize());
//    return true;
//}
//bool save_pam(std::ostream &os, mat<rgb> &img)
//{
//    os << "P7\nWIDTH " << img.cols() << "\nHEIGHT " << img.rows() <<
//        "\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
//    os.write(img.rawdata(), img.rawsize());
//
//    return true;
//
//}
//
//
//
//bool decode_qoi(std::istream & is, mat<rgba>& img)
//{
//
//    uint8_t tag;
//    uint8_t two_bit=0;
//    uint8_t index = 0;
//    std::vector<int> array;
//    uint8_t r, g, b, a;
//    //array.resize(64);
//    rgba tuple = { 0,0,0,0 };
//    std::unordered_map<uint8_t, rgba> array_;
//    
// 
//    while (1)
//    {
//        if (!(is.read(reinterpret_cast<char*>(&tag), 1)))
//        {
//            break;
//        }
//        index = 0;
//        two_bit = 0;
//        two_bit = (two_bit << 1) | ((tag >> 7) & 1);
//        two_bit = (two_bit << 1) | ((tag >> 6) & 1);
//
//        if (two_bit == 0b00) //INDEX
//        {
//
//            for (int i = 6; i > 0; --i)
//            {
//                index = (index << 1) | ((tag >> i) & 1);
//
//            }
//            //array_.insert({ index,tuple });
//            array_[index] = tuple;
//            /*r = is.get();
//            g = is.get();
//            b = is.get();
//            a = is.get();
//            array[index] = (r * 3 + g * 5 + b * 7 + a * 11) % 64;*/
//        }
//        else if (two_bit == 0b01)  //DIFF
//        { 
//        }
//        else if (two_bit == 0b10)  //LUMA
//        {
//        }
//        else if (two_bit == 0b11) //potrebbe essere una RUN/RGB/RGBA
//        {
//            if (tag != 0b1111'1110 && tag!=0b1111'1111) //RUN
//            {
//                for (int i = 6; i > 0; --i)
//                {
//                    index = (index << 1) | ((tag >> i) & 1);
//
//                }
//                int x = 0;
//            }
//            else if (tag == 0b1111'1110) { //RGB
//
//            }
//            else if (tag == 0b1111'1111)  { //RGBA
//            }
//           
//        }
//        else
//        {
//            //ERORRE
//        }
//
//    
//       
//
//
//
//    }
//
//
//    return true;
//}
//
//
////NON serve questa
//bool decode_qoi(std::istream& is, mat<rgb>& img)
//{
//
//
//    return true;
//}
//
//
//int main(int argc, char *argv[])
//{
//    // TODO: Manage the command line  
//    if (argc != 3) { return 1; }
//    // TODO: Lettura dell'header e delle dimensioni dell'immagine 
//  
//   
//    if (strlen(argv[1]) == 0 || strlen(argv[2]) == 0)
//    {
//        return 1;
//    }
//    struct Header header;
//    std::ifstream is(argv[1], std::ios::binary);
//    if (!is)
//    {
//        return 1;
//    }
//    bool res=read_header_qoi(is,header);
//
//    if (!res)
//    {
//        return 1;
//    }
//    
//    if (header.channels== 4)
//    {
//        mat<rgba> img(header.height,header.width);
//        decode_qoi(is, img);
//        std::ofstream os(argv[2], std::ios::binary);
//
//        save_pam(os, img);
//    }
//    else
//    {
//        mat<rgb> img(header.height, header.width);
//        decode_qoi(is, img);
//        std::ofstream os(argv[2], std::ios::binary);
//        save_pam(os, img);
//    }
//
//     // TODO: Dovete mettere le dimensioni corrette!
//
//    // TODO: decodificare il file QOI in input e inserire i dati nell'immagine di output
//
//    // Questo è il formato di output PAM. È già pronto così, ma potete modificarlo se volete
//     // Questo utilizza il secondo parametro della linea di comando!
//  
//    return 0;
//}