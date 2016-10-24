#include "costFunction.h"

using namespace geometry;

//-----------------------------------------------------------------------
//                      Garland cost function 
//-----------------------------------------------------------------------
garlandCostFunction::garlandCostFunction() : costFunction()
{
}

//
// Virtual Methods 
//
Real garlandCostFunction::computeCost(UInt pointId, point & actualPoint)
{
    // the "1" in this function is the ring of points connected to contractedPoint 
    std::vector<UInt> listOfPoints;
    trickPointer->getNodeAround(pointId, 1, &listOfPoints);
    
    std::vector<Real> QatTheContractingPoint = createQ(pointId);
    
    // remove the point from the list 
    removeThePointFromVector(pointId, listOfPoints);
    
    std::vector<std::vector<Real> > QatTheOtherOnes(listOfPoints.size());
    for(UInt i=0; i<listOfPoints.size(); ++i)
        QatTheOtherOnes[i] = createQ(listOfPoints[i]);
    
    // initialize
    point otherPoint = trickPointer->meshPointer->getNode(listOfPoints[0]);
    Real val = getEdgeCost(QatTheContractingPoint, QatTheOtherOnes[0], otherPoint);
    
    // look the others 
    for(UInt i=1; i<listOfPoints.size(); ++i)
    {
        otherPoint = trickPointer->meshPointer->getNode(listOfPoints[i]);
        Real tmpVal =  getEdgeCost(QatTheContractingPoint, QatTheOtherOnes[i], otherPoint);
        
        if(isTheValueBetter(tmpVal,val))
            val = tmpVal;
    }
    
    return(val);
}

void garlandCostFunction::getEdgeToRemove(UInt contractedPoint, std::vector<UInt> & edge, point & coordinates) 
{
    // the "1" in this function is the ring of points connected to contractedPoint 
    std::vector<UInt> listOfPoints;
    trickPointer->getNodeAround(contractedPoint, 1, &listOfPoints);
    
    // remove the point from the list 
    removeThePointFromVector(contractedPoint, listOfPoints);
    
    // initialize the varaibles 
    point tmpPoint;
    UInt index = 0;
    Real val = computeCost(listOfPoints[0], tmpPoint);
    
    for(UInt i=1; i<listOfPoints.size(); ++i)
    {
        point tmpPoint;
        Real tmpVal = computeCost(listOfPoints[i], tmpPoint);
        
        if(isTheValueBetter(tmpVal,val))
        {
            index = i;
            val = tmpVal;
        }
    }
    
    // set the edge 
    edge.resize(2);
    edge[0] = contractedPoint;
    edge[1] = listOfPoints[index];
    
    // get the coordinates
    coordinates = trickPointer->meshPointer->getNode(listOfPoints[index]);
    
}

//
// Processo che crea la lista e i suoi elementi  
//
vector<Real> garlandCostFunction::createK_p(UInt nodeId, UInt elemId)
{
    assert(nodeId<trickPointer->meshPointer->getNumNodes());
    assert(elemId<trickPointer->meshPointer->getNumElements());
    
    // varabile in uso 
    Real noto;
    point normal;
    vector<Real> K_p;
    
    //	prendo la normale 
    normal = trickPointer->getTriangleNormal(elemId);
    
    // setto il termine noto 
    noto = (-1.0)*(trickPointer->meshPointer->getNode(nodeId)*normal);
    
    // creo la matrice 
    K_p.assign(16,0.0);
    
    // faccio gli elementi 
    K_p[0]  = normal.getX()*normal.getX();	
    K_p[1]  = normal.getX()*normal.getY();	
    K_p[2]  = normal.getX()*normal.getZ();
    K_p[3]  = normal.getX()*noto;
    
    K_p[4]  = normal.getY()*normal.getX();	
    K_p[5]  = normal.getY()*normal.getY();	
    K_p[6]  = normal.getY()*normal.getZ();
    K_p[7]  = normal.getY()*noto;
    
    K_p[8]  = normal.getZ()*normal.getX();	
    K_p[9]  = normal.getZ()*normal.getY();	
    K_p[10] = normal.getZ()*normal.getZ();
    K_p[11] = normal.getZ()*noto;
    
    K_p[12] = noto*normal.getX();	
    K_p[13] = noto*normal.getY();	
    K_p[14] = noto*normal.getZ();
    K_p[15] = noto*noto;
    
    // ritorno la matrice 
    return(K_p);
}

