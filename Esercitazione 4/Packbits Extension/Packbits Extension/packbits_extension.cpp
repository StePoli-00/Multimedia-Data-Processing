#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>


template <typename T>
std::istream& raw_read(std::istream &is, T& val, size_t size = sizeof(T))
{
	return is.read(reinterpret_cast <char*> ( & val), sizeof(val));
}


template<typename T>
std::ostream& raw_write(std::ostream& os, T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<char*> (&val), sizeof(val));
}



int decompress(char* filein, char* fout) {
	std::ifstream is(filein, std::ios::binary);
	std::ofstream os(fout);
	if (!is)
	{
		return 1;
	}
	if (!os)
	{
		return 1;
	}
	uint8_t L, symbol;
	while (raw_read(is, L, sizeof(L)))
	{
		if (L == 128)
		{
			break;

		}
		else if (L >= 0 && L <= 127)
		{
			for (int i = 0; i < (L + 1); ++i)
			{

				raw_read(is, symbol, sizeof(symbol));
				raw_write(os, symbol, sizeof(symbol));
				//os << symbol;


			}
		}
		else if (L >= 129 && L <= 255)
		{
			int n = 257 - L;
			raw_read(is, symbol, sizeof(symbol));

			for (int i = 0; i < n; ++i)
			{
				//os << symbol;
				raw_write(os, symbol, sizeof(symbol));
			}
		}

	}
	return 0;
}



void write_copy(std::vector<char>& buff, std::ostream &os)
{
	
	for (auto& x : buff)
	{
		os.write(&x, sizeof(x));
	}
	buff.clear();
	return;
}



int compress(char* filein, char* fout) {

    std::string outname = fout;
	outname.append(".packbits");
	std::ifstream is(filein, std::ios::binary);
	std::ofstream os(outname, std::ios::binary);
	if (!is) {
		return 1;
	}
	if (!os)
	{
		return 1;
	}
	char prec, curr;
	std::vector<char> copy_buff;
	bool crc = false;
	int cnt = 1;
	is.read(&prec, sizeof(prec));
	while (is.read(&curr, sizeof(curr)))
	{
		if (curr != prec)
		{
			if (cnt == 1)
			{
				if (copy_buff.size() == 128)
				{
					//svuoto il buffer se non c'è spazio
					uint8_t dim = 127;
					os.write(reinterpret_cast <char*> (&dim), sizeof(dim));
					for (auto& x : copy_buff)
					{
						os.write(&x, sizeof(x));
					}
					copy_buff.clear();
					copy_buff.push_back(prec);
				}

				else
				{
					copy_buff.push_back(prec);

				}
			}
			//Se è run da 2
			else if (cnt == 2)
			{
				//accodo la run nel buff di copy
				for (int i = 0; i < cnt; ++i)
				{
					if (copy_buff.size() != 128)
					{
						copy_buff.push_back(prec);
					}
					else
					{
						uint8_t dim = 127;
						os.write(reinterpret_cast <char*> (&dim), sizeof(dim));
						write_copy(copy_buff, os);
						copy_buff.push_back(prec);
					}
				}
			}//è una run normale
			else
			{
				uint8_t dim = 257 - cnt;
				os.write(reinterpret_cast <char*> (&dim), sizeof(dim));
				os.write(&prec, sizeof(prec));
			}
			//metto il carattere nel buff di copy se c'è spazio

			prec = curr;
			cnt = 1;
		}
		//ho una run, incremento il contatore
		else
		{
			if (cnt > 2)
			{
				if (copy_buff.size() != 0)
				{

					uint8_t dim = copy_buff.size() - 1;
					os.write(reinterpret_cast <char*> (&dim), 1);
					write_copy(copy_buff, os);

				}
			}

			++cnt;
			prec = curr;
		

		}
	}
	
	if (copy_buff.size() != 0 )
	{
		if (cnt == 2)
		{
			copy_buff.push_back(prec);
		
		}
		//svuoto il buffer
		copy_buff.push_back(prec);
		uint8_t dim = copy_buff.size() - 1;
		os.write(reinterpret_cast <char*> (&dim), 1);
		write_copy(copy_buff, os);
		
	}
	uint8_t dim = 128;
	os.write(reinterpret_cast <char*> (&dim), sizeof(dim));
	return 0;


	
}

int main(int argc, char** argv)
{
	int ret;
	if (argc != 4)
	{
		std::cout << "incorrect number of parameters" << "\n";
		return 1;
	}


	if (strcmp(argv[1], "c") == 0)
	{
		ret = compress(argv[2], argv[3]);
		return ret;

	}
	else if (strcmp(argv[1], "d") == 0)
	{
		ret = decompress(argv[2], argv[3]);
		return ret;


	}
	else
	{
		std::cout << "incorrect input: mode must be c or d" << "\n";
		return 1;
	}
}
