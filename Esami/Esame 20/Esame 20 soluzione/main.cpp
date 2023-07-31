#include <string>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>

std::unordered_map<uint8_t, std::string> opcode = {


	{0b00000000,"sml_d"},
	{0b00000001,"sml_d"},
	{0b00000010,"sml_d"},
	{0b00000011,"sml_d"},
	{0b00000100,"sml_d"},
	{0b00000101,"sml_d"},
	{0b00000110,"eos"},
	{0b00000111,"lrg_d"},
	{0b00001000,"sml_d"},
	{0b00001001,"sml_d"},
	{0b00001010,"sml_d"},
	{0b00001011,"sml_d"},
	{0b00001100,"sml_d"},
	{0b00001101,"sml_d"},
	{0b00001110,"nop"},
	{0b00001111,"lrg_d"},
	{0b00010000,"sml_d"},
	{0b00010001,"sml_d"},
	{0b00010010,"sml_d"},
	{0b00010011,"sml_d"},
	{0b00010100,"sml_d"},
	{0b00010101,"sml_d"},
	{0b00010110,"nop"},
	{0b00010111,"lrg_d"},
	{0b00011000,"sml_d"},
	{0b00011001,"sml_d"},
	{0b00011010,"sml_d"},
	{0b00011011,"sml_d"},
	{0b00011100,"sml_d"},
	{0b00011101,"sml_d"},
	{0b00011110,"udef"},
	{0b00011111,"lrg_d"},
	{0b00100000,"sml_d"},
	{0b00100001,"sml_d"},
	{0b00100010,"sml_d"},
	{0b00100011,"sml_d"},
	{0b00100100,"sml_d"},
	{0b00100101,"sml_d"},
	{0b00100110,"udef"},
	{0b00100111,"lrg_d"},
	{0b00101000,"sml_d"},
	{0b00101001,"sml_d"},
	{0b00101010,"sml_d"},
	{0b00101011,"sml_d"},
	{0b00101100,"sml_d"},
	{0b00101101,"sml_d"},
	{0b00101110,"udef"},
	{0b00101111,"lrg_d"},
	{0b00110000,"sml_d"},
	{0b00110001,"sml_d"},
	{0b00110010,"sml_d"},
	{0b00110011,"sml_d"},
	{0b00110100,"sml_d"},
	{0b00110101,"sml_d"},
	{0b00110110,"udef"},
	{0b00110111,"lrg_d"},
	{0b00111000,"sml_d"},
	{0b00111001,"sml_d"},
	{0b00111010,"sml_d"},
	{0b00111011,"sml_d"},
	{0b00111100,"sml_d"},
	{0b00111101,"sml_d"},
	{0b00111110,"udef"},
	{0b00111111,"lrg_d"},
	{0b01000000,"sml_d"},
	{0b01000001,"sml_d"},
	{0b01000010,"sml_d"},
	{0b01000011,"sml_d"},
	{0b01000100,"sml_d"},
	{0b01000101,"sml_d"},
	{0b01000110,"pre_d"},
	{0b01000111,"lrg_d"},
	{0b01001000,"sml_d"},
	{0b01001001,"sml_d"},
	{0b01001010,"sml_d"},
	{0b01001011,"sml_d"},
	{0b01001100,"sml_d"},
	{0b01001101,"sml_d"},
	{0b01001110,"pre_d"},
	{0b01001111,"lrg_d"},
	{0b01010000,"sml_d"},
	{0b01010001,"sml_d"},
	{0b01010010,"sml_d"},
	{0b01010011,"sml_d"},
	{0b01010100,"sml_d"},
	{0b01010101,"sml_d"},
	{0b01010110,"pre_d"},
	{0b01010111,"lrg_d"},
	{0b01011000,"sml_d"},
	{0b01011001,"sml_d"},
	{0b01011010,"sml_d"},
	{0b01011011,"sml_d"},
	{0b01011100,"sml_d"},
	{0b01011101,"sml_d"},
	{0b01011110,"pre_d"},
	{0b01011111,"lrg_d"},
	{0b01100000,"sml_d"},
	{0b01100001,"sml_d"},
	{0b01100010,"sml_d"},
	{0b01100011,"sml_d"},
	{0b01100100,"sml_d"},
	{0b01100101,"sml_d"},
	{0b01100110,"pre_d"},
	{0b01100111,"lrg_d"},
	{0b01101000,"sml_d"},
	{0b01101001,"sml_d"},
	{0b01101010,"sml_d"},
	{0b01101011,"sml_d"},
	{0b01101100,"sml_d"},
	{0b01101101,"sml_d"},
	{0b01101110,"pre_d"},
	{0b01101111,"lrg_d"},
	{0b01110000,"udef"},
	{0b01110001,"udef"},
	{0b01110010,"udef"},
	{0b01110011,"udef"},
	{0b01110100,"udef"},
	{0b01110101,"udef"},
	{0b01110110,"udef"},
	{0b01110111,"udef"},
	{0b01111000,"udef"},
	{0b01111001,"udef"},
	{0b01111010,"udef"},
	{0b01111011,"udef"},
	{0b01111100,"udef"},
	{0b01111101,"udef"},
	{0b01111110,"udef"},
	{0b01111111,"udef"},
	{0b10000000,"sml_d"},
	{0b10000001,"sml_d"},
	{0b10000011,"sml_d"},
	{0b10000100,"sml_d"},
	{0b10000101,"sml_d"},
	{0b10000110,"sml_d"},
	{0b10000111,"lrg_d"},
	{0b10001000,"sml_d"},
	{0b10001001,"sml_d"},
	{0b10001010,"sml_d"},
	{0b10001011,"sml_d"},
	{0b10001100,"sml_d"},
	{0b10001101,"sml_d"},
	{0b10001110,"pre_d"},
	{0b10001111,"lrg_d"},
	{0b10010000,"sml_d"},
	{0b10010001,"sml_d"},
	{0b10010010,"sml_d"},
	{0b10010011,"sml_d"},
	{0b10010100,"sml_d"},
	{0b10010101,"sml_d"},
	{0b10010110,"pre_d"},
	{0b10010111,"lrg_d"},
	{0b10011000,"sml_d"},
	{0b10011001,"sml_d"},
	{0b10011010,"sml_d"},
	{0b10011011,"sml_d"},
	{0b10011100,"sml_d"},
	{0b10011101,"sml_d"},
	{0b10011110,"pre_d"},
	{0b10011111,"lrg_d"},
	{0b10100000,"med_d"},
	{0b10100001,"med_d"},
	{0b10100010,"med_d"},
	{0b10100011,"med_d"},
	{0b10100100,"med_d"},
	{0b10100101,"med_d"},
	{0b10100110,"med_d"},
	{0b10100111,"med_d"},
	{0b10101000,"med_d"},
	{0b10101001,"med_d"},
	{0b10101010,"med_d"},
	{0b10101011,"med_d"},
	{0b10101100,"med_d"},
	{0b10101101,"med_d"},
	{0b10101110,"med_d"},
	{0b10101111,"med_d"},
	{0b10110000,"med_d"},
	{0b10110001,"med_d"},
	{0b10110010,"med_d"},
	{0b10110011,"med_d"},
	{0b10110100,"med_d"},
	{0b10110101,"med_d"},
	{0b10110110,"med_d"},
	{0b10110111,"med_d"},
	{ 0b10111000,"med_d" },
	{ 0b10111001,"med_d" },
	{ 0b10111010,"med_d" },
	{ 0b10111011,"med_d" },
	{ 0b10111100,"med_d" },
	{ 0b10111101,"med_d" },
	{ 0b10111110,"med_d" },
	{ 0b10111111,"med_d" },
	{ 0b11000000,"sml_d" },
	{ 0b11000001,"sml_d" },
	{ 0b11000010,"sml_d" },
	{ 0b11000011,"sml_d" },
	{ 0b11000100,"sml_d" },
	{ 0b11000101,"sml_d" },
	{ 0b11000110,"pre_d" },
	{ 0b11000111,"lrg_d" },
	{ 0b11001000,"sml_d" },
	{ 0b11001001,"sml_d" },
	{ 0b11001010,"sml_d" },
	{ 0b11001011,"sml_d" },
	{ 0b11001100,"sml_d" },
	{ 0b11001101,"sml_d" },
	{ 0b11001110,"pre_d" },
	{ 0b11001111,"lrg_d" },
	{ 0b11010000,"udef" },
	{ 0b11010001,"udef" },
	{ 0b11010010,"udef" },
	{ 0b11010011,"udef" },
	{ 0b11010100,"udef" },
	{ 0b11010101,"udef" },
	{ 0b11010110,"udef" },
	{ 0b11010111,"udef" },
	{ 0b11011000,"udef" },
	{ 0b11011001,"udef" },
	{ 0b11011010,"udef" },
	{ 0b11011011,"udef" },
	{ 0b11011100,"udef" },
	{ 0b11011101,"udef" },
	{ 0b11011110,"udef" },
	{ 0b11011111,"udef" },
	{ 0b11100000,"lrg_l" },
	{ 0b11100001,"sml_l" },
	{ 0b11100010,"sml_l" },
	{ 0b11100011,"sml_l" },
	{ 0b11100100,"sml_l" },
	{ 0b11100101,"sml_l" },
	{ 0b11100110,"sml_l" },
	{ 0b11100111,"sml_l" },
	{ 0b11101000,"sml_l" },
	{ 0b11101001,"sml_l" },
	{ 0b11101010,"sml_l" },
	{ 0b11101011,"sml_l" },
	{ 0b11101100,"sml_l" },
	{ 0b11101101,"sml_l" },
	{ 0b11101110,"sml_l" },
	{ 0b11101111,"sml_l" },
	{ 0b11110000,"lrg_m" },
	{ 0b11110001,"sml_m" },
	{ 0b11110010,"sml_m" },
	{ 0b11110011,"sml_m" },
	{ 0b11110100,"sml_m" },
	{ 0b11110101,"sml_m" },
	{ 0b11110110,"sml_m" },
	{ 0b11110111,"sml_m" },
	{ 0b11111000,"sml_m" },
	{ 0b11111001,"sml_m" },
	{ 0b11111010,"sml_m" },
	{ 0b11111011,"sml_m" },
	{ 0b11111100,"sml_m" },
	{ 0b11111101,"sml_m" },
	{ 0b11111110,"sml_m" },
	{ 0b11111111,"sml_m" },
};

