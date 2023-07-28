#include <string>
#include <fstream>
#include<unordered_map>
#include <algorithm>

class Node
{
public:
	uint8_t sym_=0;
	uint8_t length_=0;
	double prob_=0;
	Node* left_=NULL;
	Node* right_=NULL;

	Node()
	{

	}

	Node(uint8_t sym, uint8_t lenght, double prob, Node* left, Node* right)
	{
		sym_ = sym;
		length_ = lenght;
		prob_ = prob;
		left_ = left;
		right_ = right;

	}

};


bool compare(Node  *a, Node *b)
{
	if (a->prob_ != b->prob_)
	{
		
		return a->prob_ > b->prob_;
	}
	else 
	{
		return a->sym_ < b->sym_;
	}
	
	
	
}
struct Code {
	uint8_t sym;
	uint8_t length;
	uint32_t code;

};


bool compare_by_length(Code a, Code b)
{
	if (a.length != b.length)
	{
		return a.length < b.length;
	}

	return a.sym < b.sym;

}



void compute_length(Node* &vec, uint8_t length)
{
	
	if (vec->left_ == 0 && vec->right_ == 0)
	{
		vec->length_ = length;
		return;
	}
	length+=1;
	compute_length(vec->left_, length);
	compute_length(vec->right_, length);


}



void getLeafs(Node* & tree, std::vector<Code>& leaf)
{
	if (tree->left_ == 0 && tree->right_ == 0)
	{
		Code c;
		c.sym = tree->sym_;
		c.length = tree->length_;
		c.code = 0; 
		leaf.push_back(c);
		return;
	}

	getLeafs(tree->left_, leaf);
	getLeafs(tree->right_, leaf);
}


template<typename T>
std::ostream& raw_write(std::ostream& os, T val, size_t size = sizeof(T))
{
	return os.write(reinterpret_cast<char*>(&val), size);
}

class Bitwriter {

	uint8_t buffer_=0;
	int n_ = 0;
	std::ostream& os_;


	std::ostream& write_bit(uint8_t bit)
	{
		buffer_ = (buffer_ << 1) | (bit & 1);
		++n_;
		if (n_ == 8)
		{
			raw_write(os_, buffer_);
			n_ = 0;
		}

		return os_;
	}

public:

	Bitwriter(std::ostream& os) : os_(os) {}


	std::ostream& write(uint32_t u, uint32_t n)
	{
		while (n-- > 0)
		{
			write_bit((u >> n));
		}

		return os_;
	}


	std::ostream& operator()(uint32_t u, uint32_t n)
	{
		return write(u, n);
	}

	std::ostream& flush(uint8_t bit = 0)
	{
		while (n_ > 0)
		{
			write_bit(bit);
		}

		return os_;
	}

	~Bitwriter() {
		flush();
	}
};



void create_code(std::vector<Code>& codes)
{
	uint32_t current_code = 0;
	uint8_t dim = 1;

	for (auto& c : codes)
	{
		if (c.length == dim)
		{
			c.code = current_code;
		}
		else
		{
			for (int i = dim; i < c.length; ++i)
			{
				current_code = (current_code << 1);
			}
			c.code = current_code;

		}
		++current_code;
		dim = c.length;
	}

	return;


}

Code find_code(std::vector<Code> &codes, char sym)
{
	for (auto& c : codes)
	{
		if (c.sym == sym)
		{
			return c;
		}
	}

	Code c;
	c.code = -1;
	return c;
}


bool write_output(std::string& filein, std::string& fileout, std::vector<Code>& codes)
{
	std::ofstream os(fileout, std::ios::binary);
	std::ifstream is(filein, std::ios::binary);
	if (!os || !is)
	{
		return false;
	}

	uint8_t table_enties = (uint8_t)codes.size();
	uint32_t num_symbols = (uint32_t)codes.size();
	Bitwriter bw(os);

	os << "HUFFMAN2";
	os << table_enties;

	//scrivo le entriers
	for (auto& c : codes)
	{
		bw(c.sym, 8);
		bw(c.length, 5);
	}

	num_symbols = (num_symbols & 0x0000FFFF) << 16 | (num_symbols & 0xFFFF0000) >> 16;
	num_symbols = (num_symbols & 0x00FF00FF) << 8 | (num_symbols & 0xFF00FF00) >> 8;

	bw(num_symbols, 32);

	char ch;
	Code val;
	//codifico i dati 
	while (is.read(&ch, 1))
	{
		val=find_code(codes, ch);
		

		if (val.code != -1)
		{
			bw(val.code, val.length);
		}
		
	}

	return true;
}


int encode_canonical_huffman(std::string filein, std::string fileout)
{
	if (filein.empty() || fileout.empty()) {
		return 1;
	}

	std::ifstream is(filein, std::ios::binary);
	std::unordered_map<char, double> tuple;
	if (!is)
	{
		return 1;
	}
	char ch;
	int sum = 0;
	while (1)
	{
		if (!(is.read(&ch, 1))) {
			break;
		}
		tuple[ch] += 1;
		sum += 1;
		
	}
	is.close();

	//conversione delle frequenze in probabilità
	for (auto& t : tuple)
	{
		t.second = (t.second / sum);
	}

	std::vector<Node *> nodes;
	
	//conversione delle tuple in nodi
	for (auto& t : tuple)
	{
		Node tmp;
		tmp.sym_ = t.first;
		tmp.prob_ = t.second;
		tmp.length_ = 0;
		tmp.left_ = NULL;
		tmp.right_ = NULL;
		nodes.push_back(new Node(tmp.sym_, tmp.length_, tmp.prob_, tmp.left_, tmp.right_));
	}
	
	//creazione albero di huffman
	while (nodes.size()>1)
	{
		Node tmp;
		std::sort(nodes.begin(), nodes.end(), compare);
		Node *right=nodes.back();
		nodes.pop_back();
		Node *left =nodes.back();
		nodes.pop_back();
		tmp.prob_ = left->prob_ + right->prob_;
		tmp.left_ = left;
		tmp.right_ = right;
		nodes.push_back(new Node(tmp.sym_,tmp.length_,tmp.prob_,tmp.left_,tmp.right_));
	}
	uint8_t i = 0;
	compute_length(nodes[0],i);
	std::vector<Code>  codes;
	getLeafs(nodes[0], codes);
	//ordino i codici per lunghezza crescente
	std::sort(codes.begin(), codes.end(), compare_by_length);
	//creo i codici
	create_code(codes);
	
	//stampo i dati sul file
	bool res=write_output(filein,fileout, codes);
	if (!res)
	{
		return 1;
	}
	
	return 0;
}


int decode_canonical_huffman(std::string filein, std::string fileout)
{
	if (filein.empty() || fileout.empty()) {
		return 1;
	}

	return 0;
}


int main(int argc, char** argv)
{
	if (argc != 4)
	{
		return 1;
	}

	std::string option = argv[1];
	int res;
	if (option == "c")
	{
		res=encode_canonical_huffman(argv[2],argv[3]);
	}
	else if (option == "d")
	{
		res=decode_canonical_huffman(argv[2],argv[3]);
	}
	else
	{
		return false;
	}

	return res;
}