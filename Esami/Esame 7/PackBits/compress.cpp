#include "mat.h"
#include "ppm.h"
#include <sstream>
void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded) {

	/*std::vector<uint8_t> img;
	for (int  r = 0; r < img.rows(); r++)
	{
		for (int  c = 0; c < img.cols(); c++)
		{

			img.push_back(img(r, c));
		}
	}*/
	int cnt = 1;
	bool end = false;
	uint8_t tmp = img.data()[0];
	std::vector<uint8_t> copy_vec;
	for (int i = 1; i < img.size(); ++i)
	{
		if (img.data()[i] != tmp) //copy
		{
			copy_vec.push_back(tmp);
			tmp = img.data()[i];
			/*serve per gestire il caso in cui siamo all'ultimo elemento
		 che va messo come una copy, altrimenti  tmp andrebbe perso*/
			if (i + 1 == img.size())
			{
				copy_vec.push_back(tmp);
			}
		}
		else {
			//scrivo copy_vec
			if (copy_vec.size() != 0)
			{
				uint8_t L = copy_vec.size() - 1;
				encoded.push_back(L);
				for (auto& x : copy_vec) {

					encoded.push_back(x);
				}
				copy_vec.clear();
			}


			while (tmp == img.data()[i])
			{
				++cnt;
				++i;
				//serve per gestire una run alla fine
				if (i == img.size())
				{
					end = true;
					break;
				}
			}

			if (end)
			{
				break;
			}

			encoded.push_back((257 - cnt));
			encoded.push_back(tmp);
			cnt = 1;
			tmp = img.data()[i];
			/*serve per gestire il caso in cui siamo all'ultimo elemento
			che va messo come una copy, altrimenti  tmp andrebbe perso*/
			if (i + 1 == img.size())
			{
				copy_vec.push_back(tmp);
			}


		}
	}

	if (end)
	{
		encoded.push_back((257 - cnt));
		encoded.push_back(tmp);
	}
	if (copy_vec.size() != 0)
	{
		uint8_t L = copy_vec.size() - 1;
		encoded.push_back(L);
		for (auto& x : copy_vec) {

			encoded.push_back(x);
		}
		copy_vec.clear();
	}

	encoded.push_back(128);
	return;

}
