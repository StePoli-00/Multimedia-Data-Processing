#define _USE_MATH_DEFINES
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <math.h>



int read_sample(std::string filename, std::vector<int16_t>& samples) {

	std::ifstream is(filename, std::ios::binary);
	if (!is)
	{
		return 1;
	}
	int16_t num;
	while (is.read(reinterpret_cast<char *>(&num), 2))
	{
		samples.push_back(num);
	}

	return 0;
}

double entropy(std::vector<int16_t> &samples)
{
	std::unordered_map<int16_t, int> frequencies;
	double total = 0;
	for (auto& s : samples)
	{
		frequencies[s] += 1;
		total += 1;
	}
	double H = 0.0;
	for (auto& f : frequencies)
	{
		double prob=f.second / total;
		H += prob * log2(prob);
	}
	return -H;
}
std::vector<int16_t > quantize(int Q, std::vector<int16_t>& samples)
{
	std::vector<int16_t> sample_qt;
	for (auto& s : samples) {
		double a = s / Q;
		sample_qt.push_back(ceil(a));
	}
	return sample_qt;
}
std::vector<int16_t > dequantize(int Q, std::vector<int16_t>& samples_qt)
{
	std::vector<int16_t> sample;
	for (auto& s : samples_qt) {
		sample.push_back(s * Q);
	}
	return sample;
}
int save_audio(std::string fileout, std::vector<int16_t> & samples)
{
	std::ofstream os(fileout, std::ios::binary);
	if (!os)
	{
		return 1;
	}
	for (auto& s : samples)
	{
		os.write(reinterpret_cast<char *> (&s),2);
	}
	return 0;
}
std::vector<double>  calculate_wn(int const N) {

	std::vector<double> wn;
		for (int n = 0; n < (2 * N - 1); ++n)
		{
			double arg =M_PI/(2*N)*(n+0.5);
			wn.push_back(sin(arg));
		}

	return wn;

}

std::vector<std::vector<double>> calculate_cosines(int const N) {

	
	std::vector<std::vector<double>> cosines(N,std::vector<double>((2 * N),0));

	for (int k = 0; k < N - 1; ++k)
	{
		double co = 0;
		for (int n = 0; n < (2 * N - 1); ++n)
		{
			double arg = M_PI / N * (n + 0.5 + N / 2) * (k + 0.5);
			co=cos(arg);
			cosines[k][n]=co;
		}
		
	}
	return cosines;
}

std::vector<int16_t> MDCT(std::vector<int16_t>& samples ,int N)
{

	
	std::vector<std::vector<double>> cosines=calculate_cosines(N);
	std::vector<double> wn = calculate_wn(N);
	std::vector<int16_t> Xk;
	//std::vector<int16_t> coeff;
	std::vector<int16_t> window(2*N);
	samples.insert(samples.begin(), 1024, 0);
	samples.insert(samples.end(), 1024, 0);
	

	
	//std::copy(samples.begin() + , samples.begin() + (N * i + 2 * N), std::back_inserter(window));
	for (int i = 0; i <= samples.size() - (2 * N); i += N)
	{
		std::copy(samples.begin() + i, samples.begin() + i + (2 * N), window.begin());
		double num = 0;
		for (int k = 0; k < N - 1; ++k)
		{	
			for (int n = 0; n < (2 * N - 1); ++n)
			{
				num += window[n] * wn[n] * cosines[k][n];
			}
		
		}
		Xk.push_back(round(num));
		//window.clear();
	}
	int a;
	return Xk;
	
}

std::vector<int16_t> IMDCT(std::vector<int16_t>& samples, int N)
{


	std::vector<std::vector<double>> cosines=calculate_cosines(N);
	std::vector<double> wn = calculate_wn(N);
	std::vector<int16_t> yk;
	std::vector<int16_t> yk_r;
	int window_size = N;
	std::vector<int16_t> window(N-1);
	

	for (int i = 0; i <= samples.size() - window_size; i += window_size)
	{
		std::copy(samples.begin() + i, samples.begin() + i + (window_size), window.begin());
		double num = 0;
		for (int n = 0; n < (2 * N ); ++n)
		{

			for (int k = 0; k < N ; ++k)
			{
				
				num += window[k] *cosines[k][n];
			}

			num += (2 / N) * wn[n];
			yk.push_back(num);
		}
	}

	yk.erase(yk.begin(), yk.begin() + 1024);
	yk.erase(yk.end() - 1024, yk.end());
	int n_window = round(yk.size() / N);
	//sovrapposizione 
	
		for (int i = 0; i <= samples.size() - window_size; i += window_size / 2)
		{
			//int j = window_size / 2;
			for (int j = window_size / 2; j > 0; --j) {
				yk[i] += yk[j];
			}
			

		}
	
	
	return yk;
	

}

std::vector<int16_t> calcolate_difference(std::vector<int16_t>& original, std::vector<int16_t>& reconstructed)
{
	std::vector<int16_t> diff;
	for (size_t i = 0; i < original.size(); i++)
	{
		diff.push_back(original[i] - reconstructed[i]);
	}

	return diff;
}


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		return 1;
	}
	std::vector<int16_t> samples;
	std::string filename(argv[1]);
	int res=read_sample(filename, samples);
	double H = entropy(samples);
	std::cout << "Entropia del segnale non quantizzato: " <<H<<"\n";
	/*std::vector<int16_t> samples_qt = quantize(2600, samples);
	double H_qt = entropy(samples_qt);
	std::cout << "Entropia del segnale quantizzato: " << H_qt << "\n";
	std::vector<int16_t> samples_rec = dequantize(2600, samples_qt);

	save_audio("output_qt.raw", samples_rec);

	//calcolo le differenze
	std::vector<int16_t> difference = calcolate_difference(samples, samples_rec);
	save_audio("error_qt.raw", difference);
	*/
	std::vector<int16_t> Xk=MDCT(samples,1024);
	std::vector<int16_t> Xk_q = quantize(10000, Xk);
	double H_qt;
	H_qt = entropy(Xk_q);
	std::cout << "Entropia dei sample trasformati e quantizzati: " << H_qt << "\n";
	std::vector<int16_t> Xk_rec = dequantize(10000, Xk_q);
	std::vector<int16_t> yk= IMDCT(Xk_rec,1024);
	return 0;

	
}