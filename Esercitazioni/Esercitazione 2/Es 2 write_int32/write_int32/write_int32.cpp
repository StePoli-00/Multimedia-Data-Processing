#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>
using std::vector;

//numeri interi in complemento a due, esattamente come sono
//little endian byte meno significativo al più significativo

 void two_complement_vector(vector<int> &v)
{
	 
	 for (int i= 0; i < v.size(); ++i)
	 {
		 v.at(i) = (~v.at(i)) + 1;
	 }
	 return;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Error: incorrect number of parameters\n";
		return 1;
	}
	std::ifstream is(argv[1]);
	std::vector<int> v;
	int32_t num;
	if (!is)
	{
		std::cout << "Error opening read file\n";
		return 1;
	}
	while (is>>num)
	{
		v.push_back(num);
	}

	//two_complement_vector(v);
	std::ofstream os(argv[2]);
	if (!os)
	{
		std::cout << "Error opening write file\n";
		return 1;
	}

	char back = '\n';
	for (int i = 0; i < v.size(); ++i)
	{
		os.write((char*)&v.at(i), sizeof(v.at(i)));
		os.write((char*)&back, sizeof(back));
	}

	return 0;


	

}
