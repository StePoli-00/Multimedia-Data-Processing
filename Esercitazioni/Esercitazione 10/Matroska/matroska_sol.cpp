#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <exception>
#include <cstdint>
#include <get>
namespace ebml {

	enum class types {
		sint, uint, flt, string, utf8, date, master, binary
	};

	std::unordered_map<uint64_t, std::pair<std::string, types>> known_elems{
		{0x1A45DFA3, {"EBML", types::master}},
		{0x4286, {"EBMLVersion", types::uint}},
		{0x42F7, {"EBMLReadVersion", types::uint}},
		{0x42F2, {"EBMLMaxIDLength", types::uint}},
		{0x42F3, {"EBMLMaxSizeLength", types::uint}},
		{0x4282, {"DocType", types::string}},
		{0x4287, {"DocTypeVersion", types::uint}},
		{0x4285, {"DocTypeReadVersion", types::uint}},
		{0xBF, {"CRC32", types::binary}},
		{0xEC, {"Void", types::binary}},
	};

	struct elem {
		using sint = int64_t;
		using uint = uint64_t;
		using flt = double;
		using string = std::string;
		using master = std::vector<struct elem>;

		uint64_t id_, size_, len_;
		std::variant<sint, uint, flt, string, master> data_;

		uint64_t read_vint(std::istream& is, bool id, uint64_t& len) {
			uint64_t val = uint8_t(is.get());
			++len;
			int more_bytes = 0;
			if (val >= 0b1000'0000) {
				val = id ? val : val & 0b0111'1111;
				more_bytes = 0;
			}
			else if (val >= 0b0100'0000) {
				val = id ? val : val & 0b0011'1111;
				more_bytes = 1;
			}
			else if (val >= 0b0010'0000) {
				val = id ? val : val & 0b0001'1111;
				more_bytes = 2;
			}
			else if (val >= 0b0001'0000) {
				val = id ? val : val & 0b0000'1111;
				more_bytes = 3;
			}
			else if (val >= 0b0000'1000) {
				val = id ? val : val & 0b0000'0111;
				more_bytes = 4;
			}
			else if (val >= 0b0000'0100) {
				val = id ? val : val & 0b0000'0011;
				more_bytes = 5;
			}
			else if (val >= 0b0000'0010) {
				val = id ? val : val & 0b0000'0001;
				more_bytes = 6;
			}
			else if (val >= 0b0000'0001) {
				val = id ? val : val & 0b0000'0001;
				more_bytes = 7;
			}
			else {
				throw std::runtime_error("0 byte starting a VINT");
			}
			for (int i = 0; i < more_bytes; ++i) {
				//sposto di 8 poi sommo i bit letti
				val = (val << 8) | uint8_t(is.get());
				++len;
			}
			return val;
		}

		elem(std::istream& is) {
			len_ = 0;
			id_ = read_vint(is, true, len_);
			size_ = read_vint(is, false, len_);
			len_ += size_;
			switch (known_elems[id_].second) {
			break; case types::sint: {
				sint val = 0;
				for (uint64_t i = 0; i < size_; ++i) {
					val = (val << 8) | uint8_t(is.get());
				}
				data_ = val;
			}
			break; case types::uint: {
				uint val = 0;
				for (uint64_t i = 0; i < size_; ++i) {
					val = (val << 8) | uint8_t(is.get());
				}
				data_ = val;
			}
			break; case types::flt:
			break; case types::string: {
				string val(size_t(size_), ' ');
				is.read(val.data(), size_);
				data_ = val;
			}
			break; case types::utf8:
			break; case types::date:
			break; case types::master: {
				uint64_t nread = 0;
				master elems;
				while (nread < size_) {
					elems.emplace_back(is);
					nread += elems.back().len_;
				}
				data_ = elems;
			}
			break; case types::binary:
			break; default:
				throw std::runtime_error("Cannot happen");
			}
		}

		
	};

}

void print(ebml::elem & header, int n)
{
	if (n == 0)
	{
		auto x=st
	}

}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		return EXIT_FAILURE;
	}
	std::ifstream is(argv[1], std::ios::binary);
	if (!is) {
		return EXIT_FAILURE;
	}

	ebml::elem header(is);
	header.print();
	return EXIT_SUCCESS;
}