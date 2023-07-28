#pragma once
#include <string>
#include <unordered_map>
#include  <vector>
#include <fstream>

std::string base64_decode(const std::string& input);

template <typename T>
std::ostream& raw_write(std::ostream& os, T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<char*>(&val), size);
}

class Bitwriter {
	uint8_t buffer_=0;
	int n_ = 0;
	std::ostream& os_;


	std::ostream& write_bit(uint32_t bit)
	{
		++n_;
		buffer_ = (buffer_ << 1) | bit;
		if (n_ == 8)
		{
			raw_write(os_, buffer_);
			n_ = 0;
		}
		
		return os_;
	}
	
	public:
	
	Bitwriter(std::ostream& os): os_(os){}

	std::ostream& write(int32_t u, int n)
	{
		while (n-- > 0)
		{
			write_bit((u >> n));
		}
		return os_;
	}
	
	std::ostream& flush(uint8_t bit = 0)
	{
		while (n_ > 0)
		{
			write_bit(bit);
		}
		return os_;
	}

	~Bitwriter()
	{
		flush();
	}

	
};
