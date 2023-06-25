#include <string>
#include <sstream>
#include <iomanip>
#include "mat.h"
#include "pgm.h"
#include <fstream>
#include <filesystem>
#include <vector>
extern bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames);



int main(int argc, char** argv)
{
	mat<uint8_t> frame;
	std::vector<mat<uint8_t>> frames;
	if (y4m_extract_gray("foreman_cif.y4m", frames))
	{
		
		for (size_t i = 0; i < frames.size(); i++)
		{
			std::stringstream ss;
			ss << std::setfill('0');
			ss << "frame" << std::setw(3) << i << ".pgm";
			save_pgm(ss.str(), frames[i]);
		}
	}
}