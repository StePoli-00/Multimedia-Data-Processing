#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>
#include <array>

/*consiglio di grana per vertical symmetry  r*cols+c-1
classe array-> wrap degli array del c */
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

struct pam_header {

	std::string magic_number_;
	int width_, height_, depth_, maxval_;
	std::string tupltype_, endhdr_;




};


struct RGB {
	std::array<int, 3> tuple;
};

void  print_header(const pam_header& ph, std::ofstream& os)
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

bool load_pam(matrix<RGB>& img, const std::string& filein, pam_header& ph)
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
			if (ph.depth_ != 3) {
				return false;
			}
		}
		else if (token == "MAXVAL") {
			ss >> ph.maxval_;
			/*if (ph.maxval_ != 255) {
				return false;
			}*/
		}
		else if (token == "TUPLTYPE") {

			ss >> ph.tupltype_;
			/*if (ph.tupltype_ != "GRAYSCALE") {
				return false;
			}*/
		}
		else {
			return false;
		}
	}
	RGB rgb;
	img.resize(ph.height_, ph.width_);
	for (int r = 0; r < img.rows(); ++r) {
		for (int c = 0; c < img.cols(); ++c) {

			
			for (int  i = 0; i < 3; i++)
			{
				//rgb.tuple.push_back(is.get());
				rgb.tuple[i] = is.get();
			}
			img(r, c) = rgb;
			
			
		

		}
	}
	return true;
}

template< typename T>
matrix<T> mirror(const matrix<T>& img)
{
	matrix<T> mirrored(img.rows(),img.cols());
	int k=0, j;
	for (int  r = 0; r < img.rows(); r++)
	{
		j = 0;
		for (int c = img.cols() - 1; c >= 0; --c)
		{
			
			mirrored(k, j) = img(r, c);
			++j;
		}
		++k;
	}

	
	return mirrored;
}

template< typename T>
void write_matrix(const matrix<T> &img,const pam_header &ph, std::string fout)
{
	std::ofstream os(fout, std::ios::binary);
	print_header(ph, os);
	int n = img.data_[0].tuple.size();
	for (int  r = 0; r < img.rows(); r++)
	{
		for (int  c = 0; c < img.cols(); c++)
		{
			for (int  i = 0; i < n; i++)
			{
				os.put(img(r, c).tuple[i]);
			}
		}
	}
	
	return; 


}
template< typename T>
matrix<T> vertical_symmetry(const matrix<T>& img)
{

	matrix<T> img2(img.rows(),img.cols());
	int half_row = img.rows() / 2;
	int half_column = (img.cols() / 2);
	int k = 0, j;
	for (int r = 0; r < img.rows(); ++r)
	{
		j = 0;
		for (int c = 0; c < half_column; ++c)
		{	
			img2(k, j) = img(r, c);
			++j;
		}
		++k;
		
	}
	k = 0;
	for (int r = 0; r < img.rows(); ++r)
	{
		j =0;
		for (int c =(img.cols() - 1); c>=0; --c)
		{
			img2(r,c) = img2(k, j);
			++j;
		}
		++k;
	}

	return img2;
}
int main(int argc, char** argv)
{
	matrix<RGB> img;
	pam_header ph;
	if (argc != 3)
	{
		std::cout << "Error number incorrect of parameters";
		return -1;
	}
	bool res = load_pam(img, argv[1], ph);
	if (!res)
	{
		std::cerr << "error loading image";
	}

	matrix<RGB> mirrored=mirror(img);
	write_matrix(mirrored,ph,argv[2]);
	matrix<RGB> img2= vertical_symmetry(img);
	write_matrix(img2, ph, "vertical_symmetry.pam");
	return 0;

}
