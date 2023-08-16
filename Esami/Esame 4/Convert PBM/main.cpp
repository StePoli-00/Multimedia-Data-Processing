#include "pbm.h"

extern void save_pbm(Image& im, std::string& fileout);
int main(int argc, char** argv)
{

	if (argc != 3)
	{
		return 1;
	}

	std::string filename(argv[1]);
	std::string fileout(argv[2]);
	BinaryImage bi;
	bool res = bi.ReadFromPBM(filename);
	Image im;
	if (!res)
	{
		return 1;
	}
	
	im = BinaryImageToImage(bi);

	save_pbm(im,fileout);
	return 0;
}