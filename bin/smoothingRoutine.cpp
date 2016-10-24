#include <iostream>

#include "meshSimplification.h"


using namespace geometry;
using namespace std;

int main(int argc, char * argv[])
{	
    // variabili in uso 
    mesh2d<Triangle> surf;
    createFile  file;
    downloadMesh down;
    
    // parameters 
    UInt iter = 20;
    Real lambda = 0.33;
    int weight = 1;
    string outputName("output");
    
    // Check of the input
    if(argc!=3 && argc!=4 && argc!=6 && argc!=7)
    {
        cout << "Wrong input for this function" << endl;
        cout << "1) file extension 1=.inp, 2=.vtk\n";
        cout << "2) name of the file\n";
        cout << "3) number of iteration (default 20)\n";
        cout << "4) lambda parameter (default 0.33) \n";
        cout << "5) kind of weights 1=uniform, 2=fuijwara, 3=Desbrun (default uniform)\n";
        cout << "6) name of the output (default output)\n";
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
    else if(argc==6)
    {
        iter = atoi(argv[3]);
        lambda = atof(argv[4]);
        weight = atoi(argv[5]);
    }
    if(argc==7)
    {
        iter = atoi(argv[3]);
        lambda = atof(argv[4]);
        weight = atoi(argv[5]);
        outputName = argv[6];
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
        
    // print the cost function
    ostringstream initailName;
    initailName << outputName << "_initial.inp";
    cout << "writing file " << initailName.str() << endl;
    file.fileForParaview(initailName.str(), &surf);
    
    //-------------------------------------------------------------------------
    //                         apply the smoothing 
    //-------------------------------------------------------------------------

    // run the smoothing 
    taubinSmoothing smooth;
    smooth.setMeshPointer(&surf);
    smooth.setClassicalSmoothingParameters(lambda, taubinSmoothingWeights);
    smooth.runTheSmoothing(iter);
    
    // plot the volumes 
    ostringstream volumeName;
    volumeName << outputName << "_volumes.m";
    smooth.writeMatlabFileWithVolumes(volumeName.str());
    
    // print the cost function
    ostringstream denoiseName;
    denoiseName << outputName << "_final.inp";
    cout << "writing file " << denoiseName.str() << endl;
    file.fileForParaview(denoiseName.str(), &surf);
    
}
