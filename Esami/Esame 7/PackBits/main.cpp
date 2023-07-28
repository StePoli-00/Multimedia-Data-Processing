#include "mat.h"
#include "ppm.h"
#include  <string>
extern bool LoadPPM(const std::string& filename, mat<vec3b>& img);
extern void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded);
extern void SplitRGB(const mat<vec3b>& img, mat<uint8_t>& img_r, mat<uint8_t>& img_g, mat<uint8_t>& img_b);

int main(int argc, char** argv) {

	if (argc != 2) {
		return 1;
	}
	mat<vec3b> img;
	bool res= LoadPPM(argv[1], img);
	mat<uint8_t> r, g, b;
	SplitRGB(img, r, g, b);
	std::vector<uint8_t> decoded_r, decoded_g, decoded_b;
	PackBitsEncode(r, decoded_r);
	PackBitsEncode(g, decoded_g);
	PackBitsEncode(b, decoded_b);

}