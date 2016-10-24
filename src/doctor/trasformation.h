#ifndef TRASFORMATION_H_
#define TRASFORMATION_H_

#include <cassert>	
#include <iostream>
#include <vector>
#include <cmath>

#include "../core/shapes.hpp"
#include "../core/point.h" 
#include "../core/tensor.h" 

namespace geometry
{

using namespace std;

/*! Classe che implementa delle trasformazioni standard che si possono fare con i punti:

<ol>
<li> affinità;
<li> traslazioni;
<li> rotazioni;
<li> dilatazioni.
</ol>

N.B. tutte le trasformazioni geometriche implementate verranno visti come casi particolari di affinità*/

class trasformation
{
	      //
	      // Costruttore 
	      //
	      public:
		      /*! Costruttore vuoto */
		      trasformation();
	      
	      //
	      // Generica affinità
	      //	
		     /*! Metodo che applica l'affinità
			  \param nodi puntatore ai nodi da trasformare
			  \param T tensore che determina i coefficienti di fronte alle coordinate
			  \param v vettore che determina il termine costante*/
		      void aff(vector<point> * nodi, tensor T, point v);
	      //
	      // Trasformazioni rigide
	      //
		      /*! Metodo che Trasla
			  \param nodi puntatore ai nodi da trasformare
			  \param v vettore che determina la traslazione */
		      void trasla(vector<point> * nodi, point v);
		      
		      /*! Metodo che ruota la mesh di un angolo attorno a un asse
			  \param nodi puntatore ai nodi da trasformare
			  \param v vettore che indica la direzione
			  \param theta angolo di rotazione in gradi*/
		      void rotAroundAxes(vector<point> * nodi, point v, Real theta);
		      
	      //
	      // Dilatazione della mesh
	      //
		      /*! Metodo che effettua un cambiamento di unità di misura uniforme su tutte le coordinate
			  \param nodi puntatore ai nodi da trasformare
			  \param fatt fattore di scala 
			  N.B. questo metodo effettua la dilatazione rispetto al centro di massa dei nodi in input*/
		      void dilata(vector<point> * nodi, point v);
};






}

#endif
