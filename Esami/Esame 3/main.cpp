#include "lzs.h"


int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		return 1;
	}
	std::ifstream is(argv[1], std::ios::binary);
	if (!is)
	{
		return 1;
	}
	std::ofstream os(argv[2]);
	if (!os)
	{
		return 1;
	}
	lzs_decompress(is, os);
}