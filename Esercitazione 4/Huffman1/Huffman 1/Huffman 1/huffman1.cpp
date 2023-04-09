#include <iostream>
#include <fstream>
#include "string.h"
#include <vector>
#include <algorithm>




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

template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}

//esiste la classe bitset che fa la stessa cosa 
class bitwriter {
	uint8_t buffer_;
	uint8_t cur_bit_;
	int n_ = 0;
	//generico output stream
	/*teniamo una reference allo stream di output*/
	std::ostream& os_;
	//gli stream non prevendono l'assegnamento


	std::ostream& write_bit(uint8_t bit)
	{
		//scrive il bit meno significativo di BIT
		buffer_ = (buffer_ << 1) | (bit & 1);
		++n_;
		if (n_ == 8) {
			raw_write(os_, buffer_);
			n_ = 0;
		}
		return os_;
	}


public:
	bitwriter(std::ostream& os) : os_(os) { }
	std::ostream& write(uint32_t u, uint8_t n)
	{
		
		for (int i = n - 1; i >= 0; --i)
		{
			write_bit(u >> i);
		}
		return os_;
	}



	//alternativa per chiamare la write
	std::ostream& operator()(uint32_t u, uint8_t n) {
		return write(u, n);

	}

	std::ostream& flush(uint32_t bit = 0)
	{
		while (n_ > 0)
		{
			write_bit(bit);
		}
		return os_;
	}

	~bitwriter() {
		flush();
	}

};

class node{

	char symbol_;
	double probability_;
	uint8_t bit_ = NULL;
	node *leftchild_=NULL;
	node *rightchild_=NULL;

public:

	node()
	{
		this->symbol_ =0;
		this->bit_ = NULL;
		this->probability_ = 0;
		this->leftchild_ = NULL;
		this->rightchild_ = NULL;
	}

	 node(char symbol, double probability,uint8_t bit, node *left, node* right)
	{
		this->symbol_ = symbol;
		this->bit_ = NULL;
		this->probability_ = probability;
		this->leftchild_ = left;
		this->rightchild_ = right;
		
	}

	 char getSymbol()
	 {
		return this->symbol_;

	 }

	 uint8_t getBit() {
		 return this->bit_;
	 }
	 double getProb()
	 {
		 return this->probability_;
	 }
	
	 void setSymbol(char symbol)
	 {
		 this->symbol_ = symbol;
		
	 }
	 void setProb(double prob)
	 {
		 this->probability_ = prob;
		 

	 }
	 void setLeftChild(node* left)
	 {
		 this->leftchild_ = left;
		 

	 }
	 void setRightChild(node* right)
	 {
		 this->rightchild_ = right;
	 }
	 void setBit(uint8_t bit)
	 {
		 this->bit_ = bit;
	 }

	 node * getRightChild()
	 {
		 return this->rightchild_;
	 }
	 node* getLeftChild()
	 {
		 return this->leftchild_;
	 }
};

void setBit(node *tree, uint8_t *num)
{
	
		if (tree->getLeftChild() == NULL && tree->getRightChild() == NULL)
		{
			*num += 1;
			return;
		}
		if (tree->getLeftChild() != NULL)
		{
			tree->getLeftChild()->setBit(0);
		}
		if (tree->getRightChild() != NULL)
		{
			tree->getRightChild()->setBit(1);
		}
		setBit(tree->getLeftChild(),num);
		setBit(tree->getRightChild(),num);
}


void createTriplets(node* tree, std::vector<char> buff, bitwriter bw, int n)
{

	if (tree->getLeftChild() == NULL && tree->getRightChild() == NULL)
	{
		buff.push_back(tree->getBit());
		bw.write(tree->getSymbol(), 8);
		
		for (auto& c : buff)
		{	
			bw.write(c,1);
		}
		
		buff.push_back(n);


		buff.pop_back();
		return;
	}
	if (n!=0)
	{
		buff.push_back(tree->getBit());
	}


	createTriplets(tree->getLeftChild(), buff, bw,++n);
	createTriplets(tree->getRightChild(), buff, bw,++n);
}


