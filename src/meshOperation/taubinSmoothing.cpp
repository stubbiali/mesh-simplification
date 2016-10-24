#include "taubinSmoothing.h"

using namespace std;
using namespace geometry;

//
// Costruttore 
//
taubinSmoothing::taubinSmoothing() : doctor2d<Triangle>(), mu(-0.331), lambda(-0.330), weightsToUse(UNIFORMWEIGHTS), smoothToUse(TAUBIN)
{
}

taubinSmoothing::taubinSmoothing(mesh2d<Triangle> * _meshPointer, Real _mu, Real _lambda, kindOfWeights _weightsToUse) 
  : doctor2d<Triangle>(_meshPointer), mu(_mu), lambda(_lambda), weightsToUse(_weightsToUse), smoothToUse(TAUBIN)
{
    checkParameters();
}

//
// Basic methods to run the routine 
//
void taubinSmoothing::runTheSmoothing(UInt iter)
{
    writeSmoothInfo(iter);
    
    vector<vector<UInt> > pointToPointConnection;
    computePointToPointConnection(pointToPointConnection);

    volumeSequence.resize(iter+1);
    volumeSequence[0] = computeVolume();
    
    vector<point> newPositions(meshPointer->getNumNodes());
    for(UInt i=0; i<iter; ++i)
    {
        for(UInt nodeId = 0; nodeId<meshPointer->getNumNodes(); ++nodeId)
        {
            // get the point to move 
            newPositions[nodeId] = moveOnePoint(nodeId, pointToPointConnection[nodeId]);
      
            cout << "moving node " << nodeId << " over " << meshPointer->getNumNodes() << " at iteration " << i;
            cout << "                       \r";
        }
        
        moveAllThePoints(newPositions);
        volumeSequence[i+1] = computeVolume();
    }
    cout << "Smoothing done!!                                            \n";
}

//
//  Internal method to exploit for the smoothing 
//
point taubinSmoothing::moveOnePoint(UInt nodeId, std::vector<UInt> & pointToPointConnection)
{
    // get the point to move 
    point pointToMove = meshPointer->getNode(nodeId);
    pointToMove.setId(nodeId); 
    
    // do the first move 
    point firstMove  = moveTheIthPoint(pointToMove, pointToPointConnection, lambda);
    firstMove.setId(nodeId);
    
    point resultingPoint;
    if(smoothToUse==TAUBIN)
        resultingPoint = moveTheIthPoint(firstMove, pointToPointConnection, mu);
    else
        resultingPoint = firstMove;
    
    return(resultingPoint);
}

void taubinSmoothing::moveAllThePoints(vector<point> & newPositions)
{
    for(UInt nodeId = 0; nodeId<meshPointer->getNumNodes(); ++nodeId)
        changeNode(nodeId, newPositions[nodeId]);
}

void taubinSmoothing::computePointToPointConnection(vector<vector<UInt> > & pointToPointConnection)
{
    pointToPointConnection.clear();
    pointToPointConnection.resize(meshPointer->getNumNodes());
    
    for(UInt i=0; i<pointToPointConnection.size(); ++i)
        createStellata(i, &pointToPointConnection[i]);
}
                
point taubinSmoothing::moveTheIthPoint(point & pointToMove, const vector<UInt> & pointToPointConnection, Real smoothPara)
{
    point result(0.0, 0.0, 0.0);
    switch(weightsToUse)
    {
        case(UNIFORMWEIGHTS):
                result = moveTheIthPointWithUniformWeights(pointToMove, pointToPointConnection, smoothPara);
                break;
        case(FUJIWARAWEIGHTS):
                result = moveTheIthPointWithFuijwaraWeights(pointToMove, pointToPointConnection, smoothPara);
                break;
        case(DESBRUNWEIGHTS):
                result = moveTheIthPointWithDesbrunWeights(pointToMove, pointToPointConnection, smoothPara);
                break;
    }
    return(result);
}

