#include "pcx.h"
#include "pgm.h"


int main(int argc, char* argv[]) {

	// FB nel main ci sono un po' di modifiche che possono essere ignorate, le ho fatto per far prima a fare debug.
	/*if (argc != 2) {
		return 1;
	}
	*/
	const std::string filename = argv[1];
	//const std::string filename = "bunny.pcx";
	mat<uint8_t> img;
	bool res = load_pcx(filename, img);
	save_pgm("prova.pgm", img);
	if (!res) {
		return 1;
	}

	return 0;
}
