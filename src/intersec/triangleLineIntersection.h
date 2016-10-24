#ifndef TRIANGLELINEINTERSECTION_H_
#define TRIANGLELINEINTERSECTION_H_

#include <cassert>	
#include <cmath>	
#include <iostream>
#include <utility>
#include <vector>

#include "../core/shapes.hpp"
#include "../core/point.h"

#include "../utility/inTriangle.h"

#include "../intersec/lineIntersection.h"

namespace geometry
{

using namespace std;

/*! Classe che implementa un di metodo che permette di calcolare l'intersezione fra un path triangolare e una spezzata sono 
previsti solo due possibilità
<ol>
<li> NON intersecano;
<li> INTERSECANO SOLO per un punto;
<li> sono CONTENUTI
</ol>
Per riuscire a analizzare tutta questa casistica abbiamo definito un enum che racchiude questi casi
*/

class triangleLineIntersection
{
	   //
	   // Variabili
	   //
	   public:		  
		  /*! Tolleranza*/
		  Real 		            toll;
		  
		  /*! Variabile che permette di intersecare le linee*/
		  lineIntersection     lineInter;
		  
		  /*! Variabile che permette sapere se un punto cade nel triangolo*/
		  inTriangle              inTria;

	   //
	   // Costruttore
	   //
	   public:
		  triangleLineIntersection();
	   
           //
	   // Metodi per trovare la posizione del punto
	   //
	   public:
		   /*! Stabilisco se il punto è dentro o fuori 
		      \param abc puntatori a un vettore di punti che definiscono il triangolo
		      \param de  puntatori a un vettore di punti che definiscono la linea
		      N.B. il metodo resituisce una coppia che contiene 
			   - un intero che dice quale tipo di intersezione abbiamo
			   - un vettore che contiene tutti i punti di intersezione */
		  pair<int, vector<point> > intersec(vector<point> * abc, vector<point> * de);
		  
		  /*! Stabilisco se il punto è dentro o fuori 
		      \param abc puntatori a un vettore di punti che definiscono il triangolo
		      \param de  puntatori a un vettore di punti che definiscono la linea
		      N.B. il metodo restituisce un booleano che dice se i segmenti intesecano o meno  */
		  bool doIntersec(vector<point> * abc, vector<point> * de);
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
		  void print(pair<int, vector<point> > result);
};


}


#endif
