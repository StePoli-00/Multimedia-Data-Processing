//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <map>
//#include <string>
//#include <cstdint>
//#include <exception>
//
//namespace ben {
//	struct elem {
//		virtual void print(size_t tab = 0) const = 0;
//	};
//
//	elem* elem_factory(std::istream& is);
//
//	struct string : public elem {
//		std::string val_;
//
//		string(std::istream& is) {
//			auto error = std::runtime_error("Error while parsing string");
//			size_t len;
//			char colon;
//			is >> len >> colon;
//			if (!is || colon != ':') {
//				throw error;
//			}
//			val_.resize(len);
//			is.read(&val_[0], len);
//			if (!is) {
//				throw error;
//			}
//		}
//
//		void print(size_t tab) const override {
//			for (const auto& x : val_) {
//				std::cout << (32 <= x && x <= 126 ? x : '.');
//			}
//		}
//	};
//
//	struct integer : public elem {
//		int64_t val_;
//
//		integer(std::istream& is) {
//			char start, stop;
//			is >> start >> val_ >> stop;
//			if (!is || start != 'i' || stop != 'e') {
//				throw std::runtime_error("Error while parsing integer");
//			}
//		}
//
//		void print(size_t tab) const override {
//			std::cout << val_;
//		}
//	};
//
//	struct list : public elem {
//		std::vector<elem*> val_;
//
//		list(std::istream& is) {
//			auto error = std::runtime_error("Error while parsing list");
//			char start;
//			is >> start;
//			if (!is || start != 'l') {
//				throw error;
//			}
//			while (is.peek() != 'e') {
//				val_.push_back(elem_factory(is));
//			}
//			is.get(); // remove 'e'
//		}
//
//		void print(size_t tab) const override {
//			std::cout << "[\n";
//			for (const auto& x : val_) {
//				std::cout << std::string(tab + 1, '\t');
//				x->print(tab + 1);
//				std::cout << "\n";
//			}
//			std::cout << std::string(tab, '\t') << "]";
//		}
//	};
//
//	struct dictionary : public elem {
//		std::map<std::string, elem*> val_;
//
//		dictionary(std::istream& is) {
//			auto error = std::runtime_error("Error while parsing dictionary");
//			char start;
//			is >> start;
//			if (!is || start != 'd') {
//				throw error;
//			}
//			while (is.peek() != 'e') {
//				string s(is);
//				val_[s.val_] = elem_factory(is);
//			}
//			is.get(); // remove 'e'
//		}
//
//		void print(size_t tab) const override {
//			std::cout << "{\n";
//			for (const auto& x : val_) {
//				std::cout << std::string(tab + 1, '\t') << x.first << " => ";
//				x.second->print(tab + 1);
//				std::cout << "\n";
//			}
//			std::cout << std::string(tab, '\t') << "}";
//		}
//	};
//
//	elem* elem_factory(std::istream& is) {
//		switch (is.peek()) {
//		break; case 'i': return new integer(is);
//		break; case 'l': return new list(is);
//		break; case 'd': return new dictionary(is);
//		break; default: return new string(is);
//		}
//	}
//}
//
////int main(int argc, char* argv[]) {
////	if (argc != 2) {
////		std::cout << "SYNTAX: " << argv[0] << "<file .torrent>\n";
////		return EXIT_FAILURE;
////	}
////
////	std::ifstream is(argv[1], std::ios::binary);
////	if (!is) {
////		std::cout << "Cannot open " << argv[1] << "\n";
////		return EXIT_FAILURE;
////	}
////
////	try {
////		ben::elem* root = ben::elem_factory(is);
////		root->print(0);
////	}
////	catch (std::exception& e) {
////		std::cout << e.what() << '\n';
////	}
////
////	return 0;
////}