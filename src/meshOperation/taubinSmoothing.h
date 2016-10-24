#ifndef TAUBINSMOOTHING_H_
#define TAUBINSMOOTHING_H_

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

#include "../geometry/geoElement.hpp"
#include "../geometry/geoElementSize.hpp"
#include "../geometry/mesh2d.hpp"

#include "../doctor/doctor2d.h"

#include "../file/createFile.h"

namespace geometry
{

/*! Enum with the kind of smoothing to use */   
enum kindOfSmoothing{TAUBIN,CLASSICAL};
    
/*! Enum with the weights type */
enum kindOfWeights{UNIFORMWEIGHTS,FUJIWARAWEIGHTS,DESBRUNWEIGHTS};
    
using namespace std;

/*! 
    class to implement the taubin smoothing procedure 
*/

class taubinSmoothing : public doctor2d<Triangle>
{
      //
      // Constructors 
      //
      public:
		/*! Costruttore vuoto */
		taubinSmoothing();
		      
		/*! Costruttore con definizione dei due puntatori
		    \param _meshPointer puntatore alla mesh
                    \param _mu parameters for the definition of the smoothing  
                    \param _lambda parameters for the definition of the smoothing */
		taubinSmoothing(mesh2d<Triangle> * _meshPointer, 
                                Real _mu=-0.34, 
                                Real _lambda=0.33, 
                                kindOfWeights _weightsToUse=UNIFORMWEIGHTS);
		
		/*! Metodo per prendere il puntatore alla mesh*/
		mesh2d<Triangle> * getMeshPointer()
                {
                    return(meshPointer);
                }
      
                /*! Method to set the parameters 
                    \param _mu parameters for the definition of the smoothing  
                    \param _lambda parameters for the definition of the smoothing 
                    \param _weightsToUse kind of weights */
                void setTaubinSmoothingParameters(Real _mu, Real _lambda, kindOfWeights _weightsToUse)
                {
                    mu = _mu; 
                    lambda = _lambda;
                    weightsToUse = _weightsToUse;
                    smoothToUse = TAUBIN;
                    checkParameters();
                }
                
                /*! Method to set the parameters 
                    \param _mu parameters for the definition of the smoothing  
                    \param _lambda parameters for the definition of the smoothing 
                    \param _weightsToUse kind of weights */
                void setClassicalSmoothingParameters(Real _lambda, kindOfWeights _weightsToUse)
                {
                    mu = 0.; 
                    lambda = _lambda;
                    weightsToUse = _weightsToUse;
                    smoothToUse = CLASSICAL;
                }

      //
      // Basic methods to run the routine 
      //
      public:
                /*! Method to run the smoothing 
                    \param iter number of iteration */
                void runTheSmoothing(UInt iter);
                
      //
      //  Internal method to exploit for the smoothing 
      //
      private:
                /*! Method to find the new coordinates of one point 
                    \param nodeId id of the node 
                    \param pointToPointConnection connection of the point */
                point moveOnePoint(UInt nodeId, std::vector<UInt> & pointToPointConnection);
                
                /*! Method to change the positio 
                    \param newPositions vector with all the positions */
                void moveAllThePoints(vector<point> & newPositions); 
                
                /*! Method to set the connections 
                    \param pointToPointConnection vector with the connections of the point */
                void computePointToPointConnection(vector<vector<UInt> > & pointToPointConnection);
                
                /*! Move the point 
                    \param pointToMove point to move  
                    \param pointToPointConnection points connected to the id point 
                    \param smoothPara parameter of smoothing 
                    \return new position of the point */
                point moveTheIthPoint(point & pointToMove, const vector<UInt> & pointToPointConnection, Real smoothPara);
      //
      // Different smoothing weighs 
      //
      private:
                /*! Uniform weights 
                    \param pointToMove point to move  
                    \param pointToPointConnection points connected to the id point 
                    \param smoothPara parameter of smoothing 
                    \return new position of the point */
                point moveTheIthPointWithUniformWeights(point & pointToMove, 
                                                        const vector<UInt> & pointToPointConnection, 
                                                        Real smoothPara);
                /*! Fuijwara weights 
                    \param pointToMove point to move  
                    \param pointToPointConnection points connected to the id point 
                    \param smoothPara parameter of smoothing 
                    \return new position of the point */
                point moveTheIthPointWithFuijwaraWeights(point & pointToMove, 
                                                         const vector<UInt> & pointToPointConnection, Real smoothPara);
                
                /*! Desbrun weights 
                    \param pointToMove point to move  
                    \param pointToPointConnection points connected to the id point 
                    \param smoothPara parameter of smoothing 
                    \return new position of the point */
                point moveTheIthPointWithDesbrunWeights(point & pointToMove, 
                                                        const vector<UInt> & pointToPointConnection, Real smoothPara);
      //
      // Method to compute the volume 
      //
      public:
                /*! Method to write the .m file to plot the graph 
                    \param filename string with the filename 
                    \param volumes list of the volumes */
                void writeMatlabFileWithVolumes(string filename);
                
                /*! Method to get the vector of the volumes 
                    \param _volumeSequence vector that will be filled*/
                void getVolumeSequence(vector<Real> _volumeSequence)
                {
                    _volumeSequence = volumeSequence;
                }
      
      private:
                /*! Method to compute the volume of the mesh
                    \return the volume */
                Real computeVolume();
                
      //
      // internal methods to do some checking 
      //
      private:
                /*! Method to write the information about the smoothing 
                    \param iter number of iterations */
                void writeSmoothInfo(UInt iter);
                
                /*! Method to check the parameters */
                void checkParameters();
                
                /*! Method to get the string of the weights */
                string translateWeights();
                        
      //
      // Internal variables 
      //
      private:    
          
      Real mu,lambda;
      
      kindOfWeights weightsToUse;
      
      kindOfSmoothing smoothToUse;
      
      vector<Real>  volumeSequence;
  
};











}













#endif
