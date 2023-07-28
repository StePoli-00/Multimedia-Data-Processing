#include "pcx.h"
#include <fstream>
#include <algorithm>


bool check_run(uint8_t byte)
{
	
	uint8_t bit = (byte & 0b11000000)>>6;
	
	return bit == 3;
}



void DecodeRunLenght(std::istream &is,std::vector<uint8_t> &scanline, int& total_bytes) {


	int cnt = 0;
	uint8_t byte;
	uint8_t L=0;

	while (cnt <  total_bytes)
	{
		byte = is.get();
		bool res = check_run(byte);

		if (res)
		{

			L = (byte & 0b0011'1111);

			byte = is.get();
			std::fill_n(scanline.begin() + cnt, L, byte);
			cnt += L;
			
		}
		else
		{
			scanline[cnt] = byte;
			++cnt;
		}

	}

	return;


}




bool load_pcx(const std::string& filename, mat<uint8_t>& img) {

	if (filename.empty()) { return false; }

	std::ifstream is (filename, std::ios::binary);
	if (!is) { return false; }
	
	char Padding[54];
	int readed = 0;
	uint8_t depth;
	std::vector<uint8_t> correct_bits_per_plane = { 1,2,4,8 };
	uint8_t manufacturer,version,encoding,BitsPerPlane,ColorPlanes,reserved,palette[48];
	uint16_t WindowXmin, WindowYmin, WindowXmax, WindowYmax,
	BytesPerPlaneLine,H,W, VertDPI, HorzDPI, PaletteInfo, HorScrSize, VerScrSize;


	manufacturer = is.get();
	++readed;
	if (manufacturer != 0x0A) {
		return false;
	}
	version = is.get();
	++readed;
	if (version != 5)
	{
		return false;
	}
	
	encoding = is.get();
	++readed;
	if (encoding != 1)
	{
		return false;
	}
	
	BitsPerPlane = is.get();
	++readed;
	auto it = std::find(correct_bits_per_plane.begin(), correct_bits_per_plane.end(), BitsPerPlane);
	if (it == correct_bits_per_plane.end())
	{
		return false;
	}
	if (!(raw_read(is, WindowXmin, sizeof(uint16_t)))) {
		return false;
	}
	readed+=2;
	if (!(raw_read(is, WindowYmin, sizeof(uint16_t)))) {
		return false;
	}
	readed += 2;
	if (!(raw_read(is, WindowXmax, sizeof(uint16_t)))) {
		return false;
	}
	readed += 2;
	if (!(raw_read(is, WindowYmax, sizeof(uint16_t)))) {
		return false;
	}
	readed += 2;

	H = WindowYmax - WindowYmin + 1;
	W = WindowXmax - WindowXmin + 1;
	if (!(raw_read(is, VertDPI, sizeof(uint16_t)))) {
		return false;
	}
	readed += 2;
	if (!(raw_read(is, HorzDPI, sizeof(uint16_t)))) {
		return false;
	}
	readed += 2;
	if (!(raw_read(is,palette, 48*sizeof(uint8_t)))) {
		return false;
	}
	readed += 48;
	reserved = is.get();
	readed += 1;
	ColorPlanes = is.get();
	depth=ColorPlanes*BitsPerPlane;
	readed += 1;

	if (!(raw_read(is, BytesPerPlaneLine, sizeof(uint16_t)))) {
		return false;
	}
	readed += 2;
	if (BytesPerPlaneLine < (W / 8 * BitsPerPlane))
	{
		return false;
	}

	if (!(raw_read(is, PaletteInfo, sizeof(uint16_t)))) {
		return false;
	}
	readed += 2;
	if (!(raw_read(is, HorScrSize, sizeof(uint16_t)))) {
		return false;
	}
	readed += 2;
	if (!(raw_read(is, VerScrSize, sizeof(uint16_t)))) {
		return false;
	}
	readed += 2;
	if (!(raw_read(is, Padding, 54 * sizeof(char)))) {
		return false;
	}
	readed += 54;

	if (readed != 128)
	{
		return  false;
	}

	img.resize(H, W);
	
	int total_bytes = ColorPlanes*BytesPerPlaneLine;
	
	int c = 0;
	uint8_t byte;
	
	for (int  r = 0; r < img.rows(); r++)
	{
		std::vector<uint8_t> scanline(total_bytes);
		DecodeRunLenght(is, scanline, total_bytes);
		for (int i = 0; i < img.cols()/8; i++)
		{
			byte = scanline[i];
			img(r, c) =((byte & 0b1000'0000) >> 7)*255;
			++c;
			img(r, c)=((byte & 0b0100'0000) >> 6)*255;
			++c;
			img(r,c) =((byte & 0b0010'0000) >> 5)*255;
			++c;
			img(r, c)=( (byte & 0b0001'0000) >> 4)*255;
			++c;
			img(r, c) = ((byte & 0b0000'1000) >> 3)*255;
			++c;
			img(r, c) =( (byte & 0b0000'0100) >> 2)*255;
			++c;
			img(r, c) = ((byte & 0b0000'0010) >> 1)*255;
			++c;
			img(r, c) = ((byte & 0b0000'0001) >> 0)*255;
			++c;
		
		}
		c = 0;
	
	}
	
	
	return true;
}