void findSymbol(node* tree, std::vector<char>&buff, char symbol,bool &found)
{
	
	if (tree->getLeftChild() == NULL && tree->getRightChild() == NULL)
	{
		if (tree->getSymbol() == symbol)
		{
			found = true;	
		}
		
		return;
	}
	
	buff.push_back(tree->getBit());
	findSymbol(tree->getLeftChild(), buff,symbol,found);
	if (found)
	{
		return;
	}
	buff.pop_back();
	findSymbol(tree->getRightChild(), buff,symbol,found);
	if (found)
	{
		return;
	}
	buff.pop_back();
}

bool compare_probability(node *e1, node  *e2)
{
	return e1->getProb() > e2->getProb();
}


void code_by_huffman(node* tree, std::ifstream& is, std::ostream &os, int32_t &n)
{
	char sym;
	std::vector<char> code;
	bool found;

	while (is.read(&sym, 1))
	{

		found = false;
		findSymbol(tree, code, sym,found);
		for (auto& x : code)
		{
			os.write(&x,1);
		}
		code.clear();
		
	}
	return;
}
int compress(char*filein, char *fileout)
{
	std::ifstream is(filein, std::ios::binary);
	if (!is)
	{
		return 1;
	}
	double v[256] = {0};
	bitreader br(is);
	uint8_t table_entries = 0;
	int total_node= 0;

	while (1)
	{
		uint32_t num = br.read(8);
		if (br.fail()) {
			break;
		}
		
		v[num] += 1;
	}
	//conteggio num elementi nella tabella e num totale di elementi
	std::string magic_number = "HUFFMAN1";
	for (int i = 0; i < 256; ++i)
	{
		if (v[i] != 0)
		{
			
			total_node+=(int) v[i];
		}
	}
	//calcolo le probabilità
	for (int i = 0; i < 256; ++i)
	{
		v[i] = v[i] / total_node;
		if (v[i] != 0)
		{
			std::cout << v[i] << '\n';
		}
		
	}

	std::vector<node *> tree;

	//AGGIUNTA ELEMENTI CON PROBABILITA' !=0 DENTRO L'ALBERO DI FOGLIE
	for (int i = 0; i < 256; ++i)
	{
		if (v[i] != 0)
		{
			tree.push_back(new node(char(i), v[i],NULL, NULL, NULL));
		}
	}

	//CREAZIONE ALBERO DI HUFFMAN
	int n = tree.size() - 1;
	
	for (int i = n; i != 0; --i)
	{
		std::sort(tree.begin(), tree.end(), compare_probability);
		double prob = tree.back()->getProb();
		node *left =tree.back();
		tree.pop_back();
		prob += tree.back()->getProb();
		node *right = tree.back();
		tree.pop_back();
		tree.push_back( new node(NULL, prob, 0, left, right));
		
	}
	//SCORRO L'ALBERO E ASSEGNO I BIT AI RAMI
	setBit(tree[0],&table_entries);
	std::ofstream os(fileout);
	if (!os)
	{
		return -1;
	}
	bitwriter bw(os);
	for (int i = 0; i < magic_number.length(); ++i)
	{
		os.write(&magic_number[i], sizeof(magic_number[i]));
	}
	std::vector<char> buffer;
	bw.write(table_entries, 8);
	createTriplets(tree[0], buffer, bw,0);
	std::ifstream is2(filein, std::ios::binary);
	int32_t num_symbol = 0;
	code_by_huffman(tree[0], is2,os,num_symbol);
	return 0;

	
	
}


int main(int argc, char** argv)
{
	if (argc != 4)
	{
		std::cout << "incorrect number of parameters" << "\n";
		return 1;
	}


	if (strcmp(argv[1], "c") == 0)
	{
		compress(argv[2],argv[3]);
	}
	else if (strcmp(argv[1], "d") == 0)
	{
		//decompress(argv[2]);

	}
	else
	{
		std::cout << "incorrect input: mode must be c or d" << "\n";
	}

	

}