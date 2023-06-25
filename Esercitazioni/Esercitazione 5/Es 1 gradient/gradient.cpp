#include <string>
#include <fstream>
#include <vector>
#include <cassert>
template <typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}

template <typename T>
struct matrix {
	int rows_, cols_;
	std::vector<T> data_;
	matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows* cols) {};

	T& operator() (int r, int c)
	{
		assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		return data_[r * cols_ + c];
	}

	int rows() const
	{
		return rows_;
	}
	int cols() const
	{
		return cols_;
	}
	int size() const
	{
		return rows_ * cols_;
	}
};


class PAM_HEADER {
		
	std::string magic_number_;
	int width_;
	int height_;
	int depth_;
	int maxval_=255;
	std::string tupltype_;
	std::string ENDHDR_="ENDHDR";

public:
	PAM_HEADER(std::string magic_number,
		int width,
		int height,
		int depth,
		int maxval,
		std::string tupltype,
		
		std::string  ENDHDR)
	{
		this->magic_number_ = magic_number;
		this->width_= width;
		this->height_= height;
		this->depth_=depth;
		this->maxval_ = maxval;
		this->tupltype_ = tupltype;
		this->ENDHDR_ = ENDHDR;

	}

	std::string getMagic_number()
	{
		return this->magic_number_;
	}


	int getWidth()
	{
		return this->width_;
	}

	int getHeight()
	{
		return this->height_;
	}

	int getDepth()
	{
		return this->depth_;
	}

	int getMaxVal()
	{
		return this->maxval_;
	}

	std::string getTuplType()
	{
		return this->tupltype_;
	}

	std::string getENDHR()
	{
		return this->ENDHDR_;
	}


	void print_header(std::ostream& os)
	{
		std::vector<std::string> fields = { "WIDTH","HEIGHT","DEPTH","MAXVAL","TUPLTYPE","ENDHDR" };
		std::vector<int> value = { width_,height_,depth_,maxval_ };
		char space = 32, line = 10;

		for (auto& c : getMagic_number())
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
		
			for (auto& c :fields[4])
			{
				os << c;
			}
			os.write(&space, 1);

			for (auto& c : tupltype_)
			{
				os << c;
			}
			os.write(&line, 1);
			for (auto& c : fields[5])
			{
				os << c;
			}
			/*os.write(&space, 1);
			for (auto& c : ENDHDR_)
			{
				os << c;
			}*/
			os.write(&line, 1);
		return ;
	}

	

};

int create_gradient(char *fileout)
{
	std::ofstream os(fileout, std::ios::binary);
	if (!os)
	{
		return -1;
	}
	matrix<uint8_t> img(256, 256);
	for (int r = 0; r < img.rows(); ++r)
	{
		for (int c = 0; c < img.cols(); c++)
		{
			img(r, c) = r;
		}
	}
	PAM_HEADER ph("P7", 256, 256, 1, 255, "GRAYSCALE", "10");
	ph.print_header(os);
	for (int r = 0; r < img.rows(); ++r)
	{
		for (int c = 0; c < img.cols(); ++c) {

			os.put(img(r, c));

		}
	}

	
	return 0;

	
}



int main(int argc, char** argv)
{
	

	create_gradient(argv[1]);



}