#include <iostream>

#include "meshSimplification.h"

int main()
{	
	using namespace geometry;
	using namespace std;
	
	//
	// Read the mesh
	//
	
    // Variables 
    mesh2d<Triangle> surf;
    downloadMesh down;
    
    // Path to the file
    string filename("../mesh/pawn.inp");
    
    // Reading
    down.fileFromParaview(filename, &surf);
    
    //
    // Simplificate
    //
    
    UInt numNodesMax(2500);
    simplification2d s(&surf);
    s.simplificate(numNodesMax);
}
