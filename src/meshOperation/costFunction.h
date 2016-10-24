#ifndef COSTFUNCTION_H_
#define COSTFUNCTION_H_

#include <algorithm>
#include <cassert>	
#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <numeric>

#include "../core/shapes.hpp"
#include "../core/point.h"
#include "../core/graphItem.h"
#include "../core/tensor.h"

#include "../utility/sortList.hpp"

#include "../geometry/geoElement.hpp"
#include "../geometry/geoElementSize.hpp"
#include "../geometry/mesh2d.hpp"

#include "../doctor/doctor2d.h"

#include "../file/createFile.h"

namespace geometry
{
    
/*! 
    Basic definition of a cost function
*/
    
class costFunction
{
    
    public:
    
    /*! Constructor */
    costFunction()
    {
    }
    
    /*! Method to set the trick class 
        \param _trickPointer a trick class pointer */
    void setTrickyClassPointer(tricky2d<Triangle> * _trickPointer)
    {
        trickPointer = _trickPointer;
    }
    
    //
    // Virtual function to be implemented to define a cost function 
    //
    
    /*! Main function to compute the cost of a point 
        \param pointId identifier of the point 
        \param actualPoint the point whose cost we are computing 
        \return the cost */
    virtual Real computeCost(UInt pointId, point & actualPoint)
    {
        return(-1.);
    }
    
    /*! Function to get the points involved with the cost function defined at contractedPoint 
        \param contractedPoint id of the point where I contract the edge 
        \param listOfPoints point involved */
    virtual void getNeightbourPoints(UInt contractedPoint, std::vector<UInt> & listOfPoints) 
    {
        // the "1" in this function is the ring of points connected to contractedPoint 
        trickPointer->getNodeAround(contractedPoint, 1, &listOfPoints);
    }
    
    /*! Method to get the next element to simplificate 
        \param listaBase where I stored the costs 
        \return the id*/
    virtual UInt getPointToRemove(sortList<geoElementSize<simplePoint> > & listaBase) const 
    {
        return(listaBase.findMin());
    }
       
    /*! Method to compare the value 
        \param actualVal value to compare  
        \param referenceVal reference value */
    virtual bool isTheValueBetter(Real actualVal, Real referenceVal) const
    {
        return(actualVal<referenceVal);
    }
       
    /*! Method to get the edge connected to the point to be removed 
        \param contractedPoint point to be removed 
        \param edge a vector with the endpoints (OUTPUT)
        \param coordinates coordinates of the point (OUTPUT)*/
    virtual void getEdgeToRemove(UInt contractedPoint, std::vector<UInt> & edge, point & coordinates)
    {
    }
    
    //
    // Utility method 
    //
    protected:

    /*! Method to remove one id from a vector 
        \param ptToRemoveFromTheList id of the point 
        \param listToBeUpadated list to change */
    void removeThePointFromVector(UInt ptToRemoveFromTheList, std::vector<UInt> & listToBeUpadated)
    {
        std::vector<UInt> tmpList;
        tmpList.reserve(listToBeUpadated.size());
        for(UInt i=0; i<listToBeUpadated.size(); ++i)
            if(listToBeUpadated[i]!=ptToRemoveFromTheList)
                tmpList.push_back(listToBeUpadated[i]);
        
        listToBeUpadated = tmpList;
    }
    
    //
    // Internal variables 
    //
    protected:
        
    tricky2d<Triangle> * trickPointer;
    
};

/*!
    Class that implements the Garland cost function 
*/

class garlandCostFunction : public costFunction
{
    public:
        
    garlandCostFunction();
    
    //
    // Virtual function to be implemented to define a cost function 
    //
    
    /*! Main function to compute the cost of a point 
        \param pointId identifier of the point 
        \param actualPoint the point whose cost we are computing 
        \return the cost */
    Real computeCost(UInt pointId, point & actualPoint);
    
    /*! Function to get the points involved with the cost function defined at contractedPoint 
        \param contractedPoint id of the point where I contract the edge 
        \param listOfPoints point involved */
    void getNeightbourPoints(UInt contractedPoint, std::vector<UInt> & listOfPoints) 
    {
        // the "1" in this function is the ring of points connected to contractedPoint 
        trickPointer->getNodeAround(contractedPoint, 1, &listOfPoints);
    }
    
