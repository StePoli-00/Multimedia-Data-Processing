#include <fstream>
#include <iostream>
#include <cstdio>
#include <ios>
#include <map>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <deque>
#include <queue>

struct tripla {
	char simbolo_;
	uint8_t len_;
	std::vector<uint8_t> code_;

	tripla() {}
};

struct node {
	float val_;
	char car_;
	int number_;
	node* left_;
	node* right_;
	uint8_t bit_;

	node() {
		val_ = NULL;
		left_ = NULL;
		right_ = NULL;
		bit_ = NULL;
		number_ = 0;
		car_ = NULL;
	}
};

int compare_probab(node& a, node& b) {
	if (a.val_ > b.val_) {
		return  1;
	}
	else if (a.val_ < b.val_) {
		return -1;
	}
	else {
		return 0;
	}
}

template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}

class bitwriter {
	uint8_t buffer_;
	int n_ = 0;
	std::ostream& os_;

	std::ostream& write_bit(uint32_t bit) {
		buffer_ = buffer_ << 1 | (bit & 1);
		++n_;
		if (n_ == 8) {
			raw_write(os_, buffer_);
			n_ = 0;
		}
		return os_;
	}

public:
	bitwriter(std::ostream& os) : os_(os) {
	}

	std::ostream& write(uint32_t u, uint8_t n) {
		for (int i = n - 1; i >= 0; --i) {
			write_bit(u >> i);
		}
		return os_;
	}

	std::ostream& operator()(uint32_t u, uint8_t n) {
		return write(u, n);
	}

	std::ostream& flush(uint32_t bit = 0) {
		while (n_ > 0) {
			write_bit(bit);
		}
		return os_;
	}

	~bitwriter() {
		flush();
	}
};

template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&val), size);
}

class bitreader {
	uint8_t buffer_;
	uint8_t n_ = 0;
	std::istream& is_;

public:
	bitreader(std::istream& is) : is_(is) {}

