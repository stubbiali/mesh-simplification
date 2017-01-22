/*!	\file	main_simplification.cpp
	\brief	Executable running the iterative mesh simplification process. */
	
#include <chrono>
	
#include "simplification.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	// Path to input and output file
	string iFile("/home/stefano/Desktop/mesh-simplification/mesh/bunny.inp");
	string oFile("/home/stefano/Desktop/mesh-simplification/results/bunny_1740_1_0_0.inp");
	
	// Desired number of nodes
	UInt numNodesMax(1740);
	
	// Simplificate!
	#ifdef NDEBUG
	high_resolution_clock::time_point start = high_resolution_clock::now();
	#endif
	
	simplification<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>> simplifier(iFile);
	simplifier.simplificate(numNodesMax, true, oFile);
	//simplifier.simplificate_greedy(numNodesMax, 5, true, oFile);
	
	#ifdef NDEBUG
	high_resolution_clock::time_point stop = high_resolution_clock::now();
	auto dif = duration_cast<milliseconds>(stop-start).count();
	cout << "Total elapsed time: " << dif << " ms" << endl;
	#endif
}
