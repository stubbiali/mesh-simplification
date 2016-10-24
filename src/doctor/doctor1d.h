#ifndef DOCTOR1D_H_
#define DOCTOR1D_H_

#include <algorithm>
#include <cassert>	
#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <numeric>

#include "../core/shapes.hpp"
#include "../core/point.h"

#include "../geometry/geoElement.hpp"
#include "../geometry/mesh1d.hpp"
#include "../geometry/connect1d.hpp"
#include "../geometry/tricky1d.h"

#include "../file/createFile.h"

// TEST si deve controllare il processo di splitting di un edge 
// TODO si dovrebbe fare il processo di collasso 

namespace geometry
{

using namespace std;

/*! Classe che permette di effettuare alcune operazioni per modificare una mesh1d */

template<typename GEOSHAPE> class doctor1d : public tricky1d<GEOSHAPE>
{
};

/*! I metodi contenuti in questa classe permettono di fare delle modifiche su una spezzata definita da una mesh1d di linee,
*/

template<> class doctor1d<Line> : public tricky1d<Line>
{
      //
      // Costruttore 
      //
      public:
		/*! Costruttore vuoto */
		doctor1d();
		      
		/*! Costruttore con definizione dei due puntatori
		    \param _meshPointer puntatore alla mesh*/
		doctor1d(mesh1d<Line> * _meshPointer);		
      //
      // Metodi di supporto 
      //
      public:
		/*! Metodo per fare un refresh delle variabili */
		void refresh();
      //
      // Raffinamento in base alla lunghezza
      //	
      public:
		/*! Questo raffinamento permette di creare partendo dalla mesh1d puntata dalla classe una mesh1d uniforme 
		    \param lung lunghezza del passo */
		void refineUniform(Real lung);
		
		/*! Metodo che permette di spezzare un segmento 
		    \param id identificatore del segmento da splittare
		    \param lung lunghezza desiderata del segmento
		    \param toto lunghezza del segemento
		    \param newNodi puntatore alla lista dei nodi a cui aggiungere i nuovi
		    \param newElem puntatore alla lista degli elementi nuovi */
		void divideLine(UInt id, Real lung, Real toto, vector<point> * newNodi, vector<geoElement<Line> > * newElem);
      //
      // Metodi che fanno delle modifiche locali
      //
      public:
		/*! Metodo che aggiunge un punto all'interno di un segmento
		    \param id identificatore del segmento a cui aggiungere il punto 
		    \param p punto da aggiungere */
		void splitEdge(UInt id, point p);
		
		/*! Metodo che collassa l'edge nel punto p 
		    \param id identificatore del segmento a cui aggiungere il punto 
		    \param p punto da aggiungere */
		void collEdge(UInt id, point p);
};
  
}


#endif
