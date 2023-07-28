#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <math.h>

template<typename T>
std::ostream& raw_write(std::ostream& os, T val, size_t size = sizeof(T))
{
	return os.write(reinterpret_cast<char*>(&val), size);
}


class Bitwriter {
	uint8_t buffer_=0;
	int n_ = 0;
	std::ostream& os_;

	std::ostream& write_bit(uint32_t bit)
	{
		buffer_ = (buffer_ << 1) | (bit );
		++n_;
		if (n_ == 8)
		{
			raw_write(os_, buffer_);
			n_ = 0;
		}
		
		
		return os_;
	}



public:
	Bitwriter(std::ostream& os) : os_(os) {}

	std::ostream& write(uint32_t u, uint8_t n)
	{
		while (n-- > 0)
		{
			write_bit((u >> n));
		}
		return os_;
	}

	std::ostream& operator()(uint32_t u, int n)
	{
		return write(u, n);
	}



	std::ostream& flush(uint8_t bit = 0)
	{
		while (n_> 0)
		{
			write_bit(bit);
		}
		return os_;
	}

	~Bitwriter()
	{
		flush();
	}


};






bool lz78encode(const std::string& input_filename, const std::string& output_filename, int maxbits)
{
	if (input_filename.empty() || output_filename.empty())
	{
		return false;
	}
	if (maxbits < 1 || maxbits>30)
	{
		return false;
	}

	std::ifstream is(input_filename, std::ios::binary);
	std::ofstream os(output_filename, std::ios::binary);
	if (!is || !os)
	{
		return false;
	}
	
	
	Bitwriter bw(os);
	std::string magic_number = "LZ78";

	for (auto& c : magic_number)
	{
		
		
			bw.write(c, 8);
		
	}
	uint8_t m = maxbits;
	bw.write(m, 5);

	std::unordered_map<std::string,int> dictionary;
	int i = 1;
	char c;
	int last_occ = 0, dim_bits = 0, max_size =(int) pow(2, maxbits);
	std::string ch;
	while (1)
	{
		if (!(is.read(&c, 1)))
		{
			if (ch.length()!=0)
			{
			
				ch.pop_back();
				auto it = dictionary.find(ch);
				int bit = std::ceil(log2l(i));
				if (it != dictionary.end())
				{
					last_occ = it->second;
				}
				else
				{
					last_occ = 0;
				}
				bw.write(last_occ, bit);
				bw.write(c, 8);
				

			}
			break;
		}
		
		ch += c;
		auto it = dictionary.find(ch);
		
		if (it != dictionary.end())
		{
			last_occ = it->second;
		}
		else
		{
			
				dictionary[ch] = i;
				int bit= std::ceil(log2l(i));
				//stampo
				if (i == 1)
				{
					bw.write(last_occ, 0);
				}
				else
				{
					bw.write(last_occ, bit);
				}
				bw.write(ch[ch.size() - 1],8);
				++i;
				
				if (dictionary.size() ==max_size) //forse ci va max_size-1
				{
					dictionary.clear(); //svuoto il dizionario
					i = 1;
				}
			
			last_occ = 0;
			ch.clear(); //svuoto

		}

	

	}


	return true;

		
}


