#include "types.h"
#include "mat.h"
#include <string>
#include <vector>
#include <fstream>
bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames) {

	std::ifstream is(filename, std::ios::binary);
	if (!is)
	{
			return false;
	}
	int W, H;
	std::string field,chroma,I,X, F, A;
	is >> field;
	if (field!="YUV4MPEG2")
	{
		return false;
	}
	while (1)
	{
		char token = is.get();
		if (token == '\n')
		{
			break;
		}
		if (token != ' ')
		{
			return false;
		}
		token = is.get();
		switch (token)
		{
			break; case 'W': is >> W;
			break; case 'H': is >> H;
			break; case 'C': { is >> chroma;if (chroma != "420jpeg") { return false; } }
			break; case 'I': is >> I;
			break; case 'F': is >> F;
			break; case 'A': is >> A;
			break; case 'X': is >> X;
		default:
			return false;
		}
		

	}
	if(H<=0 || W<=0)
	{
		return false;
	}

	while (is >> field)
	{
		if (field != "FRAME")
		{
			return false;
		}

		while (1)
		{

			char token = is.get();
			if (token == '\n')
			{
				break;
			}
			if (token != ' ')
			{
				return false;
			}
			token = is.get();
			switch (token)
			{
			break; case 'I': is >> I;
			break; case 'X': is >> X;
			default: return false;
			}
		}
		mat<uint8_t> Y(H, W);
		mat<uint8_t>Cb(H / 2, W / 2);
		mat<uint8_t>Cr(H / 2, W / 2);
		mat<vec3b> frame(H, W);
		is.read(Y.rawdata(), Y.rawsize());
		is.read(Cb.rawdata(), Cb.rawsize());
		is.read(Cr.rawdata(), Cr.rawsize());

		for (int r = 0; r < frame.rows(); r++)
		{

			for (int c = 0; c < frame.cols(); c++)
			{

				int y = Y(r, c);
				int cb = Cb(r / 2, c / 2);
				int cr = Cr(r / 2, c / 2);

				y = y < 16 ? 16 : y > 235 ? 235 : y;
				cb = cb < 16 ? 16 : cb > 240 ? 240 : cb;
				cr = cr < 16 ? 16 : cr > 240 ? 240 : cr;

			
				int R = (1164 * (y-16) + 0 * (cb-128) + 1596 * (cr-128))/1000;
				int G = (1164 * (y-16) - 392 * (cb-128) - 813 * (cr-128))/1000;
				int B = (1164 * (y-16) + 2017 * (cb-128) + 0 * (cr-128))/1000;

				R = R < 0 ? 0 : R>255 ? 255 : R;
				G = G < 0 ? 0 : G>255 ? 255 : G;
				B = B < 0 ? 0 : B>255 ? 255 : B;

				frame(r, c)={ uint8_t(R), uint8_t(G), uint8_t(B) };


			}
		}

		frames.push_back(frame);
	}

	return true;


}