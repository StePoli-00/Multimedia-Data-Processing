#include <cstdint>
#include <fstream>
#include <vector>
#include <iterator>
template<typename T>
std::ostream& raw_write(std::ostream& os,const T &val,size_t size=sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		return 1;
	}
	std::ifstream is(argv[1]);
	std::ofstream os(argv[2]);
	if (!is)
	{
		return 1;
	}

	//the most vexing parse
	std::vector < int32_t>v{ std::istream_iterator<int32_t>(is),std::istream_iterator<int32_t>() };
	if (!os)
	{
		return 1;
	}
	/*		CASTING			*/
	int a = 7;
	double b = (double)a / 10;
	//meno potente del cast del c, ma evita di cambiare il tipo ai puntatori
	double c = static_cast<double>(a) / 10;
	int32_t x = 266;
	int32_t* px = &x;
	//char* qx =px; non si può fare
	//per fare casting esplicito tra puntatori si può fare in questo modo
	char* qx = reinterpret_cast<char*>(px);

	//os.write((const char*)&x, 4);
	//os.write(reinterpret_cast<char*>(&x), sizeof(int));
	/*template deduction: non ho bisogno di specificare il tipo 
	del template e scrivere il tipo prima di chiamare la funzione*/
	for (const auto& x : v) {
		raw_write(os, x);
	}
	return 0;




}