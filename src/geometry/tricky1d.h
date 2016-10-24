#ifndef TRICKY1D_H_
#define TRICKY1D_H_

#include <algorithm>
#include <numeric>
#include <cassert>	
#include <iostream>
#include <vector>
#include <set>

#include "../core/shapes.hpp"
#include "../core/point.h"
#include "../core/graphItem.h"

#include "../geometry/geoElement.hpp"
#include "../geometry/mesh0d.hpp"
#include "../geometry/mesh1d.hpp"
#include "../geometry/connect1d.hpp"

namespace geometry
{

using namespace std;

/*! Classe che permette di esplorare la mesh e calcolare determinate quantità. Il fatto interessante che riesce a ricavare queste
    informazioni solamente parendo dalla classe mesh1d e dalla connettività nodo-elemento */
  
template<typename GEOSHAPE> class tricky1d
{
};

/*! Specializzazione per le mesh di linee */

template<> class tricky1d<Line>
{
      //
      // Variabili contenute nella classe
      //	
      public:
		/*! Puntatore alla mesh2d che si vuole modificare */
		mesh1d<Line> *    	    meshPointer;
		      
		/*! Puntatore alla classe che definisce le connessioni della mesh2d presa in esame */
		connect1d<Line>	                   conn;
		      
		/*! Set che contiene gli elementi di bordo */
		set<UInt>		          bordo;
		      
		/*! Tolleranza */
		Real			           toll;
		      
		/*! Punto Nullo */
		point 			          pNull;
      //
      // Costruttori
      //
      public:
		/*! Costruttore vuoto */
		tricky1d();
		      
		/*! Costruttore con definizione dei due puntatori
		    \param _meshPointer puntatore alla mesh*/
		tricky1d(mesh1d<Line> * _meshPointer);
		
      //
      // Set delle variabili di ambiente
      //
      public:
		/*! Set del puntatore alla mesh e alla struttura per fare le connessioni 
		    \param _meshPointer puntatore alla mesh*/
		void setMeshPointer(mesh1d<Line> * _meshPointer);
		
		/*! Set della tolleranza 
		    \param _toll tolleranza*/
		void setToll(Real _toll);
	      
		/*! Get della tolleranza */
		Real getToll();
		      
		/*! Metodo che sistema le variabili puntate dalla classe per sistemare il dottore prima di tutto crea le 
		    connessioni che servono nella classe connect1d poi passa le informazioni del bordo */
		void setUp();
      //
      // Metodi per analizzare la mesh 
      //
      public:
		/*! Metodo per capire se un nodo è degenere 
		    \param nodeId identificatore di un punto */
		bool isNodeDegenerate(UInt nodeId);
		
		/*! Metodo che setta degenere un nodo 
		    \param nodeId identificatore del punto*/
		void setNodeDegenerate(UInt nodeId);
		
		/*! Metodo che stabilisce se un triangolo ha tutti gli id distinti
		    \param elemId identificatore dell'elemento */
		bool isLineDegenerate(UInt elemId);
		      
		/*! Metodo che setta un triangolo degenere
		    \param elemId identificatore dell'elemento */
		void setLineDegenerate(UInt elemId);	      
      //
      // Metodi per andare lungo una direzione specifica
      //
      public:
		/*! Metodo che partendo da un nodo va nella direzione del suo connesso e riempie un vettore con tutti gli 
		    elementi lineari che ha toccato nel tragitto
		    \param start id del punto di paratenza
		    \param direzione id del punto connesso con start lungo cui andare
		    \param nodi puntatore a un vettore che conterrà la lista dei nodi esplorati */
		void goOn(UInt start, UInt direzione, vector<UInt> * nodi);

		/*! Metodo che partendo da un nodo va nella direzione del suo connesso e riempie un vettore con tutti gli 
		    elementi lineari che ha toccato nel tragitto
		    \param start id del punto di paratenza
		    \param direzione id del punto connesso con start lungo cui andare
		    \param end id del punto che fa terminare la corsa
		    \param nodi puntatore a un vettore che conterrà la lista dei nodi esplorati */
		void goOn(UInt start, UInt direzione, UInt end, vector<UInt> * nodi);
		
		/*! Metodo che a partire da un nodo interno a una linea restituisce tutti gli id degli elementi che sono su quella
		    stessa linea
		    \param start id del punto di paratenza
		    \param nodi puntatore a un vettore che conterrà la lista dei nodi esplorati */
		void goOn(UInt start, vector<UInt> * nodi);
		
		/*! Metodo per sistemare una lista di nodi che identificano uno spigolo di una mesh1d, per convezione faremo in 
		    modo che:
		    1) ogni nodo è legato al successivo
		    2) nel caso in cui la linea sia un loop il primo nodo della lista coincide con l'ultimo
		    3) il primo id della lista sia minore dell'ultimo della lista, nel caso di loop il primo è minore rispetto 
		       a tutti i nodi del loop ed è ripetuto alla fine
		    \param nodi puntatore al vettore che si vuole sistemare */
		void setUpList(vector<UInt> * nodi);
		
		/*! Questo metodo permette di orientare una successione che rappresenta il verso di percorrenza dei nodi di una 
		    mesh1d nello stesso verso della successione di nodi che rappresenta una sua rappresentazione più lasca 
		    \param correct successione di id che identificano il verso di percorrenza di una mesh poco fine
		    \param toTest successione di id che identificano il verso di percorrenza della stessa mesh più fine */
		void sameDirection(vector<UInt> * correct, vector<UInt> * toTest);
		
		/*! Metodo che sistema l'orientazione degli elementi puntati dalla classe 
		    \param nodi puntatore alla lista dei nodi orinetati nel modo che si vuole */
		void setUpMeshOrientation(vector<UInt> * nodi);
      //
      // Metodi che permettono di avere delle inoformazioni topologiche sui nodi 
      //
      public:	
		/*! Metodo che partendo da due nodi dice quale è il segmento che condividono 
		    \param id1 identificare del punto
		    \param id2 identificare dell'altro punto */
		UInt getElementBetween(UInt id1, UInt id2);
		
		/*! Metodo che permette di avere tutti gli elementi connessi a id1
		    \param id1 identificatore del nodo */
		UInt getNumElementAround(UInt id1);
		
		/*! Metodo che riempie il vettore ids con tutti i nodi connessi a id1 
		    \param id1 id di un nodo 
		    \param ids puntatore al vettore che verrà riempito */
		void createStellata(UInt id1, vector<UInt> * ids);
		
		/*! Metodo che riempie il vettore ids con tutti i nodi connessi a id1 con cui forma un elemento di brodo  
		    \param id1 id di un nodo 
		    \param ids puntatore al vettore che verrà riempito */
		void createStellataBor(UInt id1, vector<UInt> * ids);
		
      //
      // Metodi che permettono di valutare determinate quantità
      //
      public:
		/*! Metodo che restituisce la lunghezza di un segmento 
		    \param id1 identificatore del primo punto
		    \param id2 identificare del secondo
		    N.B. questo metodo effettua un controllo per vedere se i due nodi condividono un elemento */
		Real getElementLenght(UInt id1, UInt id2);
	      
		/*! Metodo che calcola la lunghezza nella norma definita dalla classe point della spezzata puntata dalla classe */
		Real getMeshLenght();
		
		/*! Metodo che calcola la lunghezza di una sequenza di nodi connessi
		    \param ids vettore che indica i connessi (si presuppone che ogni nodo sia connesso con il successivo */
		Real getMeshLenght(vector<UInt> * ids);
		
		/*! Metodo che permette di trovare l'area racchiusa nella mesh 1d
		    \param ids vettore che contiene la successione degli id che percorrono la mesh1d 
		    N.B. questo è stato implementato esclusivamente per mesh1d che giaciono in un piano e che formano una linea
		         chiusa*/
		Real getArea(vector<UInt> * ids);
};




}





#endif
