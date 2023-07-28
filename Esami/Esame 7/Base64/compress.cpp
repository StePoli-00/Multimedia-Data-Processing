#include "mat.h"
#include "ppm.h"
#include <sstream>
void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded) {

	std::vector<uint8_t> elements;
	for (int  r = 0; r < img.rows(); r++)
	{
		for (int  c = 0; c < img.cols(); c++)
		{

			elements.push_back(img(r, c));
		}
	}
	int cnt = 1;
	bool end = false;
	uint8_t tmp = elements[0];
	std::vector<uint8_t> copy_vec;
	for (size_t i = 1; i < elements.size(); ++i)
	{
		if (elements[i] != tmp) //copy
		{
			copy_vec.push_back(tmp);
			tmp = elements[i];
			/*serve per gestire il caso in cui siamo all'ultimo elemento
		 che va messo come una copy, altrimenti  tmp andrebbe perso*/
			if (i + 1 == elements.size())
			{
				copy_vec.push_back(tmp);
			}
		}
		else {
			//scrivo copy_vec
			if (copy_vec.size() != 0)
			{
				uint8_t L = copy_vec.size() - 1;
				encoded.push_back(L);
				for (auto& x : copy_vec) {

					encoded.push_back(x);
				}
				copy_vec.clear();
			}


			while (tmp == elements[i])
			{
				++cnt;
				++i;
				//serve per gestire una run alla fine
				if (i == elements.size())
				{
					end = true;
					break;
				}
			}

			if (end)
			{
				break;
			}

			encoded.push_back((257 - cnt));
			encoded.push_back(tmp);
			cnt = 1;
			tmp = elements[i];
			/*serve per gestire il caso in cui siamo all'ultimo elemento
			che va messo come una copy, altrimenti  tmp andrebbe perso*/
			if (i + 1 == elements.size())
			{
				copy_vec.push_back(tmp);
			}


		}
	}

	if (end)
	{
		encoded.push_back((257 - cnt));
		encoded.push_back(tmp);
	}
	if (copy_vec.size() != 0)
	{
		uint8_t L = copy_vec.size() - 1;
		encoded.push_back(L);
		for (auto& x : copy_vec) {

			encoded.push_back(x);
		}
		copy_vec.clear();
	}

	encoded.push_back(128);
	return;

}


template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T))
{
	return is.read(reinterpret_cast<char*>(&val), size);
}

class Bitreader {

	uint8_t buffer_=0, n_ = 0;
	std::istream& is_;

public:
	Bitreader(std::istream& is) : is_(is) {}

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

	uint32_t read(uint8_t n)
	{
		uint32_t u = 0;
		while (n-- > 0)
		{

			u = (u << 1) | read_bit();
		}
		return u;
	}

	//utility per poter scrivere br(6)
	uint32_t operator() (uint8_t n)
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


std::string Base64Encode(const std::vector<uint8_t>& v) {

	if (v.empty()) { return ""; }

	std::vector<uint8_t> vec = v;
	std::vector<uint8_t> decoded;

	if ((vec.size() % 3) != 0)
	{
		while ((vec.size() % 3) != 0)
		{
			vec.push_back(128);
		}

	}
	std::string data = "";
	for (auto& x : vec)
	{
		data.push_back(x);
	}

	std::istringstream ss(data);
	Bitreader br(ss);

	while (1)
	{
		uint8_t num = br.read(6);
		if (!br) {
			break;
		}
		decoded.push_back(num);

	}
	std::string decoded_string;
	for (auto& x : decoded)
	{
		if (x >= 0 && x <= 25)
		{

			//decoded_string.push_back( x +65);
			decoded_string.push_back(x + 'A');
		}
		else if (x >= 26 && x <= 51)
		{
			decoded_string.push_back('a' + (x - 26));
		}
		else if (x >= 52 && x <= 61) {
			decoded_string.push_back('0' + x - 52);
		}
		else if (x == 62)
		{

			decoded_string.push_back('+');
		}
		else if (x == 63)
		{

			decoded_string.push_back('/');
		}
	}

	return decoded_string;




}

