#ifndef INTERSECHANDLER_HPP_
#define INTERSECHANDLER_HPP_

#include "../core/shapes.hpp"

namespace geometry
{

/*! Classe che permette di stabilire quale tipo di intersezione fanno le differenti forme geometriche fra di loro, questa classe 
    ciene utilizzata dalla classe intersec per capire che risultato dare*/
  
template<typename GEOSHAPE1, typename GEOSHAPE2> class intersecHandler
{
	  //
	  // Variabili di classe 
	  //
	  public:
		  /*! Variabili intere che descrivono gli oggetti con cui si fa l'intersezione */
		  UInt                        shape1,shape2;
		    
		  /*! Variabile che dice quale forma avrà l'intersezione */
		  UInt                           shapeInter;
		  
	  //
	  // Costruttore
	  //
	  public:
		  /*! Cotruttore */
		  intersecHandler();
	
	  //
	  // Get delle informazioni
	  //
	  public:
		  /*! Metodo che restituisce il tipo di intersezione */
		  inline UInt getShapeInter();
		  
		  /*! Metodo che restituisce il primo tipo  */
		  inline UInt getShape1();
		  
		  /*! Metodo che restituisce il secondo tipo */
		  inline UInt getShape2();
}; 

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttore
//
template<typename GEOSHAPE1, typename GEOSHAPE2> intersecHandler<GEOSHAPE1,GEOSHAPE2>::intersecHandler()
{
	// salvo i tipi 
	shape1 = GEOSHAPE1::Shape;
	shape2 = GEOSHAPE2::Shape;
	
	// setto l'altra variabile 
	if     ((shape1==LINE)     && (shape2==LINE))		shapeInter = POINT;
	else if((shape1==LINE)     && (shape2==TRIANGLE))	shapeInter = POINT;
	else if((shape1==TRIANGLE) && (shape2==LINE))		shapeInter = POINT;
	else if((shape1==TRIANGLE) && (shape2==TRIANGLE))	shapeInter = LINE;
}

//
// Get delle informazioni
//
template<typename GEOSHAPE1, typename GEOSHAPE2> inline UInt intersecHandler<GEOSHAPE1,GEOSHAPE2>::getShapeInter()
{
	return(shapeInter);
}

template<typename GEOSHAPE1, typename GEOSHAPE2> inline UInt intersecHandler<GEOSHAPE1,GEOSHAPE2>::getShape1()
{
	return(shape1);
}

template<typename GEOSHAPE1, typename GEOSHAPE2> inline UInt intersecHandler<GEOSHAPE1,GEOSHAPE2>::getShape2()
{
	return(shape2);
}
  
  
  
}

#endif


