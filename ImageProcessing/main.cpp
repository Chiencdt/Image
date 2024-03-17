#include <iostream>
#include "readbmpimage.h"

int main()
{
	ReadBmpImage r;
	r.GetImage("D:\\Image\\2021_05_03\\UP0305\\", "1");
	std::system("pause");
	return 0;
}