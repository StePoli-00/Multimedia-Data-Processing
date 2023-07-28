#include <fstream>
#include <vector>
template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T))
{
	return is.read(reinterpret_cast<char*>(&val), size);
}

class Bitreader {

	uint8_t buffer_, n_ = 0;
	std::istream& is_;

public:
	Bitreader(std::istream& is): is_(is){}

	uint32_t read_bit()
	{
		if (n_ == 0)
		{
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		return (buffer_ >> n_) & 1;
	}

	uint32_t read(int n)
	{
		uint32_t u = 0;
		while(n-->0)
		{

			u = (u << 1) | read_bit();
		}
		return u;
	}
	
	//utility per poter scrivere br(6)
	uint32_t operator() (int n)
	{
		return read(n);
	}

	bool fail() const {
		return is_.fail();
	}
	//serve per poter scrivere !br nella condizione di uscita dalla lettura
	explicit operator bool() const {
		return !fail();
	}


};


//int main(int argc, char **argv)
//{
//	if (argc != 2)
//	{
//		exit(1);
//	}
//
//	std::ifstream is(argv[1], std::ios::binary);
//	if (!is)
//	{
//		return 1;
//	}
//
//	Bitreader br(is);
//	std::vector<int32_t> vec;
//
//	while (1)
//	{
//		int32_t num = br(11);
//		if (!br)
//		{
//			break;
//		}
//		vec.push_back(num);
//		
//	}
//
//}