std::string find_opcode(uint8_t& byte)
{
	
	auto it = opcode.find(byte);
	if (it != opcode.end())
	{
		return it->second;
	}
	
	return "";


}


struct tuple {

	int length = 0;
	int match = 0;
	int distance = 0;
};


void write_output(std::istream& is, tuple &t, int &last_dist ,std::vector<uint8_t> & buffer)
{

	
		uint8_t val;
		for (int i = 0; i < t.length; ++i)
		{

			val = is.get();
			buffer.push_back(val);
			//std::cout << val;
			
		}

		if (t.match != 0 && t.distance != 0)
		{

			int cursor = buffer.size();
			buffer.resize(buffer.size() + t.match);
			for (int i = 0; i < t.match; ++i)
			{
				val = buffer[cursor - t.distance + i];
				buffer[cursor + i] = val;
				//std::cout << val;

			}

			last_dist = t.distance;
		}
			


	return;

}



int decode_LZVN(std::string& filein, std::string& fileout) {

	std::vector<uint8_t> decoded_buffer;
	std::ifstream is(filein, std::ios::binary);
	std::ofstream os(fileout, std::ios::binary);
	if (!is || !os)
	{
		return 1;
	}
	int last_dist = 0;
	uint8_t byte1, byte2, byte3;

	int magic_number, output_dim, block_size;

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

	while (1)
	{
		tuple t;
		byte2=0, byte3 = 0;
		byte1 = is.get();
		std::string opcode=find_opcode(byte1);

		if (opcode == "sml_d")
		{
			byte2 = is.get();

			t.length = (byte1 & 0b11000000) >> 6;
			t.match = (byte1 & 0b00111000) >> 3;
			t.match += 3;
			t.distance = (byte1 & 0b00000111);
			t.distance = (t.distance << 8) | byte2;

		}
		else if(opcode == "med_d")
		{ 
			byte2 = is.get();
			byte3 = is.get();
			t.length = (byte1 & 0b00011000) >> 3;
			t.match = (byte1 & 0b00000111);
			t.match = (t.match << 2) | (byte2 & 0b00000011);
			t.match += 3;

			t.distance = byte3;
			t.distance = (t.distance << 5) | (byte2 & 0b11111100);

		}
		else if (opcode == "lrg_d")
		{
			byte2 = is.get();
			byte3 = is.get();
			t.length = (byte1 & 0b11000000) >> 6;
			t.match = (byte1 & 0b00111000)>>3;
			t.match = (t.match << 2) | (byte2 & 0b00000011);
			t.match += 3;
			t.distance = byte3;
			t.distance = (t.distance << 8) | (byte2);


		}
		else if (opcode == "pre_d") {
			t.length = (byte1 & 0b11000000) >> 6;
			t.match = (byte1 & 0b00111000) >> 3;
			t.match += 3;
			t.distance = last_dist;

		}
		else if (opcode == "sml_m") {
			t.length = 0;
			t.match = (byte1 & 0b00001111);
			t.distance = last_dist;
		}
		else if (opcode == "lrg_m") {
			t.length = 0;
			t.match = is.get()+16;
			t.distance = last_dist;
		}
		else if (opcode == "sml_l")
		{
			t.length = (byte1 & 0b00001111);

		}
		else if (opcode == "lrg_l") {

			t.length = is.get()+16;

		}
		else if (opcode == "nop") {

		}
		else if (opcode == "udef") {
			int a = 3;
			std::cout << "Error undefined opcode found\n";
			return 1;
		}
		else if (opcode == "eos")
		{
			break;
		}
		else
		{
			std::cout << "Error opcode not found\n";
			return 1;
		}

		write_output(is, t, last_dist,decoded_buffer);
	}

	//print buffer
	for (auto& c : decoded_buffer)
	{
		os.put(c);
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
	int res=decode_LZVN(filein, fileout);
	return res;
}