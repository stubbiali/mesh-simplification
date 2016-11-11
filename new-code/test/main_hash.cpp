#include <iostream>

int main()
{
	unsigned int x = 2;
	unsigned int xx = x << 19;
	unsigned int y = 0;
	unsigned int yy = y << 7;
	
	std::cout << (xx | yy) << std::endl;
}
