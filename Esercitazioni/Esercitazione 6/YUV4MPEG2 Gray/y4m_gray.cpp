#include <string>
#include <sstream>
#include <iomanip>
#include "mat.h"
#include "pgm.h"
#include "ppm.h"
#include "types.h"
#include "utils.h"
#include <fstream>
#include <vector>




bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames)
{
	std::ifstream is(filename, std::ios::binary);
	if (!is)
	{
		return false;
	}
	char next;
	std::string field;
	std::string magic_number, C, I,F, A, X;
	int H = 0, W = 0;
	is >> magic_number;
	if (magic_number != "YUV4MPEG2")
	{
		return false;
	}
	while (1)
	{
		char next = is.get();
		if (next == '\n') {
			break;
		}
		if (next != ' ') {
			return false;
		}
		char tag = is.get();
		switch (tag) {
		break; case 'W': is >> W;
		break; case 'H': is >> H;
		break; case 'C': is >> C;
		break; case 'I': is >> I;
		break; case 'F': is >> F;
		break; case 'A': is >> A;
		break; case 'X': is >> X;
		}
		break; default: return false;
	}

	while (is >> field) {


		if (field != "FRAME")
		{
			return false;
		}

		while (1) {

			char next = is.get();
			if (next == '\n')
			{
				break;
			}
			if (next != ' ')
			{
				return false;
			}
			next = is.get();
			switch (next)
			{
			case 'I': is >> I; break;
			case 'X': is >> X; break;
			default: return false;
			}
		}

		mat<uint8_t> Y(H, W);
		mat<uint8_t> Cb(H / 2, W / 2);
		mat<uint8_t> Cr(H / 2, W / 2);

		is.read(Y.rawdata(), Y.rawsize());
		is.read(Cb.rawdata(), Cb.rawsize());
		is.read(Cr.rawdata(), Cr.rawsize());
		frames.push_back(Y);

	}

	return true;
}





