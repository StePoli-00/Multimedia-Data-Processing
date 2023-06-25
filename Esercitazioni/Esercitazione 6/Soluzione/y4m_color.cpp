#include <fstream>
#include "mat.h"
#include "types.h"

bool y4m_extract_color(const std::string &filename, std::vector<mat<vec3b>> &frames)
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

	mat<vec3b> frame(h, w);

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

		for (int r = 0; r < frame.rows(); ++r) {
			for (int c = 0; c < frame.cols(); ++c) {
				int y = Y(r, c);
				int cb = Cb(r / 2, c / 2);
				int cr = Cr(r / 2, c / 2);

				y = y < 16 ? 16 : y>235 ? 235 : y;
				cb = cb < 16 ? 16 : cb>240 ? 240 : cb;
				cr = cr < 16 ? 16 : cr>240 ? 240 : cr;

				y -= 16;
				cb -= 128;
				cr -= 128;

				int R = (1164 * y + 0000 * cb + 1596 * cr) / 1000;
				int G = (1164 * y -  392 * cb -  813 * cr) / 1000;
				int B = (1164 * y + 2017 * cb + 0000 * cr) / 1000;

				R = R < 0 ? 0 : R>255 ? 255 : R;
				G = G < 0 ? 0 : G>255 ? 255 : G;
				B = B < 0 ? 0 : B>255 ? 255 : B;

				frame(r, c) = { uint8_t(R), uint8_t(G), uint8_t(B) };
			}
		}
		
		frames.push_back(frame);
	}

	return true;
}
