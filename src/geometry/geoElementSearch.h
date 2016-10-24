#ifndef GEOELEMENTSEARCH_H_
#define GEOELEMENTSEARCH_H_

#include <iostream>

#include "../core/shapes.hpp"
#include "../core/point.h"

namespace geometry
{
  
/*! Classe che implementa l'oggetto base con cui verranno salvati gli elementi per effettuare le ricerche in un albero. Si è
deciso di creare questo nuovo tipo di oggetto per rendere minime le informazioni da salvare degli elementi geometrici che vengono
cercati */

class geoElementSearch
{
	  //
	  // Variabili di classe 
	  //
	  public:
		  /*! Id dell'elemento */
		  UInt                                             id;
	  
		  /*! Numero che rappresenterà dove verrà allocato l'elemento, nel caso di figure geometriche sarà sempre il punto
		      medio del segmento che congiunge i due vertici opposti del bbox*/
		  Real                                        rapp[3];
		  
		  /*! Variabili che contengono le coordinate del bounding box*/
		  Real                                      boxMax[3];
		  Real                                      boxMin[3];
		  
		  
		  /*! Booleano che mi dice se il nodo è attivo */
		  bool                                        active;
	  //
	  // Costruttori
	  //
	  public:
		  /*! Costruttore vuoto */
		  geoElementSearch();
		  
		  /*! Costruttore di copia */
		  geoElementSearch(const geoElementSearch &E);
		
		  /*! Operatore uguaglianza */
		  geoElementSearch & operator=(const geoElementSearch &E);
		  
	  //
	  // Setting delle variabili 
	  //
	  public:
		  /*! Settaggio di active
		      \param _actvive variabile che setta se è attivo*/
		  inline void setActive(bool _active);
		  
		  /*! Settaggio dell'id 
		      \param _id intero che definisce l'id*/
		  inline void setId(UInt _id);
		  
		  /*! Settaggio del rappresentante
		      \param _rapp punto che contiene le coordinate del rappresentante*/
		  void setRapp(point _rapp);
		  
		  /*! Settaggio del rappresentante
		      \param i indice da cambiare
		      \param value vaolre da sostituire */
		  inline void setRapp(UInt i, Real value);
		  
		  /*! Settaggio del massimo
		      \param _rapp punto che contiene le coordinate del rappresentante*/
		  void setBoxMax(point _boxMax);
		  
		  /*! Settaggio del massimo
		      \param i indice da cambiare
		      \param value vaolre da sostituire */
		  inline void setBoxMax(UInt i, Real value);
		  
		  /*! Settaggio del minimo
		      \param _rapp punto che contiene le coordinate del rappresentante*/
		  void setBoxMin(point _boxMin);
		  
		  /*! Settaggio del minimo
		      \param i indice da cambiare
		      \param value vaolre da sostituire */
		  inline void setBoxMin(UInt i, Real value);
		  
	  //
	  // Get delle variabili 
	  //
	  public:
		  /*! Get dell'id */
		  inline bool isActive();
		  
		  /*! Get dell'id */
		  inline UInt getId();
		  
		  /*! Get del rappresentante */
		  point getRapp();
		  
		  /*! Get del rappresentante 
		      \param i indice della coordinata */
		  inline Real getRapp(UInt i);
		  
		  /*! Get del boxMax */
		  point getBoxMax();
		  
		  /*! Get del BoxMax
		      \param i indice della coordinata */
		  inline Real getBoxMax(UInt i);
		  
		  /*! Get del BoxMin */
		  point getBoxMin();
		  
		  /*! Get del BoxMin
		      \param i indice della coordinata */
		  inline Real getBoxMin(UInt i);
	  //
	  // Stampa
	  //
	  public:
		  /*! Print to screen */
		  void print();
};


//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Set delle variabili
//
inline void geoElementSearch::setId(UInt _id)
{
	id = _id;
}

//
// Get/Set delle variabili
//
inline void geoElementSearch::setActive(bool _active)
{
	active = _active;
}

inline void geoElementSearch::setRapp(UInt i, Real value)
{
  	assert(i<3);
	rapp[i] = value;
}

inline void geoElementSearch::setBoxMax(UInt i, Real value)
{
  	assert(i<3);
	boxMax[i] = value;
}

inline void geoElementSearch::setBoxMin(UInt i, Real value)
{
  	assert(i<3);
	boxMin[i] = value;
}

inline bool geoElementSearch::isActive()
{
	return(active);
}
      
inline UInt geoElementSearch::getId() 
{
	return(id);
}

inline Real geoElementSearch::getRapp(UInt i)
{
	assert(i<3);
	return(rapp[i]);
}

inline Real geoElementSearch::getBoxMax(UInt i)
{
	assert(i<3);
	return(boxMax[i]);
}

inline Real geoElementSearch::getBoxMin(UInt i)
{
	assert(i<3);
	return(boxMin[i]);
}

}

#endif
