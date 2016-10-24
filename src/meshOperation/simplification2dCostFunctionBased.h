#ifndef SIMPLIFICATION2DCOSTFUNCTIONBASED_H_
#define SIMPLIFICATION2DCOSTFUNCTIONBASED_H_

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

#include "../utility/sortList.hpp"

#include "../geometry/geoElement.hpp"
#include "../geometry/geoElementSize.hpp"
#include "../geometry/mesh2d.hpp"

#include "../doctor/doctor2d.h"

#include "../file/createFile.h"

#include "../meshOperation/costFunction.h"

namespace geometry
{

/*! 
    This is the basic class to simplify a mesh based on a cost function 
*/

class simplification2dCostFunctionBased : public doctor2d<Triangle>
{
    public:
    
    //
    // Constructor 
    //
    simplification2dCostFunctionBased(costFunction * _costFunctionPointer, 
                                      mesh2d<Triangle> * _meshPointer, 
                                      vector<UInt> & _pointMaterialId);
    
    //
    // Metodi che fanno la semplificazione 
    //
    public:

    /*! Method to simplify the mesh according to the cost function. The objective is to reach a
        mesh with numNodesMax nodes.
        \param numNodesMax target number of nodes */
    void simplificateGreedy(UInt numNodesMax);
    
    //
    // Methods to manage the list
    //
    public:
   
    /*! Method to build up the list 
        \param sortedList list of the point to store */
    void createElementList(sortList<geoElementSize<simplePoint> > & sortedList);
    
    /*! Method to update the list after a contraction and add the cost of the contractedPoint
        \param sortedList list of the point to store
        \param contractedPoint point where we contract the edge  */
    void updateAndAddElementList(sortList<geoElementSize<simplePoint> > & sortedList, UInt contractedPoint);
    
    /*! General method to create the pointWithCost 
        \param pointId identifier of the point 
        \param pointWithCost OUTPUT object point with the cost */
    void computeCostOfAPoint(UInt pointId, geoElementSize<simplePoint> & pointWithCost);    
    
    /*! Method to get the point to remove, it will remove it from listOfPossiblePointRemoval 
        \param listOfPossiblePointRemoval list where get the points 
        \return the id of the point (this function may return the number of nodes, if it do this it means that it is not 
        able to find a good point)*/
    UInt getPointToRemoveAndTakeOffItFromTheList(sortList<geoElementSize<simplePoint> > & listOfPossiblePointRemoval);
    
    //
    // Internal methods to manage the mesh 
    //
    public:
    
    /*! Metodo che controlla se il collasso è corretto, oltre a utilizzare il controllo del dottore controlla che 
        gli edge dopo abbiano sempre due triangoli 
        \param edge vettore con gli id dei nodi 
        \param pNew nuovo punto */
    bool control(vector<UInt> * edge, point pNew);
    
    /*! Method to set up the material Id of the new contracted point 
        \param pointContractedId id of the contracted point 
        \param edge vector with the edge contracted */
    void fillTheMaterialIdAfterContraction(UInt pointContractedId, const std::vector<UInt> & edge);
    
    /*! Metodo to refresh the list of points */
    void refresh();
    
    //
    // Internal methods to print 
    //
    public:
    
    /*! Method to print the cost function associated with the nodes 
        \param filename the name of the file */
    void printCostFunction(std::string filename);
    
    //
    // Internal class variable 
    //
    private:
    // pointer to the cost function 
    costFunction * costFunctionPointer;
    // list of the material ids
    std::vector<UInt> pointMaterialId;
        
};
    
}



#endif
