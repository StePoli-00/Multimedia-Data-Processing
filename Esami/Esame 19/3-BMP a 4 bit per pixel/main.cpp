#include <string>
#include <array>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <vector>
#include <cassert>
using rgb = std::array<uint8_t, 3>;
using rgbo = std::array<uint8_t, 4>;
std::vector<rgbo> palette;
template<typename T>
class mat {

	int rows_, cols_;
	std::vector<T> data_;

public:

	mat() {
		rows_ = 0;
		cols_ = 0;

	}

	mat(int rows, int cols)
	{
		resize(rows, cols);
	}


	void resize(int rows, int cols)
	{
		rows_ = rows;
		cols_ = cols;
		data_.resize(rows * cols);
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

	size_t raw_size()
	{
		return (rows_ * cols_) * sizeof(T);
	}

	char* raw_data()
	{
		return reinterpret_cast<char*>(&data_[0]);
	}

	std::vector<uint8_t>& data()
	{
		return data_;
	}
};


template<typename T>
std::istream& raw_read(std::istream& is, T &val, size_t size = sizeof(T))
{
	return is.read(reinterpret_cast<char*>(&val), size);
}

class Bitreader {

	uint8_t buffer_;
	int n_ = 0;
	std::istream &is_;

public:
	Bitreader(std::istream& is): is_(is) {}

	uint32_t read_bit()
	{
		if (n_ == 0)
		{
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		return ((buffer_ >> n_) & 1);

	}

	uint32_t read(uint8_t n)
	{
		uint32_t u = 0;
		while (n-- > 0)
		{
			u = (u << 1) | read_bit();
		}

		return u;
	}


};



bool load_bmp(std::string filein, mat<uint8_t>& img)
{

	std::ifstream is(filein, std::ios::binary);

	if (!is) {
		return false;
	}

	uint16_t header_field, reserved1, reserved2, color_planes, n_bit_per_pixel;
	uint32_t BMP_size, offset, header_size, compression_method, image_size, num_colors, important_color;
	int32_t width, height, hor_res, vert_res;


	if (!is.read(reinterpret_cast<char*>(&header_field), sizeof(uint16_t)))
	{
		return false;
	}

	if (header_field != 19778)
	{
		return false;
	}

	if (!is.read(reinterpret_cast<char*>(&BMP_size), sizeof(uint32_t)))
	{
		return false;
	}

	if (!is.read(reinterpret_cast<char*>(&reserved1), sizeof(uint16_t)))
	{
		return false;
	}

	if (!is.read(reinterpret_cast<char*>(&reserved2), sizeof(uint16_t)))
	{
		return false;
	}

	if (!is.read(reinterpret_cast<char*>(&offset), sizeof(uint32_t)))
	{
		return false;
	}
	int dim = 0;
	if (!is.read(reinterpret_cast<char*>(&header_size), sizeof(uint32_t)))
	{
		return false;
	}
	dim += 4;

	if (!is.read(reinterpret_cast<char*>(&width), sizeof(int32_t)))
	{
		return false;
	}
	dim += 4;
	if (!is.read(reinterpret_cast<char*>(&height), sizeof(int32_t)))
	{
		return false;
	}
	dim += 4;
	if (!is.read(reinterpret_cast<char*>(&color_planes), sizeof(uint16_t)))
	{
		return false;
	}
	dim += 2;
	if (color_planes != 1)
	{
		return false;
	}


	if (!is.read(reinterpret_cast<char*>(&n_bit_per_pixel), sizeof(uint16_t)))
	{
		return false;
	}
	dim += 2;
	if (n_bit_per_pixel != 1 && n_bit_per_pixel != 4 && n_bit_per_pixel != 8 && n_bit_per_pixel != 16 && n_bit_per_pixel != 24 && n_bit_per_pixel != 32)
	{
		return false;
	}

	if (!is.read(reinterpret_cast<char*>(&compression_method), sizeof(uint32_t)))
	{
		return false;
	}
	dim += 4;

	if (compression_method != 0)
	{
		return false;
	}

	if (!is.read(reinterpret_cast<char*>(&image_size), sizeof(uint32_t)))
	{
		return false;
	}
	dim += 4;

	if (!is.read(reinterpret_cast<char*>(&hor_res), sizeof(int32_t)))
	{
		return false;
	}
	dim += 4;
	if (!is.read(reinterpret_cast<char*>(&vert_res), sizeof(int32_t)))
	{
		return false;
	}
	dim += 4;
	if (!is.read(reinterpret_cast<char*>(&num_colors), sizeof(uint32_t)))
	{
		return false;
	}
	dim += 4;
	if (num_colors == 0)
	{
		num_colors = (int)pow(2, n_bit_per_pixel);
	}

	if (!is.read(reinterpret_cast<char*>(&important_color), sizeof(uint32_t)))
	{
		return false;
	}
	dim += 4;
	if (dim != header_size)
	{
		return false;
	}

	img.resize(height, width);
	uint8_t pixel;
	

	palette.resize(num_colors);
	rgbo val;
	for (int i = 0; i < palette.size(); ++i)
	{

		for (int j = 0; j < 4; ++j)
		{
			val[j] = is.get();
		}

		palette[i] = val;
	}
	Bitreader br(is);

	if ((img.rows() % 32) == 0)
	{
		
	}
	else
	{
		int padding = 0;
		int length = 32 - (img.cols() * 4) % 32;


		for (int r = img.rows() - 1; r >= 0; --r)
		{

			for (int c = 0; c < img.cols(); ++c)
			{

				img(r, c) = br.read(4);
			}
			pixel = br.read(length);


		}

	}

	return true;






}
bool  check_neighbor(mat<uint8_t>& img, int r, int c)
{
	if (r >= 0 && r < img.rows() && c >= 0 && c < img.cols())
	{
		return true;
	}
	
	return false;
}



void save_pam(std::string fileout, mat<uint8_t>& img)
{

	std::ofstream os(fileout, std::ios::binary);

	if (!os) {
		return;
	}

	os << "P7\n";
	os << "WIDTH " << std::to_string(img.cols()) << "\n";
	os << "HEIGHT " << std::to_string(img.rows()) << "\n";
	os << "DEPTH 3\n";
	os << "MAXVAL 255\n";
	os << "TUPLTYPE RGB\n";
	os << "ENDHDR\n";
	uint8_t val;
	bool res;
	uint8_t index;
	uint8_t index1, index2,prec=0;




	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			val = img(r, c);
		
			for (int i = 2; i >= 0; --i)
			{
				os.put(palette[val][i]);
			}
			
		}
		
	}

	return;

}



int main(int argc, char** argv)
{
	if (argc != 3)
	{
		return 1;
	}

	std::string filein = argv[1];
	std::string fileout = argv[2];
	if (filein.empty() || fileout.empty())
	{
		return 1;
	}
	mat<uint8_t>img;
	bool res = load_bmp(filein, img);
	if (!res)
	{
		return 1;
	}
	save_pam(fileout, img);

}