vector<Real> garlandCostFunction::createQ(UInt nodeId)
{
    assert(nodeId<trickPointer->meshPointer->getNumNodes());
    
    // varaibili in uso
    vector<Real>	    QTmp,K_p;
    
    // faccio un resize
    QTmp.assign(16,0.0);
    
    // prendo tutti i connessi 
    for(UInt i=0; i<trickPointer->conn.getNodeToElementPointer(nodeId)->getNumConnected(); ++i)
    {
	  // prendo la matrice 
	  K_p = createK_p(nodeId, trickPointer->conn.getNodeToElementPointer(nodeId)->getConnectedId(i));
	  
	  // aggiorno la matrice Q
	  for(UInt k=0; k<16; ++k)	QTmp[k] = QTmp[k]+K_p[k];
    }
    
    // ritono la matrice 
    return(QTmp);
}

Real garlandCostFunction::getEdgeCost(const vector<Real> & Q1, const vector<Real> & Q2, point pNew)
{     
    // variabili in uso 
    vector<Real> v1,vTmp,Q;
    
    // faccio il resize
    Q.assign(Q2.size(),0.0);
    
    // faccio la somma 
    for(UInt i=0; i<Q2.size(); ++i)	Q[i] = Q1[i]+Q2[i];
    
    // faccio un resize di v1
    v1.assign(4,1.0);
    for(UInt i=0; i<3; ++i)	v1[i] = pNew.getI(i);
    
    // faccio un resize e la moltiplicazione
    vTmp.assign(4, 0.0);
    
    // riempio il vettore
    vTmp[0] = Q[0]*v1[0] + Q[4]*v1[1] + Q[8]*v1[2]  + Q[12]*v1[3];
    vTmp[1] = Q[1]*v1[0] + Q[5]*v1[1] + Q[9]*v1[2]  + Q[13]*v1[3];  
    vTmp[2] = Q[2]*v1[0] + Q[6]*v1[1] + Q[10]*v1[2] + Q[14]*v1[3];  
    vTmp[3] = Q[3]*v1[0] + Q[7]*v1[1] + Q[11]*v1[2] + Q[15]*v1[3];  
          
    // ritorno il valore 
    return(inner_product(v1.begin(), v1.end(), vTmp.begin(), 0.0));
}


//-----------------------------------------------------------------------
//                      Noise cost function 
//-----------------------------------------------------------------------
noiseCostFunction::noiseCostFunction() : costFunction()
{
}

Real noiseCostFunction::computeCost(UInt pointId, point & actualPoint)
{
    // the "1" in this function is the ring of points connected to contractedPoint 
    std::vector<UInt> listOfPoints;
    trickPointer->getNodeAround(pointId, 1, &listOfPoints);

    // remove the point from the list 
    removeThePointFromVector(pointId, listOfPoints);
    
    std::vector<Real> minMax;
    findMinimumAndMaximumLength(listOfPoints, minMax);
    
    // compute the plane 
    point normal,ptOnPlane;
    computePlane(listOfPoints, normal, ptOnPlane);
    
    actualPoint = trickPointer->meshPointer->getNode(pointId);
    Real distance = computeDistanceFromPlane(actualPoint, normal, ptOnPlane);
        
    return(distance/minMax[0] + distance/minMax[1]);
}

void noiseCostFunction::getEdgeToRemove(UInt contractedPoint, std::vector<UInt> & edge, point & coordinates) 
{
    // the "1" in this function is the ring of points connected to contractedPoint 
    std::vector<UInt> listOfPoints;
    trickPointer->getNodeAround(contractedPoint, 1, &listOfPoints);
    
    // remove the point from the list 
    removeThePointFromVector(contractedPoint, listOfPoints);
    
    // initialize the varaibles 
    point tmpPoint;
    UInt index = 0;
    Real val = computeCost(listOfPoints[0], tmpPoint);
    
    for(UInt i=1; i<listOfPoints.size(); ++i)
    {
        point tmpPoint;
        Real tmpVal = computeCost(listOfPoints[i], tmpPoint);
        
        if(isTheValueBetter(tmpVal,val))
        {
            index = i;
            val = tmpVal;
        }
    }
    
    // set the edge 
    edge.resize(2);
    edge[0] = contractedPoint;
    edge[1] = listOfPoints[index];
    
    // get the coordinates
    coordinates = trickPointer->meshPointer->getNode(listOfPoints[index]);
    
}

