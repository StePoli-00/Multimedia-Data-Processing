#include <fstream>
#include <iostream>
#include <bitset>

class Bitwriter {
public:
    Bitwriter(std::string filein, std::string fout, int n)
        : m_filein(filein), m_fout(fout), m_n(n)
    {}

    bool write() {
        std::ifstream is(m_filein);
        if (!is) {
            std::cerr << "Failed to open is file\n";
            return false;
        }

        std::ofstream os(m_fout, std::ios::binary);
        if (!os) {
            std::cerr << "Failed to open os file\n";
            return false;
        }

        int num;
        while (is >> num) {
            if (num < -1 * (1 << (m_n - 1)) || num >= (1 << (m_n - 1))) {
                std::cerr << "Number " << num << " is out of range for n=" << m_n << "\n";
                return false;
            }

            std::bitset<32> bits(num);
            std::string bit_string = bits.to_string().substr(32 - m_n);

            for (int i = 0; i < m_n; i += 8) {
                std::bitset<8> byte(bit_string.substr(i, 8));
                os.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
            }
        }

        return true;
    }

private:
    std::string m_filein;
    std::string m_fout;
    int m_n;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: write_intn <filein> <fout> <n>\n";
        return 1;
    }

    std::string filein(argv[1]);
    std::string fout(argv[2]);
    int n = atoi(argv[3]);

    Bitwriter writer(filein, fout, n);
    if (!writer.write()) {
        return 1;
    }

    return 0;
}