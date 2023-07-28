#include "mat.h"
#include "ppm.h"
#include "types.h"
#include <string>
#include <algorithm>
extern bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames);

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		return 1;
	}
	std::string filename(argv[1]);
	std::vector<mat<vec3b>> frames;
	bool res = y4m_extract_color(filename, frames);
	if (!res)
	{
		return res;
	}
	int index = filename.find(".y4m");
	filename.erase(index, filename.length());
	for (int i = 0; i < frames.size(); ++i)
	{

		std::string fout = filename + std::to_string(i+1) + ".pam";
		save_ppm(fout, frames[i]);
	}

	return res;
}