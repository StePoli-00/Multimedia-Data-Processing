//#include "pbm.h"
//#include <math.h>
//
//void transform(Image& im)
//{
//	for (auto& p : im.ImageData)
//	{
//		if (p != 0)
//		{
//			p = 255;
//		}
//	}
//
//	return;
//
//}
//
//
//
//Image BinaryImageToImage(const BinaryImage& bimg) {
//
//
//	if (bimg.ImageData.empty() || bimg.H == 0 || bimg.W == 0)
//	{
//		return Image();
//	}
//
//	Image img;
//	int j = 0, n = 0;
//	uint8_t byte;
//	int C = std::round(double(bimg.W) / 8)-1;
//	img.H = bimg.H;
//	img.W = bimg.W;
//	img.resize(bimg.H, bimg.W);
//
//	
//
//		/*for (int r = 0; r < img.H; r++)
//		{
//			for (int c=0; c < img.W; c++)
//			{
//				n = 8;
//				byte = bimg.ImageData[j];
//				if (j == (C*(r+1)))
//				{
//					
//					uint8_t pixel;
//					while (n-- >= 4)
//					{
//						 pixel= (byte >> n) & 1;
//						 img(r, c) = pixel;
//						 ++c;
//					}
//					
//					
//				}
//
//				while (n-- > 0)
//				{
//					
//					uint8_t pixel= (byte >> n) & 1;
//					img(r, c) = pixel;
//					++c;
//
//				}
//				
//				++j;
//				
//			}
//			
//
//		}
//		*/
//	
//
//	/*for (int r = 0; r < img.H; r++)
//	{
//		for (int c = 0; c < img.W; c++)
//		{
//
//			if (j == (C * (r + 1)))
//			{
//				uint8_t pixel;
//				while (n-- > 0)
//				{
//					n--;
//					uint8_t pixel = (byte >> n) & 1;
//					img(r, c) = pixel;
//					++c;
//				}
//				byte = bimg.ImageData[j];
//				n = 8;
//				while (n-- >= 4)
//				{
//					pixel = (byte >> n) & 1;
//					img(r, c) = pixel;
//					++c;
//				}
//
//
//			}
//			else {
//
//
//				if (n == 0)
//				{
//					n = 8;
//					byte = bimg.ImageData[j];
//					++j;
//
//
//				}
//				
//				n--;
//				uint8_t pixel = (byte >> n) & 1;
//				img(r, c) = pixel;
//				
//
//			}
//
//
//
//			
//			
//			
//
//			
//
//		}
//
//
//	}*/
//
//
//	int r = 0, c = 0;
//		
//	while(c<img.W) {
//
//			if (r== img.H)
//			{
//				break;
//			}
//
//			n = 8;
//			byte = img.ImageData[j];
//			uint8_t pixel;
//			if (j == (C * (r + 1))) {
//
//
//				
//				while (n-- > 4)
//				{
//					pixel = (byte >> n) & 1;
//					img(r, c) = pixel;
//
//					if (c == img.cols() - 1) {
//
//						c = 0;
//
//						++r;
//						if (r == img.H)
//						{
//							break;
//						}
//
//					}
//					else {
//						++c;
//					}
//				}
//				n = 0;
//			}
//			else {
//
//				while (n-- > 0) {
//
//					pixel = (byte >> n) & 1;
//					img(r, c) = pixel;
//
//					if (c == img.cols() - 1) {
//
//						c = 0;
//						++r;
//						if (r == img.H)
//						{
//							break;
//						}
//					}
//					else { ++c; }
//
//				}
//			}
//
//			++j;
//		}
//	
//	transform(img);
//	return img;
//}