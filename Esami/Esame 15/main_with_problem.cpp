//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <map>
//#include <iostream>
//#include <cstdint>
//#include <iomanip>
//#include <cassert>
//#include <string>
//#include <cstring>
//#include <algorithm>
//
//using namespace std;
//
//void FitCRC_Get16(uint16_t& crc, uint8_t byte)
//{
//	static const uint16_t crc_table[16] =
//	{
//		0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
//		0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
//	};
//	uint16_t tmp;
//	// compute checksum of lower four bits of byte
//	tmp = crc_table[crc & 0xF];
//	crc = (crc >> 4) & 0x0FFF;
//	crc = crc ^ tmp ^ crc_table[byte & 0xF];
//	// now compute checksum of upper four bits of byte
//	tmp = crc_table[crc & 0xF];
//	crc = (crc >> 4) & 0x0FFF;
//	crc = crc ^ tmp ^ crc_table[(byte >> 4) & 0xF];
//}
//
//
//
//struct field_definition {
//
//	uint8_t number = 0, size = 0, type = 0;
//
//};
//struct definition {
//
//	uint8_t reserved = 0, architecture = 0, numfields = 0, local_message_type = 0;
//	uint16_t global_message_number = 0;
//	std::vector<field_definition> fields;
//
//
//	void resize()
//	{
//		fields.resize(numfields);
//	}
//
//};
//
//
//
//struct type {
//
//	uint8_t val8_ = 0;
//	uint16_t val16_ = 0;
//	uint32_t val32_ = 0;
//	uint8_t size_ = 0;
//
//	void save(uint8_t size, uint32_t val) {
//
//		if (size == 1)
//		{
//			size_ = 1;
//			val8_ = (uint8_t)val;
//		}
//		else if (size == 2)
//		{
//			size_ = 2;
//			val16_ = (uint16_t)val;
//		}
//		else
//		{
//			size_ = 4;
//			val32_ = val;
//		}
//	}
//};
//
//
//
//struct message {
//
//	uint8_t header = 0, lmt = 0;
//	type data;
//};
//
//
//
//struct header {
//	uint8_t header_size, protocol_version;
//	uint16_t Profile_Version_LSB, CRC_LSB;
//	int Data_Size_LSB;
//	char Data_Type[5];
//
//};
//
//
//int read_header(std::ifstream& is, header& h) {
//
//
//	uint16_t crc = 0;
//
//
//
//
//
//	if (!(is.read(reinterpret_cast<char*>(&h.header_size), 1)))
//	{
//		return 1;
//	}
//	if (!(is.read(reinterpret_cast<char*>(&h.protocol_version), 1)))
//	{
//		return 1;
//	}
//	if (!(is.read(reinterpret_cast<char*>(&h.Profile_Version_LSB), 2)))
//	{
//		return 1;
//	}
//
//	if (!(is.read(reinterpret_cast<char*>(&h.Data_Size_LSB), 4)))
//	{
//		return 1;
//	}
//
//	if (!(is.read(&h.Data_Type[0], 4)))
//	{
//		return 1;
//	}
//	h.Data_Type[4] = '\0';
//	if (strcmp(h.Data_Type, ".FIT") != 0)
//	{
//		return 1;
//	}
//
//
//
//
//
//	is.seekg(0, std::ios::beg);
//	uint8_t byte;
//	for (int i = 0; i < h.header_size - 2; ++i)
//	{
//		is.read(reinterpret_cast<char*>(&byte), 1);
//		FitCRC_Get16(crc, byte);
//
//
//	}
//
//
//	if (!(is.read(reinterpret_cast<char*>(&h.CRC_LSB), 2)))
//	{
//		return 1;
//	}
//	if (crc != h.CRC_LSB)
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
//
//
//
//bool is_definiton(uint8_t& byte, uint8_t& local_message_type)
//{
//	uint8_t val = 0;
//	for (int i = 7; i >= 4; --i)
//	{
//		val = (val << 1) | (byte >> i);
//	}
//
//	local_message_type = 0;
//	for (int i = 4; i >= 0; --i)
//	{
//		local_message_type = (local_message_type << 1) | ((byte >> i) & 1);
//	}
//	if (val == 4)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//definition read_definition(std::ifstream& is)
//{
//
//	definition d;
//	d.reserved = is.get();
//	d.architecture = is.get();
//	is.read(reinterpret_cast<char*>(&d.global_message_number), 2);
//	d.numfields = is.get();
//
//	d.resize();
//	for (int i = 0; i < d.numfields; ++i)
//	{
//		d.fields[i].number = is.get();
//		d.fields[i].size = is.get();
//		d.fields[i].type = is.get();
//
//
//	}
//	return d;
//}
//
//
//definition find_def(std::vector<definition>& definitions, uint8_t& lmt)
//{
//
//	for (auto& d : definitions)
//	{
//		if (d.local_message_type == lmt)
//		{
//			return d;
//		}
//	}
//
//	definition d;
//	d.local_message_type = 16;
//	return d;
//}
//
//
//void print_data(std::vector<message>& messages)
//{
//	double v;
//
//	std::cout << "time_created = " << messages[0].data.val32_ << endl;
//
//	for (int i = 1; i < messages.size(); ++i)
//	{
//		//messages[i].data.val16_ 
//		v = (double(messages[i].data.val16_) / 1000) * 3.6;
//		std::cout << "avg_speed = " << setprecision(4) << v << " km/h" << endl;
//
//
//	}
//
//	return;
//}
//
//
//int read(std::ifstream& is, int record_size) {
//
//	uint8_t byte, lmt;
//	uint32_t val;
//	bool res;
//	int i;
//	std::vector<definition> definitions;
//	std::vector<message> messages;
//	for (i = 0; i < record_size; ++i)
//	{
//
//
//		byte = is.get();
//		//++i;
//		res = is_definiton(byte, lmt);
//
//		if (res)
//		{
//
//
//			definition d = read_definition(is);
//			d.local_message_type = lmt;
//			i = i + 5 + (d.fields.size() * 3);
//			if (i + 1 >= record_size)
//			{
//				int a = 0;
//			}
//
//			definitions.push_back(d);
//
//
//
//		}
//		else //Data message
//		{
//
//
//			definition d = find_def(definitions, lmt);
//
//			if (d.local_message_type != 16)
//			{
//				for (auto& f : d.fields)
//				{
//
//
//
//					is.read(reinterpret_cast<char*>(&val), f.size);
//					i += f.size;
//
//					if (d.global_message_number == 0 && d.local_message_type == 0 && f.number == 4)
//					{
//
//						type t;
//						t.save(f.size, val);
//						message m;
//						m.header = byte;
//						m.lmt = lmt;
//						m.data = t;
//						messages.push_back(m);
//
//					}
//					else if (d.global_message_number == 19 && d.local_message_type == 10 && f.number == 13)
//					{
//						type t;
//						t.save(f.size, val);
//						message m;
//						m.header = byte;
//						m.lmt = lmt;
//						m.data = t;
//						messages.push_back(m);
//
//					}
//				}
//			}
//
//
//		}
//
//
//	}
//	uint16_t CRC;
//	uint16_t crc = 0;
//
//	/*
//	seekg muove la posizione del puntatore in avanti o indietro del riferimento specificato,
//	in questo caso voglio tornare indietro "-" record_size posizioni, partendo dalla posizione corrente
//	posso quindi utilizzare la funzione del controllo del crc e leggere gli utlimi due byte.
//	Potevo anche tornare all'inizio del file, muovermi di +14 posizioni e poi usare la funzione
//
//	*/
//	is.seekg((-record_size), std::ios::cur);
//
//	for (int i = 0; i < record_size; ++i)
//	{
//		is.read(reinterpret_cast<char*>(&byte), 1);
//		FitCRC_Get16(crc, byte);
//	}
//	is.read(reinterpret_cast<char*>(&CRC), 2);
//	if (crc == CRC)
//	{
//		std::cout << "Header CRC ok\n";
//		print_data(messages);
//		std::cout << "File CRC ok\n";
//		return 0;
//	}
//	return 1;
//
//
//}
//
//
//int main(int argc, char** argv)
//{
//
//	if (argc != 2)
//	{
//		return 1;
//	}
//	std::string filename = argv[1];
//	if (filename.empty()) { return 1; }
//
//	std::ifstream is(filename, std::ios::binary);
//	if (!is)
//	{
//		return 1;
//
//	}
//	header header;
//	int res = read_header(is, header);
//	if (res != 0)
//	{
//		return 1;
//	}
//
//	res = read(is, header.Data_Size_LSB);
//	return res;
//}