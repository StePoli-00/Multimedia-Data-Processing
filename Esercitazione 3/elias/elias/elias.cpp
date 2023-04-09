#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include<cmath>
#include <iterator>
#include <string>
template<typename T>
std::ostream& raw_write(std::ostream& os, T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<char*>(&val), size);
}

template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&val), size);
}

class bitreader {
	uint8_t buffer_;
	int n_=0;
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
		int cnt = 0;

		while (n-- > 0)
		{

			while (u == 0)
			{
				
				u = (u << 1) | read_bit();
				++cnt;
			}
		
			for (int i = 0; i < (cnt-1); ++i) {
				u = (u << 1) | read_bit();

			}	
			return u;
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

int count_bit( int bit) {

	int cnt = 0;

	while (bit >0) {
		//bit /= 2;
		bit=bit >> 1;
		++cnt;
	}
	return cnt;
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
			v[i] = (v[i] -1)/2;
		}

	}
	return;
}
int  compress(char *filein, char *fout)
{
	std::ifstream is(filein);
	std::ofstream os(fout, std::ios::binary);
	if (!is || !os)
	{
		return 1;
	}
	//usare le graffe most vexing parse
	std::vector<int32_t>v{ std::istream_iterator<int32_t>(is), std::istream_iterator<int32_t>() };
	map(v);
	int bit;
	bitwriter bw(os);
	for (int i = 0; i < v.size(); i++)
	{
		bit = v[i];
		auto n = count_bit(bit);
		bw(bit, 2 * n - 1);

	}
	return 0;
}



int decompress(char* filein, char* fout) {
	std::ifstream is(filein, std::ios::binary);
	std::ofstream os(fout );
	if (!is || !os)
	{
		return 1;
	}
	int zeros = 0;
	int num;
	std::vector<int> v;
	bitreader br(is);
	while (1)
	{
		num = br.read(255);
		if (!br)
		{
			break;
		}

		v.push_back(num);
	}
	unmap(v);
	for (size_t i = 0; i < v.size(); i++)
	{
		os << v[i] << '\n';
	}
	
	return 0;


}

int main(int argc, char** argv) {

	if (argc != 4)
	{
		std::cout << "Error: incorrect number of parameters\n";
		return 1;
	}

	char *mode = argv[1];
	if (strcmp(mode,"c")!=0 && strcmp(mode,"d")!=0)
	{
		std::cout << "Error: first parameter must be c or d\n";
		return 1;
	}
	if (strcmp(mode, "c")==0) {
		compress(argv[2],argv[3]);
	}
	else
	{
		decompress(argv[2], argv[3]);
	}

	return 0;
	
	


}