	uint32_t read_bit() {
		if (n_ == 0) {
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		return (buffer_ >> n_) & 1;
	}

	uint32_t read(uint8_t n) {
		uint32_t u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return u;
	}

	uint32_t operator()(uint8_t n) {
		return read(n);
	}

	bool fail() const {
		return is_.fail();
	}

	explicit operator bool() const {
		return !fail();
	}
};


bool is_leaf(node* curr) {
	if (curr->left_ == NULL && curr->right_ == NULL) {
		return true;
	}
	else {
		return false;
	}
}

void counting_entries(node* root, uint8_t* count) {
	if (root->left_ == NULL && root->right_ == NULL) {
		*count += 1;
	}
	else {
		counting_entries(root->left_, count);
		counting_entries(root->right_, count);
	}
	return;
}

void generate_triple(node* root, std::vector<uint8_t> buf, std::vector<tripla>& triple, uint32_t& count, int n) {
	if (is_leaf(root)) {
		tripla tmp;
		count += root->number_;
		buf.push_back(root->bit_);
		tmp.simbolo_ = root->car_;
		tmp.len_ = (uint8_t)buf.size();
		for (auto& c : buf) {
			tmp.code_.push_back(c);
		}
		triple.push_back(tmp);
		buf.pop_back();
		return;
	}
	else {
		if (n != 0 && !is_leaf(root)) {
			buf.push_back(root->bit_);
		}
	}
	generate_triple(root->left_, buf, triple, count, n + 1);
	generate_triple(root->right_, buf, triple, count, n + 1);
}

void set_bit(node* root) {
	if (root->left_ == NULL && root->right_ == NULL) {
		return;
	}
	else {
		if (root->left_) {
			root->left_->bit_ = 1;
		}
		if (root->right_) {
			root->right_->bit_ = 0;
		}
		set_bit(root->left_);
		set_bit(root->right_);
	}
}

node* generate_tree(std::ifstream& fin) {

	bitreader br(fin);
	float occ[256] = { 0 };
	uint32_t n_tot = 0;
	while (1) {
		uint8_t read = br.read(8);
		if (br.fail()) {
			break;
		}
		++n_tot;
		occ[read] += 1;
	}

	std::vector <node> leaf;

	for (int i = 0; i < 256; ++i) {
		if (occ[i] != 0) {
			node tmp;
			tmp.number_ = int(occ[i]);
			tmp.car_ = char(i);
			occ[i] = occ[i] / n_tot;
			tmp.val_ = occ[i];
			leaf.push_back(tmp);
		}
	}

	std::sort(leaf.begin(), leaf.end(), [](node a, node b) { return b.val_ < a.val_; });

	std::deque<node*> coda;

	for (auto& x : leaf) {
		coda.push_back(new node(x));
	}

	float probabilities = 0;
	size_t n = coda.size();
	while (1) {
		node* left = coda.back();
		probabilities = coda.back()->val_;
		coda.pop_back();
		probabilities += coda.back()->val_;
		node* right = NULL;
		right = coda.back();
		coda.pop_back();
		node* parent = new node();
		parent->val_ = probabilities;
		parent->left_ = left;
		parent->right_ = right;
		parent->car_ = NULL;
		parent->number_ = 0;
		coda.push_back(parent);
		if (probabilities == 1.0) {
			break;
		}
	}

	return coda.front();
}

int decode(char* argc[]) {
	std::ifstream fin(argc[2], std::ios::binary);
	std::ofstream fout(argc[3]);
	if (!fin || !fout) {
		std::cout << "Error opening files.";
		return 1;
	}

	bitreader br(fin);
	std::string magic_number;
	char c;
	for (int i = 0; i < 8; ++i) {
		fin.read(&c, 1);
		magic_number += c;
	}
	uint8_t table_entries;
	fin.read(reinterpret_cast <char*> (&table_entries), 1);
	int n = 0;
	std::vector<tripla> read_triple;
	while (1) {
		if (n == table_entries) {
			break;
		}
		tripla tmp;
		tmp.simbolo_ = br.read(8);
		uint8_t len = br.read(5);
		tmp.len_ = len;
		for (size_t i = 0; i < len; ++i) {
			tmp.code_.push_back(br.read(1));
		}
		read_triple.push_back(tmp);
		++n;
	}
	uint32_t num_symbol = br.read(32);
	std::map<std::string, char> bind;
	for (auto& x : read_triple) {
		std::string key;
		for (auto& c : x.code_) {
			key += (char)c;
		}
		bind[key] = x.simbolo_;
	}
	int count = 0;
	std::map<std::string, char>::iterator it;
	uint8_t clean = br.read(1);
	std::string tmp;
	while (1) {
		if (count == num_symbol) {
			break;
		}
		uint8_t r = br.read(1);
		tmp += (char)r;
		it = bind.find(tmp);
		if (it != bind.end()) {
			fout.write(&it->second, 1);
			tmp.clear();
			++count;
		}
	}

	return 0;
}

int code(char* argc[]) {
	std::ifstream fin(argc[2]);
	std::ofstream fout(argc[3], std::ios::binary);
	if (!fin) {
		std::cout << "Error opening files.";
		return 1;
	}
	if (!fout) {
		std::cout << "Error opening output file.";
		return 1;
	}

	std::string magic_number = "HUFFMAN1";
	bitwriter bw(fout);
	for (auto& c : magic_number) {
		bw.write(c, 8);
	}
	node* tree = generate_tree(fin);
	uint8_t entries = 0;
	counting_entries(tree, &entries);
	bw.write(entries, 8);
	set_bit(tree);
	uint32_t counting = 0;
	std::vector<uint8_t> buffer;
	std::vector<tripla> triple;
	generate_triple(tree, buffer, triple, counting, 0);
	for (auto& tripla : triple) {
		bw.write(tripla.simbolo_, 8);
		bw.write(tripla.len_, 5);
		for (auto& x : tripla.code_) {
			bw.write(x, 1);
		}
	}
	bw.write(counting, 32);
	bw.flush();
	std::ifstream sin(argc[2]);
	bitwriter bt(fout);
	while (1) {
		char c;
		sin.read(&c, 1);
		if (!sin) {
			break;
		}
		for (auto& item : triple) {
			if (item.simbolo_ == c) {
				for (auto& x : item.code_) {
					bt.write(x, 1);
				}
			}
		}
	}
	return 0;
}

int main(int argc, char* argv[]) {

	if (argc != 4) {
		std::cout << "Error number of parameters.";
		return 1;
	}

	if (*argv[1] != 'c' && *argv[1] != 'd') {
		std::cout << "Error first parameter must be 'c' or 'd'.";
		return 1;
	}

	if (*argv[1] == 'c') {
		int res = code(argv);
		return res;
	}
	else {
		int res = decode(argv);
		return res;
	}

	return 0;
}