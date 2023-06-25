#include <fstream>
#include <iostream>
#include <cstdio>
#include <ios>
#include <cstdint>
#include <vector>
#include <algorithm>

template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}

class bitwriter {
	uint8_t buffer_;
	int n_ = 0;
	std::ostream& os_;

	std::ostream& write_bit(uint32_t bit) {
		buffer_ = buffer_ << 1 | (bit & 1);
		++n_;
		if (n_ == 8) {
			raw_write(os_, buffer_);
			n_ = 0;
		}
		return os_;
	}

public:
	bitwriter(std::ostream& os) : os_(os) {
	}

	std::ostream& write(uint32_t u, uint8_t n) {
		for (int i = n - 1; i >= 0; --i) {
			write_bit(u >> i);
		}
		return os_;
	}

	std::ostream& operator()(uint32_t u, uint8_t n) {
		return write(u, n);
	}

	std::ostream& flush(uint32_t bit = 0) {
		while (n_ > 0) {
			write_bit(bit);
		}
		return os_;
	}

	~bitwriter() {
		flush();
	}
};

template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&val), size);
}

class bitreader {
	uint8_t buffer_;
	uint8_t n_ = 0;
	std::istream& is_;

public:
	bitreader(std::istream& is) : is_(is) {}

	uint32_t read_bit() {
		if (n_ == 0) {
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		return (buffer_ >> n_) & 1;
	}

	uint32_t read(uint8_t n) {
		uint32_t u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return u;
	}

	uint32_t operator()(uint8_t n) {
		return read(n);
	}

	bool fail() const {
		return is_.fail();
	}

	explicit operator bool() const {
		return !fail();
	}
};

int code(char *argc[]) {
	std::ifstream fin(argc[2], std::ios::binary);
	std::string outname = argc[3];
	outname.append(".packbits");
	std::ofstream fout(outname, std::ios::binary);
	if (!fin) {
		std::cout << "Error opening input file.";
		return 1;
	}

	if (!fout) {
		std::cout << "Error opening output file.";
		return 1; 
	}

	uint8_t curr;
	uint8_t succ; 
	int n = 0; 
	int count = 0;
	std::vector<char> copy_vect; 
	char carattere; 
	bool break_total = false; 
	bool copy_sequence = false;  
	while (1) {
		if (n == 0) {
			raw_read(fin, curr); 
			if (!fin) {
				break; 
			}
			++n; 
		}
		else {
			while (1) {
				raw_read(fin, succ); 
				if (!fin) {
					curr = succ;
					count = n;
					n = 1;
					if (copy_sequence == true) {
						copy_vect.push_back(curr); 
						copy_sequence = false; 
					}
					break_total = true; 
					break; 
				}
				else if (succ == curr) {
					copy_sequence = false; 
					++n; 
					if (n == 129) {
						curr = succ;
						count = n - 1; 
						n = 1; 
						break; 
					}
					carattere = succ; 
				}
				else {
					if (n == 1) {
						carattere = curr; 
						copy_sequence = true;
						if (copy_vect.size() == 128) {
							uint8_t copy_size = (uint8_t)copy_vect.size() - 1;
							raw_write(fout, copy_size);
							for (auto& x : copy_vect) {
								fout.write(&x, 1);
							}
							copy_vect.clear();
						}
						copy_vect.push_back(carattere);
					}
					curr = succ;
					count = n; 
					n = 1; 
					break; 
				}
			}
			if (copy_sequence == true) {
				continue; 
			}
			else {
				if (copy_sequence == false && copy_vect.size() > 0) {
					uint8_t copy_size = (uint8_t)copy_vect.size() - 1; 
					raw_write(fout,  copy_size);
					for (auto& x : copy_vect) {
						fout.write(&x, 1); 
					}
					copy_vect.clear();
				}
				if (copy_vect.size() == 0 && !break_total || count > 1) {
					uint8_t b = 257 - count;
					if (b != 0) {
						raw_write(fout, b);
					}
					raw_write(fout, carattere);
				} 
				if (break_total) {
					uint8_t eof = 128; 
					raw_write(fout, eof); 
					break;
				}
			}
		}
	}
	return 0; 
}

int decode(char *argc[]) {
	std::ifstream fin(argc[2], std::ios::binary);
	std::ofstream fout(argc[3]);
	if (!fin) {
		std::cout << "Error opening input file.";
		return 1;
	}

	if (!fout) {
		std::cout << "Error opening output file.";
		return 1;
	}

	uint8_t l;
	uint8_t r; 
	while (1) {
		raw_read(fin, l);
		if (!fin || l == 128) {
			break;
		}
		else if (l >= 0 && l <= 127) {
			for (size_t i = 0; i <= l; ++i) {
				raw_read(fin, r);
				if (!r) {
					break; 
				}
				else {
					raw_write(fout, r); 
				}
			}
		}
		else if (l >= 129 && l <= 255) {
			raw_read(fin, r); 
			uint8_t count = 257 - l; 
			for (size_t i = 0; i < count; ++i) {
				raw_write(fout, r); 
			}
		}

	}

	return 0; 
}

int main(int argv, char* argc[]) {

	if (argv != 4) {
		std::cout << "Error number of parameters.";
		return 1;
	}

	if (*argc[1] != 'c' && *argc[1] != 'd') {
		std::cout << "Error first parameter must be 'c' or 'd'.";
		return 1;
	}

	if (*argc[1] == 'c') {
		int res = code(argc);
		return res;
	}
	else {
		int res = decode(argc);
		return res;
	}

	return 0; 
}