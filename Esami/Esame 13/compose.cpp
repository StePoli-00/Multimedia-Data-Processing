#include <string>
#include <cassert>
#include <vector>
#include <array>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <math.h>
template<typename T>
struct mat {

	int rows_;
	int cols_;
	std::vector<T> data_;


	mat()
	{
		rows_ = 0;
		cols_ = 0;
		
	}

	mat(int rows, int cols)
	{
		rows_ = rows;
		cols_ = cols;
		data_.resize(rows * cols);
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

	int rows() {
		return rows_;
	}

	int cols() {
		return cols_;
	}

	std::vector<T> & data()
	{
		return data_;
	}

	char*  raw_data(){

		return reinterpret_cast<char*>(&data_[0]);
	}

};

using rgba = std::array<uint8_t, 4>;

bool load_pam(std::string &filename, mat<rgba>& img)
{
	std::ifstream is(filename, std::ios::binary);
	if (!is)
	{
		return false;
	}
	std::string magic_number, tupltype, endhdr, line,token;
	int rows=0, cols=0, depth=0, maxval;

	std::getline(is,magic_number);

	if (magic_number != "P7")
	{
		return false;
	}
	
	while (1)
	{
		std::getline(is, line);
		std::stringstream ss(line);
		ss >> token;

			if (token == "WIDTH")
			{
				ss >> cols;
			}
			else if (token == "HEIGHT")
			{
				ss >> rows;
			}
			else if (token == "DEPTH")
			{
				ss >> depth;
				if (depth != 3 && depth != 4)
				{
					return false;
				}
			}
			else if (token == "MAXVAL")
			{
				ss >> maxval;
				if (maxval != 255)
				{
					return false;
				}
			}
			else if (token == "TUPLTYPE")
			{
				ss >> tupltype;

				if (tupltype != "RGB_ALPHA" && tupltype != "RGB")
				{
					return false;
				}
			}
			else if (token == "ENDHDR")
			{
				break;
			}
			else
			{
				return false;
			}

	}
	img.resize(rows, cols);

	if (depth == 3)
	{
		for (int r = 0; r < img.rows(); r++)
		{
			for (int c = 0; c < img.cols(); ++c)
			{

				for (int i = 0; i < depth; ++i)
				{
					img(r, c)[i] = is.get();
				}
				img(r, c)[3] = 255;
			}
		}
	}
	else
	{
		for (int r = 0; r < img.rows(); r++)
		{
			for (int c = 0; c < img.cols(); ++c)
			{

				for (int i = 0; i < depth; ++i)
				{
					img(r, c)[i] = is.get();
				}
			}
		}

	}


	return true;


}


struct point {

	int x = 0;
	int y = 0;
};


std::pair<int, int> find_dim(std::vector<std::pair<std::string, point>>& files, std::vector<std::pair<std::string, mat<rgba>>>& images) {


	std::pair<int, int> dim = { 0,0 };
	for (int i = 0; i < files.size(); ++i)
	{
		if ((files[i].second.y + images[i].second.rows()) > dim.first)
		{
			dim.first= files[i].second.y + images[i].second.rows();
		}

		if ((files[i].second.x + images[i].second.cols()) > dim.second){

			dim.second = files[i].second.x + images[i].second.cols();
		}
	}

	return dim;

}

template<typename T>
class File
{
	mat<T> img_;
	std::string filename_;
	point position_ = { 0,0 };

public:


	File(std::string name, mat<T> im, point p)
	{
		filename_ = name;
		position_ = p;
		img_ = im;

	}

	mat<T>& img()
	{
		return img_;
	}


	std::string& filename()
	{
		return filename_;
	}

