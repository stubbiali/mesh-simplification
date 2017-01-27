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
	string oFile("/home/stefano/Desktop/mesh-simplification/mesh/cow_1160_033_033_033_greedy_30.inp");
	
	// Desired number of nodes
	UInt numNodesMax(20000);
	
	// Simplificate!
	#ifdef NDEBUG
	high_resolution_clock::time_point start = high_resolution_clock::now();
	#endif
	
	simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>> simplifier(iFile);
	simplifier.simplificate(numNodesMax, false);
	//simplifier.simplificate_greedy(numNodesMax, 30, true, oFile);
	
	#ifdef NDEBUG
	high_resolution_clock::time_point stop = high_resolution_clock::now();
	auto dif = duration_cast<milliseconds>(stop-start).count();
	cout << "Total elapsed time: " << dif << " ms" << endl;
	#endif
}
