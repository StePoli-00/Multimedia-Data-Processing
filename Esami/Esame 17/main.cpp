#include <vector>
#include <cassert>
#include <string>
#include <fstream>
#include <array>
#include <sstream>
#include <math.h>
using rgb = std::array<uint8_t, 3>;
template<typename T>
struct mat {

	int rows_, cols_;
	std::vector<T> data_;


	mat(int r, int c)
	{
		rows_ = r;
		cols_ = c;
		data_.resize(r * c);
	}

	void resize(int r, int c)
	{
		rows_ = r;
		cols_ = c;
		data_.resize(r * c);
	}

	T& operator()(int r, int c)
	{
		assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		return data_[r * cols_ + c];
	}

	const T& operator()(int r, int c) const
	{
		assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		return data_[r * cols_ + c];
	}

	int rows()
	{
		return rows_;
	}

	int cols()
	{
		return cols_;
	}

	int size()
	{
		return rows_ * cols_;
	}

	char* raw_data()
	{
		return reinterpret_cast<char*>(&data_[0]);
	}



};


bool save_pgm(std::string fout, std::string suffix, mat<uint8_t>& img)
{

	if (fout.empty())
	{
		return false;
	}
	fout += suffix;
	std::ofstream os(fout, std::ios::binary);
	if(!os)
	{
		return false;
	}
	
	os << "P5\n";
	os << std::to_string(img.cols()) + ' ' + std::to_string(img.rows()) + '\n';
	os << std::to_string(255) + '\n';


	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			
			os.put(img(r,c));
		
		}
	}

	return true;
}




bool save_ppm(std::string fout, std::string suffix, mat<rgb>& img)
{

	if (fout.empty())
	{
		return false;
	}
	fout += suffix;
	std::ofstream os(fout, std::ios::binary);
	if (!os)
	{
		return false;
	}

	os << "P6\n";
	os << std::to_string(img.cols()) + " " + std::to_string(img.rows()) + '\n';
	os << std::to_string(255) + '\n';



	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			for (int i = 0; i < 3; ++i)
			{
				
				os.put(img(r,c)[i]);
			}
			

		}
	}

	return true;
}

mat<rgb> from_gray_to_rgb(mat<uint8_t>& img)
{

	mat<rgb> img_rgb(img.rows(), img.cols());


	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{

			if (r % 2 == 0)
			{

				if (c % 2 == 0)
				{
					img_rgb(r, c) = { img(r,c),0,0 };

				}
				else if(c%2!=0)
				{
					img_rgb(r, c) = {0, img(r,c),0 };
				}
			}
			else
			{
				if (c % 2 == 0)
				{
					
					img_rgb(r, c) = { 0, img(r,c),0 };

				}
				else if (c % 2 != 0)
				{
					img_rgb(r, c) = { 0,0,img(r,c) };
				}
			}

			
		}
	}


	return img_rgb;

	
}

bool check_index(int r, int c, int rows, int cols)
{
	if (r >= 0 && r < rows && c >= 0 && c < cols)
	{
		return true;
	}
	else
	{
		return false;
	}
}







uint8_t get_pixel(int r, int c, mat<rgb> &img, int index)
{
	if (check_index(r,c,img.rows(),img.cols()))
	{
		return img(r, c)[index];
	}
	else
	{
		return 0;
	}
}



uint8_t interpolate_green(int r,int c, mat<rgb> &img,int index)
{
	uint8_t delta_h = 0, delta_v = 0;
	uint8_t G2, G4, G6, G8, X1, X3, X5, X7, X9;
	G2 = get_pixel(r - 1, c, img, 1);
	G4 = get_pixel(r, c - 1, img, 1);
	G6 = get_pixel(r, c + 1, img, 1);
	G8 = get_pixel(r + 1, c, img, 1);
	X1 = get_pixel(r - 2, c, img, index);
	X3 = get_pixel(r, c - 2, img, index);
	X5 = img(r, c)[index];
	X7 = get_pixel(r, c + 2, img, index);
	X9 = get_pixel(r + 2, c, img, index);
	delta_h = abs(G4 - G6) +abs( X5 - X3 + X5 - X7);
	delta_v = abs(G2 - G8) +abs( X5 - X1 + X5 - X9);
	uint8_t res = 0;

	if (delta_h < delta_v)
	{

		res = ((G4 + G6) / 2) + ((X5 - X3 + X5 - X7) / 4);

	}
	else if (delta_h > delta_v)
	{
		res = ((G2 + G8) / 2) + ((X5 - X1 + X5 - X9) / 4);
	}
	else if (delta_h == delta_v)
	{
		res = ((G2 + G4 + G6 + G8) / 4) + ((X5 - X1 + X5 - X3 + X5 - X7 + X5 - X9) / 8);
	}

	return res;
}

