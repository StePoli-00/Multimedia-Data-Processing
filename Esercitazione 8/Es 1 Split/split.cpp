#include <array>
#include <cassert>
#include <fstream>
#include <vector>
#include <sstream>

struct RGB {
	std::array<int,3> tuple;
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

template <typename T>
bool load_pam(matrix<T>& img, const std::string& filein, pam_header& ph)
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
		}
		else if (token == "TUPLTYPE") {

			ss >> ph.tupltype_;
			
		}
		else {
			return false;
		}
	}
	RGB rgb;
	img.resize(ph.height_, ph.width_);
	for (int r = 0; r < img.rows(); ++r) {
		for (int c = 0; c < img.cols(); ++c) {


			for (int i = 0; i < 3; i++)
			{
				//rgb.tuple.push_back(is.get());
				rgb.tuple[i] = is.get();
			}
			img(r, c) = rgb;

		}
	}
	return true;
}

bool split(matrix <RGB> const &img, matrix <uint8_t>&R, matrix <uint8_t>&G, matrix <uint8_t>&B)
{
	
	

	for (int  r = 0; r < img.rows(); r++)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			std::array<int,3> tuple = img(r, c).tuple;
			R(r, c) = tuple[0];
			G(r, c)= tuple[1];
			B(r, c)= tuple[2];
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

void save_image(pam_header const& ph, matrix<uint8_t>& img, std::string fout)
{
	
		std::ofstream os(fout, std::ios::binary);
		print_header(ph, os);
		int n =1;
		for (int r = 0; r < img.rows(); r++)
		{
			for (int c = 0; c < img.cols(); c++)
			{
				for (int i = 0; i < n; i++)
				{
					 os.put(img(r,c));
				}
			}
		}

		return;
}


int main(int argc, char** argv)
{

	if (argc != 2)
	{
		return -1;
	}
	pam_header ph;
	matrix<RGB> img;
	if (!load_pam(img, argv[1], ph)) {
		return -1;
	}
	pam_header ph2=ph;
	ph2.depth_= 1;
	ph2.tupltype_ = "GRAYSCALE";
	matrix<uint8_t> R(img.rows(), img.cols()), G(img.rows(), img.cols()), B(img.rows(), img.cols());
	if (split(img, R, G, B))
	{
		std::string filein = argv[1];
		std::string fout = filein.substr(0, filein.find('.'));
		std::string filename = fout + "_R.pam";
		save_image(ph2,R, filename);
		filename = fout + "_G.pam";
		save_image(ph2, G,filename);
		filename = fout + "_B.pam";
		save_image(ph2, B,filename);
		return 0;
	}
	return -1;

}