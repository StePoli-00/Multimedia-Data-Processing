#include <cstdint>
#include <fstream>
#include <vector>
#include <iterator>
template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast< char*>(&val), size);
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		return 1;
	}
	std::ifstream is(argv[1],std::ios::binary);
	std::ofstream os(argv[2]);
	if (!is && !os)
	{
		return 1;
	}
	std::vector<int32_t> v;
	uint32_t num;
	/*is.read(reinterpret_cast<char*>(&num), sizeof(num))*/
	while (raw_read(is,num))
	{
		v.push_back(num);
	}
	

	for (int i = 0; i < v.size(); ++i)
	{
		os << v[i] << '\n';
	}
	

	



}