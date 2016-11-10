/*! \file	main_bmesh.cpp
	\brief	A small executable to test class bmesh. */
	
#include <iostream>

#include "bmesh.hpp"

using namespace geometry;

int main()
{	
	// Read from file
	string inputfile("../../mesh/bunny.inp");
	bmesh<Triangle> bm(inputfile);
	cout << "Number of nodes   : " << bm.getNumNodes() << endl
		 << "Number of elements: " << bm.getNumElems() << endl;
		 
	// Print to file
	string outputfile("../../mesh/out_bunny.inp");
	bm.print(outputfile);
}
