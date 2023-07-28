#include "hufstr.h"
#include <sstream>





int  hufstr::load() {

   
    std::ifstream is ("table.bin", std::ios::binary);
    if (!is)
    {
        return 1;
    }
    while (1)
    {
        elem tmp;
        if (!(is.read(reinterpret_cast <char*>(&tmp.sym), sizeof(uint8_t))))
        {
            break;
        }
        if (!(is.read(reinterpret_cast <char*>(&tmp.len), sizeof(uint8_t))))
        {
            break;
        }

        if (!(is.read(reinterpret_cast <char*>(&tmp.code), sizeof(uint32_t))))
        {
            break;
        }

        this->table().push_back(tmp);
    }
	
    return 0;
}



std::vector<uint8_t> hufstr::compress(const std::string& s) const {


    if (s.length() == 0)
    {
        return {};
     }
    std::vector<uint8_t> vec;
    // std::string output
    //std::stringstream ss(output);
    std::stringstream stream;
    Bitwriter bw(stream);
    for (auto& c : s)
    {
        auto it = std::find(table_.begin(), table_.end(),c);

        if (it != table_.end())
        {
            
            bw.write(it->code, it->len);
            /*if (ss.str().size() == 1)
            {
                std::string s = ss.str();
                char c = s[0];
               
                vec.push_back((uint8_t(s[0])));
                ss.str("");
            }*/
            
        }
    }

    bw.flush(1);
    uint8_t byte;
    while (stream.read(reinterpret_cast<char*>(&byte), 1))
    {
        vec.push_back(byte);
    }
    
    
    return vec;
}


std::string hufstr::decompress(const std::vector<uint8_t>& v) const {


    return "";
   
}