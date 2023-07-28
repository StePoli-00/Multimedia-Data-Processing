#include "mat.h"
#include "pgm.h"
#include <string>
extern bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames); 

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		return 1;
	}
	std::string filename(argv[1]);
	std::vector<mat<uint8_t>> frames;
	bool res = y4m_extract_gray(filename,frames);
	if (!res)
	{
		return res;
	}

	for (int i=0; i<frames.size();++i)
	{
		
		std::string fout = filename+std::to_string(i) + ".pam";
		save_pgm(fout, frames[i]);
	}

	return res;
}