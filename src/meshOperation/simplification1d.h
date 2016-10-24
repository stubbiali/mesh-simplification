#ifndef SIMPLIFICATION1D_H_
#define SIMPLIFICATION1D_H_

#include <algorithm>
#include <cassert>	
#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <numeric>

#include <sstream> 

#include "../core/shapes.hpp"
#include "../core/point.h"
#include "../core/graphItem.h"

#include "../geometry/geoElement.hpp"
#include "../geometry/mesh1d.hpp"

#include "../doctor/doctor1d.h"
#include "../doctor/virus1d.hpp"
#include "../doctor/meshHandler.hpp"

#include "../file/createFile.h"

namespace geometry
{

using namespace std;

/*! Classe che permette di effetuare il processo di eliminazione degli elementi di una spezzata */

template<typename GEOSHAPE> class simplification1d : public doctor1d<GEOSHAPE>
{
};

/*! Specializzazione per le spezzate. Tale processo è ispirato dall'articolo "Linear-time sleeve-fitting polyline simplification
    algorithms" */

template<> class simplification1d<Line> : public doctor1d<Line>
{
      //
      // Variabili di classe 
      //
      public:
		  /*! Oggetto per la gestione della griglia */
		  meshHandler<mesh1d<Line>  >			hand;
		  
		  /*! Oggetto per la diffusione del virus */
		  virus1d<Line> 	                         vir;
      //
      // Costruttori
      //
      public:
		  /*! Costruttore */
		  simplification1d();
		  
		  /*! Costruttore con definizione dei due puntatori
		    \param _meshPointer puntatore alla mesh*/
		  simplification1d(mesh1d<Line> * _meshPointer);
		  
		  /*! Metodo che cambia il puntatore della mesh
		    \param _meshPointer puntatore alla mesh*/
		  void setMeshPointer(mesh1d<Line> * _meshPointer);	  
       //
       // Metodo per la divisione 
       //
       public:
		  /*! Metodo per l'identificazione dei punti con più di due connessio 
		      \param cloud mesh0d che contiene le bariere */
		  void findBarrier(mesh0d<simplePoint> * cloud);
		  
		  /*! Metodo che trova i vari pezzi in cui si può dividere la mesh 
		      \param pieces vettore con la lista degli elementi connessi */
		  void findPieces(vector<vector<UInt> > * pieces);
	//
	// Metodi per il controllo dell'apparteneza al cilindro 
	//
	public:
		  /*! Metodo per il calcolo dell'apparteneza al cilindro 
		      \param id1 punto che identifica il centro del cilindro nella base inferiore 
		      \param id2 punto che identifica il centro del cilindro nella base superiore
		      \param toTest punto da testare */
		  Real distace(UInt id1, UInt id2, UInt toTest);
		  
		  /*! Metodo per verificare che un punto stia dentro al cilindro
		      \param id1 punto che identifica il centro del cilindro nella base inferiore 
		      \param id2 punto che identifica il centro del cilindro nella base superiore
		      \param toTest punto da testare 
		      \param limite limite della distanza*/
		  inline bool isInCylinder(UInt id1, UInt id2, UInt toTest, Real limite);
	//
	// Metodo per la semplificazione 
	//
	public:
		  /*! Metodo per la semplificazione della mesh puntata dalla classe 
		      \param limite distanza limite per cui semplificare */
		  void simplificate(Real limite);
};

}


#endif


