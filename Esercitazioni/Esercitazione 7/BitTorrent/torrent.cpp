#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <iomanip>
#include <iostream>
#include <vector>
#include <exception>

class Type {

	int64_t num_;
	std::string string_;
	std::vector<Type> list_;
	std::map<std::string, Type> dict_;
	char type_; //i:int,s:string,l:list,d:dict

public:

	Type() {
		type_ = '0';
	}
	Type(std::ifstream &is) {
		char b, e,colon;
		int n;
		
			switch (is.peek())
			{
				break; case 'i':
					type_ = 'i';
					is >> b >> num_ >> e;
					if (b != 'i' || e != 'e')
					{
						throw std::runtime_error("invalid syntax for integer");
					}
				break; case 'l':
					is >> type_;
					while (is.peek() != 'e')
					{
						list_.push_back(Type(is));
						//list_.emplace_back(is);
					}
					is >> e;
				break; case 'd': 
					type_ = 'd';
					is >> type_;
					while (is.peek() != 'e') {
						Type key(is);
						dict_[key.string_] = Type(is);
					}
					is >> e;
				
				break; default:
					type_ = 's';
					if (!(is >> n>>colon))
					{
						throw std::runtime_error("invalid syntax for string");
					}
					string_.resize(n);
					is.read(&string_[0], n);
			
			}
	}


	void print(size_t tab) const //nesuna modifica
	{
		switch (type_)
		{
		break; case 'i': std::cout << num_;
		break; case 's':
			std::cout << "\"";
			for (auto& c : string_)
			{
				std::cout << (31 < c && c < 127 ? c : '.');
			}
			std::cout << "\"";

		break; case 'l':
			std::cout << "[";
			for (auto& item : list_)
			{
				std::cout << "\n";
				std::cout << std::string(tab + 1, '\t');
				item.print(tab + 1);
			}
			std::cout << '\n';
			std::cout << std::string(tab, '\t');
			std::cout << "]";
		break; case 'd':
			std::cout << "{";
			for (auto& item : dict_)
			{
				std::cout << "\n";
				std::cout << std::string(tab + 1, '\t');
				std::cout << "\""<<item.first <<"\""<<" => ";

				if (item.first == "pieces")
				{
					//impostare l'esadecimale
					std::cout << std::hex << std::setfill('0');
					
					for (size_t i = 0; i< item.second.string_.size(); ++i)
					{
						if (i % 20 == 0)
						{
							std::cout << "\n";
							std::cout << std::string(tab+1, '\t');
						}
						//dobbiamo trasformare item.second.string_[i] in unsigned char e poi convertirlo ad int
						//es -33 char unsigned_char=223 -> base16 DF ->int 
						//meglio usare format oppure metter un + davanti facendo diventare tutto un int
						unsigned char v= item.second.string_[i];
						int val =v;
						std::cout << std::setw(2) << val;

					}
					std::cout << std::dec;

				}
				else { item.second.print(tab + 1); }


			}
			std::cout << '\n';
			std::cout << std::string(tab + 1, '\t');
			std::cout << "}";
		break; default:;
		}
	}


	

};


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		return -1;
	}
	std::string filename = argv[1];
	if (filename.length()==0)
	{
		return -1;
	}
	std::ifstream is(filename, std::ios::binary);
	if (!is)
	{
		return -1;
	}
	Type root(is);
	root.print(0);

	return 0;
	
}
