#include <vector>
#include <cstdint>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
int main(void)
{
	
	const int nseconds = 10;
	const int sample_freq = 44100;
	const int nsamples = nseconds * sample_freq;
	const double freq = 440.0;
	const double amplitude = INT16_MAX;
	std::vector<int16_t> samples(nsamples);
	
	for (size_t i = 0; i < nsamples; i++)
	{
		//arrotondiamo la virgola che arrotonda l'intero più vicino e mette in un long
		/*samples[i] =static_cast<int16_t> (round(
		amplitude*sin(2 *freq*M_PI * i /sample_freq)));*/ //seno ad 1Hz
		double dsample = amplitude * sin(2 * freq * M_PI * i / sample_freq);
		double rsample = round(dsample);
		int16_t sample = static_cast<int16_t>(rsample);
		int quant = INT16_MAX / 64;
		int16_t qsample = sample / quant;
		int16_t rec_sample = qsample * quant;
		samples[i] = rec_sample;
		
	}
	std::ofstream os("output.raw", std::ios::binary);
	//scrivo i numeri così come sono in binario su un file
	//samples.data ritorna un puntatore a int16_t 
	os.write(reinterpret_cast<char*>(samples.data()), nsamples*sizeof(int16_t));//2 byte

		
	/*
	Supponiamo di caricare 16 bit e di visualizzare con Hxd 
	23 F1 77 42 sono 2 campioni audio, dobbiamo specificare se i dati
	sono in little/big endian
	*/

}