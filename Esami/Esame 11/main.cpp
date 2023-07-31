#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <functional>
#include <exception>
#include "ppm.h"
#include "main.h"
using namespace std;





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
			while (val != ']') {
				obj= load(is);
				array_.push_back(obj);
				val = is.peek();
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


			
			val = is.get(); //estraggo il $
			this->type_ = is.get();
			val = is.get(); //estraggo il #
			val = is.get();
			uint8_t n = is.get();

			for (int i = 0; i < n; ++i)
			{
				Ubjson obj;
				obj.type_ = 'n';
				is.read(reinterpret_cast<char*>(&obj.num_), sizeof(uint8_t));
				this->array_.push_back(obj);

			}
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