#ifndef TRICKY3D_H_
#define TRICKY3D_H_

#include <algorithm>
#include <cassert>	
#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <numeric>

#include "../core/shapes.hpp"
#include "../core/tensor.h"
#include "../core/point.h"
#include "../core/graphItem.h"

#include "../geometry/geoElement.hpp"
#include "../geometry/geoElementSize.hpp"
#include "../geometry/mesh1d.hpp"
#include "../geometry/mesh2d.hpp"
#include "../geometry/mesh3d.hpp"
#include "../geometry/connect3d.hpp"

namespace geometry
{

using namespace std;

/*! Classe che permette di esplorare la mesh e calcolare determinate quantità. Il fatto interessante che riesce a ricavare queste
    informazioni solamente parendo dalla classe mesh3d e dalla connettività nodo-elemento */

template<typename GEOSHAPE> class tricky3d
{
};

/*! Specializzazione per le mesh tetraedriche */

template<> class tricky3d<Tetra> 
{
      //
      // Variabili contenute nella classe
      //
      public:
		      /*! Puntatore alla mesh2d che si vuole modificare */
		      mesh3d<Tetra> *           meshPointer;
		      
		      /*! Puntatore alla classe che definisce le connessioni della mesh2d presa in esame */
		      connect3d<Tetra>                 conn;
		      
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
		      tricky3d();
		      
		      /*! Costruttore con definizione dei due puntatori
			  \param _meshPointer puntatore alla mesh*/
		      tricky3d(mesh3d<Tetra> * _meshPointer);
      //
      // Set delle variabili di ambiente
      //
      public:
		      /*! Set del puntatore alla mesh e alla struttura per fare le connessioni 
			  \param _meshPointer puntatore alla mesh*/
		      void setMeshPointer(mesh3d<Tetra> * _meshPointer);
		      
		      /*! Set della tolleranza 
			  \param _toll tolleranza*/
		      void setToll(Real _toll);
		      
		      /*! Get della tolleranza */
		      Real getToll();
		      
		      /*! Metodo che sistema le variabili puntate dalla classe per sistemare il dottore prima di tutto crea le 
			  connessioni che servono nella classe connect2d poi passa le informazioni del bordo */
		      void setUp();
		      
      //
      // Metodi per esplorare la mesh partendo dai vertici
      //
      public: 
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
		      
		      /*! Metodo che dati 3 nodi di un tetraedro restituisce il quarto
			  \param id1 identificatore di un punto 
			  \param id2 identificatore di un altro punto
			  \param id3 identificatore di un altro punto 
			  \param elemId identificatore del triangolo */
		      UInt lastNode(UInt id1, UInt id2, UInt id3, UInt elemId);
		      
		      /*! Metodo che dato 1 nodo di un tetraedro restituisce gli altri due
			  \param id1 identificatore di un punto 
			  \param elemId identificatore del triangolo 
			  \param ids puntatore al vettore che conterrà gli id */
		      void lastNode(UInt id1, UInt elemId, vector<UInt> * ids);
		      
		      /*! Metodo che restituisce gli elementi che insistono sull'edge identificato da due nodi
			  \param id1 id di un nodo
			  \param id2 id dell'altro nodo
			  \param ele vettore che conterrà gli elementi connessioni */
		      void elementOnEdge(UInt id1, UInt id2, vector<UInt> * ele);
		      
		      /*! Metodo che restituisce gli elementi che insistono sulla faccia identificata da tre nodi
			  \param id1 id di un nodo
			  \param id2 id dell'altro nodo
			  \param id3 id dell'altro nodo
			  \param ele vettore che conterrà gli elementi connessioni */
		      void elementOnFace(UInt id1, UInt id2, UInt id3, vector<UInt> * ele);
		      
		      /*! Metodo che riempie il vettore ids con tutti gli elementi connessi a id1 
			  \param id1 id di un nodo 
			  \param ids puntatore al vettore che verrà riempito */
		      void getElementAround(UInt id1, vector<UInt> * ids);
		      
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
		      
		      /*! Metodo che crea la stellata di un edge con i nodi connessi 
			  \param edge vettore che contiene gli estremi dell'edge 
			  \param ids puntatore a un vettore che verrà riempito*/
		      void createStellataEdgeNode(vector<UInt> * edge, vector<UInt> * ids);
		      
		      /*! Metodo che crea la stellata di un edge coinvolgendo anche i tetrahedri connessi a quelli ottenuti dal 
		          metodo "createStellataEdge"
			  \param edge vettore che contiene gli estremi dell'edge 
			  \param ids puntatore a un vettore che verrà riempito*/
		      void createBigStellataEdge(vector<UInt> * edge, vector<UInt> * ids);
			  
		      /*! Metodo che riempie il vettore ids con tutti i nodi connessi a id1 con cui forma un elemento di brodo  
			  \param id1 id di un nodo 
			  \param ids puntatore al vettore che verrà riempito */
		      void createStellataBor(UInt id1, vector<UInt> * ids);
		      		      
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
		      
		      /*! Metodo che controlla se intorno ai punti id1, id2 e id3 ci siano gli stessi geoId 
			  \param id1 id di un nodo
			  \param id2 id dell'altro 
			  \param id3 id dell'altro 
			  \param id4 id dell'altro */
		      bool sameGeoIdAround(UInt id1, UInt id2, UInt id3, UInt id4);
		      
		      /*! Metodo che permette di ottenere tutti i geoId che ha attorno il nodo in esame
			  \param id1 id di un nodo
			  \param geoIds puntatore a un vettore che conterrà i nodi*/
		      void geoIdAround(UInt id1, vector<UInt> * geoIds);
      //
      // Metodi per esplorare la mesh partendo dai triangoli
      //
      public:		      
		      /*! Metodo che stabilisce se un triangolo è ottuso 
			  \param edge puntatore a un vettore che identifica un edge*/
		      bool isBoundary(vector<UInt> * edge);
		      
		      /*! Metodo che stabilisce se un triangolo ha tutti gli id distinti
			  \param elemId identificatore dell'elemento 
		      p.s. questo metodo è identico a quello di prima mette semplicemente una firma più generale */
		      bool isElementDegenerate(UInt elemId);
		      
		      /*! Metodo che setta un triangolo degenere
			  \param elemId identificatore dell'elemento */
		      void setElementDegenerate(UInt elemId);
		      
		      /*! Metodo che ritorna la distanza con segno del quarto vertice del tetraedro rispetto al piano definito 
		          dagli altri 3
		          \param elemId identificatore dell'elemento */
		      Real orientation(UInt elemId);
		      
		      /*! Metodo che permette di ricavare la stellata del triangolo partendo dalla connettività nodo-elemento
			  \param elemId identificatore dell'elemento 
			  \param ele puntatore al vettore che conterrà gli elementi connessi*/
		      void stellataTetra(UInt elemId, vector<UInt> * ele);
		      
		      /*! Metodo che permette di ricavare il tetraedro opposto a una faccia di un tetraedro 
			  \param id1 identificatore di un nodo
			  \param id2 identificatore di un altro punto
			  \param id3 identificatore di un altro punto
			  \param elemId identificatore di un elemento*/
		      UInt oppositeTetra(UInt id1, UInt id2, UInt id3, UInt elemId);
		      
		      /*! Metodo per stabilire se due triangoli hanno lo stesso geoId 
		          \param elemId1 identificatore di un triangolo 
			  \param elemId2 identificatore di un triangolo */
		      bool sameGeoId(UInt elemId1, UInt elemId2);
		      
		      /*! Metodo che partendo da due tetraedri dice quale è la faccia o l'edge in comune  
			  \param elemId1 identificatore di un triangolo
			  \param elemId2 identificatore di un triangolo
			  \param ids vettore che conterrà gli estremi dell'edge o della faccia */
		      void commonFace(UInt elemId1, UInt elemId2, vector<UInt> * ids);      
		      
		      /*! Metodo che stabilisce se c'è un salto per il geoId dei tetrahedri che condividono una faccia con 
			  quello dato in input
			  \param elemId identificatore dell'elemento da analizzare*/
		      bool jumpGeoIdTetra(UInt elemId);
      //
      // Metodi per fare alcuni calcoli sui punti della mesh
      //
      public:
		      /*! Metodo che calcola l'angolo che ha un vertice di un triangolo
		          \param nodeId identificatore del nodo
			  \param elemId identificatore dell'elemento */
		      Real angolo(UInt nodeId, UInt elemId);
      //
      // Metodi per fare alcuni calcoli sui triangoli della mesh
      //
      public:	      	      
		      /*! Metodo che permette di ricavare il colume di un tetraedro
			  \param elemId identificatore dell'elemento */
		      Real getTetraVolume(UInt elemId);
		      
		      /*! Metodo che permette di ricavare il colume di un tetraedro
			  \param elemId identificatore dell'elemento */
		      Real getTetraVolumeOriented(UInt elemId);
		      
		      /*! Metodo che permette di ricavare il perimetro di un tetraedro
			  \param elemId identificatore dell'elemento */
		      Real getTetraPerimeter(UInt elemId);
		      
		      /*! Metodo che riempie il vettore in input con gli id dei lati dell'elemento elemId
			  \param elemId identificatore dell'elemenento 
			  \param edge puntatore al vettore contenente gli edge */
		      void getEdges(UInt elemId, vector<vector<UInt>  > * edges);
		      
		      /*! Metodo che restituisce una coppia che contiene la lunghezza di tutti gli edge e gli estremi degli edge 
		          tale coppia è ordianta dall'edge più corto a quello più lungo 
			  \param elemId id dell'elemento preso in esame*/
		      pair<vector<Real>, vector<vector<UInt> > > getMinEdge(UInt elemId);
		      
		      /*! Metodo che restituisce una coppia che contiene la lunghezza di tutti gli edge e gli estremi degli edge 
		          tale coppia è ordianta dall'edge più lungo a quello più corto
			  \param elemId id dell'elemento preso in esame
			  N.B. restituisce la lunghezza massima */
		      pair<vector<Real>, vector<vector<UInt> > > getMaxEdge(UInt elemId);
};

}

#endif
