#include <vector>
#include <cassert>
#include <iostream>
template <typename T>
struct mat {

	int rows_, cols_;
	std::vector<T> data_;

	mat(int rows,int cols){
		rows_=rows;
		cols_=cols;
		data_.resize(rows * cols);
	}
	
	void resize(int rows, int cols) { data_.resize(rows * cols); }

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

	size_t raw_size() {
		return (rows_ * cols_ * sizeof(T));
	}

	const char * raw_data()
	{
		return reinterpret_cast<const char*>(&data_[0]);
	}

	int rows() { return rows_; }
	int cols() { return cols_; }
	int data() { return data_.size(); }
};

int main()
{
	 mat<int> img(3,3);
	 
	 for (int r = 0; r < img.rows(); r++)
	 {
		 for (int  c = 0; c < img.cols(); c++)
		 {
			 img(r, c) = c+1;
			 
			 if (c == img.cols() - 1)
			 {
				 std::cout << img(r, c) << "\n";
			 }
			 else
			 {
				 std::cout << img(r, c) << ", ";
			 }
		 }
	 }
}