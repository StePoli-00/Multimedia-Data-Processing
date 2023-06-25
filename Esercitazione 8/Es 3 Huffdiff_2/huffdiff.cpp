#include <array>
#include <cassert>
#include <fstream>
#include <vector>
#include <sstream>
#include <bitset>
#include <iterator>
#include <unordered_map>
#include <algorithm>


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
	auto begin() { return data_.begin(); }
	auto begin() const { return data_.begin(); }
	auto end() { return data_.end(); }
	auto end() const { return data_.end(); }
};
bool load_pam(std::ifstream& is, pam_header& ph)
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
		else if (token == "MAXVAL") {
			ss >> ph.maxval_;
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
	return true;


}

template<typename T>
std::ostream& raw_write(std::ostream& os, const T& num, size_t size = sizeof(T))
{
	return os.write(reinterpret_cast<const char*>(&num), size);
}
class bitwriter {
	std::ostream& os_;
	uint8_t buffer_;
	size_t nbits_;

	std::ostream& write_bit(uint32_t u) {
		// buffer_ = buffer_ * 2 + u % 2;
		buffer_ = (buffer_ << 1) | (u & 1);
		++nbits_;
		if (nbits_ == 8) {
			raw_write(os_, buffer_);
			nbits_ = 0;
		}
		return os_;
	}

public:
	bitwriter(std::ostream& os) : os_(os), nbits_(0) {}

	~bitwriter() {
		flush();
	}

	std::ostream& write(uint32_t u, size_t n) {
		while (n-- > 0) {
			write_bit(u >> n);
		}
		return os_;
	}

	std::ostream& operator()(uint32_t u, size_t n) {
		return write(u, n);
	}