//
// Methods to identify the outlayers 
//
void noiseCostFunction::findMinimumAndMaximumLength(const std::vector<UInt> & listOfPoints, std::vector<Real> & minMax)
{
    // get the points 
    std::vector<point> coordinates(listOfPoints.size());
    for(UInt i=0; i<coordinates.size(); ++i)
        coordinates[i] = trickPointer->meshPointer->getNode(listOfPoints[i]);
    
    minMax.resize(2);
    minMax[0] = (coordinates[0] - coordinates[1]).norm2();
    minMax[1] = minMax[0];
    
    for(UInt i=1; i<coordinates.size(); ++i)
    {
        for(UInt j=i+1; j<coordinates.size(); ++j)
        {
          Real dist = (coordinates[i] - coordinates[j]).norm2();
          // checks 
          if(dist<minMax[0])
              minMax[0] = dist;
          if(dist>minMax[1])
              minMax[1] = dist;
        }
    }
}
    
void noiseCostFunction::computePlane(const std::vector<UInt> & listOfPoints, point & normal, point & ptOnPlane)
{
    // get the points 
    std::vector<point> coordinates(listOfPoints.size());
    for(UInt i=0; i<coordinates.size(); ++i)
        coordinates[i] = trickPointer->meshPointer->getNode(listOfPoints[i]);
    
    // initialize the varabile to compute the normal and the ptOnPlane
    tensor MtM;
    ptOnPlane.setX(0.);    ptOnPlane.setY(0.);    ptOnPlane.setZ(0.);
    for(UInt i=0; i<coordinates.size(); ++i)
    {
        Real x = coordinates[i].getX();
        Real y = coordinates[i].getY();
        Real z = coordinates[i].getZ();
        
        // fill the point 
        ptOnPlane.setX(ptOnPlane.getX()+x);
        ptOnPlane.setY(ptOnPlane.getY()+y);
        ptOnPlane.setZ(ptOnPlane.getZ()+z);
    }
    
    // set the plane 
    ptOnPlane.setX(ptOnPlane.getX()/static_cast<Real>(coordinates.size()));
    ptOnPlane.setY(ptOnPlane.getY()/static_cast<Real>(coordinates.size()));
    ptOnPlane.setZ(ptOnPlane.getZ()/static_cast<Real>(coordinates.size()));
    
    for(UInt i=0; i<coordinates.size(); ++i)
    {
        Real x = coordinates[i].getX()-ptOnPlane.getX();
        Real y = coordinates[i].getY()-ptOnPlane.getY();
        Real z = coordinates[i].getZ()-ptOnPlane.getZ();
        
        // fill the matrix 
        MtM.setIJ(0,0, MtM.getIJ(0,0)+x*x);
        MtM.setIJ(1,1, MtM.getIJ(1,1)+y*y);
        MtM.setIJ(2,2, MtM.getIJ(2,2)+z*z);
        
        MtM.setIJ(0,1, MtM.getIJ(0,1)+x*y);
        MtM.setIJ(0,2, MtM.getIJ(0,2)+x*z);
        
        MtM.setIJ(1,2, MtM.getIJ(1,2)+y*z);
    }

    // fill the lower diagonal part 
    MtM.setIJ(1,0, MtM.getIJ(0,1));
    MtM.setIJ(2,0, MtM.getIJ(0,2));
    MtM.setIJ(2,1, MtM.getIJ(1,2));
    
    // fin the eigenvalues 
    MtM.findAutoval();
    
    Real tmpVal = fabs(MtM.getAutoVal(0));
    normal = MtM.getAutoVect(0);
    for(UInt i=1; i<3; ++i)
    {
        if(fabs(MtM.getAutoVal(i))<tmpVal)
        {
            tmpVal = fabs(MtM.getAutoVal(i));
            normal = MtM.getAutoVect(i);
            normal.normalize();
        }
    } 
        
    
}

