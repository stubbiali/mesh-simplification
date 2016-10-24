#ifndef INTRIANGLE_H_
#define INTRIANGLE_H_

#include <cassert>	
#include <cmath>	
#include <iostream>
#include <vector>

#include "../core/shapes.hpp"
#include "../core/point.h"

#include "../utility/barCoordinates.h"
#include "../utility/inSegment.h"

namespace geometry
{
 
using namespace std;
using namespace predicates;

/*! Classe che implementa una serie di metodi che permettono di stabilire se un punto è all'interno di un triangolo*/

class inTriangle
{
	   //
	   // Variabili
	   //
	   public:
		  /*! Variabili che contengono le coordinate dei punti che identificano il segmento */
		  REAL *            A;
		  REAL *            B;
		  REAL *            C;
		  
		  /*! Punto che contiene le coordinate del punto da testare */
		  REAL *            D;
		  
		  /*! Tolleranza*/
		  Real           toll;
		  
		  /*! Variabile che fa il test sui segmenti */
		  inSegment     inSeg;
		  
		  /*! Variabile che permette di costruire le coordinate baricentriche*/
		  barCoordinates  bar;

	   //
	   // Costruttore
	   //
	   public:
		  inTriangle();
	   
	   
	   //
	   // Metodi per trovare la posizione del punto
	   //
	   public:
		   /*! Stabilisco se il punto è dentro o fuori 
		      \param vertices puntatore ai nodi  
		      \param p punto che si vuole analizzare 
		      N.B. il metodo resituisce una coppia che contiene un booleano che dice se il punto sta o meno nel triangolo
		           e un vettore che contiene:
			   <ol>
			   <li> nulla se il punto è interno al triangolo;
			   <li> gli estremi dell'edge a cui appartine;
			   <li> l'id del nodo con cui coincide.
			   </ol> */
		  pair<bool, vector<UInt> > intersec(vector<point> * vertices, point p);	    
		  
		  /*! Stabilisco se il punto è dentro o fuori 
		      \param vertices puntatore ai nodi  
		      \param p punto che si vuole analizzare 
		      Il metodo stabilisce solamente se il punto appartiene o meno al segmento */
		  bool isIn(vector<point> * vertices, point p);
		  
		  /*! Metodo che ritorna la distanza con segno sfruttando i robusti predicati
		      \param vertices puntatore ai nodi  
		      \param p punto che si vuole analizzare */
		  Real signedDist(vector<point> * vertices, point p);
	   //
	   // Metodi per controllare il triangolo 
	   //
	   public:
		  /*! Metodo che dice se i tre punti in input sono allineati e definiscono un triangolo degenere
		     \param p1 primo punto
		     \param p2 secondo punto 
		     \param p3 terzo punto*/
		  bool triangleControl(point p1, point p2, point p3);
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
 		  ~inTriangle();
};

}


#endif
