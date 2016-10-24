#ifndef INTERVALINTERSECTION_H_
#define INTERVALINTERSECTION_H_

#include <cassert>	
#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

#include "../core/shapes.hpp"

namespace geometry
{

using namespace std;

/*! Classe che implementa le principali funzioni per effettuare le intersezioni di due intervalli */

class intervalIntersection
{
	    //
	    // Variabili
	    //
	    public:
		    /*! Tolleranza */
		    Real                 toll;
		    
	    //
	    // Costruttore
	    //
	    public:
		    /*! Costruttore void */
		    intervalIntersection();
		    
	    //
	    // Intersezione degli intervalli
	    //
	    public:
		    /*! Controllo intersezione di due intervalli 
		        \param A1 primo estremo dell'intervallo A
			\param A2 secondo estremo dell'intervallo A 
			\param B1 primo estremo dell'intervallo B
			\param B2 secondo estremo dell'intervallo B
			La funzione ritorna vero se intersecano e l'intervalli di intersezione */
		    pair<bool, vector<Real> > intersec(Real A1, Real A2, Real B1, Real B2);
		    
		    /*! Controllo intersezione di due intervalli 
		        \param A1 primo estremo dell'intervallo A
			\param A2 secondo estremo dell'intervallo A 
			\param B1 primo estremo dell'intervallo B
			\param B2 secondo estremo dell'intervallo B
			La funzione ritorna vero se intersecano */
		    bool doIntersec(Real A1, Real A2, Real B1, Real B2);
		   
	    //
	    // Messa a punto di toll
	    //
	    public:
		  /*! Set della tolleranza */
		  void setToll(Real _toll);
		      
		  /*! Get della tolleranza */
		  Real getToll();
		  
	    //
	    // Metodo che stampa
	    //
	    public:
		  /*! Metodo utilizzato per testare la funzione */
		  void print(pair<bool, vector<Real> > result);
};


}


#endif
