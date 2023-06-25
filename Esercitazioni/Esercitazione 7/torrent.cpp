//#include <string>
//#include <fstream>
//#include <map>
//#include <exception>
//#include <vector>
//
//
//
//class Generic {
//
//public: 
//	virtual void print() const=0;
//	
//};
//Generic* bencode(std::ifstream& is);
//
//class Integer : public Generic {
//
//	int64_t num;	
//public:
//	Integer(std::ifstream& is)
//	{
//		char begin, end;
//		is >> begin >> num >> end;
//		if (begin != 'i' || end != 'e')
//		{
//			throw "invalid integer format";
//		}
//	}
//	void print() const override
//	{
//		
//	}
//
//};
//
//class String : public Generic {
//
//	std::string string;
//
//public:
//	String(std::ifstream& is)
//	{
//		int num;
//		char c;
//		if(is >> num)
//		{
//			throw "strings must  be preceded integer value";
//		}
//		is >> c;
//		string.resize(num);
//		is.read(&string[0], num);
//	}
//
//	std::string getString()
//	{
//		return this->string;
//	}
//
//	void print() const override {
//
//	}
//};
//
//
//class List : public Generic {
//
//	std::vector<Generic*> list;
//public:
//	List(std::ifstream& is)
//	{
//		char c;
//		is >> c; 
//		while (is.peek() != 'e')
//		{
//			list.push_back(bencode(is));
//		}
//		is.get();
//	}
//
//
//	void print() const override {
//
//	}
//	
//};
//
//class Dictionary : public Generic {
//	std::map<std::string, Generic *> dict;
//public:
//	Dictionary(std::ifstream& is)
//	{
//		char c;
//		is >> c;
//		if (c != 'd')
//		{
//			throw "invalid syntax for Dictiornary ";
//		}
//
//
//		while (is.peek() != 'e')
//		{
//			String str(is);
//			dict[str.getString()] = bencode(is);
//		}
//		is.get();
//	}
//
//	void print() const override{
//		
//	}
//};
//
//Generic* bencode(std::ifstream& is)
//{
//	while (is.peek() != 'e')
//	{
//		switch (is.peek())
//		{
//		case 'i': return new Integer(is);
//		case 'l': return new List(is);
//		case 'd': return new Dictionary(is);
//		default: return new String(is);
//			break;
//		}
//	}
//
//
//	 
//}
//
//
//int main(int argc, char** argv)
//{
//	if (argc != 2)
//	{
//		return -1;
//	}
//
//	std::string filename = argv[1];
//	if (filename.length() == 0)
//	{
//		return -1;
//	}
//	std::ifstream is(filename, std::ios::binary);
//	if (!is)
//	{
//		return -1;
//	}
//
//
//}