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

template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&val), size);
}

class bitwriter {
	uint8_t buffer_;
	int n_ = 0;
	std::ostream& os_;

	std::ostream& write_bit(uint32_t bit) {
		buffer_ = (buffer_ << 1) | (bit & 1);
		++n_;
		if (n_ == 8) {
			raw_write(os_, buffer_);
			n_ = 0;
		}
		return os_;
	}

public:

	bitwriter(std::ostream& os) : os_(os) {}

	std::ostream& write(uint32_t u, uint8_t n) {

		for (int i = n - 1; i >= 0; --i) {
			write_bit(u >> i);
		}
		return os_;
	}

	std::ostream& operator()(uint32_t u, uint8_t n) {
		return write(u, n);
	}
	//std::ostream::&bitwriter::flush
	std::ostream& flush(uint32_t bit = 0) {
		while (n_ > 0) {
			write_bit(bit);
		}
		return os_;
	}

	~bitwriter() {
		flush();
	}
};

class bitreader {
	uint8_t buffer_;
	int n_ = 0;
	std::istream& is_;

public:
	bitreader(std::istream& is) : is_(is) {}

	uint32_t read_bit() {
		if (n_ == 0)
		{
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		//ritorna il bit corrente
		return (buffer_ >> n_) & 1;
	}

	uint32_t read(uint8_t n) {
		uint32_t u = 0;
		

		while (n-- > 0)
		{

			u = (u << 1) | read_bit();
		}

		return u;
	}

	bool fail()const {
		return is_.fail();
	}

	uint32_t operator()(uint8_t n) {
		return read(n);
	}
	explicit operator bool() const {
		return !fail();
	}
};
//extern void unmap(std::vector<int>& v);
//extern void map(std::vector<int>& v);
//extern int count_bit(int bit);



void map(std::vector<int>& v) {

	for (size_t i = 0; i < v.size(); ++i)
	{

		if (v[i] < 0)
		{
			v[i] = 2 * v[i] * -1;
		}
		else
		{
			v[i] = 2 * v[i] + 1;
		}
	}
	return;
}

int count_bit(int bit) {

	int cnt = 0;

	while (bit > 0) {
		//bit /= 2;
		bit = bit >> 1;
		++cnt;
	}
	return cnt;
}

void unmap(std::vector<int>& v) {

	for (size_t i = 0; i < v.size(); i++)
	{
		if (v[i] % 2 == 0) {
			v[i] = (v[i] / 2) * -1;
		}
		else
		{
			v[i] = (v[i] - 1) / 2;
		}

	}
	return;
}