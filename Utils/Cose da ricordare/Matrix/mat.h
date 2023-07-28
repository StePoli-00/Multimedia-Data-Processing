#pragma once
#include <vector>
#include <cassert>
template <typename T>
struct mat {
	int  rows_, cols_;
	std::vector<T> data_;

	//mat(int  rows, int cols): rows_(rows), cols_(cols), data_(rows*cols){} oppure
	mat(int  rows, int cols) {
		rows_ = rows;
		cols_ = cols;
		//data_.resize(rows * cols); oppure
		resize(rows, cols);
	}


	void resize(int rows, int cols)
	{
		data_.resize(rows * cols);
	}

	//const T& at(int r, int c) const {
	//	assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
	//	return data_[r * cols_ + c];
	//}

	 //T& at(int r, int c) {
		////return data_[r * cols_ + c];
		//return const_cast<T&>(static_cast<const mat*>(this)->at(r, c));}

	 const T& operator()(int r, int c) const {
		 assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		 return data_[r * cols_ + c];
		 //return at(r, c);
	 }

	  T& operator()(int r, int c){
		  assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		  return data_[r * cols_ + c];
		  //return at(r, c);
	 }

	  int rows() const{ return rows_; }
	  int cols() const{ return cols_; }
	  int size() const { return rows_ * cols_; }

	  size_t raw_size() const { return rows_ * cols_ * sizeof(T); }

	  const char *raw_data() const {
		  return reinterpret_cast<const char*>(&data_[0]);
	  }
};