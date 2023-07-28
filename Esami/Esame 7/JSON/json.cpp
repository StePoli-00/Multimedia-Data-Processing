#include "mat.h"
#include "ppm.h"
#include <fstream>
#include <map>
#include <vector>

#include <sstream>
void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded) {

	std::vector<uint8_t> elements;
	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
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
				uint8_t L = (uint8_t)copy_vec.size() - 1;
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
		uint8_t L =(uint8_t) (copy_vec.size() - 1);
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

	uint8_t buffer_, n_ = 0;
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


	//trasformati
	return decoded_string;




}
bool LoadPPM(const std::string& filename, mat<vec3b>& img) {

	if (filename.empty())
	{
		return false;
	}
	std::ifstream is(filename, std::ios::binary);
	if (!is) { return false; }
	std::string magic_number, line, token;
	char ch;
	int H = 0, W = 0, max_val = 0;
	//estrae anche il /n is.get()=prossimo carattere ss>>[char][int] mette il valore e salta i whitespace
	std::getline(is, magic_number);
	if (magic_number != "P6") { return false; }
	ch = is.peek();
	if (ch == '#') { std::getline(is, line); }
	std::getline(is, line);
	std::stringstream ss(line);
	if (!(ss >> W)) {
		return false;
	}
	if (!(ss >> H)) {
		return false;
	}
	std::getline(is, line);
	max_val = std::stoi(line);

	if (max_val < 0 || max_val>65536) { return false; }

	img.resize(H, W);
	uint8_t R, G, B;
	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			//is >> img(r, c);
			R = is.get();
			G = is.get();
			B = is.get();
			img(r, c) = { R,G,B };
		}


	}

	return true;






}

void SplitRGB(const mat<vec3b>& img, mat<uint8_t>& img_r, mat<uint8_t>& img_g, mat<uint8_t>& img_b) {

	vec3b vec;

	img_r.resize(img.rows(), img.cols());
	img_g.resize(img.rows(), img.cols());
	img_b.resize(img.rows(), img.cols());


	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			vec = img(r, c);
			img_r(r, c) = vec[0];
			img_g(r, c) = vec[1];
			img_b(r, c) = vec[2];
		}


	}
	return;
}





std::string JSON(const std::string& filename) {

	if (filename.empty())
	{
		return "{}";
	}
	mat<vec3b> img;
	bool res = LoadPPM(filename, img);
	if (!res) { return "{}"; }
	mat<uint8_t> r, g, b;
	SplitRGB(img, r, g, b);
	std::vector<uint8_t> decoded_r, decoded_g, decoded_b;
	PackBitsEncode(r, decoded_r);
	PackBitsEncode(g, decoded_g);
	PackBitsEncode(b, decoded_b);

	std::string r_base64 = Base64Encode(decoded_r);
	std::string g_base64 = Base64Encode(decoded_g);
	std::string b_base64 = Base64Encode(decoded_b);
	std::vector<std::pair<std::string,std::string>> objects = {
		{"\"width\"",std::to_string(img.rows())},
		{"\"height\"",std::to_string(img.cols())},
		{"\"red\"","\"" + r_base64 + "\""},
		{"\"green\"","\"" + g_base64 + "\""},
		{"\"blue\"","\"" + b_base64 + "\""}
	};
	std::string json = "";
	
	
	json.push_back('{');
	json.push_back('\n');
	size_t i = 0;

	for (auto x : objects)
	{
		json.push_back('\t');
		json += x.first;
		json += " : ";
		json += x.second;
		++i;
		if (i != objects.size())
		{
			json += ",";
		}
		json += "\n";

	}
	json.push_back('}');
	
	return json;
}


