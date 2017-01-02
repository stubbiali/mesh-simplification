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
    
    UInt numNodesMax(1000);
    simplification2d<Triangle> s(&surf);
    s.simplificate(numNodesMax);
    
    createFile up;
    up.fileForParaview("../mesh/out_pawn.inp", &surf);
}
