#include "mat.h"
#include "types.h"
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames)
{

	std::ifstream is(filename, std::ios::binary);
	std::vector<std::string> inter = { "Ip", "It","Ib" };
	if (!is)
	{
		return false;
	}
	std::string line, token, F, A, X, br;
	int H = 0, W = 0;

	if (!(is >> line))
	{
		return false;
	}

	if (line != "YUV4MPEG2") { return false; }
	std::getline(is, line);
	std::stringstream ss(line);
	while (ss >> token)
	{

		if (token[0] == 'H')
		{
			H = atoi(&token[1]);

		}
		else if (token[0] == 'W')
		{
			W = atoi(&token[1]);
		}
		else if (token[0] == 'C') {

			if (token != "C420jpeg") { return false; }
			/*auto it = chroma.find(token);
			if(it==chroma.end())
			{
				return false;
			}*/
		}

		else if (token[0] == 'I') {

			auto it = std::find(inter.begin(), inter.end(), token);
			if (it == inter.end())
			{
				return false;
			}
		}

		else if (token[0] == 'F') {
			F = token.substr(1, token.size());

		}
		else if (token[0] == 'A') {
			A = token.substr(1, token.size());

		}
		else if (token[0] == 'X') {

			X = token.substr(1, token.size());
		}
		else { return false; }


	}



	while (1) {

		std::getline(is, line);
		if (line != "FRAME")
		{
			break;
		}
		char ch;

		while (1)
		{


			ch = is.peek();
			if (ch == 'I') {
				std::getline(is, line);
				ch = is.get();
			}
			else if (ch == 'X')
			{
				std::getline(is, line);
				ch = is.get();
			}
			else
			{
				break;
			}

		}

		mat<uint8_t> Y, Cb, Cr;
		mat<vec3b> img(H,W);
		Y.resize(H, W);
		Cb.resize(H / 2, W / 2);
		Cr.resize(H / 2, W / 2);
		is.read(Y.rawdata(), Y.cols() * Y.rows());
		is.read(Cb.rawdata(), Cb.cols() * Cb.rows());
		is.read(Cr.rawdata(), Cr.cols() * Cr.rows());



		for (int  r = 0; r < img.rows(); r++)
		{
			for (int  c = 0; c < img.cols(); c++)
			{
				int y = Y(r, c);
				int cb = Cb(r / 2, c / 2);
				int cr = Cr(r / 2, c / 2);

				y = (y < 16) ? 16 :  (y > 235) ? 235 : y;
				cb = (cb < 16) ? 16 : (cb > 240) ? 240 : cb;
				cr = (cr < 16) ? 16 : (cr > 240) ? 240 : cr;

				int R =(1164 * (y - 16) + 0 * (cb - 128) + 1596 * (cr - 128)) / 1000;
				int G =(1164 * (y - 16) - 392 * (cb - 128) - 813 * (cr - 128)) / 1000;
				int B=(1164 * (y - 16) + 2017 * (cb - 128) + 0 * (cr - 128)) / 1000;

				R = (R < 0) ? 0 : (R > 255) ? 255 : R;
				G = (G < 0) ? 0 : (G > 255) ? 255 : G;
				B = (B < 0) ? 0 : (B > 255) ? 255 : B;
				vec3b vec = { uint8_t(R),uint8_t(G),uint8_t(B) };
				img(r, c) = vec;
			}
		}

		frames.push_back(img);

		




	}

	return true;


}