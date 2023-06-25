#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <array>
#include <cstdint>

template <typename T>
struct matrix {
	int rows_, cols_;
	std::vector<T> data_;

	matrix(int rows = 0, int cols = 0) : rows_(rows), cols_(cols), data_(rows* cols) {}

	void resize(int rows, int cols) {
		rows_ = rows;
		cols_ = cols;
		data_.resize(rows * cols);
	}

	const T& at(int r, int c) const {
		assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		return data_[r * cols_ + c];
	}
	T& at(int r, int c) {
		return const_cast<T&>(static_cast<const matrix*>(this)->at(r, c));
	}

	const T& operator()(int r, int c) const {
		assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		return data_[r * cols_ + c];
	}
	T& operator()(int r, int c) {
		assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		return data_[r * cols_ + c];
	}

	int rows() const { return rows_; }
	int cols() const { return cols_; }
	int size() const { return rows_ * cols_; }

	size_t raw_size() const {
		return rows_ * cols_ * sizeof(T);
	}
	const char* raw_data() const {
		return reinterpret_cast<const char*>(&data_[0]);
	}
};


struct tuple_type{

	int size_;
	std::array<int, 3> tuple_;
};

struct pam_header {
	
	std::string magic_number_;
	int width_, height_, depth_,maxval_;
	std::string tupltype_,endhdr_;
};


void  print_header(const pam_header &ph,std::ofstream& os)
{
	std::vector<std::string> fields = { "WIDTH","HEIGHT","DEPTH","MAXVAL","TUPLTYPE","ENDHDR" };
	std::vector<int> value = { ph.width_,ph.height_,ph.depth_,ph.maxval_ };
	char space = 32, line = 10;

	for (auto& c : ph.magic_number_)
	{
		os << c;
	}
	os.write(&line, 1);

	for (int i = 0; i < fields.size() - 2; ++i)
	{
		for (auto& c : fields[i])
		{
			os << c;
		}
		os.write(&space, 1);
		os << value[i];
		os.write(&line, 1);

	}

	for (auto& c : fields[4])
	{
		os << c;
	}
	os.write(&space, 1);

	for (auto& c : ph.tupltype_)
	{
		os << c;
	}
	os.write(&line, 1);
	for (auto& c : fields[5])
	{
		os << c;
	}
	os.write(&line, 1);
	return;
}

template <typename T>
bool load_pam(matrix<T> &img,const std::string &filein,pam_header &ph)
{
	std::ifstream is(filein, std::ios::binary);
	if (!is)
	{
		return false;
	}
	std::string magic_number;
	std::getline(is, magic_number);
	if (magic_number != "P7")
	{
		return false;
	}
	ph.magic_number_ = magic_number;
	while (1) {
		std::string line;
		std::getline(is, line);
		if (line == "ENDHDR") {
			ph.endhdr_ = line;
			break;
		}
		std::stringstream ss(line);
		std::string token;
		ss >> token;
		if (token == "WIDTH") {
			ss >> ph.width_;
		}
		else if (token == "HEIGHT") {
			ss >> ph.height_;
		}
		else if (token == "DEPTH") {
			
			ss >> ph.depth_;
			if (ph.depth_ != 1) {
				return false;
			}
		}
		else if (token == "MAXVAL") {
			ss >> ph.maxval_;
			if (ph.maxval_ != 255) {
				return false;
			}
		}
		else if (token == "TUPLTYPE") {
			
			ss >> ph.tupltype_;
			if (ph.tupltype_ != "GRAYSCALE") {
				return false;
			}
		}
		else {
			return false;
		}
	}
	std::vector<T> v;
	img.resize(ph.height_, ph.width_);
	for (int r = 0; r < img.rows(); ++r) {
		for (int c = 0; c < img.cols(); ++c) {

			
			for (int  i = 0; i < img.data_.size(); i++)
			{
				v.push_back(is.get());
			}

			img(r, c) = v;
		}
	}
	return true;
}

template <typename T>
matrix<T> flipped(const matrix<T> &img) {

	int k = 0,j;
	matrix<uint8_t> img_flipped(img.rows(),img.cols());
	for (int r = img.rows()-1; r >= 0; --r)
	{
		j = 0;
		for (int c = img.cols()-1; c >=0; --c)
		{
			img_flipped(k, j) = img(r, c);
			++j;
		}
		++k;
	}

	return img_flipped;
	
}
template<typename T>
void print_matrix(const matrix<T> &img, const pam_header &ph )
{
	std::ofstream os("flipped.pam", std::ios::binary);
	print_header(ph, os);

	for (int r = 0; r < img.rows(); r++)
	{
		for (int  c = 0; c < img.cols(); c++)
		{
			os.put(img(r, c));
		}
	}
	return;
}

int main(int argc, char**argv)
{
	matrix<tuple_type> img;
	pam_header ph;
	int res=load_pam(img,argv[1],ph);
	matrix<tuple_type> flip=flipped(img);
	print_matrix(flip,ph);


}



