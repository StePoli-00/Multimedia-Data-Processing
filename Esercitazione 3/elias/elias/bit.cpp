#include "bit.h"

int count_bit(int bit) {

	int cnt = 0;

	while (bit > 0) {
		//bit /= 2;
		bit = bit >> 1;
		++cnt;
	}
	return cnt;
}





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


template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&val), size);
}

	uint32_t bitreader::read_bit() {
		if (n_ == 0)
		{
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		//ritorna il bit corrente
		return (buffer_ >> n_) & 1;
	}

	uint32_t bitreader::read(uint8_t n) {
		uint32_t u = 0;
		int cnt = 0;

		while (n-- > 0)
		{

			while (u == 0)
			{

				u = (u << 1) | read_bit();
				++cnt;
			}

			for (int i = 0; i < (cnt - 1); ++i) {
				u = (u << 1) | read_bit();

			}
			return u;
		}

		return u;
	}

	bool bitreader::fail()const {
		return is_.fail();
	}

	uint32_t bitreader::operator()(uint8_t n) {
		return read(n);
	}
	 bitreader::operator bool() const {
		return !fail();
	}





template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}


std::ostream& bitwriter::write_bit(uint32_t bit) {
	buffer_ = (buffer_ << 1) | (bit & 1);
	++n_;
	if (n_ == 8) {
		raw_write(os_, buffer_);
		n_ = 0;
	}
	return os_;
}



std::ostream& bitwriter::write(uint32_t u, uint8_t n) {

	for (int i = n - 1; i >= 0; --i) {
		write_bit(u >> i);
	}
	return os_;
}

std::ostream& bitwriter::operator()(uint32_t u, uint8_t n) {
	return write(u, n);
}

std::ostream& bitwriter::flush(uint32_t bit) {
	while (n_ > 0) {
		write_bit(bit);
	}
	return os_;
}

bitwriter::~bitwriter() {
	flush();
}