point taubinSmoothing::moveTheIthPointWithUniformWeights(point & pointToMove, 
                                                         const vector<UInt> & pointToPointConnection, Real smoothPara)
{
    //
    // The formula behind this smoothing method come from the paper "Curved and Surface Smoothing wihtout shrinkage"
    // 
    // https://graphics.stanford.edu/courses/cs468-01-fall/Papers/taubin-smoothing.pdf
    //
    
    Real weigth = 1./static_cast<Real>(pointToPointConnection.size());
    point deltaV(0.0, 0.0, 0.0);
    for(UInt i=0; i<pointToPointConnection.size(); ++i)
    {
        point tmpPt = meshPointer->getNode(pointToPointConnection[i]);        
        deltaV.setX(deltaV.getX()+((tmpPt.getX()-pointToMove.getX())*weigth));
        deltaV.setY(deltaV.getY()+((tmpPt.getY()-pointToMove.getY())*weigth));
        deltaV.setZ(deltaV.getZ()+((tmpPt.getZ()-pointToMove.getZ())*weigth));
    }
    
    deltaV.setX(deltaV.getX()*smoothPara);
    deltaV.setY(deltaV.getY()*smoothPara);
    deltaV.setZ(deltaV.getZ()*smoothPara);

    return(pointToMove+deltaV);
}

point taubinSmoothing::moveTheIthPointWithFuijwaraWeights(point & pointToMove, 
                                                          const vector<UInt> & pointToPointConnection, Real smoothPara)
{
    //
    // The formula behind this smoothing method come from the paper "Geometric Signal Processing on Polygonal Meshes"
    // 
    // http://mesh.brown.edu/taubin/pdfs/taubin-eg00star.pdf
    //
    Real totalWeight = 0.;
    std::vector<Real> allLenghts(pointToPointConnection.size());
    for(UInt i=0; i<pointToPointConnection.size(); ++i)
    {
        allLenghts[i] = (pointToMove - meshPointer->getNode(pointToPointConnection[i])).norm2();
        totalWeight += 1./allLenghts[i];
    }
    
    point deltaV(0.0, 0.0, 0.0);
    for(UInt i=0; i<pointToPointConnection.size(); ++i)
    {
        Real weight = (1./allLenghts[i])/totalWeight;
        point tmpPt = meshPointer->getNode(pointToPointConnection[i]);        
        
        deltaV.setX(deltaV.getX()+((tmpPt.getX()-pointToMove.getX())*weight));
        deltaV.setY(deltaV.getY()+((tmpPt.getY()-pointToMove.getY())*weight));
        deltaV.setZ(deltaV.getZ()+((tmpPt.getZ()-pointToMove.getZ())*weight));
    }
    
    deltaV.setX(deltaV.getX()*smoothPara);
    deltaV.setY(deltaV.getY()*smoothPara);
    deltaV.setZ(deltaV.getZ()*smoothPara);

    return(pointToMove+deltaV);
}

point taubinSmoothing::moveTheIthPointWithDesbrunWeights(point & pointToMove, 
                                                         const vector<UInt> & pointToPointConnection, Real smoothPara)
{
    //
    // The formula behind this smoothing method come from the paper "Geometric Signal Processing on Polygonal Meshes"
    // 
    // http://mesh.brown.edu/taubin/pdfs/taubin-eg00star.pdf
    //
    
    // save all the angles 
    std::vector<std::vector<Real> > oppositeAngles(pointToPointConnection.size());
    for(UInt i=0; i<oppositeAngles.size(); ++i)
    {
        std::vector<UInt> elem;
        UInt idPointToMove = pointToMove.getId();
        elementOnEdge(idPointToMove, pointToPointConnection[i], &elem);
        // I do in this way since there can me the boundary points 
        oppositeAngles[i].resize(elem.size());
        for(UInt j=0; j<oppositeAngles[i].size(); ++j)
        {
            UInt idOpposite = lastNode(idPointToMove, pointToPointConnection[i], elem[j]);
            oppositeAngles[i][j] = angolo(idOpposite, elem[j]);
        }
    }
    
    // compute all the weights 
    Real totalWeight = 0.;
    std::vector<Real> allCotAlphaCotBeta(pointToPointConnection.size(), 0.0);
    for(UInt i=0; i<pointToPointConnection.size(); ++i)
    {       
        for(UInt j=0; j<oppositeAngles[i].size(); ++j)
            allCotAlphaCotBeta[i] += 1./tan(oppositeAngles[i][j]);
        totalWeight += allCotAlphaCotBeta[i];
    }
    
    point deltaV(0.0, 0.0, 0.0);
    for(UInt i=0; i<pointToPointConnection.size(); ++i)
    {
        Real weight = allCotAlphaCotBeta[i]/totalWeight;
        point tmpPt = meshPointer->getNode(pointToPointConnection[i]);        
        
        deltaV.setX(deltaV.getX()+((tmpPt.getX()-pointToMove.getX())*weight));
        deltaV.setY(deltaV.getY()+((tmpPt.getY()-pointToMove.getY())*weight));
        deltaV.setZ(deltaV.getZ()+((tmpPt.getZ()-pointToMove.getZ())*weight));
    }
    
    deltaV.setX(deltaV.getX()*smoothPara);
    deltaV.setY(deltaV.getY()*smoothPara);
    deltaV.setZ(deltaV.getZ()*smoothPara);

    return(pointToMove+deltaV);
    
}

