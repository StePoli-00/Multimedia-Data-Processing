#include "bit.h"

int count_bit(int bit) {

	int cnt = 0;

	while (bit > 0) {
		//bit /= 2;
		bit = bit >> 1;
		++cnt;
	}
	return cnt;
}


void map(std::vector<int>& v) {

	for (size_t i = 0; i < v.size(); ++i)
	{

		if (v[i] < 0)
		{
			v[i] = 2 * v[i] * -1;
		}
		else
		{
			v[i] = 2 * v[i] + 1;
		}
	}
	return;
}
void unmap(std::vector<int>& v) {

	for (size_t i = 0; i < v.size(); i++)
	{
		if (v[i] % 2 == 0) {
			v[i] = (v[i] / 2) * -1;
		}
		else
		{
			v[i] = (v[i] - 1) / 2;
		}

	}
	return;
}


