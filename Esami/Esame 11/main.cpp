#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <functional>
#include <unordered_map>
#include <exception>
#include "ppm.h"
#include "main.h"
using namespace std;

unordered_map<char,unsigned long long> types = {

	{'Z',sizeof(NULL)},
	{'N',0},
	{'T',sizeof(bool)},
	{'F',sizeof(bool)},
	{'i',sizeof(int8_t)},
	{'U',sizeof(uint8_t)},
	
};


bool check_optimized_format(std::istream& is)
{
	uint8_t val = is.peek();
	if (val == '#' || val == '$')
	{
		return true;
		sizeof(char)
	}
	return false;
}

void read_optimized_format(std::istream& is, Ubjson& obj)
{
	
	uint8_t val = is.get(); //estraggo il $
	uint8_t type;
	if (val == '$')
	{
		type = is.get();

		
	}
	else if(val=='#')

}


class Ubjson {
	std::string type_;
	std::string string_;
	int num_;
	std::vector<Ubjson> array_;
	std::vector<std::pair<string,Ubjson>> object_;
	//std::istream& is_;
public:
	Ubjson(){}

	Ubjson load(std:: istream &is)
	{
		char val=is.peek();

		if (val == '{')
		{
			val = is.get();
			type_ = "o";
			
			while (val != '}') {
				Ubjson obj = load(is);
				std::string field= obj.string_;
				Ubjson data = load(is);
				object_.push_back({field,data});
				val = is.peek();
			}
		}
		else if (val == '[')
		{
			val = is.get();

			Ubjson obj;
			obj.type_ = 'a';
			bool res=check_optimized_format(is);
			if (res)
			{

			}
			else {
				while (val != ']') {
					obj = load(is);
					array_.push_back(obj);
					val = is.peek();
				}

			}
			
			return obj;
		}
		else if (val == 'i' || val=='U')
		{
			Ubjson ob;
			val = is.get();
			int dim= is.get();
			char ch = is.peek();

			if (isalpha(ch) && ch!='i')
			{
			
				ob.type_ = 's';
				char str[255];
				is.read(&str[0], dim);
				str[dim] = '\0';
				ob.string_ = str;
				return ob;
			}
			
			ob.type_ = 'n';
			ob.num_ = dim;
			return ob;

			
		}
		else if (val == '$')
		{


			
			
			return 




		}
		else if (val == '#')
		{
			val = is.get(); //estraggo il $
			int n = is.get();

			for (int i = 0; i < n; ++i)
			{

				this->array_.push_back(load(is));

			}
		}
		
	}

};


int convert(const string& sInput, const string& sOutput) {

	// Dal file UBJ devo estrarre le informazioni e creare il canvas
	std::ifstream is(sInput, std::ios::binary);
	std::ofstream os(sOutput, std::ios::binary);
	if (!is || !os)
	{
		return 1;
	}
	Ubjson ubjson;
	ubjson.load(is);











	unsigned w = 100; // TODO : modificare
	unsigned h = 100; // TODO : modificare

	image<vec3b> img(w, h);

	// Per accedere ai pixel di img posso usare img(x,y) oppure img.begin() e img.end()

	// Dal file UBJ devo estrarre le informazioni sulle immagini da incollare su img 

	// Output in formato PPM
	if (!writeP6(sOutput, img))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

	// TODO : gestire la linea di comando


	if (argc != 3)
	{
		return 1;
	}
	string sInput = argv[1];
	string sOutput = argv[2];

	return convert(sInput, sOutput);
}