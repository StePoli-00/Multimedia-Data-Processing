#include "pbm.h"


int main(int argc, char** argv)
{
	
	if (argc != 2)
	{
		return 1;
	}

	std::string filename(argv[1]);
	BinaryImage bi;
	bool res=bi.ReadFromPBM(filename);
	return res;
}