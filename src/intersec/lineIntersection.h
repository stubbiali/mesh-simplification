#ifndef LINEINTERSECTION_H_
#define LINEINTERSECTION_H_

#include <cassert>	
#include <cmath>	
#include <iostream>
#include <utility>
#include <vector>

#include "../core/shapes.hpp"
#include "../core/point.h"

#include "../utility/inSegment.h"

namespace geometry
{

using namespace std;

/*! Classe che implementa un di metodo che permette di calcolare l'intersezione di due linee. Questo tipo di analisi può 
    produrre cinque diversi casi 
<ol>
<li> NON intersecano;
<li> intersecano SOLO per un punto;
<li> sono CONSECUTIVI
<li> sono CONTENUTI
<li> COINCIDONO
</ol>
Per riuscire a analizzare tutta questa casistica abbiamo definito un enum che racchiude questi casi
*/

class lineIntersection
{
	   //
	   // Variabili
	   //
	   public:		  
		  /*! Tolleranza*/
		  Real 			toll;
		  
		  /*! Variabile inSegment che serve per fare i test */
		  inSegment            inSeg;

	   //
	   // Costruttore
	   //
	   public:
		  lineIntersection();
	   
           //
	   // Metodi per trovare la posizione del punto
	   //
	   public:
		   /*! Stabilisco se il punto è dentro o fuori 
		      \param ab puntatori a un vettore di punti che definiscono la prima linea 
		      \param cd puntatori a un vettore di punti che definiscono la seconda linea
		      N.B. il metodo resituisce una coppia che contiene 
			   - un intero che dice quale tipo di intersezione abbiamo
			   - un vettore che contiene tutti i punti di intersezione */
		  pair<int, vector<point> > intersec(vector<point> * ab, vector<point> * cd);
		  
		  /*! Stabilisco se il punto è dentro o fuori 
		      \param ab puntatori a un vettore di punti che definiscono la prima linea 
		      \param cd puntatori a un vettore di punti che definiscono la seconda linea
		      N.B. il metodo restituisce un booleano che dice se i segmenti intesecano o meno  */
		  bool doIntersec(vector<point> * ab, vector<point> * cd);
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
