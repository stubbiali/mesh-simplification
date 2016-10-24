#include <iostream>
#include "meshSimplification.h"

using namespace geometry;
using namespace std;

int main()
{	
	// variabili in uso 
	mesh2d<Triangle>	       surf;
	simplification2d<Triangle>     simp;	
	createFile                     file;
	downloadMesh		       down;

	// dowload di una mesh di superficie
//	down.fileFromParaview("../mesh/pawn.inp", &surf);
	down.fileFromParaview("../mesh/cow.inp", &surf);
//	down.fileFromParaview("../mesh/bunny.inp", &surf);

	// setto simp
	simp.setMeshPointer(&surf);

	// faccio il ciclo 
	simp.simplificateGreedy(1000);

	file.fileForParaview("mesh2.inp", &surf);
}
