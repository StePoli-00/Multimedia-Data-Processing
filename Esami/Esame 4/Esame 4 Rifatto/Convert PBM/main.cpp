#include <string>
#include "pbm.h"

extern void save_pbm(Image& im, std::string& fileout);
int main(int argc, char** argv)
{
	BinaryImage bim;

	bool res = bim.ReadFromPBM(argv[1]);
	if (res)
	{
		Image im = BinaryImageToImage(bim);
		std::string fileout = argv[2];
		save_pbm(im, fileout);
		return 0;
	}
	return 1;
}

