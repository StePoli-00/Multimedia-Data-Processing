#include <string>

bool lz78encode(const std::string& input_filename, const std::string& output_filename, int maxbits);


int main(int argc, char** argv)
{
	if (argc != 4)
	{
		return 1;
	}

	bool res=lz78encode(argv[1], argv[2], atoi(argv[3]));
	return res;
}