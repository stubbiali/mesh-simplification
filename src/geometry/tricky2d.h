#ifndef TRICKY2D_H_
#define TRICKY2D_H_

#include <algorithm>
#include <cassert>	
#include <iostream>
#include <vector>
#include <set>

#include "../core/shapes.hpp"
#include "../core/point.h"
#include "../core/graphItem.h"

#include "../geometry/geoElement.hpp"
#include "../geometry/geoElementSize.hpp"
#include "../geometry/mesh1d.hpp"
#include "../geometry/mesh2d.hpp"
#include "../geometry/connect2d.hpp"

#include "../file/createFile.h"

namespace geometry
{

using namespace std;

/*! Classe che permette di esplorare la mesh e calcolare determinate quantità. Il fatto interessante che riesce a ricavare queste
    informazioni solamente parendo dalla classe mesh2d e dalla connettività nodo-elemento */

template<typename GEOSHAPE> class tricky2d
{
};

/*! Specializzazione per le mesh triangolari */

template<> class tricky2d<Triangle> 
{
      //
      // Variabili contenute nella classe
      //
      public:
		      /*! Puntatore alla mesh2d che si vuole modificare */
		      mesh2d<Triangle> *        meshPointer;
		      
		      /*! Puntatore alla classe che definisce le connessioni della mesh2d presa in esame */
		      connect2d<Triangle>              conn;
		      
		      /*! Set che contiene gli elementi di bordo */
		      set<geoElement<Line> >	      bordo;
		      
		      /*! Tolleranza */
		      Real			       toll;
		      
		      /*! Punto Nullo */
		      point 			      pNull;
      //
      // Costruttore 
      //
      public:
		      /*! Costruttore vuoto */
		      tricky2d();
		      
		      /*! Costruttore con definizione dei due puntatori
			  \param _meshPointer puntatore alla mesh*/
		      tricky2d(mesh2d<Triangle> * _meshPointer);
      //
      // Set delle variabili di ambiente
      //
      public:
		      /*! Set del puntatore alla mesh e alla struttura per fare le connessioni 
			  \param _meshPointer puntatore alla mesh*/
		      void setMeshPointer(mesh2d<Triangle> * _meshPointer);
		      
		      /*! Set della tolleranza 
			  \param _toll tolleranza*/
		      void setToll(Real _toll);
		      
		      /*! Get della tolleranza */
		      Real getToll();
		      
		      /*! Metodo che sistema le variabili puntate dalla classe per sistemare il dottore prima di tutto crea le 
			  connessioni che servono nella classe connect2d poi passa le informazioni del bordo */
		      void setUp();
		      
		      /*! Metodo che riempie la variabile bordo da un vettore esterno 
			  \param _bordo vettore che contiene il bordo (la numerazione DEVE essere la stessa) */
		      void upDateBordo(vector<geoElement<Line> > * _bordo);
		      
		      /*! Metodo per prendere la variabile che contiene il bordo 
			  \param bordoVect vettore che conterrà il bordo */
		      void getBordo(vector<geoElement<Line> > * bordoVect);
		      
		      /*! Metodo che libera le variabili */
		      void clear();
      //
      // Metodi per esplorare la mesh partendo dai vertici
      //
      public: 
		      /*! Metodo che permette di capire se due triangoli adiacenti sono concordi 
			  \param elemId1 primo triangolo 
			  \param elemId2 secondo triangolo */
		      bool areCoherent(UInt elemId1, UInt elemId2);
		      
		      /*! Metodo per capire se un nodo è degenere 
			  \param nodeId identificatore di un punto */
		      bool isNodeDegenerate(UInt nodeId);
		      
		      /*! Metodo che setta degenere un nodo 
			  \param nodeId identificatore del punto*/
		      void setNodeDegenerate(UInt nodeId);
		      
		      /*! Metodo che preso un punto di un triangolo permette di individuare il successivo in modo da 
			  conservare l'orientazione
			  \param id1 identificatore di un punto 
			  \param elemId identificatore del triangolo */
		      UInt nextNode(UInt id1, UInt elemId);
		      
		      /*! Metodo che preso un punto di un triangolo permette di individuare il precedente contro l'orientazione
			  \param id1 identificatore di un punto 
			  \param elemId identificatore del triangolo */
		      UInt precNode(UInt id1, UInt elemId);
		      
		      /*! Metodo che dati 2 nodi di un triangolo restituisce il terzo 
			  \param id1 identificatore di un punto 
			  \param id2 identificatore di un altro punto 
			  \param elemId identificatore del triangolo */
		      UInt lastNode(UInt id1, UInt id2, UInt elemId);
		      
		      /*! Metodo che dato 1 nodo di un triangolo restituisce gli altri due
			  \param id1 identificatore di un punto 
			  \param elemId identificatore del triangolo 
			  \param ids puntatore al vettore che conterrà gli id */
		      void lastNode(UInt id1, UInt elemId, vector<UInt> * ids);
		      
		      /*! Metodo che restituisce gli elementi che insistono sull'edge identificato da due nodi
			  \param id1 id di un nodo
			  \param id2 id dell'altro nodo
			  \param ele vettore che conterrà gli elementi connessioni */
		      void elementOnEdge(UInt id1, UInt id2, vector<UInt> * ele);
		      
		      /*! Metodo che restituisce il numero di elementi che insistono sull'edge identificato da due nodi
			  \param id1 id di un nodo
			  \param id2 id dell'altro nodo*/
		      UInt numElementOnEdge(UInt id1, UInt id2);
		      
		      /*! Metodo che restituisce i geoId che inistono sull'edge 
			  \param id1 id di un nodo
			  \param id2 id dell'altro nodo
			  \param geo vettore che conterrà i geoId
		      N.B. ne restituisce sempre due */
		      void geoIdOnEdge(UInt id1, UInt id2, vector<UInt> * geo);
		      
		      /*! Metodo che riempie il vettore ids con tutti gli elementi connessi a id1 
			  \param id1 id di un nodo 
			  \param ids puntatore al vettore che verrà riempito */
		      void getElementAround(UInt id1, vector<UInt> * ids);
		      
		      /*! Metodo che riempie il vettore ids con tutti gli elementi connessi a id1 e ai nodi a lui connessi  
			  \param id1 id di un nodo 
			  \param ids puntatore al vettore che verrà riempito */
		      void getElementAroundBig(UInt id1, vector<UInt> * ids);
		      
		      /*! Metodo che permette di ottenere il numero di elementi connessi a id1 
			  \param id1 id di un nodo */
		      UInt getNumElementAround(UInt id1);
		      
		      /*! Metodo che riempie il vettore ids con tutti i nodi connessi a id1 
			  \param id1 id di un nodo 
			  \param ids puntatore al vettore che verrà riempito */
		      void createStellata(UInt id1, vector<UInt> * ids);
		      
		      /*! Metodo che crea la stellata di un edge con gli elementi connessi
			  \param edge vettore che contiene gli estremi dell'edge 
			  \param ids puntatore a un vettore che verrà riempito*/
		      void createStellataEdge(vector<UInt> * edge, vector<UInt> * ids);
		      
		      /*! Metodo che crea la stellata di un edge con gli elementi connessi tranne quelli sull'edge 
			  \param edge vettore che contiene gli estremi dell'edge 
			  \param ids puntatore a un vettore che verrà riempito*/
		      void createStellataEdgeWithoutOnEdge(vector<UInt> * edge, vector<UInt> * ids);
		      
		      /*! Metodo che crea la stellata di un edge con i nodi connessi 
			  \param edge vettore che contiene gli estremi dell'edge 
			  \param ids puntatore a un vettore che verrà riempito*/
		      void createStellataEdgeNode(vector<UInt> * edge, vector<UInt> * ids);
		      
		      /*! Metodo che crea la stellata di un edge coinvolgendo anche i triangoli connessi a quelli ottenuti dal 
		          metodo "createStellataEdge"
			  \param edge vettore che contiene gli estremi dell'edge 
			  \param ids puntatore a un vettore che verrà riempito*/
		      void createBigStellataEdge(vector<UInt> * edge, vector<UInt> * ids);
			  
		      /*! Metodo che riempie il vettore ids con tutti i nodi connessi a id1 con cui forma un elemento di bordo  
			  \param id1 id di un nodo 
			  \param ids puntatore al vettore che verrà riempito */
		      void createStellataBor(UInt id1, vector<UInt> * ids);
		      		      
		      /*! Metodo che permette di sapere quale nodo della stellata di center forma con lui l'angolo massimo e un
			  altro nodo l'angolo massimo
			  \param center id di un nodo 
			  \param nodeId id di un altro nodo*/
		      UInt getMaxAngle(UInt center, UInt nodeId);
		      
		      /*! Metodo che controlla se intorno al punto id1 ci sono gli stessi geoId 
			  \param id1 id di un nodo*/
		      bool sameGeoIdAround(UInt id1);
		      
		      /*! Metodo che controlla se intorno ai punti id1 e id2 ci siano gli stessi geoId 
			  \param id1 id di un nodo
			  \param id2 id dell'altro */
		      bool sameGeoIdAround(UInt id1, UInt id2);
		      
		      /*! Metodo che controlla se intorno ai punti id1, id2 e id3 ci siano gli stessi geoId 
			  \param id1 id di un nodo
			  \param id2 id dell'altro 
			  \param id3 id dell'altro */
		      bool sameGeoIdAround(UInt id1, UInt id2, UInt id3);
		      
		      /*! Metodo che permette di ottenere tutti i geoId che ha attorno il nodo in esame
			  \param id1 id di un nodo
			  \param geoIds puntatore a un vettore che conterrà i nodi*/
		      void geoIdAround(UInt id1, vector<UInt> * geoIds);
		      
		      /*! Metodo che permette di ottenere tutti i geoId che ha attorno il nodo in esame
			  \param id1 id di un nodo
			  \param geoIds puntatore a un vettore che conterrà i nodi*/
		      void geoIdAroundBor(UInt id1, vector<UInt> * geoIds);
		      
		      /*! Metodo per trovare i vicini a un nodo dato con una determinata profondità
			  \param elemId id del nodo in cui cercare
			  \param ids puntatore a un vettore di interi che conterrà gli id dei nodi*/
		      void getNodeAroundElement(UInt elemId, vector<UInt> * ids);
		      
		      /*! Metodo per trovare i vicini a un nodo dato con una determinata profondità
			  \param id1 id del nodo in cui cercare
			  \param deph intero che identifica la profondità con cui effettuare la ricerca
			  \param ids puntatore a un vettore di interi che conterrà gli id dei nodi*/
		      void getNodeAround(UInt id1, UInt deph, vector<UInt> * ids);
		      
		      /*! Metodo per trovare i vicini a un nodo dato con una determinata profondità
			  \param id1 id del nodo in cui cercare
			  \param deph intero che identifica la profondità con cui effettuare la ricerca
			  \param ids puntatore a un vettore di interi che conterrà gli id dei nodi
			  \param limite limite di punti da considerare */
		      void getNodeAround(UInt id1, UInt deph, vector<UInt> * ids, UInt limite);
		      
		      /*! Metodo che dice se il vettore ha quegli estremi (Utile principalmente per debug)
			  \param edge puntatore all'edge 
			  \param id1 estremo dell'edge
			  \param id2 altro estremo */
		      bool sameEdge(vector<UInt> * edge, UInt id1, UInt id2);
		      
		      /*! Metodo che controlla se esiste un edge 
			  \param edge vettore che contiene l'edge */
		      bool isAnEdge(vector<UInt> * edge);
		      
		      /*! Metodo l'edge flippato 
			  \param edge vettore che contiene l'edge 
			  \param altro vettore che verrà riempito con l'atro edge 
		      N.B. se l'edge non può essere flippato perché ci sono più di due elementi ritorna un vettore vuoto */
		      void atroEdge(vector<UInt> * edge, vector<UInt> * altro);
		      
		      /*! Metodo che crea i link di un edge 
			  \param edge vettore che identifica l'edge 
			  \param link vettore che conterrà gli edge che rappresentano il link */
		      void getLink(vector<UInt> * edge, vector<vector<UInt> > * link);
		      
		      /*! Metodo che crea i link di un nodo
			  \param nodeId identificatore del nodo 
			  \param link vettore che conterrà gli edge che rappresentano il link */
		      void getLinkNode(UInt nodeId, vector<vector<UInt> > * link);
      //
      // Metodi per esplorare la mesh partendo dai triangoli
      //
      public:
		      /*! Metodo che stabilisce se un triangolo è ottuso 
			  \param elemId identificatore dell'elemento */
		      bool isObtuse(UInt elemId);
		      
		      /*! Metodo che stabilisce se un edge è di bordo 
			  \param edge puntatore a un vettore che identifica un edge*/
		      bool isBoundary(vector<UInt> * edge);
		      
		      /*! Metodo che restituisce il colore dell'edge 
			  \param edge puntatore a un vettore che identifica un edge*/
		      UInt getBoundaryGeoId(vector<UInt> * edge);
		      
		      /*! Metodo che stabilisce se un triangolo ha tutti gli id distinti
			  \param elemId identificatore dell'elemento */
		      bool isTriangleDegenerate(UInt elemId);
		      
		      /*! Metodo che stabilisce se un triangolo ha tutti gli id distinti
			  \param elemId identificatore dell'elemento 
		      p.s. questo metodo è identico a quello di prima mette semplicemente una firma più generale */
		      bool isElementDegenerate(UInt elemId);
		      
		      /*! Metodo che setta un triangolo degenere
			  \param elemId identificatore dell'elemento */
		      void setTriangleDegenerate(UInt elemId);
		      		      
		      /*! Metodo che permette di ricavare la stellata del triangolo partendo dalla connettività nodo-elemento
			  \param elemId identificatore dell'elemento 
			  \param ele puntatore al vettore che conterrà gli elementi connessi*/
		      void stellataTria(UInt elemId, vector<UInt> * ele);
		      
		      /*! Metodo che trova tutti gli elementi che condividono i nodi dell'elemento in input 
			  \param elemId identificatore dell'elemento 
			  \param ele puntatore a un vettore che contiene tutti gli elementi 
			  p.s. il metodo ritorna il numero di elementi contenuti in ele */
		      UInt bigStellataTria(UInt elemId, vector<UInt> * ele);
		      
		      /*! Metodo che permette di ricavare i triangoli opposti a un lato di un triangolo fissato 
			  \param id1 identificatore di un nodo
			  \param id2 identificatore di un altro punto
			  \param elemId identificatore di un elemento
			  \param ids puntatore a un vettore che conterrà i triangoli opposti  */
		      void oppositeTria(UInt id1, UInt id2, UInt elemId, vector<UInt> * ids);
		      
		      /*! Metodo per stabilire se due triangoli hanno lo stesso geoId 
		          \param elemId1 identificatore di un triangolo 
			  \param elemId2 identificatore di un triangolo */
		      bool sameGeoId(UInt elemId1, UInt elemId2);
		      
		      /*! Metodo per stabilire se due triangoli sono complanari
		          \param elemId1 identificatore di un triangolo 
			  \param elemId2 identificatore di un triangolo 
			  \param angolo valore dell'angolo in radianti*/
		      bool normalVarr(UInt elemId1, UInt elemId2, Real angolo);
		      
		      /*! Metodo che partendo da due triangoli dice quale è l'edge in comune 
			  \param elemId1 identificatore di un triangolo
			  \param elemId2 identificatore di un triangolo
			  \param ids vettore che conterrà gli estremi dell'edge */
		      void commonEdge(UInt elemId1, UInt elemId2, vector<UInt> * ids);      
		      
		      /*! Metodo che orienta il triangolo in base a una normale
			  \param elemId identificatore dell'elemento 
			  \param normal normale con cui orientare*/
		      void orient(UInt elemId, point normal);
		      
		      
      //
      // Metodi per fare alcuni calcoli sui punti della mesh
      //
      public:
		      /*! Metodo che calcola l'angolo che ha un vertice di un triangolo
		          \param nodeId identificatore del nodo
			  \param elemId identificatore dell'elemento */
		      Real angolo(UInt nodeId, UInt elemId);
		      
		      /*! Metodo che permette di calcolare la normale a un punto 
			  \param nodeId identificatore del punto*/
		      point getPointNormal(UInt nodeId);
		      
		      /*! Metodo che serve per calcolare l'area di voronoi attorno al punto preso in esame 
		         \param nodeId identificatore del nodo */
		      Real getVoronoiAreaAround(UInt nodeId);
		      
		      /*! Metodo che serve per calcolare la curvatura di un punto della mesh 
		         \param nodeId identificatore del nodo */
		      Real getNodeCurvature(UInt nodeId);
		      
      //
      // Metodi per fare alcuni calcoli sui triangoli della mesh
      //
      public:	      
		      /*! Metodo che permette di ricavare la normale a un triangolo 
			  \param elemId identificatore dell'elemento */
		      point getTriangleNormal(UInt elemId);
		      
		      /*! Metodo per avere la normale al lato 
			  \param elemId identificatore dell'elemento 
			  \param id1 primo identificatore del lato 
			  \param id2 secondo identificatore del lato */
		      point getEdgeNormal(UInt elemId, UInt id1, UInt id2);
		      
		      /*! Metodo che permette di ricavare l'area di un triangolo
			  \param elemId identificatore dell'elemento */
		      Real getTriangleArea(UInt elemId);
		      
		      /*! Metodo che permette di ricavare l'area di un triangolo partendo dai suoi punti 
			  \param p1 primo punto 
			  \param p2 secondo punto 
			  \param p3 terzo punto */
		      Real getTriangleArea(point p1, point p2, point p3);
		      
		      /*! Metodo che permette di ricavare il perimetro di un triangolo
			  \param elemId identificatore dell'elemento */
		      Real getTrianglePerimeter(UInt elemId);
		      
		      /*! Metodo che permette di calcolare l'area di voronoi del triangolo in input riapetto 
			  al nodo anch'esso in input partendo
			  \param nodeId identificatore del nodo
			  \param elemId identificatore dell'elemento */
		      Real getVoronoiArea(UInt nodeId, UInt elemId);
		      
		      /*! Metodo che riempie il vettore in input con gli id del lato più corto dell'elemento elemId
			  \param elemId id dell'elemento preso in esame
			  \param edge puntatore a un vettore che verrà riempito con gli estremi dell'edge 
			  N.B. restituisce la lunghezza minima */
		      Real getMinEdge(UInt elemId, vector<UInt> * edge);
		      
		      /*! Metodo che riempie il vettore in input con gli id del lato più lungo dell'elemento elemId
			  \param elemId id dell'elemento preso in esame
			  \param edge puntatore a un vettore che verrà riempito con gli estremi dell'edge 
			  N.B. restituisce la lunghezza massima */
		      Real getMaxEdge(UInt elemId, vector<UInt> * edge);

		      /*! Metodo che riempie il vettore in input con gli id dei lati dell'elemento elemId
			  \param elemId identificatore dell'elemenento 
			  \param edge puntatore al vettore contenente gli edge */
		      void getEdges(UInt elemId, vector<vector<UInt>  > * edges);		      
		      
		      /*! Metodo che riempie il vettore in input con gli id dei lati di bordo dell'elemento elemId
			  \param elemId identificatore dell'elemenento 
			  \param edge puntatore al vettore contenente gli edge 
		      Il metodo ritorna vero se trova gli edge */
		      bool getBoundEdges(UInt elemId, vector<vector<UInt>  > * edges);
		      
		      /*! Metodo per calclare l'angolo di bordo 
			  \param nodeId nodo su cui si vuole calcolare l'angolo 
		      N.B. il metodo restituisce una coppia in cui il primo dice se ha trovato l'angolo di bordo 
		      potrebbe non trovarlo se il nodo ha più di un esge di bordo e il coseno dell'angolo per evitare casini di
		      roundoff */
		      pair<bool,Real> getBoundaryAngle(UInt nodeId);
		      
		      /*! Metodo per vedere se ottenere l'angolo massimo della spezzata indentificata dall'edge di bordo e dai 
		      suoi vicini 
			  \param edge vettore che identifica l'edge
		      il metodo restituisce il coseno minuimo dell'angolo delle spezzate per evitare problemi di roundoff*/
		      Real getAngleAlongEdge(vector<UInt> * edge);
		      
		      /*! Metodo che riempie il vettore in input con gli id del lato più corto dell'elemento elemId
			  \param elemId id dell'elemento preso in esame
			  N.B. restituisce la lunghezza minima */
		      Real getMinEdge(UInt elemId);
		      
		      /*! Metodo che riempie il vettore in input con gli id del lato più lungo dell'elemento elemId
			  \param elemId id dell'elemento preso in esame
			  N.B. restituisce la lunghezza massima */
		      Real getMaxEdge(UInt elemId);
		      
		      /*! Metodo che restituisce l'angolo minimo 
			  \param elemId id dell'elemento preso in esame*/
		      Real getMinAngle(UInt elemId);
		      
		      /*! Metodo che restituisce l'angolo massimo
			  \param elemId id dell'elemento preso in esame*/
		      Real getMaxAngle(UInt elemId);
	//
	// Metodo di stampa  
	//
	public:
		      /*! Metodo che stampa il bordo 
			  \param s stringa */
		      void printBoundary(string s);
};

}

#endif
