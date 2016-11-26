/*!	\file	main_bconnect.cpp
	\brief	A small executable to test class bconnect. */
	
#include <iostream>
#include <chrono>

#include "mesh.hpp"
#include "bconnect.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	high_resolution_clock::time_point start = high_resolution_clock::now();
	
	// Import mesh
	string inputfile("../../mesh/bunny.inp");
	//auto grid = make_shared<mesh<Triangle>>(inputfile);
	
	// Create connections
	//bconnect<Triangle, MeshType::GEO> bc(grid);
	bconnect<Triangle, MeshType::GEO> bc(inputfile);
	
	// Print mesh to file
	string outputfile("../../mesh/out_bunny.inp");
	//grid->print(outputfile);
	bc.getMesh().print(outputfile);
	
	// Elapsed time
	high_resolution_clock::time_point end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end-start).count();
    cout << "Elapsed time: " << duration << " ms" << endl;
}
