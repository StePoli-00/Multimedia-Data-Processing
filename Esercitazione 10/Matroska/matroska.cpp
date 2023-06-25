//#include <unordered_map>
//#include <string>
//#include <vector>
//#include <fstream>
//#include <variant>
//namespace ebml {
//
//	/*Le enum class a differenza delle enum del c non 'inquinano' il namespace
//	globale, nel c una volta dato un nome con un enum non si poteva scrivere un altra variabile
//	con il nome uguale a quelal dell'enum*/
//	enum class types {
//		sint, uint, flt, string, utf8, date, master, binary
//
//	};
//	std::unordered_map<uint64_t, std::pair<std::string, types>> known_elems{
//		//id //stringa e tipo
//		{0x11,{"EBML",types::master}},
//
//	};
//
//
//
//struct elem {
//	/*definisco il tipo di dati necessari*/
//	using sint = int64_t;
//	using uint = uint64_t;
//	using flt = double;
//	using string = std::string;
//	using master = std::vector<struct elem>;
//	
//	
//	uint64_t id_, size_, len_;
//	/*attributi di un elemento,la lunghezza che non è fissa
//	variant è una union che risolvono i problemmi della 
//	union simile ad uan struct all'interno della stessa zona di memoria
//	le union non sono puntatory
//	le variant contengono un indice */
//	std::variant<sint, uint, flt, string, master> data_;
//
//	
//	uint64_t read_vint(std::istream& is, bool id, uint64_t& len)
//	{
//		uint64_t val = uint8_t(is.get()); //leggo il primo byte
//		++len;//tengo traccia della lunghezza del campo
//		int more_bytes = 0;
//		/*bisogna vedere il primo byte per leggere un vint*/
//		/*0b=serve per indicare che il numreo è binario
//		 '=serve per separare i byte, viene ignorato*/
//		if (val >= 0b1000'0000) {
//			/*se è un id teniamo tutto
//			altrimenti tolgo il primo byte*/
//
//			val = id ? val : val & 0b0111'1111;
//		}
//		else if (val >= 0b0100'0000) {
//
//			
//		}
//
//		//...
//
//		for (int i = 0; i < more_bytes; ++i)
//		{
//			val = (val << 8)|uint8_t(is.get());
//		}
//	}
//
//
//	elem(std::istream& is)
//	{
//		len_ = 0;
//		id_ = read_vint(is, true, len_);
//		size_ = read_vint(is, false, len_);
//		len_ += size_;
//		switch (known_elems[id_].second) {
//		break; case types::sint: {
//			sint val = 0;
//			for (int i = 0; i <size_; ++i)
//			{
//				//sposto a sinitra di 8 bit
//				val = (val << 8) | uint8_t(is.get());
//			}
//			data_ = val; //basta assegnare
//		}
//		}
//		
//		
//	}
//};
//
//};