void first_pass(mat<rgb>& img)
{

	
	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{


			if ((r % 2 == 0 && c % 2 == 0)) //R
			{
					img(r, c)[1] =interpolate_green(r,c,img,0);
			}
			else if (r % 2 != 0 && c % 2 != 0) //B
			{
				img(r, c)[1] = interpolate_green(r, c, img, 2);
			}

		}
	}

	return;

}





//void first_pass(mat<rgb>& img)
//{
//
//	int index;
//	double delta_h = 0, delta_v = 0;
//	for (int r = 0; r < img.rows(); r++)
//	{
//		for (int c = 0; c < img.cols(); c++)
//		{
//			index = -1;
//			for (int i = 0; i < 3; ++i)
//			{
//				
//				if (img(r, c)[i] != 0 && i != 1)
//				{
//					index = i;
//					break;
//				}
//			}
//
//			if (index == 0 || index == 2)
//			{
//				double G2, G4, G6, G8, X1, X3, X5, X7, X9;
//				G2 = get_pixel(r - 1, c, img, 1);
//				G4 = get_pixel(r, c - 1, img, 1);
//				G6 = get_pixel(r, c + 1, img, 1);
//				G8 = get_pixel(r + 1, c, img, 1);
//				X1 = get_pixel(r - 2, c, img, index);
//				X3 = get_pixel(r, c - 2, img, index);
//				X5 = img(r, c)[index];
//				X7 = get_pixel(r, c + 2, img, index);
//				X9 = get_pixel(r + 2, c, img, index);
//				delta_h = abs(G4 - G6+X5 - X3 + X5 - X7);
//				delta_v = abs(G2 - G8+X5 - X1 + X5 - X9);
//				double res = 0;
//
//				if (delta_h < delta_v)
//				{
//
//					res = ((G4 + G6) / 2) + ((X5 - X3 + X5 - X7) / 4);
//
//				}
//				else if (delta_h > delta_v)
//				{
//					res = ((G2 + G8) / 2) + ((X5 - X1 + X5 - X9) / 4);
//				}
//				else if (delta_h == delta_v)
//				{
//					res = ((G2 + G4 + G6 + G8) / 4)+ ((X5 - X1 + X5 - X3 + X5 - X7 + X5 - X9) / 8);
//				}
//
//				img(r, c)[1] = (uint8_t)res;
//
//			}
//
//
//
//
//		}
//	}
//
//	return;
//
//}


uint8_t interpolate_RB(int r, int c,mat<rgb> &img,int index)
{
	uint8_t G1, G3, G5, G7, G9, X1, X3, X7, X9;
	uint8_t res;
	uint8_t delta_n = 0, delta_p = 0;
	G1 = get_pixel(r - 1, c - 1, img, 1);
	G3 = get_pixel(r - 1, c + 1, img, 1);
	G5 = img(r, c)[1];
	G7 = get_pixel(r + 1, c - 1, img, 1);
	G9 = get_pixel(r + 1, c + 1, img, 1);
	X1 = get_pixel(r - 1, c - 1, img, index);
	X3 = get_pixel(r - 1, c + 1, img, index);
	X7 = get_pixel(r + 1, c - 1, img, index);
	X9 = get_pixel(r + 1, c + 1, img, index);

	delta_n = abs(X1 - X9) + abs(G5 - G1 + G5 - G9);
	delta_p = abs(X3 - X7) + abs(G5 - G3 + G5 - G7);


	if (delta_n < delta_p)
	{

		res = (X1 + X9) / 2 + (G5 - G1 + G5 - G9) / 4;

	}
	else if (delta_n > delta_p)
	{
		res = (X3 + X7) / 2 + (G5 - G3 + G5 - G7) / 4;
	}
	else if (delta_n == delta_p)
	{
		res = (X1 + X3 + X7 + X9) / 4 + (G5 - G1 + G5 - G3 + G5 - G7 + G5 - G9) / 8;
	}

	return res;

}


