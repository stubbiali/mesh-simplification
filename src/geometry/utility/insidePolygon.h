#ifndef INSIDEPOLIGON_H_
#define INSIDEPOLIGON_H_

#include <cassert>	
#include <cmath>	
#include <iostream>
#include <vector>

#include "../core/shapes.hpp"
#include "../core/point.h"
#include "../core/graphItem.h"

#include "../geometry/geoElement.hpp"
#include "../geometry/mesh2d.hpp"
#include "../geometry/connect2d.hpp"
#include "../geometry/meshSearch.hpp"

#include "../intersec/meshIntersec.hpp"

namespace geometry
{

using namespace std;

/*! Classe che implementa una serie di metodi che permettono di stabilire se un punto è all'interno al volume rachciuso da una superficie chiusa*/

class insidePolygon
{
	   //
	   // Variabili
	   //
	   public:		  
		  /*! Puntatore alla mesh deve essere superficie chiusa */
		  mesh1d<Line>  * 	     	  	   meshPointer;
		  
		  /*! Tolleranza*/
		  Real                		  	  	  toll;	
		  
		  /*! Metodo per fare le ricerche */
		  meshSearch<mesh1d<Line>,2>	 		search;
		  
		  /*! Metodo per fare le intersezioni */
		  meshIntersec<mesh1d<Line> , mesh1d<Line>, 2>	 inter;
		

	   //
	   // Costruttori
	   //
	   public:
		  /*! Costruttore vuoto */
		  insidePolygon();
	   
	   //
	   // Metodo che servono per settare le variabili 
	   //
	   public:
		  /*! Setto il puntatore della mesh2d
		      \param mesh puntatore alla mesh di superficie */ 
		  void setMeshPointer(mesh1d<Line> * _meshPointer);
	   
           //
	   // Metodo per trovare la posizione del punto
	   //  
	   public:
		  /*! Stabilisco se il punto è dentro o fuori dal volume.
		    Restituisce 1=interno 0=esterno 2=di bordo
		      \param p_to_test punto da testare*/
		  int isInside(point p_to_test); 
		  
		  /*! Metodo che trova un punto interno alla mesh, ritorna un punto interno*/
		  point findInternal();
	   //
	   // Metodi per toll
	   //
	   public:
		  /*! Set della tolleranza */
		  void setToll(Real _toll);
		      
		  /*! Get della tolleranza */
		  Real getToll();
};


}


#endif
