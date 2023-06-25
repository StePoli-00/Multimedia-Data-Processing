#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <exception>
#include <functional>
#include <format>

namespace ben {
	struct elem {
		//se abbiamo un metodo abbiamo bisogno di un distruttore
		virtual void print(size_t tab = 0) const = 0;
		virtual ~elem() {};

	};

	elem* elem_factory(std::istream& is);

	struct string : public elem {
		std::string val_;

		string(std::istream& is) {
			auto error = std::runtime_error("Error while parsing string");
			size_t len;
			char colon;
			is >> len >> colon;
			if (!is || colon != ':') {
				throw error;
			}
			val_.resize(len);
			is.read(&val_[0], len);
			if (!is) {
				throw error;
			}
		}

		void print(size_t tab) const override {
			for (const auto& x : val_) {
				std::cout << (32 <= x && x <= 126 ? x : '.');
			}
		}
	};

	struct integer : public elem {
		int64_t val_;

		integer(std::istream& is) {
			char start, stop;
			is >> start >> val_ >> stop;
			if (!is || start != 'i' || stop != 'e') {
				throw std::runtime_error("Error while parsing integer");
			}
		}

		void print(size_t tab) const override {
			std::cout << val_;
		}
	};

	struct list : public elem {
		std::vector<elem*> val_;

		list(std::istream& is) {
			auto error = std::runtime_error("Error while parsing list");
			char start;
			is >> start;
			if (!is || start != 'l') {
				throw error;
			}
			while (is.peek() != 'e') {
				val_.push_back(elem_factory(is));
			}
			is.get(); // remove 'e'
		}

		void print(size_t tab) const override {
			std::cout << "[\n";
			for (const auto& x : val_) {
				std::cout << std::string(tab + 1, '\t');
				x->print(tab + 1);
				std::cout << "\n";
			}
			std::cout << std::string(tab, '\t') << "]";
		}

		~list() {
			for (auto& x : val_)
			{
				delete x;
			}

		}
	};

	struct dictionary : public elem {
		std::map<std::string, elem*> val_;

		/*
		* mappa stringa-funzione
		* function= puntatore a funzione
		* void(const elem*, size_t)=funzione
		* static= variabile della classe legato alla classe, è una variabile globale
		* per chiamarla dictionary::special_pring
		*/
		static std::map<std::string, std::function<void(const elem*, size_t)>> special_print;

		dictionary(std::istream& is) {
			auto error = std::runtime_error("Error while parsing dictionary");
			char start;
			is >> start;
			if (!is || start != 'd') {
				throw error;
			}
			while (is.peek() != 'e') {
				string s(is);
				val_[s.val_] = elem_factory(is);
			}
			is.get(); // remove 'e'
		}

		void print(size_t tab) const override {
			std::cout << "{\n";
			/*per ogni elemento dentro al dizionario
			* stampa diversamente a seconda del tipo
			*/
			for (const auto& x : val_) {
				std::cout << std::string(tab + 1, '\t') << x.first << " => ";
				//cerco la chiave dentro la mappa
				//find ritorna un iteratore (iteratore di un albero perchè le mappe sono degli alberi
				auto sp = special_print.find(x.first);
				//se non trova l'elemento ritorna END
				if (sp != special_print.end()) {
					//chiamo la funzione per stampare quell'elemento
					sp->second(x.second, tab + 1);
				}
				else {
					x.second->print(tab + 1);
				}
				std::cout << "\n";
			}
			std::cout << std::string(tab, '\t') << "}";
		}

		~dictionary() 
		{

			for (auto& x : val_)
			{
				delete x.second;
			}

		}



	};
	//bisogna definirla anche al di fuori della variabile anche questa andrà deallocata
	std::map<std::string, std::function<void(const elem*, size_t)>> dictionary::special_print;

	elem* elem_factory(std::istream& is) {
		switch (is.peek()) {
		break; case 'i': return new integer(is);
		break; case 'l': return new list(is);
		break; case 'd': return new dictionary(is);
		break; default: return new string(is);
		}
	}
}

/*ricevo un elemento generico, perchè si ha una mappa di funzioni che devono avere tutti lo stesso tipo di dato
* L'elem generico lo utilizzo solo se è un pieces 
*/
void print_pieces(const ben::elem* e, size_t tab) {
	/*dynamic_cast come il reinterpret_ ma se il tipo è sbagliato
	ritorna un null pointer e lo controlla a tempo di esecuzione 
	all'interno delle classi si trovano i tipi delle classsi 
	RTTI: runtime type  information 
	esiste anche istance of */
	auto pieces = dynamic_cast<const ben::string*>(e);
	if (!pieces) {
		throw std::runtime_error("pieces is not a string!");
	}
	if (pieces->val_.size() % 20 != 0) {
		throw std::runtime_error("pieces len is not a multiple of 20!");
	}
	for (size_t i = 0; i < pieces->val_.size(); i += 20) {
		std::cout << "\n" << std::string(tab + 1, '\t');
		for (size_t j = 0; j < 20; ++j) {
			//formattazione ogni 20 caratteri in esadecimale
			//std::cout << std::format("{:02x}", unsigned char(pieces->val_[i + j]));
		}
	}
}

int main(int argc, char* argv[]) 
{
	if (argc != 2) {
		std::cout << "SYNTAX: " << argv[0] << "<file .torrent>\n";
		return EXIT_FAILURE;
	}

	std::ifstream is(argv[1], std::ios::binary);
	if (!is) {
		std::cout << "Cannot open " << argv[1] << "\n";
		return EXIT_FAILURE;
	}
	//quando il dictionary conterrà pieces stamperà pieces
	ben::dictionary::special_print["pieces"] = print_pieces;

	try {
		ben::elem* root = ben::elem_factory(is);
		root->print(0);
		delete root;
	}
	catch (std::exception& e) {
		std::cout << e.what() << '\n';
	}
	//verifica se tutto è stato deallocato
	_CrtDumpMemoryLeaks();
	/*con le new non deallochiamo mai la memoria*/
	return 0;
}
