#include "mat.h"
#include "ppm.h"
#include <fstream>
#include <sstream>
bool LoadPPM(const std::string& filename, mat<vec3b>& img) {

	if (filename.empty ())
	{
		return false;
	}
	std::ifstream is(filename, std::ios::binary);
	if (!is) { return false; }
	std::string magic_number, line,token;
	char ch;
	int H=0, W=0, max_val=0;
	//estrae anche il /n is.get()=prossimo carattere ss>>[char][int] mette il valore e salta i whitespace
	std::getline(is, magic_number);   
	if (magic_number != "P6") { return false; }
	ch = is.peek();
	if (ch == '#') { std::getline(is, line); }
	std::getline(is, line);
	std::stringstream ss(line);
	if (!(ss >> W)) {
		return false;
	}
	if (!(ss >> H)) {
		return false;
	}
	std::getline(is, line);
	max_val=std::stoi(line);

	if (max_val < 0 || max_val>65536) { return false; }

	img.resize(H, W);
	uint8_t R, G, B;
	for (int  r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			//is >> img(r, c);
			R = is.get();
			G = is.get();
			B = is.get();
			img(r, c) = { R,G,B };
		}

		
	}

	return true;


	
	

	
}


void SplitRGB(const mat<vec3b>& img, mat<uint8_t>& img_r, mat<uint8_t>& img_g, mat<uint8_t>& img_b) {

	vec3b vec;

	img_r.resize(img.rows(), img.cols());
	img_g.resize(img.rows(), img.cols());
	img_b.resize(img.rows(), img.cols());


	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			vec = img(r, c);
			img_r(r,c) = vec[0];
			img_g(r,c) = vec[1];
			img_b(r,c) = vec[2];
		}


	}
	return;
}