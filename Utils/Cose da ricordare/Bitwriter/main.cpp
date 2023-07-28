#include <iostream>

int main()
{
	int n;
	int v, v2;
	std::cin >> n;
	std::cin >> v;
	std::cin >> v2;
	int res = 1;

	for (int  i = 2; i < n; i++)
	{

		if (v < v2)
		{
			v = v2;
			std::cin >> v2;
		}
		else
		{
			//res = 0;
			std::cout << "la sequenza  non e' crescente\n";
			return 1;
		}
		
	}

	if (res == 1)
	{
		std::cout << "la sequenza e' crescente\n";
	}
	else
	{
		std::cout << "la sequenza  non e' crescente\n";
	}
	
	

	
}