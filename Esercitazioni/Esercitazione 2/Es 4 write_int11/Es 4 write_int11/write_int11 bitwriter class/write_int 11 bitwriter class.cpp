#include <cstdint>
#include <fstream>
#include <vector>
#include <iterator>
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
		/*mentre n tende a zero*/
		//while (n-->0) /*n-- >0 decrementa n fino a che è maggiore di 0 
		//	il confronto viene fatto prima di decrementare*/
		//{
		//	write_bit(u >> n);
		//}
		//return os_;
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
int main(int argc, char** argv)
{
	if (argc != 3)
	{
		return 1;
	}
	std::ifstream is(argv[1]);
	if (!is)
	{
		return 1;
	}
	std::vector < int32_t>v{ std::istream_iterator<int32_t>(is),std::istream_iterator<int32_t>() };
	std::ofstream os(argv[2], std::ios::binary);

	bitwriter bw(os);
	for (const auto& x : v)
	{
		bw(x, 11);
	}

	return 0;




}