void second_pass(mat<rgb>& img)
{

	uint8_t R1, R2, B1, B2;
	

	//INTERPOLAZIONE G
	for (int  r = 0; r < img.rows(); r++)
	{

		for (int  c = 0; c < img.cols(); c++)
		{

			if (r % 2 == 0)
			{

				if (c % 2 != 0) //R SX/DX e B TOP/BOTTOM
				{
					

					R1 = get_pixel(r, c-1, img, 0);
					R2 = get_pixel(r, c + 1, img, 0);
					B1 = get_pixel(r-1, c , img, 2);
					B2 = get_pixel(r+1, c , img, 2);
					//img(r, c)[1] =(uint8_t) ((R1 + R2 + B1 + B2) / 4);
					img(r,c)[0]= (R1 + R2)/2;
					img(r, c)[2] =(B1 + B2) / 2;

				}
				
			}
			else
			{
				if (c % 2 == 0) // B SX / DX e R TOP / BOTTOM
				{
					B1 = get_pixel(r, c - 1, img, 2);
					B2 = get_pixel(r, c + 1, img, 2);
					R1 = get_pixel(r - 1, c, img, 0);
					R2 = get_pixel(r + 1, c, img, 0);
					//img(r, c)[1] = (uint8_t)((R1 + R2 + B1 + B2) / 4);
					img(r, c)[0] = (R1 + R2) / 2;
					img(r, c)[2] = (B1 + B2) / 2;
				}
			}

		}
	}
	


	//INTERPOLAZIONE R & B
	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{

			if (r % 2 == 0)
			{

				if (c % 2 == 0) //R
				{
					img(r,c)[2]=interpolate_RB(r, c, img, 2);

				}
				
			}
			else
			{
				
				if (c % 2 != 0) //B
				{
					img(r, c)[0] = interpolate_RB(r, c, img, 0);
				}
			}


		}
	}

	//SATURAZIONE
	for (auto& pixel : img.data_)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (pixel[i] < 0)
			{
				pixel[i] = 0;
			}

			if (pixel[i] > 255)
			{
				pixel[i] = 255;
			}
		}
	}



	return;
}





bool bayer_decode(std::string  filename, std::string  fout, mat<uint8_t>& img)
{

	if (filename.empty())
	{
		return false;
	}

	std::ifstream is(filename, std::ios::binary);
	if (!is)
	{
		return false;
	}

	std::string magic_number,line,token;
	int rows, cols, maxval;

	std::getline(is, magic_number);
	if (magic_number != "P5")
	{
		return false;
	}
	std::getline(is,line);
	std::stringstream ss(line);
	if (!(ss >> token))
	{
		return false;
	}
	cols = std::stoi(token);
	if (!(ss >> token))
	{
		return false;
	}
	rows = std::stoi(token);
	std::getline(is, line);
	maxval = std::stoi(line);

	if (maxval != 65535)
	{
		return false;
	}

	img.resize(rows, cols);
	uint16_t pixel,pixel2;

	for (int  r = 0; r < img.rows(); r++)
	{
		for (int  c = 0; c < img.cols(); c++)
		{
			pixel = is.get();
			pixel2 = is.get();
			pixel = (pixel << 8) | (pixel2);
			pixel = (uint8_t)((double)pixel / 256);
			img(r, c) = pixel;

			
		}
	}

	//save_pgm(fout,"_gray.pgm", img);
	mat<rgb> img_rgb= from_gray_to_rgb(img);
	//save_ppm(fout, "_bayer.ppm",img_rgb);
	first_pass(img_rgb);
	
	//save_ppm(fout, "_green.ppm", img_rgb);
	second_pass(img_rgb);
	save_ppm(fout, "_interp.ppm", img_rgb);
	return true;
	

}


int main(int argc, char** argv)
{

	if (argc != 3)
	{
		return -1;
	}

	mat<uint8_t> img(0,0);
	bool res=bayer_decode(argv[1],argv[2],img);
	if (res)
	{
		return 0;
	}
	
	return -1;



}