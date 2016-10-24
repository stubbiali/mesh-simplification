#ifndef ISOTROPICQUALITY2D_H_
#define ISOTROPICQUALITY2D_H_

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

#include "../doctor/doctor2d.h"

#include "../file/createFile.h"

// TODO ci sono dei problemi se non si aggiorna dopo il collasso principalmente il problema è sul riconoscimento dei nodi di 
//      bordo 

// TODO rifai tutto con le liste 

namespace geometry
{

using namespace std;

/*! Classe che permette di utilizzare le principali operazioni per miglioare la qualità della mesh */

template<typename GEOSHAPE> class isotropicQuality2d : public doctor2d<GEOSHAPE>
{
};

/*! Classe che implementa una serie di algoritmi per il miglioramento isotropico della qualità della griglia nel caso di mesh 
    triangolari */

template<> class isotropicQuality2d<Triangle> : public doctor2d<Triangle>
{
      //
      // Costruttore 
      //
      public:
		/*! Costruttore vuoto */
		isotropicQuality2d();
		      
		/*! Costruttore con definizione dei due puntatori
		    \param _meshPointer puntatore alla mesh*/
		isotropicQuality2d(mesh2d<Triangle> * _meshPointer);
		
		/*! Metodo per prendere il puntatore alla mesh*/
		inline mesh2d<Triangle> * getMeshPointer() {return(meshPointer);};
      //
      // Metodi per calcolare la qualità
      //
      public:
		/*! Metodo che calcola la qualità di un triangolo a partire dal suo id  
		    \param elemId identificatore dell'elemento*/
		Real triangleQual(UInt elemId);
		
		/*! Metodo che calcola la qualità di un triangolo a partire dai suoi nodi 
		    \param id1 identificatore del primo punto del triangolo
		    \param id2 identificatore del secondo punto del triangolo
		    \param id3 identificatore del terzo punto del triangolo*/
		Real triangleQual(UInt id1, UInt id2, UInt id3);
		
		/*! Metodo che calcola la qualità di un triangolo a partire dai suoi nodi 
		    \param p1 primo punto del triangolo
		    \param p2 secondo punto del triangolo
		    \param p3 terzo punto del triangolo*/
		Real triangleQual(point p1, point p2, point p3);
		
		/*! Metodo che restituisce la più piccola qualità fra i triangoli adiacenti a un lato 
		    \param edge puntatore a un vettore che contiene gli id dei nodi sull'edge*/
		Real qualityOnEdge(vector<UInt> * edge);
		
		/*! Metodo che restituisce la più piccola qualità fra i triangoli adiacenti a un lato che verrà sostituito con 
		    un punto 
		    \param edge puntatore a un vettore che contiene gli id dei nodi sull'edge
		    \param pt punto che sostiuirà il lato 
		    N.B. non viene presa in esame la qualità dei triangoli che sono adiacenti al lato */
		Real qualityOnEdge(vector<UInt> * edge, point pt);
		
		/*! Metodo che restituisce la più piccola qualità fra i triangoli che condividono gli estremi dell'edge
		    \param edge puntatore a un vettore che contiene gli id dei nodi sull'edge*/
		Real qualityAroundEdge(vector<UInt> * edge);
		
		/*! Metodo che restituisce la più piccola qualità fra i triangoli adiacenti a un lato che dovrà essere swappato 
		    \param edge puntatore a un vettore che contiene gli id dei nodi sull'edge*/
		Real qualityAfterSwap(vector<UInt> * edge);
		
		/*! Metodo che restituisce la più piccola qualità fra i triangoli adiacenti a un nodo 
		    \param nodeId identificatore del nodo*/
		Real qualityOnNode(UInt nodeId);
		
		/*! Metodo che restituisce la più piccola qualità fra i triangoli adiacenti a un nodo avendolo sostituito con un 
		    altro
		    \param nodeId identificatore del nodo
		    \param pt nuove coordinate di nodeId*/
		Real qualityOnNode(UInt nodeId, point pt);
	//
	// Metodi che creano le liste per fare i metodi greedy
	//
	public:
		/*! Metodo che crea la lista dei punti ordinati in base alla loro qualità 
		    \param lista puntatore al set che verrà riempito */
		void createNodeList(set<geoElementSize<simplePoint> > * lista);
		
		/*! Metodo che crea la lista dei triangoli ordinati in base alla loro qualità 
		    \param lista puntatore al set che verrà riempito */
		void createElementList(set<geoElementSize<Triangle> > * lista);
		
		/*! Metodo che rimuove dalla lista dei nodi 
		    \param lista puntatore alla lista da aggiornare
		    \param ids vettore con gli id da eliminare */
		void removeNodeList(set<geoElementSize<simplePoint> > * lista, vector<UInt> * ids);
		
		/*! Metodo che rimuove dalla lista degli elementi  
		    \param lista puntatore alla lista da aggiornare
		    \param ids vettore con gli id da eliminare */
		void removeElementList(set<geoElementSize<Triangle> > * lista, vector<UInt> * ids);
		
		/*! Metodo che aggiunge alla lista dei nodi 
		    \param lista puntatore alla lista da aggiornare
		    \param ids vettore con gli id da aggiungere */
		void addNodeList(set<geoElementSize<simplePoint> > * lista, vector<UInt> * ids);
		
		/*! Metodo che aggiunge alla lista degli elementi  
		    \param lista puntatore alla lista da aggiornare
		    \param ids vettore con gli id da aggiungere */
		void addElementList(set<geoElementSize<Triangle> > * lista, vector<UInt> * ids);
	//
	//  Metodi per calcolare la nuova posizione dei punti 
	//
	public:
		/*! Metodo che implementa un modo per trovare la corretta posizione di un nodo in modo da incrementare la qualità
		    \param i identificatore del nodo */
		point newSmoothingPoint(UInt i);
		
		/*! Metodo che implementa un modo per trovare il nodo per il collasso
		    \param edge vettore che contiene gli id dei lati */
		point newCollapsingPoint(vector<UInt> * edge);
		
		/*! Metodo che implementa un modo per trovare il nodo per lo split
		    \param edge vettore che contiene gli id dei lati */
		point newSplittingPoint(vector<UInt> * edge);
	//
	//  Metodi per cercare l'edge da modificare 
	//
	public:
		/*! Metodo per trovare il migliore lato da swappare nel triangolo elemId
		    \param elemId identificatore del triangolo 
		    \param limite angolo limite per fare lo swap 
		    Il metodo ritorna una coppia con un booleano che dice se fare lo swap e il vettore con l'edge da swappare */
		pair<bool, vector<UInt> > findEdgeToSwap(UInt elemId, Real limite);
		
		/*! Metodo per trovare il lato che deve essere collassato 
		    \param elemId identificatore del triangolo 
		    \param lung lunghezza al di sotto della quale si deve collassare il lato
		    Il metodo ritorna una coppia con un booleano che dice se fare il collasso e il vettore con l'edge da
		    collassare */
		pair<bool, vector<UInt> > findEdgeToColl(UInt elemId, Real lung);
		
		/*! Metodo per trovare il lato che deve essere splittato
		    \param elemId identificatore del triangolo 
		    \param lung lunghezza al di sopra della quale si deve splittare il lato
		    Il metodo ritorna una coppia con un booleano che dice se fare lo split e il vettore con l'edge da
		    splittare */
		pair<bool, vector<UInt> > findEdgeToSplit(UInt elemId, Real lung);
		
	//
	// Processi che ciclano sulla mesh e fanno delle operazioni 
	//
	public:
		/*! Processo di Smoothing
		    \param iter numero di iterazioni*/
		void smoothing(UInt iter);
		
		/*! Processo di Smoothing che muove solamente i nodi all'interno della griglia
		    \param iter numero di iterazioni*/
		void smoothingOnlyInternal(UInt iter);
		
		/*! Processo di Swap
		    \param limite angolo limite*/
		void swapping(Real limite=60.0);
		
		/*! Processo di Collapsing
		    \param lung lunghezza limite*/
		void collapsing(Real lung);
		
		/*! Processo di Collapsing che collassa solamente i nodi interni
		    \param lung lunghezza limite*/
		void collapsingOnlyInternal(Real lung);
		
		/*! Processo di Splitting
		    \param lung lunghezza limite*/
		void splitting(Real lung);
		
		/*! Processo di raffinamento uniforme */
		void refineUniform();
		
		/*! Processo che elimina i nodi con num connessioni 
		    \param num numero di connessioni */
		void removeNodes(UInt num=3);
		
		/*! Metodo che migliora la qualità seguendo i seguenti passaggi
		    <ol>
		    <li> collasso lati troppo piccoli 
		    <li> smoothing / swapping
		    </ol>
		    \param iter numero di iterazioni 
		    \param iterSmooth numero di iterazioni per il processo di smoothing
		    \param lungMax percentuale della lunghezza massima
		    \param lungMin percentuale della lunghezza minima
		    \param angolo angolo per il processo di swapping */
		void standardImprove(UInt iter=2, UInt iterSmooth=5, Real lungMax=0.3, Real lungMin=0.7, Real angolo=60.0);
		
		/*! Metodo che migliora la qualità seguendo i seguenti passaggi
		    <ol>
		    <li> collasso lati troppo piccoli 
		    <li> smoothing / swapping
		    </ol>
		    \param iter numero di iterazioni 
		    \param iterSmooth numero di iterazioni per il processo di smoothing
		    \param lungMax percentuale della lunghezza massima
		    \param lungMin percentuale della lunghezza minima
		    \param angolo angolo per il processo di swapping */
		void standardImproveOnlyInternal(UInt iter=2, UInt iterSmooth=5, 
						 Real lungMax=0.3, Real lungMin=0.7, Real angolo=60.0);
	//
	// Processi che ciclano sulla mesh e fanno delle operazioni partendo sempre da quello con qualità più bassa
	//
	public:
		/*! Processo di Smoothing
		    \param minQual qualità minima richiesta*/
		void smoothingGreedy(Real minQual=1.0);
		
		/*! Processo di Swap
		    \param limite angolo limite
		    \param minQual qualità minima richiesta */
		void swappingGreedy(Real limite=60.0, Real minQual=1.0);
		
		/*! Processo di Smoothing
		    \param lung lunghezza al di sotto della quale collassare
		    \param minQual qualità minima richiesta*/
		void collapsingGreedy(Real lung, Real minQual=1.0);
		
		/*! Metodo che migliora la qualità seguendo i seguenti passaggi
		    <ol>
		    <li> collasso lati troppo piccoli 
		    <li> smoothing / swapping
		    </ol>
		    \param lungMin lunghezza minima al di sotto della quale collassare (di default si usa la lunghezza massima 
				    della mesh in modo tale da esplorare più configurazioni)
		    \param iter numero di iterazioni 
		    \param qualMin minima qualità consentita
		    \param angolo angolo per il processo di swapping */
		void standardImproveGreedy(Real lungMax=0.4, Real lungMin=0.6, UInt iter=4, Real qualMin=1.0, Real angolo=60.0);
	
	//
	// Metodi che stampano la griglia
	//
	public:
		/*! Processo che stampa la qualità dei nodi 
		    \param nome stringa che contiene il nome */
		void printNodeQuality(string nome);
		
		/*! Processo che stampa la qualità degli elementi 
		    \param nome stringa che contiene il nome */
		void printElementQuality(string nome);
		
		/*! Processo che stampa il grado dei nodi 
		    \param nome stringa che contiene il nome */
		void printNodeDegree(string nome);
  
};











}













#endif
