#include <string.h>
#include <fstream>
#include <vector>
#include<cmath>
#include "bit.h"
#include <iterator>
#include <string>


int count_bit( int bit) {

	int cnt = 0;

	while (bit >0) {
		//bit /= 2;
		bit=bit >> 1;
		++cnt;
	}
	return cnt;
}


void map(std::vector<int>& v) {

	for (size_t i = 0; i < v.size(); ++i)
	{

		if (v[i] < 0)
		{
			v[i] = 2 * v[i] * -1;
		}
		else
		{
			v[i] = 2 * v[i] + 1;
		}
	}
	return;
}
void unmap(std::vector<int>& v) {
	
	for (size_t i = 0; i < v.size(); i++)
	{
		if (v[i] % 2 == 0) {
			v[i] = (v[i] / 2) * -1;
		}
		else
		{
			v[i] = (v[i] -1)/2;
		}

	}
	return;
}
int  compress(char *filein, char *fout)
{
	std::ifstream is(filein);
	std::ofstream os(fout, std::ios::binary);
	if (!is || !os)
	{
		return 1;
	}
	//usare le graffe most vexing parse
	std::vector<int32_t>v{ std::istream_iterator<int32_t>(is), std::istream_iterator<int32_t>() };
	map(v);
	int bit;
	bitwriter bw(os);
	for (int i = 0; i < v.size(); i++)
	{
		bit = v[i];
		auto n = count_bit(bit);
		bw(bit, 2 * n - 1);

	}
	return 0;
}



int decompress(char* filein, char* fout) {
	std::ifstream is(filein, std::ios::binary);
	std::ofstream os(fout );
	if (!is || !os)
	{
		return 1;
	}
	int zeros = 0;
	int num;
	std::vector<int> v;
	bitreader br(is);
	while (1)
	{
		num = br.read(255);
		if (!br)
		{
			break;
		}

		v.push_back(num);
	}
	unmap(v);
	for (size_t i = 0; i < v.size(); i++)
	{
		if (i == v.size() - 1)
		{
			os << v[i];
		}
		else
		{
			os << v[i] << "\n";
		}
	}
	
	return 0;


}

int main(int argc, char** argv) {

	if (argc != 4)
	{
		std::cout << "Error: incorrect number of parameters\n";
		return 1;
	}

	char *mode = argv[1];
	if (strcmp(mode,"c")!=0 && strcmp(mode,"d")!=0)
	{
		std::cout << "Error: first parameter must be c or d\n";
		return 1;
	}
	if (strcmp(mode, "c")==0) {
		compress(argv[2],argv[3]);
	}
	else
	{
		decompress(argv[2], argv[3]);
	}

	return 0;
	
	


}