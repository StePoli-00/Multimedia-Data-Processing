//#include <map>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <algorithm>
//#include <iomanip>
//using std::vector;
//using std::pair;
//bool compare(const pair<uint8_t, int>& a,const pair<uint8_t, int>& b)
//{
//	//< ordina in modo crescente, > in ordine decrescente
//	return (a.second > b.second);
//}
//
//
//int main(int argc, char** argv) {
//
//	if (argc != 3)
//	{
//		std::cout << "errore numero parametri\n";
//		return 1;
//	}
//	std::map<uint8_t, int> map;
//	std::ifstream is(argv[1], std::ifstream::binary);
//
//	if (!is) {
//		std::cout << "errore apertura file\n";
//		return 1;
//	}
//
//	uint8_t num;
//	int occ = 0;
//
//	while (is.read((char*) &num,sizeof(num)))
//	{
//		auto it = map.find(num);
//		if ( it!= map.end())
//		{
//			it->second += 1;
//		}
//		else
//		{
//			map.insert({ num, 0 });
//		}	
//	}
//	//creazione di un vettore da una mappa
//	vector<pair<uint8_t, int>>vec(map.begin(), map.end());
//
//	std::sort(vec.begin(), vec.end(), compare);
//
//	std::ofstream os(argv[2]);
//	if (!os)
//	{
//		std::cout << "error opening writing file\n";
//		return 1;
//	}
//
//	auto start = vec.begin();
//	auto stop = vec.end();
//	for (auto it = start; it != stop; ++it)
//	{
//		os << std::hex << std::setw(2) << std::setfill('0') << (int)it->first << '\t';
//		os<< it->second << '\n';
//
//	}
//
//	return 0;
//
//}