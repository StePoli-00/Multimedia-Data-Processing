#include <array>
#include <cassert>
#include <fstream>
#include <vector>
#include <sstream>

struct RGB {
	std::array<int, 3> tuple;
};

struct pam_header {

	std::string magic_number_;
	int width_, height_, depth_, maxval_;
	std::string tupltype_, endhdr_;


};

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


bool load_pam( std::ifstream &is, pam_header& ph)
{
	
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
		else if (token =="MAXVAL") {
			ss >> ph.maxval_;
		}
		else if (token == "TUPLTYPE") {

			ss >> ph.tupltype_;

		}
		else {
			return false;
		}
	}
	return true;
}



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


void load_image(std::ifstream& is, matrix<uint8_t>& im)
{
	for (int r = 0; r < im.rows(); r++)
	{
		for (int c = 0; c < im.cols(); c++)
		{
			im(r,c)=is.get();
		}
	}

}

void save_image(pam_header const& ph, matrix<RGB>& img, std::string fout)
{
	std::ofstream os(fout, std::ios::binary);
	print_header(ph, os);
	int n = 3;
	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			for (int i = 0; i < n; i++)
			{
				
				os.put(img(r, c).tuple[i]);
			}
		}
	}
}

void save_image(pam_header const& ph, matrix<uint8_t>& img, std::string fout)
{

	std::ofstream os(fout, std::ios::binary);
	print_header(ph, os);
	int n = 1;
	for (int r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			for (int i = 0; i < n; i++)
			{
				os.put(img(r, c));
			}
		}
	}

	return;
}


matrix<RGB> combine(std::vector < matrix<uint8_t>>&images)
{
	matrix<RGB> im;
	im.resize(images[0].rows(), images[0].cols());
	int i = 0;
	for (int  r = 0; r < im.rows() ; r++)
	{
		for (int  c = 0; c < im.cols(); c++)
		{
			im(r, c).tuple[0] = images[0](r, c);
			im(r, c).tuple[1] = images[1](r, c);
			im(r, c).tuple[2] = images[2](r, c);
		}
	}

	return im;
}

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		return -1;
	}
	std::string filename = argv[1];
	std::vector < std::string > names= {(filename + "_R.pam"), (filename + "_G.pam"), (filename + "_B.pam")};
	pam_header ph;
	std::vector<matrix<uint8_t>> images(3);

	for (int i = 0; i < 3; ++i)
	{

		std::ifstream is(names[i], std::ios::binary);
		if (!is)
		{
			return -1;
		}
		load_pam(is, ph);
		images[i].resize(ph.height_, ph.width_);
		load_image(is,images[i]);	


		
	}
	
	matrix<RGB> reconstr = combine(images);
	std::string fout = filename + "_reconstructed.pam";
	ph.depth_ = 3;
	ph.tupltype_ = "RGB";
	save_image(ph, reconstr, fout);


	
	
	


	
}