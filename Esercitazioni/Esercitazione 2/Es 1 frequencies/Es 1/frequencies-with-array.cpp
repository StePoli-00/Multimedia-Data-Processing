//#include <map>
//#include <iostream>
//#include <fstream>
////mappa non ordinata che tiene traccia dei dati 
//#include <unordered_map>
//#include <format>
////c<nome libreria del c>
//#include <cstdint>
//#include <algorithm>
////manipolatori di output
//#include <iomanip>
//using std::pair;
//bool compare(const pair<uint8_t, int>& a, const pair<uint8_t, int>& b)
//{
//	//< ordina in modo crescente, > in ordine decrescente
//	return (a.second > b.second);
//}
//
//
//int main(int argc, char** argv) {
//
//	using namespace std;
//	if (argc != 3)
//	{
//		cout << "Errore  incorrect number of parameters\n";
//		return 1;
//	}
//
//	std::map<uint8_t, int> map;
//	//modalità di apertura, default testo
//	std::ifstream is(argv[1], ios::binary);
//
//	if (!is) {
//		std::cout << "Errore apertura file\n";
//		return 1;
//	}
//
//	//creo un vettore pieno di zeri
//	uint32_t count[256] = { 0 };
//	while (1)
//	{
//		int num;
//		//get,restituisce un int come una fget
//		num = is.get();
//		if (num == EOF)
//		{
//			break;
//		}
//		++count[num];
//
//	}
//	//aperto in modalità testo
//	ofstream os(argv[2]);
//	if (!os)
//	{
//		return 1;
//	}
//	//int può assumere al massimo 256 caratteri
//	for (size_t i = 0; i < 256; ++i)
//	{
//		//se in count[i] c'è qualcosa di diverso da zero lo scrivo in output
//		if (count[i] > 0)
//		{
//			//se viene impostato in base 16 anche gli altri verranno stampati in base 16
//			/*os << setbase(16)<<setw(2)<<setfill('0') << i;
//			os << '\t' <<setbase(10)<< count[i] << '\n';*/
//
//			os << hex<<setw(2) << setfill('0') << i<<'\t';
//			os<<dec << count[i] << '\n';
//			//guarda std format per usarlo andare su proprietà->c/c++->linguaggio->imposta c++ 20
//			//os <<std::format("{:02X}\t{ }\n",i,count[i]);
//		}
//	}
//
//	
//
//	return 0;
//
//}