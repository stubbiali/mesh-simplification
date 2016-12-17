/*!	\file	main_simplification.cpp
	\brief	Executable running the iterative mesh simplification process. */
	
#include <chrono>
	
#include "simplification.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	// Path to input and output file
	string iFile("../../mesh/bunny.inp");
	string oFile("../../mesh/bunny_05.inp");
	
	// Desired number of nodes
	UInt numNodesMax(1740);
	
	// Simplificate!
	#ifdef NDEBUG
	high_resolution_clock::time_point start = high_resolution_clock::now();
	#endif
	
	simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>> 
		simplifier(iFile);
	simplifier.simplificate(numNodesMax, oFile);
	
	#ifdef NDEBUG
	high_resolution_clock::time_point stop = high_resolution_clock::now();
	auto dif = duration_cast<milliseconds>(stop-start).count();
	cout << "Total elapsed time: " << dif << " ms" << endl;
	#endif
}
