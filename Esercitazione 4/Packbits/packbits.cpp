#include <iostream>
#include <fstream>
#include <vector>

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

	/*u: contenitore di buffer
	  n: numero di bit da scrivere
	 */
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

void write_copy(std::vector<char> &buff_copy,bitwriter &bw) {
	
		bw.write(buff_copy.size() - 1, 8);
		for (auto& x : buff_copy)
		{
			bw.write(x, 8);
		}
		buff_copy.clear();
		return;

}
bool write_run(std::istream &is,char& prec, char& curr, int &cnt, bitwriter& bw) {

	prec = curr;

	while (curr == prec)
	{
		if (cnt == 128)
		{
			bw.write(129, 8);
			bw.write(prec, 8);
			cnt = 1;
			prec = curr;
			return false;
		}
		
		is.read(&curr, sizeof(curr));


		if (!is)
		{

			bw.write(257 - cnt, 8);
			bw.write(prec, 8);
			bw.write(128, 8);
			return true;
		}
		++cnt;
	
	

	}
	bw.write(257 - cnt, 8);
	bw.write(prec, 8);
	prec = curr;
	cnt = 1;
	return false;
}

int compress(char* filen, char* fout)
{
	std::ifstream is(filen, std::ios::binary);
	std::ofstream os(fout, std::ios::binary);
	bitwriter bw(os);
	if (!is || !os)
	{
		return 1;
	}

	char curr,prec=NULL;
	std::vector<char> buff_copy,buff_run;
	int cnt=0;
	bool is_end;

	//leggi e controlli, while is non va bene
	while (is.read(&curr, sizeof(curr)))
	{
		if (cnt == 0)
		{
			cnt = 1;
			prec = curr;
			continue;
			
		}
		if (curr != prec)
		{
			if (buff_copy.size() != 128)
			{
				buff_copy.push_back(prec);
				prec = curr;
			}
			else
			{
				write_copy(buff_copy, bw);
				prec = curr;

			}
			
		}
		else
		{
			//controllare una run o una copy
				if (buff_copy.size() != 0)
				{
					write_copy(buff_copy, bw);
				}
				is_end=write_run(is, prec, curr, cnt, bw);
				if (is_end == true)
				{
					return 0;
				}
			

			
		}
		//COPY RUN COPY DA EVITARE


	}
	//controllare una run o una copy
	buff_copy.push_back(curr);
	write_copy(buff_copy, bw);
	bw.write(128, 8);
	return 0;
	


}

int decompress(char* filen, char* fout) {
	std::ifstream is(filen, std::ios::binary);
	std::ofstream os(fout);
	if (!is)
	{
		return 1;
	}
	if (!os)
	{
		return 1;
	}
	bitreader br(is);
	uint8_t L=0, symbol=0;
	while (raw_read(is,L,sizeof(L)))
	{
	
		if (L == 128)
		{
			break;

		}else if (L >= 0 && L <= 127)
		{
			
		
			for (int i = 0; i < (L + 1); ++i)
			{

				raw_read(is, symbol, sizeof(symbol));
				os << symbol;
				
			}
			

		}
		else if (L >= 129 && L <= 255)
		{
			int n = 257 - L;
			raw_read(is, symbol, sizeof(symbol));

				for (int i = 0; i < n; ++i)
				{
					os << symbol;
				}
	
			
		
			
		}
		
	}
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
		compress(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "d") == 0)
	{
		decompress(argv[2],argv[3]);

	}
	else
	{
		std::cout << "incorrect input: mode must be c or d" << "\n";
	}



}
