#include <cstdint>
#include <fstream>
#include <vector>
#include <iterator>
template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}

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
	std::ofstream os(argv[2],std::ios::binary);

	uint8_t buffer;
	uint8_t cur_bit;
	int n = 0;
	for (const auto& x : v)
	{
		
		/*i bit vengono scritti sul file dal + significativo al meno significativo
		usa l'int per i cicli al contrario*/
		
		for (int i = 10; i >= 0; --i) {
			//prendo il bit corrente 
			cur_bit = (x >> i) & 1;
			//aggiungo un bit a destra
			buffer=(buffer << 1) | cur_bit;
			//incremento i bit nel buffer
			++n;
			if (n == 8) {
				raw_write(os, buffer);
				n = 0;
			}
		}
		//scriviamo gli ultimi byte che non hanno 8 bit
		while (n > 0)
		{
			cur_bit = 0;
			buffer=(buffer << 1) | cur_bit;
			//incremento i bit nel buffer
			++n;
			if (n == 8) {
				raw_write(os, buffer);
				n = 0;
			}
		}
		
	}
	return 0;




}