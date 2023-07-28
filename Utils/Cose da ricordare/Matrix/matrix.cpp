#include "mat.h"
#include <iostream>
#include <array>

struct RGB {
	std::array<int,3> vec;
};

//int main(void)
//{
//
//	//mat<RGB> mat2(3, 3);
//	mat<int> mat(3, 3);
//
//	for (int r = 0; r < mat.rows(); r++)
//	{
//		for (int c = 0; c < mat.cols(); c++)
//		{
//			mat(r, c) = 1;
//		}
//	}
//
//	
//	for (int r = 0; r < mat.rows(); r++)
//	{
//		for (int c = 0; c < mat.cols(); c++)
//		{
//			if (c == mat.cols() - 1)
//			{
//				std::cout << mat(r, c)<<"\n";
//			}
//			else
//			{
//				std::cout << mat(r, c) << ", ";
//			}
//			
//		}
//		
//	}
//
//}
//
//
