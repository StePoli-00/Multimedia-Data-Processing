#include "lzs.h"
#include <vector>
#include <unordered_map>
#include <utility>

std::unordered_map<int, int >huff_table = {
	{0b00,2},{0b01,3},{0b10,4},{0b1100,5},{0b1101,6},{0b1110,7}
};

int find_lenght(Bitreader& br)
{

	int num = 0;
	num=(num << 1) | (br.read(1));
	while (1) {

		num = (num << 1) | (br.read(1));
		auto it = huff_table.find(num);
		if (it != huff_table.end())
		{
			return it->second;
		}
		else if (num == 0b1111)
		{
			int N = 1;

			while (1)
			{
				num = br(4);
				if (num != 0b1111)
				{
					break;
				}

				++N;
			}
			int L = N * 15 - 7;
			return (L+num);

		}
	}
	
	return -1;
}


void decode_LZ77(std::string & vec, std::pair<int, int> &pair)
{
	size_t cur = (vec.size()) -(size_t) pair.first;

	for (size_t i = 0; i < (size_t)pair.second; ++i)
	{
		char ch = vec[(cur + i)];
		vec.push_back(ch);
	}
	return;
}

void lzs_decompress(std::istream& is, std::ostream& os) {



	if (!is || !os)
	{
		return;
	}
	
	Bitreader br(is);
	std::string decoded;
	std::pair<int, int> pair;
	std::vector<std::pair<int, int>> off_len;
	int off = 0;
	while (1)
	{
		uint8_t bit=br.read(1);
		if (!br)
		{
			break;
		}

		if (bit == 0)
		{
			decoded.push_back(br.read(8));
		}
		else if (bit == 1)
		{
			bit = br.read(1);

			if (bit == 1)
			{

				off = br.read(7);
				if (off == 0)
				{
					break;
				}
			}
			else
			{

				 off = br.read(11);
				
			}
			
			pair.first = off;
			pair.second=find_lenght(br);
			if (pair.second == -1)
			{
				return; 
			}
			decode_LZ77(decoded,pair);
		}
	}


	//for (size_t i=0; i<decoded.size();++i)
	//{
	//	if (decoded[i] == '\r' && decoded[i + 1] == '\n')
	//	{
	//		++i;
	//	}
	//	os.write(&decoded[i], 1);
	//}


	for (size_t i=0; i<decoded.size();++i)
	{
		
		os.write(&decoded[i], 1);
	}


	return;

}