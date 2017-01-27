#ifndef MESHDATASIMPLIFICATION_H_
#define MESHDATASIMPLIFICATION_H_

#include <algorithm>
#include <cassert>	
#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <numeric>
#include <limits>

#include "../core/shapes.hpp"
#include "../core/point.h"
#include "../core/graphItem.h"

#include "../utility/sortList.hpp"
#include "../utility/barCoordinates.h"

#include "../geometry/geoElement.hpp"
#include "../geometry/geoElementSize.hpp"
#include "../geometry/meshSearch.hpp"
#include "../geometry/mesh2d.hpp"

#include "../doctor/doctor2d.h"

#include "../meshOperation/isotropicQuality2d.h"
#include "../meshOperation/simplification2d.h"
#include "../meshOperation/mementoElement.hpp"

#include "../intersec/triangleIntersection.h"
#include "../intersec/meshIntersec.hpp"

/*
#include "Epetra_ConfigDefs.h"
#include "Epetra_SerialComm.h"
#include "Epetra_Vector.h"
#include "Epetra_SerialDenseMatrix.h"
#include "Epetra_SerialDenseVector.h"
#include "Epetra_SerialDenseSolver.h"
#include "Epetra_LinearProblem.h"
*/

#include "../file/createFile.h"

namespace geometry
{

using namespace std;

/*! Classe che permette di effetuare il processo di eliminazione degli elementi di una griglia che però tiene traccia dei nodi 
    di partenza */

template<typename GEOSHAPE> class meshDataSimplification : public simplification2d<GEOSHAPE>
{
};

/*! Specializzazione per gli elementi di forma triangolare */

template<> class meshDataSimplification<Triangle> : public simplification2d<Triangle>
{
	//
	// Variabili di classe
	//
	public:
		  /*! vettore che contiene le associazioni */
		  vector<mementoElement<Triangle> >	  	  mem;
		  
		  /*! Variabile che permette di settare il numero di iterazioni massime */
		  Real 					        times;
		  
		  /*! Variabile che contiene il valor medio delle associazioni */
		  Real 		 	    		meanAssValue;
		  
		  /*! Variabili che contengono il valore massimo dei costi*/
		  Real 	            maxGeoCost,maxAssCost,maxDistCost;
		  
		  /*! Variabili che contengono il valore il peso */
		  Real 	                     pesoGeo,pesoAss,pesoDist;
		  
		  /*! Variabili per la gestione della mesh */
		  Real 			     numGlobElem,numGlobPtAss;
		  
		  /*! Variabile interna che permette di sapere se devo aggiornare la lista o no */
		  bool 					 listToUpdate;
		  
		  
		  /*! Variabile che setta un triangolo che non deve essere modificato */
		  bool 				            dontTouch;
		  UInt 				        elemDontTouch;
		  UInt				   colorElemDontTouch;
		  
		  /*! vettore che contiene la struttura di ricerca */
		  meshSearchStructured<mesh2d<Triangle>,3>     finder;
		  
		  /*! vettore con i punti da proiettare */
		  vector<point>				      toTrack;
		  
		  /*! lista con l'elenco degli elementi da eliminare */
		  sortList<geoElementSize<Triangle> >     sortedList;
		  
		  /*! mappa che fa le associazioni */
		  map<UInt, vector<UInt> >		elemIdToEdge;
		  map<UInt, point>		       elemIdToPoint;
		  
		  /*! metodo che permette di fare le intersezioni con il triangolo */
		  triangleIntersection 		             intersec;
		  
		  /*! Metodo per calcolare le coordinate baricentriche */
		  inTriangle			               inTria;
		  
      //
      // Costruttori
      //
      public:
		  /*! Costruttore */
		  meshDataSimplification();
		  
		  /*! Metodo che cambia il puntatore della mesh
		    \param _meshPointer puntatore alla mesh*/
		  void setMeshPointer(mesh2d<Triangle> * _meshPointer);
		  
		  /*! Metodo che fa il refresh delle variabili dopo il processo di collasso
		      N.B. fa le stesse cose del refresh della classe doctor2d ma tiene conto anche di track */
		  void refresh();
		  
      //
      // Setting e getting delle variabili 
      //
      public:
		  /*! Metodo che setta la tolleranza 
		      \param _toll tolleranza */
		  void setToll(Real _toll);
		  inline Real getToll()             {return(toll);};
		  
		  /*! get e set della variabile times
		      \param _times valore della variabile */
		  inline void setTimes(Real _times) {times=_times;};
		  inline Real getTimes()	    {return(times);};
		  
		  /*! get e set della varaibile dontTouch*/
		  inline void activeDontTouch()	    {dontTouch=true;};
		  inline void disactiveDontTouch()  {dontTouch=false;};
		  inline bool getDontTouch()	    {return(dontTouch);};
		  
		  /*! get e set della variabile elemDontTouch */
		  inline void setElemDontTouch(UInt _elemDontTouch)  {elemDontTouch=_elemDontTouch;};
		  inline UInt getElemDontTouch()		     {return(elemDontTouch);};
		  
		  /*! get e set della variabile che colora l'elemento da non toccare */
		  inline UInt getColorElemDontTouch() 		     		{return(colorElemDontTouch);};
		  inline void setColorElemDontTouch(UInt _colorElemDontTouch) 	{colorElemDontTouch=_colorElemDontTouch;};
		  
		  /*! Metodo che setta la variabile fatt a tutti gli elementi di memento */
		  void setFattMemento(Real _fatt);
		  Real getFattMemento();
		  
		  /*! set della variabile di classe che tiene memoria dei punti 
		      \param _toTrack vettore che contiene i punti da tracciare */
		  void setPointToTrack(vector<point>  * _toTrack);
		  
		  /*! Metodo che calcola il valor medio delle associazioni */
		  Real getMeanElemAss();
		  inline void upDateMeanAssValue() {meanAssValue = getMeanElemAss();};
		  
		  /*! Metodo per set i pesi */
		  inline void setPesoGeo(Real _pesoGeo)   {pesoGeo  = _pesoGeo;};
		  inline void setPesoAss(Real _pesoAss)   {pesoAss  = _pesoAss;};
		  inline void setPesoDist(Real _pesoDist) {pesoDist = _pesoDist;};
      //
      // Metodi per il calcolo dello score relativo alla lunghezza del lato 
      //
      public:  
		  /*! Metodo per ottenere l'informazione dell'edge 
		      \param edge puntatore a un vettore che identifica l'edge 
		      \param pNew nuova locazione del punto */
		  Real getEdgeMetricCost(vector<UInt> * edge, point pNew);
		  
		  /*! Metodo che permette di ottenere l'edge del triangolo elemId con il minore costo geometrico 
		      \param elemId identificatore dell'elemento 
		      \param edges lista degli edge che possono essere collassati 
		      \param nodi lista dei punti per il collasso dell'edge 
		      \param cost lista dei costi degli edge 
		  Il metodo ritorna una coppia con costo ed vettore che identifica l'edge */
		  void getElemEdgeMetricCost(UInt elemId, 
					     vector<vector<UInt> > * edges, 
					     vector<point> * nodi, 
					     vector<Real> * cost);
					     
		  /*! Metodo per il refresh della lista */
		  void refreshList();
      //
      // Metodi per il calcolo dello score relativo allo score delle associazioni
      //
      public:
		  /*! Metodo che permette di creare una lista di memento element che saranno generati al seguito di un collasso 
		      di un edge 
		      \param edge edge da collassare 
		      \param pNew nuovo punto 
		      \param newMem nuova lista di memento elements*/
		  void changeMemento(vector<UInt> * edge, 
				     point pNew, 
				     vector<mementoElement<Triangle> > * newMem);
				     
		  /*! Metodo che crea una mesh temporanea per controllare le associazioni 
		      \param newMem nuova lista di memento elements
		      \param pNew nuovo punto inserito 
		      \param tmpMesh mesh temporanea che viene creata
		      \param newToOld mappa per passare dalla nuova alla vecchia 
		      \param newToOld mappa per passare dalla vecchia alla nuova*/
		  void buildTmpMesh(vector<mementoElement<Triangle> > * newMem, 
				    point pNew, 
				    mesh2d<Triangle> * tmpMesh,
	  		            map<UInt,UInt> & newToOld,
	  		            map<UInt,UInt> & oldToNew);
				    
				     
		  /*! Metodo che associa un valore al nodo 
		      \param elemId identificatore dell'elemento
		      \param nodiTria lista dei vertici 
		      \param pt vetice da calcolare */
		  Real evaluateWeight(UInt elemId, vector<point> * nodiTria, point pt);
		  
		  /*! Metodo per calcolare lo score di un triangolo 
		      \param newMem nuova lista di memento elements*/
		  Real getElemAssCost(vector<mementoElement<Triangle> > * newMem);
      
      //
      // Metodi per il calcolo dello score relativo allo score della distanza 
      //
      public:
		  /*! Metodo che calcola le distanze per le associazioni 
		      \param newMem nuova lista di memento elements*/
		  Real getElemDistCost(vector<mementoElement<Triangle> > * newMem);
      
      //
      // Metodi che creano le liste 
      //
      public:
		  /*! Metodo che trova in automatico l'elemento da non toccare */
		  void findElemDontTouch();
		  
		  /*! Metodo che aggiorna la variabile che tiene conto se si debba o meno aggiornare le liste 
		      \param costo puntatore al vettore che contiene i costi */
		  void controlUpdatingList(vector<Real> * costi);
		  
		  /*! Metodo che crea la lista degli elementi da eliminare */
		  void createElementList();
		  
		  /*! Metodo che elimina gli elementi che insistono sull'edge 
		      \param edge edge di input
		      \param toUpdate lista di quelli che dovranno essere aggiornati */
		  void deleteElementList(vector<UInt> * edge, vector<UInt> * toUpDate);
		  
		  /*! Metodo che aggiorna la lista nelle locazioni date in input 
		      \param vicini vettore con gli identificatori degli elementi da aggiornare */
		  void upDate(vector<UInt> * vicini);
		  
		  /*! Metodo che aggiorna la lista memento 
		      \param newMem nuovi elementi da inserire */
		  void upDateMemento(vector<mementoElement<Triangle> > * newMem);
		  
		  /*! Metodo che aggiorna la struttura di ricerca se viene collassato un lato 
		      \param edge edge di input
		      \param pNew nuovo punto inserito */
		  void upDateFinder(vector<UInt> * edge, point pNew);
      //
      // Processi che effettuano i controlli geometrici 
      //
      public:
		  /*! Metodo che fa un controllo delle intersezioni con la mesh 
		     \param edge vettore con gli id dei nodi 
		     \param pNew nuovo punto */
		  bool controlInt(vector<UInt> * edge, point pNew);
		  
		  /*! Metodo che fa un controllo per vedere se l'edge influenzerà l'elemento da non toccare 
		      \param edge edge che si vorrebbe collassare */
		  bool controlDontTouch(vector<UInt> * edge);
		  
		  /*! Metodo che controllo che non ci siano triangoli invertiti 
		      \param edge vettore con gli id dei nodi 
		      \param pNew nuovo punto */
		  bool controlInv(vector<UInt> * edge, point pNew);
		  
		  /*! Metodo che permette di capire se la mesh puntata dalla classe si interseca con se stessa */
		  void controlAutoIntersection();
      
      //
      // Processo di semplificazione
      //
      public:
		  /*! Metodo per trovare gli edge da collassare 
		      \param edge edge da semplificare 
		      il metodo restitutisce il punto */
		  point getEdgeToSimplificate(vector<UInt> * edge);
		  
		  /*! Metodo che valuta il peso di un elemento 
		      \param elemId identificatore delll'elemento 
		      \param costi puntatore a un vettore che contiene i costi separati, [0] geometrico, [1] associazioni
		      [2] distanza 
		      \param edge edge a cui è associato il costo 
		      \param p punto con le cordinate del punto 
		      N.B. ritorna il valore totale */
		  Real getElementCost(UInt elemId, vector<Real> * costi, vector<UInt> * edge, point * p);
		  
		  /*! Metodo che fa il processo di semplificazione 
		      \param numNodesMax numero di nodi massimo 
		      \param elemDontTouch identificatore dell'elemento da non toccare*/
		  void simplificationProcess(UInt numNodesMax, 
					     UInt _elemDontTouch=numeric_limits<UInt>::max());
		  
		  /*! Metodo che elimina i triangoli 
		      \param numNodesMax numero di nodi massimo */
		  void simplificate(UInt numNodesMax);
      //
      // Metodi di stampa 
      //
      public:
		  /*! Metodo che crea quattro file con le varie componenti della funzione di costo 
		      \param s stringa */
		  void printElementCost(string s);
		  
		  /*! Metodo che stampa il costo in termini geometrici 
		      \param s stringa */
		  void printEdgeMetricCost(string s);
		  
		  /*! Metodo che stampa il costo per quanto riguarda le associazioni
		      \param s stringa */
		  void printAssCost(string s);
		  
		  /*! Metodo che stampa il costo per quanto riguarda le distanze
		      \param s stringa */
		  void printDistCost(string s);
		  
		  /*! Metodo che salva i dati 
		      \param s stringa con il nome del file */
		  void saveAss(string s);
		  
		  /*! Metodo che salva i dati 
		      \param s stringa con il nome del file per salvare il mapping 
		      \param link stringa con il nome del file per salvare i link, gli elementi hanno salvata la loro lunghezza*/
		  void saveAss(string s, string link);
		  
		  /*! Metodo che permette di analizzare gli angoli 
		      \param s stringa con il nome del file */
		  void printAngle(string s);
		  
      
};










}

#endif
