#include <fstream>
#include <string>
#include <intrin.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <array>
#include <math.h>

template <typename T>
struct RGB {

	std::array<T, 3> rgb;
};

bool check_big_endian_columns(int& M, std::ifstream& is)
{
	uint16_t bit;
	char col_bin;
	int col = 0;
	for (int i = 0; i < 2; ++i)
	{
		bit = is.get();

		if (bit != 2)
		{
			return 1;
		}
	}
	is.read((&col_bin), sizeof(char));
	bit = (bit << 16) | col_bin;
	is.read((&col_bin), sizeof(char));
	bit = (bit << 8) | col_bin;
	
	uint16_t num;
	is >> num;
	//uint16_t bit = _byteswap_ushort(num);

	if (M == bit)
	{
		return true;
	}

	return false;

}
void rle_encoding(std::vector<uint8_t>& decoded, std::istream& is, int& M)
{
	uint8_t L;
	int i = 0;
	while (i < M)
	{

		L = is.get();
		if (L <= 127) //copy
		{

			for (int j = 0; j < L; j++)
			{
				decoded.push_back(is.get());
				++i;
			}
		}
		else //run
		{
			
			char c = is.get();
			for (int j = 0; j < (L - 128); j++)
			{
				decoded.push_back(c);
				++i;
				
			}
		}
	}

	return;
}


std::vector<RGB<float>> convert_byte_to_float(std::vector<uint8_t>& R, std::vector<uint8_t>& G, std::vector<uint8_t>& B, std::vector<uint8_t>& E) {

	std::vector<RGB<float>> vec;
	size_t n = R.size();
	int exp;
	float r, g, b;
	for (size_t i = 0; i < n; ++i)
	{
		exp = E[i] - 128;
		r = ((R[i] + 0.5) / 256) * pow(2, exp);
		g = ((G[i] + 0.5) / 256) * pow(2, exp);
		b = ((B[i] + 0.5) / 256) * pow(2, exp);
		vec.push_back({ r, g, b });
	}

	return vec;

}

float find_min(std::vector<RGB<float>>& vec)
{
	float min = vec[0].rgb[0];


	for (auto& x : vec)
	{
		if (x.rgb[0] < min)
		{
			min = x.rgb[0];
		}
		if (x.rgb[1] < min) {
			min = x.rgb[1];
		}
		if (x.rgb[2] < min) {
			min = x.rgb[2];
		}
	}
	return min;
}

float find_max(std::vector<RGB<float>>& vec)
{
	float max = 0;


	for (auto& x : vec)
	{
		if (x.rgb[0] > max)
		{
			max = x.rgb[0];
		}
		if (x.rgb[1] > max) {
			max = x.rgb[1];
		}
		if (x.rgb[2] > max) {
			max = x.rgb[2];
		}
	}
	return max;
}

std::vector<RGB<uint8_t>>  map_linear(std::vector<RGB<float>>& vec, float& min, float& max)
{
	float base;
	float exp;
	std::vector<RGB<uint8_t>> output;
	float r, g, b;
	for (auto& p : vec)
	{
		base = (p.rgb[0] - min) / (max - min);
		r = 255 * powf(base, 0.45);
		base = (p.rgb[1] - min) / (max - min);
		g = 255 * powf(base, 0.45);
		base = (p.rgb[2] - min) / (max - min);
		b = 255 * powf(base, 0.45);
		output.push_back({ uint8_t(r),uint8_t(g),uint8_t(b) });

	}

	return output;
}

bool save_pam(std::vector<RGB<uint8_t>>& vec, int& N, int& M,std::string &fout ) {

	
	std::ofstream os(fout, std::ios::binary);
	if (!os) { return false; }
	os << "P7\n";
	os << "WIDTH " + std::to_string(M) + "\n";
	os << "HEIGHT " + std::to_string(N) + "\n";
	os << "DEPTH 3\n";
	os << "MAXVAL 255\n";
	os << "TUPLTYPE RGB\n";
	os << "ENDHDR\n";
	char ch;
	for (int i = 0; i < vec.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			//ch = vec[i].rgb[j];
			//os.write(&ch, 1);
			os.put(vec[i].rgb[j]);
		}
	}

	return true;

}


int hdr_decode(std::string& filename, std::string& fout)
{
	std::ifstream is(filename, std::ios::binary);
	if (!is) { return 1; }

	std::string magic_number, line, format;
	int M=0, N=0; //N riga, M colonna
	char ch;

	std::getline(is, line);
	if (line != "#?RADIANCE") { return 1; }

	while (1)
	{
		std::getline(is, line);

		if (line.empty())
		{
			break;
		}
		if (line[0] == '#')
		{

		}
		else if (line == "FORMAT=32-bit_rle_rgbe")
		{

		}
		else
		{
			return 1;
		}

	}
	std::getline(is, line);
	std::stringstream ss(line);
	std::string token;

	while (ss >> token)
	{
		if (token == "-Y")
		{
			ss >> N;
		}
		else if (token == "+X") {
			ss >> M;
		}

		else { return 1; }
	}
	
	std::vector<uint8_t> R, G, B, E;
	for(int i=0; i<N;++i){

		if (!check_big_endian_columns(M, is)) { return 1; }
		rle_encoding(R, is, M);
		rle_encoding(G, is, M);
		rle_encoding(B, is, M);
		rle_encoding(E, is, M);

	}
	std::vector<RGB<float>> vec;
	std::vector<RGB<uint8_t>> linearized;

	vec = convert_byte_to_float(R, G, B, E);
	float min = find_min(vec);
	float max = find_max(vec);
	linearized = map_linear(vec, min, max);
	bool res = save_pam(linearized, N, M,fout);

	return res;


}

int main(int argc, char** argv)
{

	if (argc != 3) { return 1; }
	std::string filename(argv[1]);
	std::string fout(argv[2]);

	hdr_decode(filename,fout);
}