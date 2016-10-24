#include <iostream>

#include "meshSimplification.h"


using namespace geometry;
using namespace std;

// method to make some noise on the data 
void noiseTheData(mesh2d<Triangle> & surf);

int main(int argc, char * argv[])
{	
    // variabili in uso 
    mesh2d<Triangle> surf;
    createFile  file;
    downloadMesh down;
    
    // parameters 
    UInt iter = 20;
    Real lambda = 0.33;
    Real mu = -0.34;
    int weight = 1;
    string outputName("output");
    
    // Check of the input
    if(argc!=3 && argc!=4 && argc!=7 && argc!=8)
    {
        cout << "Wrong input for this function" << endl;
        cout << "1) file extension 1=.inp, 2=.vtk\n";
        cout << "2) name of the file\n";
        cout << "3) number of iteration (default 20)\n";
        cout << "4) lambda parameter (default 0.33) \n";
        cout << "5) mu parameter (default -0.34) \n";
        cout << "6) kind of weights 1=uniform, 2=fuijwara, 3=Desbrun (default uniform)\n";
        cout << "7) name of the output (default output)\n";
        exit(1);
    }
    
    //-------------------------------------------------------------------------
    //                          save the parameters
    //-------------------------------------------------------------------------
    UInt extension = atoi(argv[1]);
    string filename(argv[2]);
    
    if(argc==4)
    {
        iter = atoi(argv[3]);
    }
    else if(argc==7)
    {
        iter = atoi(argv[3]);
        lambda = atof(argv[4]);
        mu = atof(argv[5]);
        weight = atoi(argv[6]);
    }
    if(argc==8)
    {
        iter = atoi(argv[3]);
        lambda = atof(argv[4]);
        mu = atof(argv[5]);
        weight = atoi(argv[6]);
        outputName = argv[7];
    }
    
    kindOfWeights taubinSmoothingWeights;
    if(weight==1)
    {
        taubinSmoothingWeights = UNIFORMWEIGHTS;
    }
    else if(weight==2)
    {
        taubinSmoothingWeights = FUJIWARAWEIGHTS;
    }
    else if(weight==3)
    {
        taubinSmoothingWeights = DESBRUNWEIGHTS;
    }
    else
    {
        cout << "!! !! Unknown weight " << weight << std::endl;
        exit(1);
    }
    
    //-------------------------------------------------------------------------
    //                           get the file 
    //-------------------------------------------------------------------------
    if(extension==1)
    {
        down.fileFromParaview(filename, &surf);
    }
    else if(extension==2)
    {
        down.fileFromVTK(filename, &surf);
    }
    else 
    {
        cout << "!! !! Unknown file extension " << extension << std::endl;
        exit(1);
    }
    
    //-------------------------------------------------------------------------
    //                           make some noise 
    //-------------------------------------------------------------------------
    noiseTheData(surf);
    
    // print the cost function
    ostringstream noiseName;
    noiseName << outputName << "_noisy.inp";
    cout << "writing file " << noiseName.str() << endl;
    file.fileForParaview(noiseName.str(), &surf);
    
    //-------------------------------------------------------------------------
    //                         apply the smoothing 
    //-------------------------------------------------------------------------

    // run the smoothing 
    taubinSmoothing smooth(&surf, mu, lambda, taubinSmoothingWeights);
    smooth.runTheSmoothing(iter);
    
    // print the cost function
    ostringstream denoiseName;
    denoiseName << outputName << "_denoise.inp";
    cout << "writing file " << denoiseName.str() << endl;
    file.fileForParaview(denoiseName.str(), &surf);
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

