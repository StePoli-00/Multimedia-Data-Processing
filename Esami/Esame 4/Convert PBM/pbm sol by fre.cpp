#include "pbm.h"
#include <fstream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>

bool BinaryImage::ReadFromPBM(const std::string& filename) {
	/*
	8 pixels per byte
	*/

	std::ifstream is(filename);
	if (!is) {
		return false;
	}
	
	std::string magic_num;
	char curr, next;

	//magic number
	is >> magic_num;
		
	if (magic_num != "P4" || !is) return false;
		
	//TODO #
	curr = is.get();
	
	if (curr == '\n') {
		is >> W;
	}
	curr = is.get(); // spazio
	is >> H;
	
	//\n
	curr = is.get();


	//DATA
	for (int h = 0; h < H; h++) {
		//leggo w bit
		int w_ = std::ceil((float)W / 8);
		for (int w = 0; w < w_; w++) {
			ImageData.push_back(is.get());
		}
	}
	

	return true;

}

void masking(std::vector<uint8_t>& ImageData, uint8_t curr, int8_t n, int8_t cnt = 0) {
	
	while (n-- > cnt) {
		uint8_t mask = 1 << n;
		ImageData.push_back((curr & mask) >> n);
	}

}

Image BinaryImageToImage(const BinaryImage& bimg) {

	Image im;
	im.H = bimg.H;
	im.W = bimg.W;
	
	int bimg_cols = bimg.W / 8;
	int resto = bimg.W % 8;
	if (resto != 0) {
		bimg_cols++;
	}
	
	for (int r = 0; r < bimg.H; r++) {	
		for (int c = 0; c < bimg_cols; c++) {
			uint8_t curr = bimg.ImageData.at(r * bimg_cols + c);
			if (c == bimg_cols - 1) {
				masking(im.ImageData, curr, 8, (8-resto));
				break;
			}
			else {
				masking(im.ImageData, curr, 8);
			}
		}

	}	

	//conversione 0 -> 255,
	for (int i = 0; i < im.ImageData.size(); i++) {
		if (im.ImageData[i] == 0) {
			im.ImageData[i] = 255;
		}
		else {
			im.ImageData[i] = 0;
		}
	}
	
	return im;
}


