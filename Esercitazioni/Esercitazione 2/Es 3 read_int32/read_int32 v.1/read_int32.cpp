#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
using std::vector;



int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Error: incorrect number of parameters\n";
		return 1;
	}
	std::ifstream is(argv[1],std::ios::binary);
	std::vector<int> v;
	int32_t num;
	if (!is)
	{
		std::cout << "Error opening read file\n";
		return 1;
	}
	while (is >> num)
	{
		v.push_back(num);
	}

	
	std::ofstream os(argv[2]);
	if (!os)
	{
		std::cout << "Error opening write file\n";
		return 1;
	}

	char back = '\n';
	for (int i = 0; i < v.size(); ++i)
	{
		os << v.at(i) << '\n';
	}

	return 0;




}
