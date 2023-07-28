//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <map>
//#include <cmath>
//#include <bitset>
//
//template <typename T>
//std::ostream& raw_write(std::ostream &os, T &val, size_t size = sizeof(T))
//{
//	return os.write(reinterpret_cast<char*>(&val), size);
//}
//
//class bitwriter {
//	int  n_=0;
//	uint8_t buffer_;
//	std::ostream &os_;
//
//	std::ostream& write_bit(uint8_t bit)
//	{
//		buffer_ = (buffer_ << 1) | (bit & 1);
//		++n_;
//		if (n_ == 8)
//		{
//			raw_write(os_, buffer_);
//			n_ = 0;
//		}
//		return os_;
//	}
//
//public:
//	bitwriter(std::ostream &os): os_(os){}
//
//	std::ostream& write(uint32_t u, uint8_t n)
//	{
//		for (int i = n - 1; i >= 0; --i)
//		{
//			write_bit(u >> i);
//		}
//		return os_;
//	}
//
//	std::ostream& operator()(uint32_t u, uint8_t n) {
//		return write(u, n);
//	}
//
//	std::ostream& flush(uint32_t bit = 0)
//	{
//		while(n_>0)
//		{
//			write_bit(bit);
//		}
//		return os_;
//	}
//
//	~bitwriter() {
//		flush();
//	}
//};
//
//
//void write_with_n_bit(int x,int &numbit)
//{
//	int n_bit = (int)log2(x) + 1;
//	if (n_bit > numbit)
//	{
//		numbit = n_bit;
//	}
//	return;
//}
//
//bool is_full (int size, int maxbits)
//{
//	int n =(int) pow(2, maxbits);
//	if (size == n)
//	{
//		return true;
//	}
//	return false;
//}
//
//bool lz78encode(const std::string& input_filename, const std::string& output_filename, int maxbits)
//{
//	if (maxbits < 1 || maxbits>30)
//	{
//		return false;
//	}
//	std::ifstream is(input_filename, std::ios::binary);
//	std::ofstream os(output_filename);
//	if (!is || !os)
//	{
//		return false;
//	}
//	std::string magic_number ="LZ78";
//	bitwriter bw(os);
//	for (auto &c: magic_number)
//	{
//		os.write(reinterpret_cast<char*> (&c), 1);
//	}
//
//	std::map<std::string,int> dict; //mappa con chiave:match valore: posizione all'interno del dizionario
//	bw.write(maxbits, 5);
//	char c;
//	int i = 0;
//	std::string str;
//	int numbit = 0, last_occ=0;
//
//	while (1)
//	{
//		if (!is.read(&c, 1))
//		{
//			break;
//		}
//		str += c;
//		//se il dizionario è vuoto
//		if (i == 0)
//		{
//			
//			
//			
//			dict[str] = i; //aggiungo l'elemento dentro al dizionario
//			++i;
//			
//			bw.write(c, 8); //devo scriverlo senza alcuna occorrenza
//			str.clear();
//		}
//		else
//		{
//			
//			auto it = dict.find(str);
//			if (it != dict.end()) //provo a fare match con la sequenza attuale
//			{
//				
//				last_occ = it->second; //se ho fatto match mi segno la posizone del match
//			}
//			else
//			{
//				
//				
//				dict[str] = i; //se non ho fatto match, salvo la stringa
//				++i;
//				
//				numbit = (int)log2((dict.size() - 1))+1; //determino il numero di bit che servono per scrivere l'occorrenza
//				bw.write((last_occ+1), numbit);// devo scrivere la posizione+1 perchè parte da zero
//				bw.write(c, 8);
//				
//				str.clear();//svuoto la stringa per fare matching
//				
//				if (is_full(dict.size(), maxbits)) //controllo che non si abbia raggiunto la massima dimensione di rappresentazione
//				{
//
//					/*se ho raggiunto la massima dimensione 
//					svuoto il dizionario*/
//					i = 0;
//					dict.clear();
//					numbit = 0;
//				}
//				/*la metto pari ad -1 per gestire i casi 
//				in cui non sia presente 
//				un match nessun match nel dizionario, 
//				quindi last_occ sarà pari a zero*/
//				last_occ = -1;
//			
//			}
//		}	
//	}
//	last_occ = -1;
//	c = str[str.length() - 1];
//	str.pop_back();
//	auto it = dict.find(str);
//	if (it != dict.end())
//	{
//		last_occ = it->second+1;
//	}
//	else
//	{
//		last_occ = 0;
//	}
//	bw.write(last_occ, numbit); //scrivo un ultima volta l'occ e il carattere
//	bw.write(c, 8);
//	return true;
//	
//
//
//}
//
////int main(int argc, char** argv)
////{
////	if (argc != 4)
////	{
////		std::cout << "error number incorrect of parameters";
////		return false;
////	}
////	int maxbits= atoi(argv[3]);
////	if (maxbits < 1 || maxbits>30)
////	{
////		std::cout << "maxbits must be between 1 and 30";
////		return false;
////	}
////	bool res = lz78encode(argv[1], argv[2], maxbits);
////	return res;
////}