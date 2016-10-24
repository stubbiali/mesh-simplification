#include <iostream>
#include "meshSimplification.h"

using namespace geometry;
using namespace std;

// method to make some noise on the data 
void noiseTheData(mesh2d<Triangle> & surf);

int main()
{	
    // variabili in uso 
    mesh2d<Triangle> surf;
    createFile  file;
    downloadMesh down;

    // dowload di una mesh di superficie
//     down.fileFromParaview("../mesh/cow.inp", &surf);
    down.fileFromParaview("../mesh/bunny.inp", &surf);
    
    // make some noise 
    noiseTheData(surf);
    
    // print the cost function
    file.fileForParaview("bunnyNoise.inp", &surf);

//     noiseCostFunction cost;
    garlandCostFunction cost;
    std::vector<UInt> materialId(surf.getNumNodes(), 10);
    simplification2dCostFunctionBased simp(&cost, &surf, materialId);

    // print the cost function
    simp.printCostFunction("bunnyNoiseCost.inp");
}

//
// Implementation of functions
//
void noiseTheData(mesh2d<Triangle> & surf)
{
    tricky2d<Triangle> trick(&surf);
    
    for(UInt i=0; i<surf.getNumNodes(); ++i)
    {
        if(i%100==1)
        {
            // get all the points around 
            std::vector<UInt> around;
            trick.getNodeAround(i, 1, &around);
            
            // get the distance for the noise
            Real dist = 0.;
            for(UInt j=0; j<around.size(); ++j)
            {
                REAL tmpDist = (surf.getNode(i)-surf.getNode(around[j])).norm2();
                if(dist<tmpDist)
                    dist = tmpDist;
            }
            
            // get the new position
            point normal = trick.getPointNormal(i);
            point newPosition = surf.getNode(i) + normal * dist;
            
            // change the position 
            surf.getNodePointer(i)->setX(newPosition.getX());
            surf.getNodePointer(i)->setY(newPosition.getY());
            surf.getNodePointer(i)->setZ(newPosition.getZ());
        }
    }
    
}

