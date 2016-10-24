#ifndef TRIANGLEINTERSECTION_H_
#define TRIANGLEINTERSECTION_H_

#include <cassert>	
#include <cmath>	
#include <iostream>
#include <utility>
#include <vector>

#include "../core/shapes.hpp"
#include "../core/point.h"

#include "../intersec/lineIntersection.h"
#include "../intersec/triangleLineIntersection.h"

#include "../utility/barCoordinates.h"
#include "../utility/inTriangle.h"

namespace geometry
{

using namespace std;

/*! Classe che implementa un di metodo che permette di calcolare l'intersezione di due traingoli. Questo tipo di analisi può 
    produrre cinque diversi casi 
<ol>
<li> NON intersecano;
<li> intersecano SOLO per un PUNTO;
<li> intersecano SOLO per una LINEA;
<li> COINCIDONO;
<li> sono CONTENUTI.
</ol>
Per riuscire a analizzare tutta questa casistica abbiamo definito un enum che racchiude questi casi
*/

class triangleIntersection
{
	   //
	   // Variabili
	   //
	   public:		  
		  /*! Tolleranza*/
		  Real 				   toll;
		  
		  /*! Variabile inTriangle che serve per fare i test */
		  inSegment             	   inSeg;
		  
		  /*! Variabile inTriangle che serve per fare i test */
		  inTriangle         		  inTria;
		  
		  /*! Oggetto che serve per effettuare le intersezioni fra segmenti */
		  lineIntersection  	        lineInter;
		  
		  /*! Oggetto che serve per effettuare le intersezioni fra segmenti */
		  triangleLineIntersection  triaLineInter;

	   //
	   // Costruttore
	   //
	   public:
		  triangleIntersection();
	   
           //
	   // Metodi per trovare la posizione del punto
	   //
	   public:
		   /*! Metodo che effettua l'intersezione 
		      \param abc puntatore al primo triangolo 
		      \param def puntatore al secondo triangolo 
		      N.B. il metodo resituisce una coppia che contiene 
			   - un intero che dice quale tipo di intersezione abbiamo
			   - un vettore che contiene tutti i punti di intersezione */
		  pair<int, vector<point> > intersec(vector<point> * abc, vector<point> * def);
		  
		  /*! Stabilisco se i due traingoli intersecano 
		      \param abc puntatore al primo triangolo 
		      \param def puntatore al secondo triangolo 
		      N.B. il metodo restituisce un booleano che dice se i segmenti intesecano o meno  */
		  bool doIntersec(vector<point> * abc, vector<point> * def);
		  
		  /*! Metodo che permette di trovare l'intersezione fra i lati del triangolo e il piano 
		      \param plane puntatore a un vettore di punti che identificano il piano 
		      \param tria puntatore a un vettore che contiene il triangolo 
		      \param pt puntatore a in vettore che contiene i punti*/
		  void findPoint(vector<point> * plane, vector<point> * tria, vector<point> * pt);
		
		  /*! Metodo che trova il punto di intersezione fra un segmento e un piano 
		      \param A primo estremo
		      \param B secondo estremo
		      \param normal normale al piano 
		      \param noto termine noto del piano 
		      N.B. il metodo restituisce il punto di intersezione */
		  point findPoint(point A, point B, point normal, Real noto);
		  
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