    /*! Method to get the next element to simplificate 
        \param listaBase where I stored the costs 
        \return the id*/
    UInt getPointToRemove(sortList<geoElementSize<simplePoint> > & listaBase) const 
    {
        return(listaBase.findMin());
    }
       
    /*! Method to compare the value 
        \param actualVal value to compare  
        \param referenceVal reference value */
    bool isTheValueBetter(Real actualVal, Real referenceVal) const
    {
        return(actualVal<referenceVal);
    }
       
    /*! Method to get the edge connected to the point to be removed 
        \param contractedPoint point to be removed 
        \param edge a vector with the endpoints (OUTPUT)
        \param coordinates coordinates of the point (OUTPUT)*/
    void getEdgeToRemove(UInt contractedPoint, std::vector<UInt> & edge, point & coordinates);
    
    //
    // Methods in the simplification paper of Garland 
    //
    private:
    
    /*! Method to get the matrix K_p 
        \param nodeId identifier of the node  
        \param elemId identifier of the element  */
    vector<Real> createK_p(UInt nodeId, UInt elemId);

    /*! Method to create the matrix Q
        \param nodeId identifier of the node  */
    vector<Real> createQ(UInt nodeId);
    
    /*! Method to get the cost of an edge 
        \param Q1 matrix associated with the first endpoint 
        \param Q2 matrix associated with the second endpoint 
        \param pNew new position of the point  */
    Real getEdgeCost(const vector<Real> & Q1, const vector<Real> & Q2, point pNew);    
    
};

/*!
    Class that implements the Garland cost function 
*/

class noiseCostFunction : public costFunction
{
    public:
        
    noiseCostFunction();
    
    //
    // Virtual function to be implemented to define a cost function 
    //
    
    /*! Main function to compute the cost of a point 
        \param pointId identifier of the point 
        \param actualPoint the point whose cost we are computing 
        \return the cost */
    Real computeCost(UInt pointId, point & actualPoint);
    
    /*! Function to get the points involved with the cost function defined at contractedPoint 
        \param contractedPoint id of the point where I contract the edge 
        \param listOfPoints point involved */
    void getNeightbourPoints(UInt contractedPoint, std::vector<UInt> & listOfPoints) 
    {
        // the "1" in this function is the ring of points connected to contractedPoint 
        trickPointer->getNodeAround(contractedPoint, 1, &listOfPoints);
    }
    
    /*! Method to get the next element to simplificate 
        \param listaBase where I stored the costs 
        \return the id*/
    UInt getPointToRemove(sortList<geoElementSize<simplePoint> > & listaBase) const 
    {
        return(listaBase.findMin());
    }
       
    /*! Method to compare the value 
        \param actualVal value to compare  
        \param referenceVal reference value */
    bool isTheValueBetter(Real actualVal, Real referenceVal) const
    {
        return(actualVal<referenceVal);
    }
       
    /*! Method to get the edge connected to the point to be removed 
        \param contractedPoint point to be removed 
        \param edge a vector with the endpoints (OUTPUT)
        \param coordinates coordinates of the point (OUTPUT)*/
    void getEdgeToRemove(UInt contractedPoint, std::vector<UInt> & edge, point & coordinates);
    
    //
    // Methods in the simplification paper of Garland 
    //
    private:
            
    /*! Method to get the minimum and maximun diameter 
        \param listOfPoints list of the points of the plane 
        \param minMax vector which will be filled with the min max information */
    void findMinimumAndMaximumLength(const std::vector<UInt> & listOfPoints, std::vector<Real> & minMax);
    
    /*! Method to compute the regression plane 
        \param listOfPoints list of the points of the plane 
        \param normal normal to the plane (OUTPUT) 
        \param toOnPlane point on plane (OUTPUT) */
    void computePlane(const std::vector<UInt> & listOfPoints, point & normal, point & ptOnPlane);
    
    /*! Method to compute the distance from a given plane  
        \param externalPoint point from which evaluate the distance 
        \param normal normal to the plane  
        \param toOnPlane point on plane 
        \return the distance from the plane */
    Real computeDistanceFromPlane(point & externalPoint, point & normal, point & ptOnPlane)
    {
        return(fabs((externalPoint-ptOnPlane)*normal));
    }
    

    
    
};

    
}

#endif 
