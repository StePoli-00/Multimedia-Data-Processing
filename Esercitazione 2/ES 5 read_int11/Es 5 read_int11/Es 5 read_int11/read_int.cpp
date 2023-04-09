#include <cstdint>
#include <fstream>
#include <vector>
#include <iterator>
template<typename T>
std::istream& raw_read(std::istream& is, const T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<const char*>(&val), size);
}

//num 11 bit con segno l'ultimo 1 essendo in complemento a due vale - il valore del bit in quella posizione
//1-modo: prendo l'ultimo bit e controllo se è ad 1-> e sottraggo il suo valore al numero
//2-modo: se il num è maggiore di 1024 sottraggo 2048
//3-modo: metto in complemento tutti i restanti bit->estensione del numero
class bitreader{
	uint8_t buffer_;
	int n_;
	std::istream& is_;
	
public:
	bitreader(std::istream &is): is_(is){}

	uint32_t read_bit() {
		if (n_ == 0)
		{
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		//ritorna il bit corrente
			return (buffer_ >> n_) & 1;
	}

	uint32_t read(uint8_t n) {
		uint32_t u = 0;
		while (n-- > 0)
		{
			u = (u << 1) | read_bit();
		}
		return u;
	}

	bool fail()const {
		return is_.fail();
	}

	uint32_t operator()(uint8_t n) {
		return read(n);
	}
	explicit operator bool() const {
		return !fail();
	}
};
int main(int argc, char** argv)
{
	if (argc != 3)
	{
		return 1;
	}
	std::ifstream is(argv[1],std::ios::binary);
	if (!is)
	{
		return 1;
	}
	bitreader br(is);
	std::vector<int32_t>v;
	while (1)
	{
		int32_t num = br(11);
		if (!br.fail())
		{
			break;
		}
		if (num >= 1024) {
			num -= 2048;
		}
		v.push_back(num);
	}

	std::ofstream os(argv[2]);
	if (!os)
	{
		return 1;
	}
	copy(v.begin(), v.end(), std::ostream_iterator<int32_t>(os,"\n"));

		//for (const auto& x : v) {
		//	os << x << '\n';
		//}
	
		return 0;




}