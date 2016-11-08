#include <iostream>

#include "point.hpp"

int main()
{
	using namespace std;
	using namespace geometry;
	
	point p1(2,3,1);
	point p2(3,76,-9);
	
	p2[1] = 5;
	
	auto p = p1 - p2;
}
