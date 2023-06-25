#include <fstream>
#include "mat.h"

bool y4m_extract_gray(const std::string &filename, std::vector<mat<uint8_t>> &frames)
{
	std::ifstream is(filename, std::ios::binary);
	if (!is) {
		return false;
	}
	std::string header;
	is >> header;
	if (header != "YUV4MPEG2") {
		return false;
	}
	int w = -1, h = -1;
	std::string chroma = "420jpeg";
	std::string interlacing, frame_rate,
		aspect_ratio, application;
	while (true) {
		char next = is.get();
		if (next == '\n') {
			break;
		}
		if (next != ' ') {
			return false;
		}
		char tag = is.get();
		switch (tag) {
		break; case 'W': is >> w;
		break; case 'H': is >> h;
		break; case 'C': is >> chroma;
		break; case 'I': is >> interlacing;
		break; case 'F': is >> frame_rate;
		break; case 'A': is >> aspect_ratio;
		break; case 'X': is >> application;
		break; default: return false;
		}
	}
	if (w < 0 || h < 0 || chroma != "420jpeg") {
		return false;
	}

	mat<uint8_t> Y(h, w);
	mat<uint8_t> Cb(h / 2, w / 2);
	mat<uint8_t> Cr(h / 2, w / 2);

	while (is >> header) {
		if (header != "FRAME") {
			return false;
		}
		while (true) {
			char next = is.get();
			if (next == '\n') {
				break;
			}
			if (next != ' ') {
				return false;
			}
			char tag = is.get();
			switch (tag) {
			break; case 'I': is >> interlacing;
			break; case 'X': is >> application;
			break; default: return false;
			}
		}
		is.read(Y.rawdata(), Y.rawsize());
		is.read(Cb.rawdata(), Cb.rawsize());
		is.read(Cr.rawdata(), Cr.rawsize());

		frames.push_back(Y);
	}

	return true;
}
