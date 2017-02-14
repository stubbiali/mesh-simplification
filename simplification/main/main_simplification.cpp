/*!	\file	main_simplification.cpp
	\brief	Executable running the iterative mesh simplification process. */
	
#include <chrono>
	
#include "simplification.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	// Path to input and output file
	string iFile("/home/stefano/Desktop/mesh-simplification/mesh/brain.inp");
	string oFile("/home/stefano/Desktop/mesh-simplification/results/brain_30000_033_033_033.inp");
	
	// Desired number of nodes
	UInt numNodesMax(30000);
	
	// Simplificate!
	#ifdef NDEBUG
	high_resolution_clock::time_point start = high_resolution_clock::now();
	#endif
	
	simplification<Triangle, MeshType::DATA, DataGeo> simplifier(iFile);
	simplifier.simplificate(numNodesMax, true, oFile);
	//simplifier.simplificate_greedy(numNodesMax, 30, true, oFile);
	
	#ifdef NDEBUG
	high_resolution_clock::time_point stop = high_resolution_clock::now();
	auto dif = duration_cast<milliseconds>(stop-start).count();
	cout << "Total elapsed time: " << dif << " ms" << endl;
	#endif
}