#ifndef INSEGMENT_H_
#define INSEGMENT_H_

#include <cassert>	
#include <cmath>
#include <set>	
#include <iostream>
#include <utility>
#include <vector>
#include <numeric>

#include "../core/shapes.hpp"
#include "../core/point.h"

#include "predicates.h"

namespace geometry
{

using namespace std;

/*! Classe che implementa un di metodo che permette di stabilire se un punto è all'interno di un segmento */

class inSegment
{
	   //
	   // Variabili
	   //
	   public:		  
		  /*! Tolleranza*/
		  Real toll;
		  
		  /*! Variabili che contengono le coordinate dei punti che identificano il segmento */
		  REAL *            A;
		  REAL *            B;
		  
		  /*! Punto che contiene le coordinate del punto da testare */
		  REAL *            C;

	   //
	   // Costruttore
	   //
	   public:
		  inSegment();
	   
           //
	   // Metodi per trovare la posizione del punto
	   //
	   public:
		   /*! Stabilisco se il punto è dentro o fuori 
		      \param vertices puntatore ai nodi
		      \param p punto che si vuole analizzare 
		      N.B. il metodo resituisce una coppia che contiene un booleano che dice se il punto sta o meno nel segmento
		           e un vettore che contiene eventualmente il punto con cui coincide p altrimenti è vuoto */
		  pair<bool, vector<UInt> > intersec(vector<point> * vertices, point p);	    
		  
		  /*! Stabilisco se il punto è dentro o fuori 
		      \param vertices puntatore alla mesh 
		      \param p punto che si vuole analizzare 
		      Il metodo stabilisce solamente se il punto appartiene o meno al segmento */
		  bool isIn(vector<point> * vertices, point p);
	   //
	   // Metodi per toll
	   //
	   public:
		  /*! Set della tolleranza 
		      \param _toll valore della tolleranza*/
		  void setToll(Real _toll);
		      
		  /*! Get della tolleranza */
		  Real getToll();
		  
	   //
	   // Metodi di stampa 
	   //
	   public:
		  /*! Metodo che stampa 
		      \param reult coppia risultante dal test */
		  void print(pair<bool, vector<UInt> > result);
	    //
	   // Distruttore
	   //
           public:
		  /*! distruttore della classe */
		  ~inSegment();
};


}


#endif
