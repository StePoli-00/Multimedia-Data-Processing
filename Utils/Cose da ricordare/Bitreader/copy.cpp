#include <fstream>

template <typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T))
{
	is.read(reinterpret_cast<char*>(&val), size);
}


class Bitreader {

	uint8_t buffer_ = 0, n_ = 0;
	std::istream& is_;

	uint32_t read_bit()
	{
		if (n_ == 0)
		{
			raw_read(is_, buffer_);
			n_ = 8;

		}
		--n_;
		return ((buffer_ >> n_) & 1);
	}

public:

	Bitreader(std::istream& is) : is_(is) {}

	uint32_t read(int n)
	{
		uint32_t u = 0;
		while (n-- > 0)
		{
			u = (u << 1) | read_bit();
		}

		return u;
	}

	const bool fail() const {
		return is_.fail();
	}

	explicit operator bool() const {
		return !fail();
	}


	uint32_t operator()(uint8_t n)
	{
		return read(n);
	}



};