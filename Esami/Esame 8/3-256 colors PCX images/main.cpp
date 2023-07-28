#include "pcx.h"
#include "ppm.h"


int main(int argc, char** argv)
{

	mat<vec3b> img;
	bool res = load_pcx(argv[1], img);
	if (!res)
	{
		return 1;
	}

	save_ppm(argv[2], img);
	


}