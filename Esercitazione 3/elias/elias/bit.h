#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include<cmath>
#include <iterator>
#include <string>
#include <bit>

template<typename T>
std::ostream& raw_write(std::ostream& os, T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<char*>(&val), size);
}

class bitwriter {
	uint8_t buffer_;
	int n_ = 0;
	std::ostream& os_;
	std::ostream& write_bit(uint32_t bit);
public:

	bitwriter(std::ostream& os) : os_(os) {}
	std::ostream& write(uint32_t u, uint8_t n);
	std::ostream& operator()(uint32_t u, uint8_t n);
	std::ostream& flush(uint32_t bit = 0);
	~bitwriter();
	
};

class bitreader {
	uint8_t buffer_;
	int n_ = 0;
	std::istream& is_;
public:
	bitreader(std::istream& is) : is_(is) {}
	uint32_t read_bit();
	uint32_t read(uint8_t n);
	bool fail()const;
	uint32_t operator()(uint8_t n);
	explicit operator bool() const;
};

extern void unmap(std::vector<int>& v);
extern void map(std::vector<int>& v);
extern int count_bit(int bit);





