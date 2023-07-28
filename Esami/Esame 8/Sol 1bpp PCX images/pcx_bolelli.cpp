#include "pcx.h"
#include "pgm.h"
#include <fstream>

//template<typename T>
//std::istream& raw_read(std::istream& in, T& val, size_t size = sizeof(T)) {
//	return in.read(reinterpret_cast<char*>(&val), size); 
//}

bool check_run(uint8_t byte) {
	uint8_t bit = 0; 
	for (int i = 7; i > 5; --i) {
		bit = (bit << 1) | ((byte >> i) & 1);
	}
	// FB+-
	return bit == 3; // FB l'if/else finale posso evitarlo.
	//if (bit == 3) {
	//	return true;
	//}
	//else {
	//	return false;
	//}

	// FB++ O, ancora meglio: 
	// return (byte & 0b11000000) == 0b11000000 ; // o anche (byte & 0b11000000) == 3 
}

bool load_pcx(const std::string& filename, mat<uint8_t>& img) {

	std::ifstream in(filename, std::ios::binary); 
	if (!in) {
		return false; 
	}

	//INIZIO PARSER DELL'HEADER PCX
	uint16_t image_width, image_height; 
	uint8_t manufacturer, version, encoding, bitsPerPlane, color_planes, palette[48], reserved;
	uint16_t windowxmin, windowymin, windowxmax, windowymax, bytesPerPlaneLine, vertDPI, horzDPI;
	uint16_t paletteinfo, horscrsize, verscrsize;
	char byte[54];
	size_t b_read = 0;

	manufacturer = in.get();
	if (manufacturer != 0x0A) {
		return false;
	}
	++b_read;
	in >> version;
	if (version != 5) {
		return false;
	}
	++b_read;
	in >> encoding;
	if (encoding != 1) {
		return false;
	}
	++b_read;
	in >> bitsPerPlane;
	// FB-- (non è sbagliato, ma non lo avrei mai scritto così, ma piuttosto (vedi sotto):
	/*if (bitsPerPlane != 1) {
		if (bitsPerPlane != 2) {
			if (bitsPerPlane != 4) {
				if (bitsPerPlane != 8) {
					return false;
				}
			}
		}
	}*/
	if (bitsPerPlane != 1 && bitsPerPlane != 2 && bitsPerPlane != 4 && bitsPerPlane != 8) {// FB++
		return false;
	}
	++b_read;

	if (!in.read(reinterpret_cast<char*> (&windowxmin), 2 /** sizeof(char)*/)) { //FB-+ visto che devo leggere due byte scriverei solo 2 e non 2*sizeof(char). Questo potrebbe essere sbagliato su alcune architetture.
		return false;
	}
	b_read += 2;
	if (!in.read(reinterpret_cast<char*> (&windowymin), 2 * sizeof(char))) {
		return false;
	}
	b_read += 2;
	if (!in.read(reinterpret_cast<char*> (&windowxmax), 2 * sizeof(char))) {
		return false;
	}
	b_read += 2;
	if (!in.read(reinterpret_cast<char*> (&windowymax), 2 * sizeof(char))) {
		return false;
	}
	b_read += 2;

	image_width = windowxmax - windowxmin + 1;
	image_height = windowymax - windowxmin + 1;

	if (!in.read(reinterpret_cast<char*> (&vertDPI), 2 * sizeof(char))) {
		return false;
	}
	b_read += 2;

	if (!in.read(reinterpret_cast<char*> (&horzDPI), 2 * sizeof(char))) {
		return false;
	}
	b_read += 2;

	raw_read(in, palette, 48/* * sizeof(uint8_t)*/); //FB+- stesso discorso qui, non è mai sbagliato in questo caso, ma è decisamente fuorviante
	b_read += 48;
	in >> reserved;
	++b_read;
	in >> color_planes;
	if ((color_planes % bitsPerPlane) != 0) {
		return false;
	}
	++b_read;
	if (!in.read(reinterpret_cast<char*> (&bytesPerPlaneLine), 2 * sizeof(char))) {
		return false;
	}
	b_read += 2;
	if (bytesPerPlaneLine < (image_width / (8 * bitsPerPlane))) {
		return false;
	}

	if (!in.read(reinterpret_cast<char*> (&paletteinfo), sizeof(uint16_t))) {
		return false;
	}
	b_read += 2;

	if (!in.read(reinterpret_cast<char*> (&horscrsize), sizeof(uint16_t))) {
		return false;
	}
	b_read += 2;

	if (!in.read(reinterpret_cast<char*> (&verscrsize), sizeof(uint16_t))) {
		return false;
	}
	b_read += 2;

	raw_read(in, byte, 54 * sizeof(char));
	b_read += 54;

	if (b_read != 128) {
		return false;
	}
	//FINE PARSER DELL'HEADER

	//INIZIO LA DECODIFICA DEI DATI
	//int nplanes = image_height * color_planes * bitsPerPlane;  //FB--

	
	//DUBBIO 1 --> mi sembra corretta la formula commentata ma torna il numero di byte totali da mettere nella matrice
	//int byteperline = bytesPerPlaneLine *  color_planes; 
	// int byteperline = bitsPerPlane * color_planes; //FB--

	// FB: how many bytes are required to hold one complete uncompressedscan line (from text)
	int totalbytes = color_planes * bytesPerPlaneLine; //FB-+

	img.resize(image_height, image_width); 
	// FB Per non fare confusione, andrei per scanlines, che so per certo essere image rows.
	for (int r = 0; r < img.rows(); ++r) {
		// FB Qui facccio la decodifica di una linea, possiamo farla direttamente dentro l'immagine, ma forse 
		// conviene farlo su un vettore separato e poi fare la copia (così ho già pronto il codice per i casi successivi)
		
		std::vector<uint8_t> scanline(totalbytes); //FB+ questo per memorizzare una scanline intera. 
												   //Non è indispensabile usarlo, ma male non fa


		int count = 0;
		uint8_t b;
		uint8_t L = 0;
		int k = 0;

		while (k < totalbytes) {
			b = in.get();
			//++count; //FB--
			if (check_run(b)) {
				// FB-- anche questo non era sbagliato, ma come scritto sotto mi sembra più semplice ed immediato
				//for (int i = 5; i >= 0; --i) {
				//	L = (L << 1) | ((b >> i) & 1);
				//}
				// FB++
				L = b & 0b00111111; // Numero di ripetizioni (usi dei nomi più significativi, altrimenti si perde!!)

				b = in.get(); // FB valore
				//++count; //FB--
				// FB--
				/*for (int j = 0; j < L; ++j) {
					img.data()[k] = b;
					++k;
				}*/
				// FB++
				std::fill_n(scanline.begin() + k, L, b);
				k += L; // FB++ il conteggio fa fato sul numero di byte decodificati, quindi 
				//L = 0; // FB-- questo è inutile se assegno ad L il risultato dell'and bit a bit 
			}
			else {
				scanline[k] = b; //FB++
				// img.data()[k] = b; // FB--
				++k;
			}
		}
		// FB Alla fine di ogni scanline copio i dati dentro l'immagine (questo dovrà cambiare a seconda dei piani 
		// colore a disposizione e della loro dimensione) e quindi nei 3 esercizi. Questa cosa potevo farla 
		// direttamente durante la lettura senza scrivere i dati nella scanline.
		for (int i = 0, c = 0; i < img.cols()/8; ++i) {
			uint8_t byte = scanline[i];
			img(r, c++) = ((byte & 0b10000000) >> 7) * 255;
			img(r, c++) = ((byte & 0b01000000) >> 6) * 255;
			img(r, c++) = ((byte & 0b00100000) >> 5) * 255;
			img(r, c++) = ((byte & 0b00010000) >> 4) * 255;
			img(r, c++) = ((byte & 0b00001000) >> 3) * 255;
			img(r, c++) = ((byte & 0b00000100) >> 2) * 255;
			img(r, c++) = ((byte & 0b00000010) >> 1) * 255;
			img(r, c++) = ((byte & 0b00000001) >> 0) * 255;
		}
		
	}

	return true; 

	//nei vari file di test su olj tutti iniziano a dare problemi dopo circa 30k byte, 
	//mentre il file hose.pcx sbaglia già al byte 39 e non ne comprendo il motivo anche debuggando attentamente
}

