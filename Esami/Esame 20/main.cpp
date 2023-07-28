#include <string>
#include <fstream>
#include <vector>
#include <iostream>


bool check_undefined(uint8_t& val)
{
	switch (val)
	{
	break; case(0b00011110): return true;
	break; case(0b00100110): return true;
	break; case(0b00101110): return true;
	break; case(0b00110110): return true;
	break; case(0b00111110): return true;
	default: break;
	}
	return false;
}


void write_output(std::istream &is,std::ostream& os, std::vector<uint8_t>& decoder_buffer, int& literal_lenght, int& match_lenght, int& dist, int& last_dist)
{

	if (literal_lenght != 0)
	{

		uint8_t val;
		for (int i = 0; i < literal_lenght; ++i)
		{
			val = is.get();
			decoder_buffer.push_back(val);
			os.put(val);
		}
	}
		
		if (match_lenght>=dist)
		{
			int index ,j = 0;
			int size = decoder_buffer.size();
			for (int i = 0; i < match_lenght; ++i)
			{
				if (j == size)
				{
					j = 0;
				}
				index = size - dist+j;
				++j;
				decoder_buffer.push_back(decoder_buffer[index]);
				os.put(decoder_buffer[index]);
			}
			last_dist = dist;
		}
		else
		{
			int index, size = decoder_buffer.size();
			for (int i = 0; i < match_lenght; ++i)
			{
				index = size - dist + i;
				decoder_buffer.push_back(decoder_buffer[index]);
				os.put(decoder_buffer[index]);
			}
			last_dist = dist;
		}
		

	
	
	return;
}



int decode_LZVN(std::string filein, std::string fileout)
{
	if (filein.empty() || fileout.empty())
	{
		return 1;
	}
	std::ifstream is(filein, std::ios::binary);
	std::ofstream os(fileout, std::ios::binary);
	std::vector<uint8_t> decoder_buffer;
	if (!is|| !os)
	{
		return 0;
	}

	uint32_t magic_number, output_dim, block_size;

	if (!(is.read(reinterpret_cast<char*>(&magic_number), sizeof(uint32_t))))
	{
		return 1;
	}

	if (magic_number != 1853388386)
	{
		return 1;
	}

	if (!(is.read(reinterpret_cast<char*>(&output_dim), sizeof(uint32_t))))
	{
		return 1;
	}

	if (!(is.read(reinterpret_cast<char*>(&block_size), sizeof(uint32_t))))
	{
		return 1;
	}

	uint8_t val,high,low;
	uint8_t byte1, byte2, byte3;
	int dist = 0, literal_lenght = 0, match_lenght = 0, last_dist = 0;
	while (1)
	{

		val = is.peek();
		literal_lenght = 0, match_lenght = 0;
		dist = 0;
		if (val == 0b00000110) //EOS
		{
			val = is.get(); 
			break;
		}
		else if (val == 0b00001110 || val == 0b00010110) //nop
		{
			val = is.get(); 
		}
		else if (check_undefined(val))
		{
			val = is.get(); 
			std::cout << "incontrata una operazione indefinita";
			return 1;
		}
		else if (val == 0b11110000) //corrisp grande
		{
			val = is.get(); 
			match_lenght = is.get() + 16;
			dist = last_dist;

		}
		else if (val == 0b11100000) //grande letterale
		{
			val = is.get(); //estraggo il byte
			literal_lenght = is.get() + 16;

			

		}
		else //controllare i primi 4 bit
		{

			high = (val & 0b1111'0000) >> 4;
			if (high == 0b1111) //corrisp piccola
			{
				val = is.get();
				match_lenght = (val & 0b0000'1111);
				dist = last_dist;
			}
			else if (high == 0b1110) //piccolo letterale
			{
				val = is.get(); //estrallo il byte che contiene la lunghezza
				literal_lenght = (val & 0b0000'1111);
				//devo scrivere in output il literale

			}
			else if (high == 0b0111 || high == 1101) //undef 4 bit
			{
				std::cout << "incontrata una operazione indefinita";
				return 1;
			}
			else
			{
				high = (val & 0b1110'0000) >> 5;


				if (high == 0b101) //media distanza
				{
					byte1 = is.get(); //estrallo il byte che contiene la lunghezza
					byte2 = is.get();
					byte3 = is.get();
					literal_lenght = (byte1 & 0b00011000) >> 3;
					match_lenght = (byte1 & 0b00000111);
					val=(byte2 & 0b00000011);
					//match_lenght = (match_lenght * 4 + val);
					match_lenght = (match_lenght << 2) | val;
					match_lenght += 3;
					dist = (byte3 <<5)| ((byte2 & 0b11111100) >> 2);
					

					//leggo il literale
				}
				else // piccola distanza,grande dist,dist prec
				{

					low = (val & 0b00000111);
					if (low == 0b111) //grande distanza
					{
						/*
						literal_lenght = ((byte1 & 0b00011000) >> 3);
						match_lenght = (byte1 & 0b00000111);
						match_lenght += (byte2 & 0b00000011);
						match_lenght += 3;
						dist = (byte2 & 0b11111100) >> 2;
						dist += byte3;*/
						byte1 = is.get();
						literal_lenght = ((byte1 & 0b11000000) >> 6);
						match_lenght += 3;
						match_lenght = ((byte1 & 0b00111000) >> 3);
						is.read(reinterpret_cast<char*>(&dist), sizeof(uint16_t));



					}
					else if (low == 0b110) //distanza precedente
					{
						val = is.get();
						literal_lenght = (val & 0b11000000) >> 6;
						match_lenght = (val & 0b00111000) >> 3;
						dist = last_dist;
					}
					else // small dist
					{
					
						byte1 = is.get();
						byte2 = is.get();
						literal_lenght = (byte1 & 0b1100'0000) >> 6;
						match_lenght = (byte1 & 0b00111000) >> 3;
						match_lenght += 3;
						dist = (byte1 & 0b00000111);
						dist = (dist << 8) | byte2;


					}
				}



			}

			write_output(is,os,decoder_buffer, literal_lenght, match_lenght, dist, last_dist);


		}
	}

	return 0;
}


int main(int argc, char** argv)
{

	if (argc != 3)
	{
		return 1;
	}
	std::string filein = argv[1];
	std::string fileout = argv[2];
	int res = decode_LZVN(filein, fileout);




}