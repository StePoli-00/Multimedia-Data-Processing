#include "mat.h"
#include "types.h"
#include "pgm.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames) {

	std::unordered_map<std::string, int> chroma = { {"C420mpeg2",0}, {"C420paldv",1}, {"C411",2}, {"C422",3}, {"C444",4}, {"C444alpha",5}, {"Cmono",6} };
	std::ifstream is(filename, std::ios::binary);
	std::vector<std::string> inter = {"Ip", "It","Ib"};
	if (!is)
	{
		return false;
	}
	std::string line,token, F, A,X,br;
	int H=0, W=0;

	if (!(is >> line))
	{
		return false;
	}

	if (line != "YUV4MPEG2") { return false; }
	std::getline(is, line);
	std::stringstream ss(line);
	while (ss >> token)
	{
	
		if (token[0] == 'H')
		{
			H =atoi(&token[1]);

		}
		else if (token[0] == 'W')
		{
			W = atoi(&token[1]);
		}
		else if (token[0] == 'C') {

			if (token != "C420jpeg") { return false; }
			/*auto it = chroma.find(token);
			if(it==chroma.end())
			{
				return false;
			}*/
		}

		else if (token[0] == 'I') {

			auto it=std::find(inter.begin(), inter.end(), token);
			if (it == inter.end())
			{
				return false;
			}
		}

		else if (token[0] == 'F') {
			F = token.substr(1, token.size());
			
		}
		else if (token[0] == 'A') {
			A = token.substr(1, token.size());

		}
		else if (token[0] == 'X') {
			
			X = token.substr(1, token.size());
		}
		else { return false; }
		

	}



	while (1) {
		
		std::getline(is, line);
		if (line != "FRAME") 
		{
			break;
		}
		char ch;

		while (1)
		{
		

			ch = is.peek();
			if (ch == 'I') {
				std::getline(is, line);
				ch = is.get();
			}
			else if (ch == 'X')
			{
				std::getline(is, line);
				ch = is.get();
			}
			else
			{
				break;
			}
			
		}

		mat<uint8_t> Y, cb, cr;
		Y.resize(H,W);
		cb.resize(H / 2, W / 2);
		cr.resize(H / 2, W / 2);
		is.read(Y.rawdata(), Y.cols() * Y.rows());
		is.read(cb.rawdata(), cb.cols() * cb.rows() );
		is.read(cr.rawdata(), cr.cols() * cr.rows());
		frames.push_back(Y);
	




	}

	return true;


}


