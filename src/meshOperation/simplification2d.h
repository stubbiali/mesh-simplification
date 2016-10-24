#ifndef SIMPLIFICATION2D_H_
#define SIMPLIFICATION2D_H_

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

#include "../utility/sortList.hpp"

#include "../geometry/geoElement.hpp"
#include "../geometry/geoElementSize.hpp"
#include "../geometry/mesh2d.hpp"

#include "../doctor/doctor2d.h"

#include "../meshOperation/isotropicQuality2d.h"

#include "../file/createFile.h"

namespace geometry
{

using namespace std;

/*! Classe che permette di effetuare il processo di eliminazione degli elementi di una griglia */

template<typename GEOSHAPE> class simplification2d : public doctor2d<GEOSHAPE>
{
};

/*! Specializzazione per gli elementi di forma triangolare. Tale processo è stato preso dall'articolo "Surface Simplification
Using Quadratic Error Matrics" di Michael Garland e Paul S.Heckbert*/

template<> class simplification2d<Triangle> : public doctor2d<Triangle>
{
      //
      // Variabili di classe 
      //
      public:
		  /*! vettore con le matrici */
		  vector<vector<Real> > 		Q;     
		  
		  /*! Vettore che tiene traccia */
		  vector<graphItem>             trackList;
      //
      // Costruttori
      //
      public:
		  /*! Costruttore */
		  simplification2d();
		  
		  /*! Costruttore con definizione dei due puntatori
		    \param _meshPointer puntatore alla mesh*/
		  simplification2d(mesh2d<Triangle> * _meshPointer);
		  
		  /*! Metodo che cambia il puntatore della mesh
		    \param _meshPointer puntatore alla mesh*/
		  void setMeshPointer(mesh2d<Triangle> * _meshPointer);
		  
		  /*! Metodo che fa il refresh delle variabili dopo il processo di collasso
		      N.B. fa le stesse cose del refresh della classe doctor2d ma tiene conto anche di track */
		  void refresh();
		  
      //
      // Processo che crea la lista e i suoi elementi  
      //
      public:
		  /*! Metodo che costruisce la matrice K_p 
		      \param nodeId identificatore del nodo
		      \param elemId identificatore dell'elemento */
		  vector<Real> createK_p(UInt nodeId, UInt elemId);
		  
		  /*! Metodo che costruisce la matrice Q 
		      \param nodeId identificatore del nodo*/
		  vector<Real> createQ(UInt nodeId);
		  
		  /*! Metodo che riempie i vettore delle Q della classe */
		  void setUpQ();		  
      //
      // Metodo per trovare i punti e per definire le matrici e il costo degli edge 
      //
      public:
		  /*! Metodo che costruisce la matrice Q 
		      \param edge puntatore all'edge */
		  vector<Real> createQ(vector<UInt> * edge);
		  
		  /*! Metodo che crea l'intera lista dei nodi da testare, il metodo tiene conto sia di problemi legati alla 
		      inversione di triangoli, del fatto che non si possa trovare l'ottimale con il metodo "createPointFromMatrix" 
		      e del fatto che gli estremi possono essere di bordo 
		      \param edge puntatore al vettore che identifica i punti 
		      \param list puntatore a un vettore di punti */
		  void createPointList(vector<UInt> * edge, vector<point> * newNodes);
		  
		  /*! Metodo che da il costo di un edge per il collasso 
		      \param Q matrice realtiva all'edge
		      \param pNew posizione del nuovo nodo */
		  Real getEdgeCost(vector<Real> * Q, point pNew);
		  
		  /*! Metodo che restituisce una coppia con il punto dove verrà collassato l'edge e la sua matrice Q partendo 
		      dall'edge restituisce la nuova posizione che ha il minor costo
		      \param edge puntatore all'edge*/
 		  pair<point, vector<Real> > getEdgeCost(vector<UInt> * edge);
		  
		  /*! Metodo che trova per ogni elemento quale è l'edge con il minore costo 
		      \param elemId identificatore dell'elemento 
		      \param edge puntatore a un vettore che conterrà l'edge
		      Il metodo restituisce una coppia con vertice e matrice Q*/
		  pair<point, vector<Real> > getMinEdgeCost(UInt elemId, vector<UInt> * edge);
	//
	// Metodo che creano la lista
	//
	public:		  
		  /*! Metodo che crea la lista 
		      \param lista puntatore a un contenitore vector che contiene gli elementi */
		  void createElementList(sortList<geoElementSize<Triangle> > * lista);
		  
		  /*! Metodo che elimina dalla lista gli elementi coinvolti nel collasso dell'edge  
		      \param listaBase lista da cui si deve partire con gli elementi 
		      \param edge edge che è stato eliminato
		      \param toUpDate lista degli elementi da ridefinire*/
		  void deleteElementList(sortList<geoElementSize<Triangle> > * listaBase, vector<UInt> * edge, 
					 vector<UInt> * toUpDate);
		  
		  /*! Metodo che aggiorna gli elementi nella lista vicini 
		      \param listaBase lista in cui si devono aggiornare gli elementi 
		      \param vicini lista degli elementi da ridefinire*/
		  void upDate(sortList<geoElementSize<Triangle> > * listaBase, vector<UInt> * vicini);
		  
		  /*! Metodo che permette di ottenere l'elemento ottimale da collassare 
		      \param listaBase lista degli elementi ordinati*/
		  UInt getElementToSimplificate(sortList<geoElementSize<Triangle> > * listaBase);
	//
	// Metodi che fanno i controlli
	//
	public:
		 /*! Metodo che controlla se il collasso è corretto, oltre a utilizzare il controllo del dottore controlla che 
		     gli edge dopo abbiano sempre due triangoli 
		     \param edge vettore con gli id dei nodi 
		     \param pNew nuovo punto */
		 bool control(vector<UInt> * edge, point pNew);
	
	//
	// Metodi che fanno la semplificazione 
	//
	public:
		  /*! Metodo che spezza gli edge finché non raggiunge un numero di nodi obiettivo dato in input
		      \param numNodesMax numero massimo di nodi */
		  void simplificate(UInt numNodesMax);
		  
		  /*! Metodo che spezza gli edge finché non raggiunge un numero di nodi obiettivo dato in input questo 
		      processo è fatto partendo sempre dall'elemento meno costoso 
		      \param numNodesMax numero massimo di nodi 
		      N.B. spesso può accadere che tale metodo non arrivi al risultato sperato perché i triangoli diventano 
			   degeneri */
		  void simplificateGreedy(UInt numNodesMax);
		  
		  /*! Metodo che spezza gli edge finché non raggiunge un numero di nodi obiettivo dato in input questo 
		      processo è fatto partendo sempre dall'elemento meno costoso 
		      \param numNodesMax numero massimo di nodi 
		      N.B. a differenza di quello prima si limita a collassare ogni edge senza utilizzare il controllo
			   implementato nella classe*/
		  void simplificateGreedyEasy(UInt numNodesMax);
		  
		  /*! Metodo che effettua il collasso a cascata, cerca di ragguingere l'obiettivo di numNodesMax eliminando 
		      di volta in volta step nodi in questi step non aggiorna le strutture dati 
		      \param numNodesMax numero massimo di nodi 
		      \param step quanti collassarne di fila */
		  void simplificateStep(UInt numNodesMax, UInt step);
	//
	// Metodi che stampano 
	//
	public:
		  /*! Metodo che crea un file di paraview che permette di visualizzare quale è il costo di collassare un
		      elemento 
		      \param s stringa che dice il nome del file */
		  void print(string s);
		  
};

}


#endif


