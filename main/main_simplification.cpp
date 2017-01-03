/*!	\file	main_simplification.cpp
	\brief	Executable running the iterative mesh simplification process. */
	
#include <chrono>
	
#include "simplification.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	// Path to input and output file
	string iFile("/home/stefano/Desktop/mesh-simplification/new-code/mesh/pawn.inp");
	string oFile("/home/stefano/Desktop/mesh-simplification/new-code/mesh/bunny_5000_07_015_015.inp");
	
	// Desired number of nodes
	UInt numNodesMax(1000);
	
	// Simplificate!
	#ifdef NDEBUG
	high_resolution_clock::time_point start = high_resolution_clock::now();
	#endif
	
	simplification<Triangle, MeshType::DATA, DataGeo> 
		simplifier(iFile, 0.7, 0.15, 0.15);
	simplifier.simplificate(numNodesMax, true);
	
	#ifdef NDEBUG
	high_resolution_clock::time_point stop = high_resolution_clock::now();
	auto dif = duration_cast<milliseconds>(stop-start).count();
	cout << "Total elapsed time: " << dif << " ms" << endl;
	#endif
}
