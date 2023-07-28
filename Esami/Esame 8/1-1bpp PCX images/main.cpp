#include "pcx.h"
#include "pgm.h"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		return 1;
	}
	mat<uint8_t> img;
	load_pcx(argv[1], img);
	std::string fileout = argv[2];
	save_pgm(fileout, img);
}

