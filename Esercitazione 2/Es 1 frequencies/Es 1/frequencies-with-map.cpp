#include <map>
#include <iostream>
#include <fstream>
//mappa non ordinata che tiene traccia dei dati 
#include <unordered_map>
//mappe ordinati
#include <map>
#include <format>
//c<nome libreria del c>
#include <cstdint>
#include <algorithm>
//manipolatori di output
#include <iomanip>
#include<array>

using std::pair;


template<typename T,typename CountT=uint32_t> 
struct freq {

	std::unordered_map<T, CountT> count_;
	//functor,oggetto che si comporta come una funzione
	void operator()(const T&val) {
		++count_[val];
	}

	auto begin() {return count_.begin();}
	auto end() {return count_.end();}
	//restituisce la const del begin
	auto cbegin() { return count_.begin();}
	auto cend() { return count_.end(); }
};

int main(int argc, char** argv) {

	using namespace std;
	if (argc != 3)
	{
		cout << "Errore  incorrect number of parameters\n";
		return 1;
	}

	//array
	std::array<uint32_t, 256> arr;
	//fill è all interno di algorithm
	fill(arr.begin(), arr.end(), 0);
	std::map<uint8_t, int> map;
	//modalità di apertura, default testo
	std::ifstream is(argv[1], ios::binary);

	if (!is) {
		std::cout << "Errore apertura file\n";
		return 1;
	}


	//freq<int> count;
	//while (1)
	//{
	//	int num;
	//	//get,restituisce un int come una fget
	//	num = is.get();
	//	if (num == EOF)
	//	{
	//		break;
	//	}
	//	/*per accedere ad una mappa si accede tramite[], se non esiste l'oggetto cercato
	//	crea l' oggetto azzerando i dati se è un int*/
	//	count(num);

	//}


	//nella unordered map è strutturata come coppia valore-chiave
	unordered_map <int, uint8_t>m_map;
	while (1)
	{
		int num;
		//get,restituisce un int come una fget
		num = is.get();
		if (num == EOF)
		{
			break;
		}
		/*per accedere ad una mappa si accede tramite[], se non esiste l'oggetto cercato 
		crea l' oggetto azzerando i dati se è un int*/
		++m_map[num];

	}

	//aperto in modalità testo
	ofstream os(argv[2]);
	if (!os)
	{
		return 1;
	}
	//for each
	for (const auto &x :m_map)
	{
		//all'interno della mappa vi sono coppia-valore

			os << hex << setw(2) << setfill('0') << x.first<< '\t';
			os << dec << x.second << '\n';
			//guarda std format per usarlo andare su proprietà->c/c++->linguaggio->imposta c++ 20
			//os <<std::format("{:02X}\t{ }\n",x.first,x.second);
		
	}



	return 0;

}