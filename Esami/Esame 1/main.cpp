#include "pgm16.h"

int main(int argc, char** argv) {

	if (argc != 2) {
		return 1;
	}
	mat<uint16_t> img;
	uint16_t maxvalue;
	std::string filename(argv[1]);
	bool ret=load(filename, img, maxvalue);
	return ret;
}