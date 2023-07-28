#include "base64.h"
#include <iostream>
int main()
{
	std::string case1 = "AP8A";
	std::string case2 = "MQ==";
	//001100 010000->00110001 00000000
	std::string case3 = "MTI=";
	//001100 
	std::string case4 = "UHJldHR5IGxvbmcgdGV4dCB3aGljaCByZXF1aXJlcyBtb3JlIHRoYW4gNzYgY2hhcmFjdGVycyB0byBlbmNvZGUgaXQgY29tcGxldGVseS4";

	std::cout<<base64_decode(case1)<<"\n";
	std::cout << base64_decode(case2) << "\n";
	std::cout << base64_decode(case3) << "\n";
	std::cout << base64_decode(case4) << "\n";

}