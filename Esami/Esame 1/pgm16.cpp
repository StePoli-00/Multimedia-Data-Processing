#include "pgm16.h"
#include <sstream>
#include <intrin.h>
#include <fstream>
bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue) {

	if (filename.empty())
	{
		return 1;
	}

	int H, W;
	uint16_t ch;
	std::string comment, magic_number,line;
	//std::ifstream is(filename, std::ios::binary);
	std::ifstream is(filename, std::ios::binary);
	if (!is)
	{
		return false;
	}
	
	std::getline(is, magic_number);

	if (magic_number != "P5")
	{
		return false;
	}
	std::getline(is, line); //commento
	if (line[0] != '#')
	{
		return false;
	}
	if (!(is >> W))
	{
		return false;
	}
	if (!(is >> H))
	{
		return false;
	}
	if (!(is >> maxvalue))
	{
		return false;
	}
	
	img.resize(H, W);
	is.get();
	for (int  r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			if (maxvalue < 256) {
				
				img(r, c) = is.get();
			}
			else
			{
				is.read(reinterpret_cast<char*>(&ch),sizeof(uint16_t));
				uint8_t b1 = (ch >> 8) & 0xFF; 
				uint8_t b2 = ch & 0xFF;  
				img(r, c) = (b2<<8)+b1;
				//oppure
				//img(r, c) = _byteswap_ushort(ch);


			}
			
		}
	}

	return true;

}