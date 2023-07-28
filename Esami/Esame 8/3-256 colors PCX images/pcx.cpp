#include "pcx.h"
#include <algorithm>


bool check_run(uint8_t byte)
{

	uint8_t bit = (byte & 0b11000000) >> 6;

	return bit == 3;
}

void DecodeRunLenght(std::istream& is, std::vector<uint8_t>& scanline, int& total_bytes) {


	int cnt = 0;
	uint8_t byte;
	int L = 0;

	while (cnt < total_bytes)
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


bool read_header(std::istream& is, mat<vec3b>& img, int& total_bytes,std::vector<vec3b>&palette)
{


	char Padding[54];
	int readed = 0;
	uint8_t depth;
	std::vector<uint8_t> correct_bits_per_plane = { 1,2,4,8 };
	uint8_t manufacturer, version, encoding, BitsPerPlane, ColorPlanes, reserved, palette_to_skip[48];
	uint16_t WindowXmin, WindowYmin, WindowXmax, WindowYmax,
		BytesPerPlaneLine, H, W, VertDPI, HorzDPI, PaletteInfo, HorScrSize, VerScrSize;


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
	readed += 2;
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
	if (!(raw_read(is, palette_to_skip, 48 * sizeof(uint8_t)))) {
		return false;
	}
	readed += 48;
	reserved = is.get();
	readed += 1;
	ColorPlanes = is.get();
	depth = ColorPlanes * BitsPerPlane;
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

	total_bytes = ColorPlanes * BytesPerPlaneLine;
	
	palette.resize(256);
	
	is.seekg(-769, std::ios::end);

	uint8_t val = is.get();

	if (val == 12)
	{
		for (int i = 0; i < 256; ++i)
		{

			for (int j = 0; j < 3; ++j)
			{
				palette[i][j] = is.get();
			}
		}
	}
	is.seekg(128, std::ios::beg);


	return true;
}





bool load_pcx(const std::string& filename, mat<vec3b>& img)
{
	if (filename.empty()) { return false; }

	std::ifstream is(filename, std::ios::binary);
	if (!is) { return false; }
	int total_bytes;
	std::vector<vec3b> palette;
	bool res = read_header(is, img, total_bytes,palette);

	if (!res)
	{
		return false;
	}

	uint8_t byte;
	int i = 0;
	int dim = total_bytes / 3;
	for (int r = 0; r < img.rows(); r++)
	{
		std::vector<uint8_t> scanline(total_bytes);
		DecodeRunLenght(is, scanline, total_bytes);
	
		for (int c = 0; c < img.cols(); c++)
		{

			byte = scanline[c];

			for (int i = 0; i < 3; ++i)
			{
				img(r, c)[i] = palette[byte][i];
			}
	
		}

	}

	return true;



}