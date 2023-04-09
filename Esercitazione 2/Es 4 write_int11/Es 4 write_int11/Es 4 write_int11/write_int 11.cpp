//base10 120->hex 00000078 bin_with_11->000.0111.1000
//		 35		  00000023=			    000.0010.0011
//		 -98	  FFFFFF9E=			    111.1001.1110
//output file:
//000011110000000010001111110011110
//x=120=00000078=000.0111.1000
// 						  *voglio questo bit
//			    000.0000.1000 faccio un and con questa maschera
//se faccio x>>4  x=000.0000.0111 ora il bit desiderato è in posizione 0 
//faccio la maschera000.0000.0001
//in c: x&1 
// suppmo di avere un buffer di 8 bit abcd.edgh e volessi mettere il primo bit di x
// abcd.edgh<--0-->bcd.edgh0
// bcd.edgh0<--0-->cd.edgh00
// cd.edgh00<--0-->d.edgh000
// d.edgh000<--0-->edgh0000
// edgh0000<--1-->dgh00001
// dgh00001<--1-->gh000011
// gh000011<--1-->h0000111
#include <cstdint>
#include <vector>
#include <fstream>

std::vector<int *> dec_to_binary(std::vector<int> &v) {

	int j,int num=0;
	std::vector<int > binary;
	std::vector<int> binaryNum;
	for (int i = 0; i < v.size(); ++i)
	{
		j = 0;
		num = v[i];
		while (num> 0)
		{
			binaryNum[j] = num % 2;
			num /= 2;
			++j;
		}

		binary[i] = binaryNum;

	}
	
	
	return binary;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		return 1;
	}
	std::ifstream is(argv[1]);
	if (!is)
	{
		return 1;
	}
	std::vector<int> v{ std::istream_iterator<int>(is),std::istream_iterator<int>() };
	std::vector<int *> binary=dec_to_binary(v);
	
	uint8_t buf;

	//uint8_t buf;
	//uint8_t bit; //deve essere 0 o 1
	//buf = buf * 2 + bit;
	//buf = (buf << 1) | bit; //mettere la parentesi perchè l'or potrebbe avere precendenza sullo shift
	//ogni volta che si aggiunge un bit, quando arriviamo ad 8 il buffer è pieno lo scrivo su file
	//scrivo i primi 8 bit e continuo 

}