//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <string.h>
//#include <unordered_map>
//#include <algorithm>
//
//template<typename T>
//std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
//	return is.read(reinterpret_cast<char*>(&val), size);
//}
//
//class bitreader {
//	uint8_t buffer_;
//	uint8_t n_ = 0;
//	std::istream& is_;
//
//public:
//	bitreader(std::istream& is) : is_(is) {}
//
//	uint32_t read_bit() {
//		if (n_ == 0) {
//			raw_read(is_, buffer_);
//			n_ = 8;
//		}
//		--n_;
//		return (buffer_ >> n_) & 1;
//	}
//
//	uint32_t read(uint8_t n) {
//		uint32_t u = 0;
//		while (n-- > 0) {
//			u = (u << 1) | read_bit();
//		}
//		return u;
//	}
//
//	uint32_t operator()(uint8_t n) {
//		return read(n);
//	}
//
//	bool fail() const {
//		return is_.fail();
//	}
//
//	explicit operator bool() const {
//		return !fail();
//	}
//};
//
//template<typename T>
//std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
//	return os.write(reinterpret_cast<const char*>(&val), size);
//}
//class bitwriter {
//	uint8_t buffer_;
//	uint8_t cur_bit_;
//	int n_ = 0;
//	//generico output stream
//	/*teniamo una reference allo stream di output*/
//	std::ostream& os_;
//	//gli stream non prevendono l'assegnamento
//
//
//	std::ostream& write_bit(uint8_t bit)
//	{
//		//scrive il bit meno significativo di BIT
//		buffer_ = (buffer_ << 1) | (bit & 1);
//		++n_;
//		if (n_ == 8) {
//			raw_write(os_, buffer_);
//			n_ = 0;
//		}
//		return os_;
//	}
//
//
//public:
//	bitwriter(std::ostream& os) : os_(os) { }
//	std::ostream& write(uint32_t u, uint8_t n)
//	{
//
//		for (int i = n - 1; i >= 0; --i)
//		{
//			write_bit(u >> i);
//		}
//		return os_;
//	}
//
//
//
//	//alternativa per chiamare la write
//	std::ostream& operator()(uint32_t u, uint8_t n) {
//		return write(u, n);
//
//	}
//
//	std::ostream& flush(uint32_t bit = 0)
//	{
//		while (n_ > 0)
//		{
//			write_bit(bit);
//		}
//		return os_;
//	}
//
//	~bitwriter() {
//		flush();
//	}
//
//};
//
//
//
//struct node {
//
//	char symb_;
//	double prob_;
//	int len_ = 0;
//	node* left_;
//	node* right_;
//
//
//	node(char sym, double prob, int len, node *left, node *right) {
//		symb_ = sym;
//		prob_ = prob;
//		left_ = left;
//		len_ = len;
//		right_ = right;
//	}
//
//};
//
//struct triplets {
//
//	char sym_;
//	uint8_t len_, val_;
//	
//	triplets(char sym, uint8_t len, uint8_t val)
//	{
//		sym_ = sym;
//		len_ = len;
//		val_ = val;
//	}
//	
//};
//
//bool compare_by_prob(const node* a, const node* b)
//{
//	return a->prob_  >b->prob_;
//}
//bool compare_by_length(const triplets& a, const triplets& b)
//{
//	return a.len_ < b.len_;
//}
//void calcolate_length(node*tree,std::vector<triplets> &code,int  len)
//{
//	if (tree->left_ == NULL && tree->right_ == 0)
//	{
//		tree->len_ = len;
//		code.push_back(triplets(tree->symb_, tree->len_, 0));
//		return;
//	}
//
//	calcolate_length(tree->left_, code, len + 1);
//	calcolate_length(tree->right_, code, len + 1);
//
//
//}
//
//
//int compress_by_huff(std::string filein, std::string fileout) {
//	
//	std::ifstream is(filein, std::ios::binary);
//	if (!is)
//	{
//		return 1;
//	}
//	std::unordered_map<char,double> frequencies;
//	bitreader br(is);
//	int sum = 0;
//	std::vector<char> symbol;
//	//memorizzo in posizione del num letto l'occorrenza di tale valore
//	while (1)
//	{
//		int num = br.read(8);
//		if (br.fail())
//		{
//			break;
//		}
//		symbol.push_back(num);
//		frequencies[num] += 1;
//		sum += 1;
//	}
//	//converto in probabilità
//	for (auto& x : frequencies)
//	{
//		x.second /= sum;
//	}
//	//creo l'albero di Huffman
//	std::vector<node*> tree;
//	for (auto& x : frequencies)
//	{
//		tree.push_back(new node(x.first, x.second, 0, NULL, NULL));
//	}
//	//ordino il vettore per probabilità decrescente
//	
//	while (tree.size() != 1)
//	{
//		std::sort(tree.begin(), tree.end(), compare_by_prob);
//		node* left = tree.back();
//		tree.pop_back();
//		node* right = tree.back();
//		tree.pop_back();
//		double prob = left->prob_ + right->prob_;
//		tree.push_back(new node(0, prob, 0, left, right));
//	}
//	std::vector<triplets> codes;
//	calcolate_length(tree[0], codes, 0);
//	std::sort(codes.begin(), codes.end(), compare_by_length);
//	triplets curr = { 0,0,0 };
//	//calcolo i codici canonici
//	for (auto& c : codes)
//	{
//		c.val_ = curr.val_ <<= c.len_ - curr.len_;
//		curr.len_ = c.len_;
//		++curr.val_;
//	}
//	std::ofstream os(fileout, std::ios::binary);
//	bitwriter bw(os);
//	std::string magicnumber = "HUFFMAN2";
//	os << magicnumber;
//	uint8_t entries = codes.size();
//	os << entries;
//	std::sort(codes.begin(), codes.end(), compare_by_length);
//	for (auto& c : codes)
//	{
//		os << c.sym_;
//		bw.write(c.len_, 5);
//	}
//	std::unordered_map<char, triplets>  table;
//	for (auto& c : codes)
//	{
//		table[c.sym_] = c;
//
//	}
//	//codifica dei simboli
//	for (auto& x : symbol)
//	{
//		
//		auto it=table.find(x);
//		if (it!=table.end())
//		{
//			bw.write(it->second.val_, 5);
//		}
//		
//	}
//
//	return 0;
//}
//
//
//
//int main(int argc, char* argv[])
//{
//	if (argc != 4)
//	{
//		std::cout << "Error incorrect number of parameters\n";
//		return 1;
//	}
//
//	
//	std::string filein(argv[2]);
//	std::string fileout(argv[3]);
//	if (strcmp(argv[1],"c")==0)
//	{
//		compress_by_huff(filein,fileout);
//	}
//
//
//
//
//
//
//}
//
