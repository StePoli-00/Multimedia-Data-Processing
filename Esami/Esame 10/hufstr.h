#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class elem {

public:
    uint8_t sym=0;
    uint8_t len = 0;
    uint32_t code = 0;

    elem(){}
 
    bool operator ==(const char& ch) const {

       return this->sym == ch;

    }


   
  

};


class hufstr {

 
    std::vector <elem> table_;
    
public:
    hufstr(){
        load();
 
        
    }

    int load();

    std::vector<uint8_t> compress(const std::string& s) const;
    std::string decompress(const std::vector<uint8_t>& v) const;
    std::vector <elem>& table()
    {
        return table_;
    }
};



template <typename T>
std::ostream& raw_write(std::ostream& os, T& val, size_t size = sizeof(T)) {
    return os.write(reinterpret_cast<char*>(&val), size);
}


class Bitwriter {


    uint8_t buffer_=0;
    int n_ = 0;
    std::ostream &os_;

    std::ostream &  write_bit(uint8_t bit)
    {

        buffer_ = (buffer_ << 1) | (bit & 1);
        ++n_;
        if (n_ == 8)
        {
            raw_write(os_, buffer_);
            n_ = 0;
        }

        return os_;
    }
public:


    std::ostream& write(uint32_t u, int n)
    {
        while (n-- > 0)
        {
            write_bit((u >> n));
        }

        return os_;
    }
    
    void flush( uint8_t bit=0)
    {
        while (n_ > 0)
        {
            write_bit(bit);
        }
    }

    Bitwriter(std::ostream & os): os_(os){  }

    ~Bitwriter()
    {
        flush();
    }

    int getN()
    {
        return n_;
    }


};