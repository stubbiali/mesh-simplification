/*!	\file	main_intersection.cpp
	\brief	A small executable testing some functionality provided
			by the class intersection<Triangle>. */
			
#include <chrono>
#include "intersection.hpp"

using namespace geometry;

int main()
{
	using point2d = geoPoint<2>;
	using point3d = geoPoint<3>;
	/*
	//
	// Test segment-segment intersection (2D)
	//
	
	{
		// First segment
		point2d q1(7,5);
		point2d r1(1,2);
		
		// Second segment
		point2d q2(1,2);
		point2d r2(1.5,3);
		
		// Output
		utility::printIntersectionType(gutility::intSegSeg2d(q1,r1,q2,r2), "Segments");
		cout << endl;
	}
	
	//
	// Test point in triangle (2D)
	//
	
	{
		// The triangle
		point2d a(0,0);
		point2d b(2,0);
		point2d c(1,1);
		
		// The points to test
		point2d p1(1,0.5);
		point2d p2(0.5,0.499999999);
		point2d p3(0.5,0.500000001);
		point2d p4(0.5,0.5);
		point2d p5(c);
		
		// Output
		utility::printPoint2Tri(intersection<Triangle>::inTri2d(p1,a,b,c));
		utility::printPoint2Tri(intersection<Triangle>::inTri2d(p2,a,b,c));
		utility::printPoint2Tri(intersection<Triangle>::inTri2d(p3,a,b,c));
		utility::printPoint2Tri(intersection<Triangle>::inTri2d(p4,a,b,c));
		utility::printPoint2Tri(intersection<Triangle>::inTri2d(p5,a,b,c));		
		cout << endl;
	}
	
	//
	// Test segment-plane intersection
	//
	
	{
		// The triangle defining the plane
		point3d A(1,0,0);
		point3d B(0,1,0);
		point3d C(0,0,1);
		
		// Compute N and D
		auto N = (B-A)^(C-B);
		auto D = N*A;
		
		// The segment
		point3d Q(0,1,-1);
		point3d R(1,0,-1);
		
		// Output
		auto ans = intersection<Triangle>::intSegPlane(Q,R,N,D);
		utility::printLine2Plane(get<0>(ans));
		utility::printPoint2Seg(get<1>(ans));
		cout << "t = " << get<2>(ans) << endl;
		cout << endl;
	}
	
	//
	// Test triangle-triangle intersection (non-static API)
	//
	
	{
		// Read mesh
		string inputfile("../../mesh/bunny.inp");
		intersection<Triangle> ntr(inputfile);
		
		// Id's of triangles to consider
		UInt id1 = 8034;
		UInt id2 = 8194;
		
		// Test intersection
		#ifndef NDEBUG
		auto ans = ntr.intersect(id1,id2);
		#else
		using namespace std::chrono;
		high_resolution_clock::time_point start, stop; 
		start = high_resolution_clock::now();
		for (UInt i = 0; i < 1e6; i++)
			auto ans = ntr.intersect(id1,id2);
		#endif
		
		#ifndef NDEBUG
		if (ans)
			cout << "Triangles " << id1 << " and " << id2 << " intersect." << endl;
		else
			cout << "Triangles " << id1 << " and " << id2 << " do not intersect." << endl;
		#else
		stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop-start).count();
		cout << "Elapsed time: " << duration << " ms" << endl;
		#endif
		cout << endl;
	}
	*/
	//
	// Test triangle-triangle intersection (static API)
	//
	// Just work in 2d for the sake of simplicity
	
	{
		// First triangle
		point3d A(1,0,0);
		point3d B(4,0,0);
		point3d C(2.5,3,0);
		
		// Second triangle
		point3d D(0,0,0);
		point3d E(2,0,0);
		point3d F(0,2,0);
		
		// Output
		auto ans = intersection<Triangle>::intersect(A,B,C,D,E,F);
		if (ans)
			cout << "Triangles intersect." << endl;
		else
			cout << "Triangles do not intersect." << endl;
	}
}
