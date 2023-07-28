#include <fstream>
#include <string>
#include <sstream>
#include <vector>


std::vector<uint8_t> load_ppm(std::string& filein)
{
	std::ifstream is(filein, std::ios::binary);
	if (!is)
	{
		return {};
	}
	std::string magic_number, line, token;
	char ch;

	std::vector<uint8_t> data;
	int width, height, maxval;

	std::getline(is, magic_number);
	if (magic_number != "P6")
	{
		return {};
	}
	ch = is.peek();
	if (ch == '#')
	{
		std::getline(is, line);
		line.empty();
	}
	std::getline(is, line);
	std::stringstream ss(line);
	ss >> token;
	width = std::stoi((token));
	ss >> token;
	height = std::stoi((token));
	std::getline(is, line);
	maxval = std::stoi(line);
	if (maxval < 0 || maxval> 65536)
	{
		return {};
	}

	data.resize(width * height);

	is.read(reinterpret_cast<char*>(&data[0]), sizeof(uint8_t) * data.size());

	/*for (int i = 0; i < data.size(); ++i)
	{
		data[i] = is.get();
	}*/

	return data;
}


//1111 1111 0000 0000 0000 0000 1111 1111


bool encode_Z85rot(std::string filein, std::string filout, int  &N) {


	std::vector<uint8_t> data = load_ppm(filein);
	if (data.empty())
	{
		return false;
	}

	if ((data.size() % 4) != 0)
	{
		while ((data.size() % 4) != 0)
		{
			data.push_back(0);
		}
	}
	

	for (auto& d : data)
	{

	}

	//0101 0001


	return true;
}




bool decode_Z85rot(std::string filein, std::string filout, int& N) {


	return true;
}

// 86-> 1000 0110
// H=43
//e=14
//e=21


int main(int argc, char** argv)
{

	if (argc != 5)
	{
		return 1;
	}

	std::string option = argv[1];
	std::string N = argv[2];
	bool res;
	
	for (auto& c : N)
	{
		if (std::isdigit(c) == 0)
		{
			return 1;
		}
	}
	int rotation = std::stoi(N);
	if (option == "c")
	{
		res=encode_Z85rot(argv[3], argv[4],rotation);
	}
	else if (option == "d")
	{
		res=decode_Z85rot(argv[3], argv[4], rotation);
	}

	if (res)
	{
		return !res;
	}
	return res;
}