//
// Method to compute the volume 
//
Real taubinSmoothing::computeVolume()
{
    //
    // compute the volume via the divergence theorem
    //
    Real volume = 0.;
    for(UInt i=0; i<meshPointer->getNumElements(); ++i)
    {
        point normal = getTriangleNormal(i);
        Real area = getTriangleArea(i);
        
        std::vector<point> listOfPoints;
        meshPointer->getNodeOfElement(i, &listOfPoints);
        
        point pBar;
        pBar.replace(listOfPoints[0], listOfPoints[1], listOfPoints[2], 1./3.);
    
        volume += area * pBar.getX() * normal.getX();
    }
    
    return(volume);
}
                                 
void taubinSmoothing::writeMatlabFileWithVolumes(string filename)
{
    cout << "Writing file "  << filename << endl;
    ofstream out(filename.c_str());
                                 
    out << "A = [ ";
    for(UInt i=0; i<volumeSequence.size(); ++i)
        out << volumeSequence[i] << "  " << (fabs(volumeSequence[0] - volumeSequence[i])/volumeSequence[0]) << endl;
    out << " ];\n";
    
    out << "figure(1);                               " << endl;
    out << "plot(A(:,1));                            " << endl;
    out << "title('Values of the volumes');          " << endl;
    
    out << "figure(2);                               " << endl;
    out << "plot(A(:,2));                            " << endl;
    out << "title('Values of the relative error');   " << endl;
    
    
    
    out.close();
}

//
// internal methods to do some checking 
//
void taubinSmoothing::writeSmoothInfo(UInt iter)
{
    switch(smoothToUse)
    {
        case(TAUBIN):
                     cout << "Starting Taubin smoothing\n";
                     cout << "\t iteration: " << iter << endl;
                     cout << "\t    lambda: " << lambda << endl;
                     cout << "\t        mu: " << mu   << endl;
                     cout << "\t   weights: " << translateWeights() << endl;
                     break;
        case(CLASSICAL):
                     cout << "Starting Classical smoothing\n";
                     cout << "\t iteration: " << iter << endl;
                     cout << "\t    lambda: " << lambda << endl;
                     cout << "\t   weights: " << translateWeights() << endl;
                     break;
    }
}

void taubinSmoothing::checkParameters()
{
    if(mu>0.0)
    {
        cout << "!! !! !! The mu parameter is positive" << endl;
        exit(1);
    }
    else if(lambda<0.0)
    {
        cout << "!! !! !! The lambda parameter is negative" << endl;
        exit(1);
    }
    else if(lambda>((-1.)*mu))
    {
        cout << "!! !! !! The parameters lambda and mu do not satisfy lambda < -mu " << endl;
        exit(1);
    }
}

string taubinSmoothing::translateWeights()
{
    string name;
    switch(weightsToUse)
    {
        case(UNIFORMWEIGHTS):
                            name = "Uniform Weights";
                            break;
        case(FUJIWARAWEIGHTS):
                            name = "Fujiwara Weights";
                            break;
        case(DESBRUNWEIGHTS):
                            name = "Desbrum Weights";
                            break;
    }
    return(name);
}
                
