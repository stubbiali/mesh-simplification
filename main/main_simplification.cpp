/*!	\file	main_simplification.cpp
	\brief	Executable running the iterative mesh simplification process. */
	
#include <chrono>
	
#include "simplification.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	// Path to input and output file
	string iFile("/home/stefano/Desktop/mesh-simplification/mesh/pawn.inp");
	string oFile("/home/stefano/Desktop/mesh-simplification/mesh/pawn_1400_033_033_033.inp");
	
	// Desired number of nodes
	UInt numNodesMax(1400);
	
	// Simplificate!
	#ifdef NDEBUG
	high_resolution_clock::time_point start = high_resolution_clock::now();
	#endif
	
	simplification<Triangle, MeshType::DATA, DataGeo> simplifier(iFile);
	simplifier.simplificate(numNodesMax, true);
	//simplifier.simplificate_greedy(numNodesMax, 5, true);
	
	#ifdef NDEBUG
	high_resolution_clock::time_point stop = high_resolution_clock::now();
	auto dif = duration_cast<milliseconds>(stop-start).count();
	cout << "Total elapsed time: " << dif << " ms" << endl;
	#endif
}