	point& position()
	{
		return position_;
	}

};



std::pair<int, int> find_dim(std::vector<File<rgba>> & images) {


	std::pair<int, int> dim = { 0,0 };
	for (int i = 0; i < images.size(); ++i)
	{
		if ((images[i].position().y + images[i].img().rows()) > dim.first)
		{
			dim.first = images[i].position().y + images[i].img().rows();
		}

		if ((images[i].position().x + images[i].img().cols()) > dim.second) {

			dim.second = images[i].position().x + images[i].img().cols();
		}
	}

	return dim;

}





void alpha_compositing(mat<rgba>& B, mat<rgba>& A, point &coord) //A sopra B-> A in primo piano, B secondo piano
{
	
	int8_t alpha_o;
	uint8_t alpha_a, alpha_b, val;
	int rows, cols;

	for (int  r = 0; r < A.rows(); r++)
	{
		for (int  c = 0; c < A.cols(); c++)
		{
			rows = r + coord.y;
			cols = c + coord.x;
			alpha_a = A(r,c)[3];
			alpha_b = B(rows,cols )[3];
			alpha_o = alpha_a + alpha_b * (1 - alpha_a);

			for (int i = 0; i < 3; ++i)
			{
				val = (A(r, c)[i] * alpha_a + B(rows, cols)[i] * alpha_b * (1 - alpha_a)) / alpha_o;
				B(rows, cols)[i] =val;
			}
			B(rows, cols)[3] = alpha_o;

		}
	}

	return;
}

bool save_pam_original(std::string fout, mat<rgba>& img)
{
	std::ofstream os(fout, std::ios::binary);

	if (!os)
	{
		return false;
	}

	os << "P7\n";
	os << "WIDTH " + std::to_string(img.cols()) + "\n";
	os << "HEIGHT " + std::to_string(img.rows()) + "\n";
	os << "DEPTH 4\n";
	os << "MAXVAL 255\n";
	os << "TUPLTYPE RGB_ALPHA\n";
	os << "ENDHDR\n";
	uint8_t val;
	for (int r = 0; r < img.rows(); r++)
	{

		for (int c = 0; c < img.cols(); c++)
		{
			for (int i = 0; i < 3; ++i)
			{
				val = img(r, c)[i];
				os.put(val);
			}
			
			
		}
	}

	return true;


}



bool save_pam(std::string fout,mat<rgba>& img)
{
	std::ofstream os(fout, std::ios::binary);

	if (!os)
	{
		return false;
	}

	os << "P7\n";
	os << "WIDTH " +  std::to_string(img.cols()) + "\n";
	os << "HEIGHT " +  std::to_string(img.rows()) + "\n";
	os << "DEPTH 4\n";
	os << "MAXVAL 255\n";
	os << "TUPLTYPE RGB_ALPHA\n";
	os << "ENDHDR\n";
	uint8_t val;
	for (int r = 0; r < img.rows(); r++)
	{

		for (int  c = 0; c < img.cols(); c++)
		{
			for (int i = 0; i < 4; ++i)
			{
				val = img(r, c)[i];
				os.put(val);
			}
		}
	}

	return true;








}




int main(int argc, char** argv)
{

	if (argc < 4)
	{
		return 1;
	}
	std::string outfile = argv[1];
	outfile += ".pam";
	std::vector<File<rgba>> images;

	for (int i = 2; i < argc; ++i)
	{
		struct point p;
		std::string file = argv[i];
		if (file == "-p")
		{

			++i;
			p.x = atoi(argv[i]);
			++i;
			p.y = atoi(argv[i]);
			++i;
			file = argv[i];
			file += ".pam";
			File<rgba> im(file,mat<rgba>(), p);
			images.push_back(im);
			

		}
		else
		{
			file += ".pam";
			File<rgba> im(file, mat<rgba>(), p);
			images.push_back(im);
			
		}
	}


	for (int  i = 0; i < images.size(); i++)
	{
		load_pam(images[i].filename(), images[i].img());
	}

	std::pair<int, int> dim = find_dim(images);
	mat<rgba> output(dim.first, dim.second);
	
	for (int i = 0; i < images.size(); ++i)
	{
		alpha_compositing(output, images[i].img(), images[i].position());

	}

	bool res = save_pam(outfile, output);

	if (res)
	{
		return !res;
	}

	return 1; 

	



	
}