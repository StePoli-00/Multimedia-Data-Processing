#include <sstream>
#include <iomanip>
#include <fstream>
#include "pgm.h"
#include "ppm.h"

bool y4m_extract_gray(const std::string &filename, std::vector<mat<uint8_t>> &frames);
bool y4m_extract_color(const std::string &filename, std::vector<mat<vec3b>> &frames);

int main()
{
	//std::string filename = "test1.y4m";
	//std::string filename = "720p_stockholm_ter.y4m";
	std::string filename = "foreman_cif.y4m";

	//std::vector<mat<uint8_t>> frames;
	//if (y4m_extract_gray(filename, frames)) {
	//	for (size_t i = 0; i < frames.size(); ++i) {
	//		std::stringstream ss;
	//		ss << std::setfill('0');
	//		ss << "frame" << std::setw(3) << i << ".pgm";
	//		save_pgm(ss.str(), frames[i]);
	//	}
	//}

	std::vector<mat<vec3b>> colors;
	if (y4m_extract_color(filename,colors)) {
		for (size_t i = 0; i < colors.size(); ++i) {
			std::stringstream ss;
			ss << std::setfill('0');
			ss << "color" << std::setw(3) << i << ".ppm";
			save_ppm(ss.str(), colors[i]);
		}
	}

	return 0;
}