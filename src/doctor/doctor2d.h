#ifndef DOCTOR2D_H_
#define DOCTOR2D_H_

#include <algorithm>
#include <cassert>	
#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <numeric>

#include "../core/shapes.hpp"
#include "../core/point.h"
#include "../core/graphItem.h"

#include "../geometry/geoElement.hpp"
#include "../geometry/geoElementSize.hpp"
#include "../geometry/mesh2d.hpp"
#include "../geometry/tricky2d.h"

namespace geometry
{

using namespace std;

/*! Classe che permette di utilizzare le principali operazioni di modifica della mesh*/

template<typename GEOSHAPE> class doctor2d : public tricky2d<GEOSHAPE>
{
};

/*! Classe che implementa le principali operazioni sulla mesh2d composta da triangoli. Overview dei metodi implementati:

<ol>
<li> smoothing;
<li> edge swapping
<li> edge collapsing
<li> edge splitting. 
</ol>

Alcune note importanti sulla mesh di input:

<ol>
<li> si presuppone che la mesh di input sia conforme 
<li> si presuppone che la mesh abbia anche le informazioni del bordo 
</ol>

*/

template<> class doctor2d<Triangle> : public tricky2d<Triangle>
{
      //
      // Costruttore 
      //
      public:
		/*! Costruttore vuoto */
		doctor2d();
		      
		/*! Costruttore con definizione dei due puntatori
		    \param _meshPointer puntatore alla mesh*/
		doctor2d(mesh2d<Triangle> * _meshPointer);
      //
      // Metodi per il refresh
      //
      public:
		/*! Metodo che mette a posto gli elementi nulli creati con i metodi di collasso e splitting */
		void refresh();
		
		/*! Metodo che elimina dalla lista l'edge di bordo 
		    \param edge vettore che conterrà gli estremi dell'edge
		    Ritorna vero se l'edge è stato eliminato */
		bool eraseBoundEdge(vector<UInt> * edge);
		
		/*! Metodo che aggiunge alla lista l'edge 
		    \param edge vettore che conterrà gli estremi dell'edge
		    \param geoId identificatore di bordo dell'edge (di default è 0 )*/
		void addBoundEdge(vector<UInt> * edge, UInt geoId = 0 );
		
		/*! Metodo che cambia le coordinate di un nodo 
		    \param id identificatore del nodo
		    \param newPos punto che contiene le nuove coordinate*/
		void changeNode(UInt id, point newPos);
		
      //
      // Metodi di smoothing
      //
      public:		
		/*! Metodo che permette di controllare se la nuova posizione porta alla nascita di incongruenze nella mesh 
		   \param i identificatore del punto
		   \param newPos nuove coordinate del punto
		   \param angLim angolo limite per il salto degli edge */
		bool controlPositionForMetricAda(UInt i, point newPos, Real angLim=5.);
		
		/*! Metodo che permette di controllare se la nuova posizione porta alla nascita di incongruenze nella mesh 
		   \param i identificatore del punto
		   \param newPos nuove coordinate del punto
		   \param angMin angolo minimo consentito 
		   N.B. questo controllo è basato sulla metrica e effettua anche un controllo sull'angolo minimo */
		bool controlPosition(UInt i, point newPos, Real angMin=0.);
      //
      // Metodi di swapping
      //
      public:
		/*! Metodo che permette di controllare se è possibile effettuare lo swap
		    \param edge vettore che conterrà gli estremi dell'edge
		    \param limite angolo limite per il controllo sulle normali */
		bool controlSwap(vector<UInt> * edge, Real limite);
		
		/*! Metodo che permette di controllare se è possibile effettuare lo swap
		    \param edge vettore che conterrà gli estremi dell'edge
		    \param limite angolo limite per il controllo sul salto delle normali */
		bool controlSwapForMetricAda(vector<UInt> * edge, Real limite);
		
		/*! Metdo che effettua lo swap di un edge 
		    \param edge puntatore a un vettore che contiene le informazioni per lo swap
		    \param setUpNormal variabile che dice se sistemare le normali*/
		void swap(vector<UInt> * edge, bool setUpNormal=true);      
      //
      // Metodi di collasso
      //
      public:		
		/*! Metodo per controllare la validità del collasso dell'edge
		    \param edge puntatore al vettore che contiene le informazioni dell'edge che verrà collassato
		    \param pNew coordinate del nuovo punto */
		bool controlColl(vector<UInt> * edge, point pNew);
		
		/*! Metodo per controllare la validità del collasso dell'edge non effettua il controllo sull'angolo
		    \param edge puntatore al vettore che contiene le informazioni dell'edge che verrà collassato
		    \param pNew coordinate del nuovo punto 
		    \param angLim angolo limite per il salto degli edge */
		bool controlCollForMetricAda(vector<UInt> * edge, point pNew, Real angLim=5.);
		
		/*! Metodo che effettua il collasso di un edge 
		     \param edge puntatore a un vettore che contiene le informazioni dell'edge 
		     N.B. per comodità le connettività vengono messe a posto con il nodo in fondo alla lista dei nodi della mesh*/
		void collEdge(vector<UInt> * edge);
		
		/*! Metodo che serve per eliminare un punto della griglia. 
		    \param nodeId identificatore del nodo */
		bool removeNode(UInt nodeId);
      //
      // Splitting
      //
      public:	
	        /*! Metodo che aggiunge un punto dentro al triangolo 
		    \param elemId identificatore dell'elemento 
		    \param setUpNormal variabile che dice se sistemare le normali  
		    N.B. per comodità le connettività vengono messe a posto con il nodo in fondo alla lista dei nodi della mesh*/
		void splitTria(UInt elemId, bool setUpNormal=true);
		
		/*! Metodo che controlla se il punto aggiunto crea triangoli invertiti 
		    \param edge lato da dividere 
		    \param pNew nuovo punto */
		bool controlSplit(vector<UInt> * edge, point pNew);
		
		/*! Metodo che permette di splittare l'edge dato in input 
		    \param edge puntatore a un vettore che contiene gli estremi dell'edge
		    \param setUpNormal variabile che dice se sistemare le normali 
		 N.B. per comodità le connettività vengono messe a posto con il nodo in fondo alla lista dei nodi della mesh*/
		void splitEdge(vector<UInt> * edge, bool setUpNormal=true);
		
		/*! Metodo che fa il raffinamento rosso inserendo gli elementi nella lista in input 
		    \param elemId identificatore dell'elemento 
		    \param ids puntatore a un vettore che contiene gli id dei nodi che si devono aggiungere ai tre lati 
			       per convenzione il primo è relativo al primo lato, il secondo al secondo e il terzo al terzo 
		    \param tmpElem puntatore a un vettore che conterrà tutti gli elementi */
		void splitRed(UInt elemId, vector<UInt> * ids, vector<geoElement<Triangle> > * tmpElem);
		
		/*! Metodo che fa il raffinamento blu o verde inserendo gli elementi nella lista in input 
		    \param elemId identificatore dell'elemento 
		    \param ids puntatore a un vettore che contiene gli id dei nodi che si devono aggiungere ai tre lati 
			       per convenzione il primo è relativo al primo lato, il secondo al secondo e il terzo al terzo 
		    \param edge puntatore a un vettore di booleani che dice quale edge splittare
		    \param tmpElem puntatore a un vettore che conterrà tutti gli elementi */
		void splitBlue(UInt elemId, vector<UInt> * ids, vector<bool> * edge, vector<geoElement<Triangle> > * tmpElem);
		void splitGreen(UInt elemId, vector<UInt> * ids, vector<bool> * edge, vector<geoElement<Triangle> > * tmpElem);
		
		/*! Metodo che fa il raffinamento red-blu-green 
		    \param nodi vettore con la lista dei nodi da aggiungere 
		    \param edges vettore con il edge a cui appartengono */
		void splitRGB(vector<point> * nodi, vector<geoElement<Line> > * edges);
		
};

}


#endif
