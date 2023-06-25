#include "types.h"
#include "mat.h"
#include "ppm.h"
#include <string>
#include <vector>
#include <windows.h>
#include <filesystem>
#include <iomanip>
#include <codecvt>
#include <sstream>

std::wstring ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}


extern bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames);
int main(int argc, char** argv)
{
	std::vector<mat<vec3b>> frames;
	std::string f = "frame";

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string filename = argv[1];
	std::string path = converter.to_bytes(ExePath());
	path = path.substr(0, path.find("x64"));
	path += filename.substr(0, filename.find(".y4m"));
	if (y4m_extract_color(filename, frames)) {

		for (size_t i = 0; i < frames.size(); i++)
		{
			std::stringstream ss;
			ss << std::setfill('0');
			ss << "color" << std::setw(3) << i << ".ppm";
			save_ppm(ss.str(), frames[i]);
		}
	}
}