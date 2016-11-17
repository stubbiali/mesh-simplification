/*!	\file	main_intersect.cpp
	\brief	A small executable testing some functionality provided
			by the class intersect<Triangle>. */
			
#include "intersect.hpp"

int main()
{
	using namespace geometry;
	using point2d = geoPoint<2>;
	using point3d = geoPoint<3>;
	
	//
	// Test intersection segment-segment
	//
	
	{
		point2d q1(-1,0);
		point2d r1(1,0);
		point2d q2(1,0);
		point2d r2(10,0);
		
		auto ans = intersect<Triangle>::intSegSeg2d(q1,r1,q2,r2);
		if (ans == IntersectionType::NONE)
			cout << "Segments do not intersect." << endl;
		else if (ans == IntersectionType::VALID)
			cout << "Segments share a vertex." << endl;
		else
			cout << "Segments intersect in a non-conformal way." << endl;
	}
}