	void flush(uint32_t u = 0) {
		while (nbits_ > 0) {
			write_bit(u);
		}
	}
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
void load_image(std::ifstream& is, matrix<uint8_t>& im)
{
	for (int r = 0; r < im.rows(); r++)
	{
		for (int c = 0; c < im.cols(); c++)
		{
			im(r, c) = is.get();
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


matrix<int16_t> calcolate_diff(matrix<uint8_t>  const& im)
{
	int i = 0;
	matrix<int16_t> diff(im.rows(), im.cols());
	for (int r = 0; r < im.rows(); r++)
	{
		
		/*element D(0,0) is the value of pixel I(0,0)
			elements D(0,y) are computed as I(0,y)-I(0,y-1), with y>0
			elements D(x,y) are computed as I(x,y)-I(x-1,y), with x>0

		*/
		for (int  c = 0; c < im.cols(); c++)
		{
			if (r == 0 && c == 0)
			{
				diff(r, c) = im(r, c);
			}
			else if (r==i && c==0)
			{
				diff(r, c) = im(r, c) - im(r - 1, c);
			}
			else if(c!=0)
			{
				diff(r, c) = im(r, c) - im(r, c - 1);
				
			}
		}
		++i;
	}
	return diff;
}



class node {
public:
	std::pair<int16_t,double> pair_;
	uint8_t len_ = 0;
	node* left_ = NULL;
	node* right_ = NULL;

	node(std::pair<int16_t,double> pair, node* l, node *r)
	{
		pair_ = pair;
		left_ = l;
		right_ = r;
	}

	node(){}
	node(std::pair<int16_t,double> pair)
	{
		pair_ = pair;
	}
};

struct Code {

	int16_t sym_;
	uint32_t len_, val_;


};


bool compare_by_value(node  *a, node  *b)
{
	//ordinamento crescente
	return a->pair_.second > b->pair_.second;
}
bool compare_by_lenght(const Code& a, const Code& b)
{
	return a.len_ < b.len_;
}





void count_lenght(std::vector<Code> &codes,node* node, int lenght)
{
	if (node->left_ == NULL && node->right_ == NULL)
	{
		
		Code code;
		code.len_ = lenght;
		code.sym_ = node->pair_.first;
		code.val_ = 0;
		 codes.push_back(code);
		return;
	}

	count_lenght(codes,node->left_, lenght + 1);
	count_lenght(codes,node->right_, lenght + 1);
}


int encode_diff(matrix<int16_t> const& diff, std::string& fout)
{
	
	std::unordered_map<int16_t, double> prob;
	double sum=0;
	//calcolo occorrenze
	for (int r = 0; r < diff.rows(); r++)
	{
		for (int c = 0; c < diff.cols(); c++)
		{
			prob[diff(r, c)] += 1;
		}
	}
	//trasformazione delle occorrenze in probabilità
	for (auto& x : prob) {

		 x.second/=diff.size();
	}
	std::vector<node*> tree;
	for (auto& x : prob)
	{
		std::pair<int16_t, double> pair;
		pair.first = x.first;
		pair.second = x.second;
		tree.push_back(new node(pair));
	}

	//ordinamento della mappa per probabilità decrescente
	//std::vector<std::pair<int16_t,double>> vectorMap(prob.begin(), prob.end());
	//
	// Da chiedere 
	// while (tree.size() != 1)
	//{
	//	
	//	std::sort(tree.begin(), tree.end(), compare_by_value);
	//	node tmp;
	//	tmp.left_ = tree.back();
	//	tree.pop_back();
	//	tmp.right_ = tree.back();
	//	tree.pop_back();
	//	tmp.pair_.second = tmp.right_->pair_.second + tmp.left_->pair_.second;
	//	tmp.pair_.first = 0;
	//	tree.push_back(new node(tmp));
	//}

	while (tree.size() != 1)
	{

		std::sort(tree.begin(), tree.end(), compare_by_value);
		node tmp;
		node *left_ = tree.back();
		tree.pop_back();
		node *right_ = tree.back();
		tree.pop_back();
		double p = right_->pair_.second + left_->pair_.second;
		//300 valore noto per sapere il nodo non è una foglia
		std::pair<int16_t, double> pair(0, p);
		tree.push_back(new node(pair,left_,right_));
	}
	std::vector<Code> codes;
	count_lenght(codes, tree[0], 0);
	std::sort(codes.begin(), codes.end(),compare_by_lenght);
	Code curr = { 0,0,0 };
	for (auto& x : codes)
	{
		x.val_ = curr.val_ <<= x.len_ - curr.len_;
		curr.len_ = x.len_;
		++curr.val_;
	}

	std::ofstream os(fout, std::ios::binary);
	if (!os)
	{
		return -1;
	}
	os << "HUFFDIFF";
	uint32_t width = diff.cols();
	uint32_t height = diff.rows();
	os << width;
	os << height;
	bitwriter bw(os);
	bw.write(static_cast<uint32_t>(codes.size()), 9);
	for (auto& x : codes)
	{
		bw.write(x.sym_, 9);
		bw.write(x.len_, 5);
	}
	// ARRIVATO FINO A QUI
	std::unordered_map<int16_t, Code> huffman_table;
	for (auto& x : codes)
	{
		huffman_table[x.sym_] = x;
	}

	for (auto& x : diff)
	{
		bw(huffman_table[x].val_, huffman_table[x].len_);
	}




	return 0;
}



matrix<std::string> calcolate_diff(matrix<std::string>  const& im)
{
	int i = 0;
	matrix<std::string> diff(im.rows(), im.cols());
	for (int r = 0; r < im.rows(); r++)
	{
		for (int c = 0; c < im.cols(); c++)
		{
			if (r == 0 && c == 0)
			{
				diff(r, c) = im(r, c);
			}
			else if (r == i && c == 0)
			{
				diff(r, c) = im(r, c)+"-"+im(r - 1, c);
			}
			else 
			{
				diff(r, c) = im(r, c) + "-" + im(r, c - 1);

			}
		}
		++i;
	}
	return diff;

}




int main(int argc, char** argv)
{
	if (argc != 4)
	{
		return -1;
	}
	if (*argv[1] == 'c')
	{
		std::ifstream  is(argv[2],std::ios::binary);
		pam_header ph;
		
		matrix<int16_t> diff;
		if (!load_pam(is, ph)) {
			return false;
		}
		matrix<uint8_t> im(ph.height_, ph.width_);
		load_image(is, im);
		diff = calcolate_diff(im);
		std::string fout = argv[3];
		int res=encode_diff(diff,fout);
		return res;
		
	}
	else if(*argv[1] == 'd')
	{
		
	}
	else
	{
		return -1;
	}
	

	//matrix<uint8_t> mat(2,4);
	//mat.data_ = { 1, 2, 3, 4, 5, 6, 7, 8 };
	//matrix<std::string> mat2(4, 4);
	///*mat2.data_={"a", "b","c","d","e", "f", "g", "h", "i","j","k","l","m","n","o","p"};
	//matrix<std::string> diff = calcolate_diff(mat2);*/
	//int a;

	
	







}