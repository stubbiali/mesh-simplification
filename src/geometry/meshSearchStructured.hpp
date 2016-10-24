#ifndef MESHSEARCHSTRUCTURED_HPP_
#define MESHSEARCHSTRUCTURED_HPP_

#include <cassert>
#include <ctime>
#include <iostream>
#include <vector>
#include <cmath>

#include "../core/shapes.hpp"
#include "../core/point.h"
#include "../core/graphItem.h"

#include "../utility/inSegment.h"
#include "../utility/inTriangle.h"

#include "../geometry/geoElement.hpp"
#include "../geometry/mesh1d.hpp"
#include "../geometry/mesh2d.hpp"
#include "../geometry/mesh3d.hpp"

#include "../file/createFile.h"

namespace geometry
{
  
using namespace std;

/*! 
    Questa classe implementa le funzionalità findamentali per la creazione di una strittura dati per effettuare le ricerche in
    modo veloce. Utilizza una struttura dati di tipo bin tree in cui ogni elemento viene messo in un segmento,quadrato o cubo in
    una griglia strutturata. È possibile usare questa struttura dati SOLAMENTE per mesh1d, mesh2d e mesh3d.

*/

// TODO pensa a quelle anisotrope
  
template<class MESH, UInt DIM=3> class meshSearchStructured
{
	//
	// Variabili
	// 
	public:
		/*! Tolleranza*/
		Real                        toll;
		
		/*! Struttura che conterrà gli elementi presenti in una cella*/
		vector<vector<UInt> >       grid;
		
		/*! Puntatore alla mesh */
		MESH		*    meshPointer;
		
		/*! coordinate del bounding box della mesh */
		point                  pMax,pMin;
		
		/*! punto che contiene "il passo" delle celle*/
		point                         H;
		
		/*! punto che contiene le dimensioni massime delle celle */
		point                      hMax;
		
		/*! numero di divisioni lungo le 3 coordinate */
		UInt                   div[DIM];
		
		/*! numero di elementi */
		UInt                     number;
		
		/*! Classi che permettono di fare i test di appartenenza */
		inSegment		  inSeg;
		inTriangle		 inTria;
	//
	// Costruttuore
	//
	public:
		/*! Costruttore di default */
		meshSearchStructured();
		
		/*! Pulizia delle variabili */
		void clear();
		
	//
	// Set/Get delle variabili
	//
	public:		
		/*! Set di tolleranza
		    \param _toll valore della tolleranza*/
		void setToll(Real _toll=1e-15);
		
		/*! get di tolleranza*/
		inline Real getToll();
		
		/*! get dello spacing della griglia */
		inline point getSpacing();
		
		/*! get del punto più piccolo della griglia di ricerca */
		inline point getMin();
		
		/*! get del punto più grande della griglia di ricerca */
		inline point getMax();
		
		/*! get delle divisioni lungo x, y e z*/
		inline UInt getDiv(UInt i);
		
		/*! set del puntatore alla mesh 
		    \param _meshPointer puntatore alla mesh */
		inline void setMeshPointer(MESH * _meshPointer);
		
		/*! get del puntatore alla mesh */
		inline MESH * getMeshPointer();
	//
	// Creo la struttura dati per la ricerca
	//
	public:
		/*! Metodo per creare la struttura dati */
		void buildDataStructure();
		
		/*! Metodo che partendo da delle coordinate trova le coordinate nel vettore grid 
		    \param P coordinate del punto da cercare
		    \param coor puntatore a un vettore che conterrà 
				coo[0] = la coordinata nel vettore grid
				coo[1] = la coordinata lungo asse X
				coo[2] = la coordinata lungo asse Y
				coo[3] = la coordinata lungo asse Z
				coo[4] = 1 o 0 se ha trovato o meno il punto all'interno del grid */
		void getPointToGridCoor(point P, vector<UInt> * coor);
		
		/*! Metodo che permette di trovare l'inseme di elementi della mesh vicini al punto dato in input
		    \param P coordinate del punto 
		    \param raggio ampiezza della ricerca
		    \param neight puntatore al vettore che sarà riempito degli id degli elementi che sono vicini a P
		    La fuzione ritorna vero o falso se trova o meno questi elementi*/
		bool getElementAroundPoint(point P, UInt raggio, vector<UInt> * neight);
		
		/*! Metodo che permette di esplorare la grid partendo da un nodo e lungo una direzione restituisce vero se
		    all'interno del cubo in cui si è fermato il punto sono presenti elementi
		    \param P punto da cui si parte
		    \param dir direzione che si deve percorrere lungo la griglia 
		    \param passo numero di passi che si devono fare lungo questa direzione
		    \param coor vettore che verrà riempito con le coordinate del grid
		    */
		bool moveAroundGrid(point P, point dir, UInt passo, vector<UInt> * coor);
	//
	// Metodi per trovare l'intersezione 
	//
	public:	
		/*! Metodo per i punti all'interno di un bbox
		      \param boxMax punto massimo del box
		      \param boxMin punto minimo del box
		Il metodo ritorna una coppia che contiene un booleano che dice se ha trovato elementi e un vettore con quali 
		elementi*/
		pair<bool, vector<UInt> > findIntersection(point boxMax, point boxMin);
		
		/*! Metodo che serve per stabilire se un punto è dentro alla mesh o comunque a un patch
		      \param node punto da testare
		Il metodo ritorna una coppia che contiene un booleano che dice se ha trovato elementi e un vettore con quali 
		elementi*/
		pair<bool, vector<UInt> > isIn(point node);
	//
	// Metodi per "editare la grid"
	//
	public:		
		/*! Metodo che aggiunge un elemento nella grid a una posizione specifica
		    \param gridId identificatore della posizione di grid in cui aggiungere 
		    \param id identificatore dell'elemento da aggiungere */
		void addToGrid(UInt gridId, UInt id);
		
		/*! Metodo che elimina un elemento nella grid a una posizione specifica
		    \param gridId identificatore della posizione di grid in cui eliminare
		    \param id identificatore dell'elemento da eliminare */
		void removeToGrid(UInt gridId, UInt id);
		
		/*! Metodo che inserisce un elemento nella struttura dati
		      \param elemId identificatore dell'elemento
		      N.B. tale elemento deve essere nella mesh*/
		void insertElement(UInt elemId);
		    
		/*! Metodo che toglie un elemento nella struttura dati
		     \param elemId identificatore dell'elemento
		    N.B. tale elemento deve essere nella mesh*/
		void eraseElement(UInt elemId);
	//
	// Print
	//
	public:
		/*! Metodo che stampa la griglia su un file per paraview usato per il debug 
		    \param s nome del file 
		    \param all boolenao che dice se stampare l'intera struttura (true) o solamente i quadrati che contengono 
			       elementi */
		void print(string s, bool all=true);


};

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

template<class MESH, UInt DIM>
meshSearchStructured<MESH, DIM>::meshSearchStructured()
{
	toll               = 1e-15;
}

template<class MESH, UInt DIM>
void meshSearchStructured<MESH, DIM>::clear()
{
	grid.clear();
	pMax.setX(0.0);	pMax.setY(0.0);	pMax.setZ(0.0);
	pMin.setX(0.0);	pMin.setY(0.0);	pMin.setZ(0.0);
	H.setX(0.0);	H.setY(0.0);	H.setZ(0.0);
	hMax.setX(0.0);	hMax.setY(0.0);	hMax.setZ(0.0);
	for(UInt i=0; i<DIM; ++i)	div[i]=0;
}

//
// Set/Get delle variabili
// 

template<class MESH, UInt DIM>
void meshSearchStructured<MESH, DIM>::setToll(Real _toll)
{
	toll = _toll;
	inSeg.setToll(toll);
	inTria.setToll(toll);
}

template<class MESH, UInt DIM>
inline  Real meshSearchStructured<MESH, DIM>::getToll()
{
	return(toll);
}

template<class MESH, UInt DIM>
inline  point meshSearchStructured<MESH, DIM>::getSpacing()
{
	return(H);
}

template<class MESH, UInt DIM>
inline point meshSearchStructured<MESH, DIM>::getMin()
{
	return(pMin);
}

template<class MESH, UInt DIM>
inline point meshSearchStructured<MESH, DIM>::getMax()
{
	return(pMax);
}

template<class MESH, UInt DIM>
inline UInt meshSearchStructured<MESH, DIM>::getDiv(UInt i)
{
	assert(i<DIM);
	return(div[i]);
}

template<class MESH, UInt DIM>
inline void meshSearchStructured<MESH, DIM>::setMeshPointer(MESH * _meshPointer)
{
    meshPointer = _meshPointer;
    
    // creo la struttura dati dato che è fortemente legata alla mesh 
    buildDataStructure();
}

template<class MESH, UInt DIM>
inline MESH * meshSearchStructured<MESH, DIM>::getMeshPointer()
{
    return(meshPointer);
}


//
// Metodi per la struttura dati di ricerca
//
template<class MESH, UInt DIM>
void meshSearchStructured<MESH, DIM>::buildDataStructure()
{
	// Pulisco eventuali informazioni messe
	clear();

	// Variabili temporanee
	bool         found=false;
	UInt   valTmp=1,fattore=1;
	point                 tmp;
	vector<UInt>         coor;
	vector<point>     bBoxMax;
	vector<point>     bBoxMin;
	
	// controllo che ci siano elementi nella mesh 
	if(meshPointer->getNumElements()==0)
	{
	    cout << "ATTENZIONE: la mesh puntata dalla classe è vuota non posso costruire la struttura di ricerca" << endl;
	    return;
	}
	
	// inizializzo le variabili
	bBoxMax.resize(meshPointer->getNumElements());
	bBoxMin.resize(meshPointer->getNumElements());
	pMax = meshPointer->getNode(meshPointer->getElement(0).getConnectedId(0));
	pMin = meshPointer->getNode(meshPointer->getElement(0).getConnectedId(0));
	
	// Ciclo sugli elementi per trovare il boundigbox e inizializzare le informazioni sulle celle
	for(UInt i=0; i<meshPointer->getNumElements(); ++i)
	{
		// prendo il massimo delle coordinate dei punti per creare il bbox del triangolo 
		meshPointer->createBBox(i, bBoxMax[i], bBoxMin[i]);
		
		// metto a posto il bounding box prima il massimo
		for(UInt j=0; j<DIM; ++j)
		{  
		    // setto il bbOx
		    pMax.setI(j, max(bBoxMax[i].getI(j),pMax.getI(j)));
		    pMin.setI(j, min(bBoxMin[i].getI(j),pMin.getI(j)));
		
		    // setto lo spacing 
		    hMax.setI(j, max(hMax.getI(j),(bBoxMax[i].getI(j)-bBoxMin[i].getI(j))));
		}
	}
	
	
	// amplio il bounding box per problemi legati alle superfici che coincidono con esso
	for(UInt i=0; i<DIM; ++i)	tmp.setI(i, pMax.getI(i)+hMax.getI(i)); 
	pMax = tmp;
	
	for(UInt i=0; i<DIM; ++i)	tmp.setI(i, pMin.getI(i)-hMax.getI(i)); 
	pMin = tmp;
	
	// trovo la spaziatura 
	while(!found)
	{
	    // resetto valTmp
	    valTmp = 1;
	    
	    // conto le divisioni lungo gli assi
	    for(UInt i=0; i<DIM; ++i)
	    {
		// setto le divisioni
		div[i] = static_cast<UInt>(max(static_cast<int>((pMax.getI(i)-pMin.getI(i))/ (hMax.getI(i))),3))/fattore;
	      
		// sett quante celle
		valTmp = valTmp*div[i];
	    }
	    
	    // se sono a posto con la capacità mi fermo 
	    if(valTmp<grid.max_size())	found=true;
	    else			fattore=fattore*2;
	}
	
	
	// conto le divisioni lungo gli assi
	for(UInt i=0; i<DIM; ++i)
	{
	    // setto lo spacing
	    H.setI(i, ((pMax.getI(i)-pMin.getI(i)) / (div[i])));
	}
	
	// faccio un resize
	grid.resize(valTmp);
	
	// resize del grid e coor
	coor.resize(5);
	
	// ciclo sugli elementi per riempire il vettore grid
	for(UInt i=0; i<meshPointer->getNumElements(); ++i)
	{	
		  // ricavo il punto medio della diagonale del bounding box dell'elemento i-esimo
		  tmp.setX(0.0);	tmp.setY(0.0);		tmp.setZ(0.0);
		  tmp.replace(bBoxMax[i],bBoxMin[i],0.5);
		  
		  // ricavo le coordinate
		  getPointToGridCoor(tmp, &coor);
		  
		  // se test è falso vuol dire che il punto non è nella griglia
		  if(coor[4]==0)	cout << "ERRORE: punto non trovato nella griglia" << endl;
		  else			grid[coor[0]].push_back(i);
	}
}

template<class MESH, UInt DIM>
void meshSearchStructured<MESH, DIM>::getPointToGridCoor(point P, vector<UInt> * coor)
{
	// variabili temporanee
	bool 	            inside=true;
	UInt        		id[DIM];
	
	// controllo la dimensione di coor
	coor->clear();
	coor->resize(5);
	
	// inizializzo
	for(UInt i=0; i<DIM; ++i) id[i] = 0;
	
	for(UInt i=0; i<DIM; ++i)
	{
	  // controllo che sia dentro alla coordinata i-esima 
	  if((P.getI(i)<(pMax.getI(i)+toll)) && (P.getI(i)>(pMin.getI(i))-toll)) inside = inside && true;
	  else									 inside = inside && false;
	}
	
	// controllo che il punto P sia dentro il mio BBox
	if(inside)
	{	    
	    // prendo tutti i valori 
	    for(UInt i=0; i<DIM; ++i)	id[i] = static_cast<UInt>(floor(((P.getI(i)-pMin.getI(i))/(H.getI(i)))));
	    
	    // faccio differenti cose in base al parametro dim
	    switch(DIM)
	    {
	      case(1):
		      coor->at(0)= id[0];
		      coor->at(1)= id[0];
		      coor->at(2)= 0;
		      coor->at(3)= 0;
		      coor->at(4)= 1;
		      break;
	      case(2):
		      coor->at(0)= id[0]+id[1]*div[0];
		      coor->at(1)= id[0];
		      coor->at(2)= id[1];
		      coor->at(3)= 0;
		      coor->at(4)= 1;
		      break;
	      case(3):
		      coor->at(0)= id[0]+id[1]*div[0]+id[2]*div[0]*div[1];
		      coor->at(1)= id[0];
		      coor->at(2)= id[1];
		      coor->at(3)= id[2];
		      coor->at(4)= 1;
		      break;
	    }	    
	} 
	else
	{
	    // prendo tutti i valori 
	    for(UInt i=0; i<DIM; ++i)
	    {
	      // controllo che sia dentro alla coordinata i-esima 
	      if((P.getI(i)<(pMax.getI(i)+toll)) && (P.getI(i)>(pMin.getI(i))-toll))
		id[i] = static_cast<UInt>(floor(((P.getI(i)-pMin.getI(i))/(H.getI(i)))));
	      // se è più alta metto il massimo 
	      else if(P.getI(i)>=(pMax.getI(i)+toll))
		id[i] = (div[i]-1);
	      // se è più bassa metto lo zero 
	      else if(P.getI(i)<=(pMin.getI(i)+toll))
		id[i] = 0;
	    }
	    
	    // faccio differenti cose in base al parametro dim
	    switch(DIM)
	    {
	      case(1):
		      coor->at(0)= id[0];
		      coor->at(1)= id[0];
		      coor->at(2)= 0;
		      coor->at(3)= 0;
		      coor->at(4)= 0;
		      break;
	      case(2):
		      coor->at(0)= id[0]+id[1]*div[0];
		      coor->at(1)= id[0];
		      coor->at(2)= id[1];
		      coor->at(3)= 0;
		      coor->at(4)= 0;
		      break;
	      case(3):
		      coor->at(0)= id[0]+id[1]*div[0]+id[2]*div[0]*div[1];
		      coor->at(1)= id[0];
		      coor->at(2)= id[1];
		      coor->at(3)= id[2];
		      coor->at(4)= 0;
		      break;
	    }
	}
}

template<class MESH, UInt DIM>
bool meshSearchStructured<MESH, DIM>::getElementAroundPoint(point P, UInt raggio, vector<UInt> * neight) 
{
	// Variabili temporanee
	vector<UInt>                coor;
	UInt indexMax[DIM],indexMin[DIM];
	vector<UInt>               celle;
	
	// setto le dimensioni di coor e libero la vaciabile neight
	coor.resize(5);
	neight->clear();
	
	// cerco le sue coordinate
	getPointToGridCoor(P,&coor);
	
	// ciclo sulle dimensioni per prendere le dimensioni di ricerca 
	for(UInt i=0; i<DIM; ++i)
	{
	    indexMax[i] = min((static_cast<int>(coor[1+i])+static_cast<int>(raggio)), (static_cast<int>(div[i])-1));
	    indexMin[i] = max((static_cast<int>(coor[1+i])-static_cast<int>(raggio)), 0);
	}
	
	// prendo gli indici delle celle corrispondenti
	switch(DIM)
	{
	      case(1):
		      // faccio un reserve
		      celle.reserve(indexMax[0]-indexMin[0]);
		      
		      // metto i valori 
		      for(UInt i=indexMin[0]; i<=indexMax[0]; ++i)
			celle.push_back(i);
		      break;
	      case(2):
		      // faccio un reserve
		      celle.reserve((indexMax[0]-indexMin[0])*(indexMax[1]-indexMin[1]));
		      
		      // metto i valori 
		      for(UInt i=indexMin[0]; i<=indexMax[0]; ++i)
			for(UInt j=indexMin[1]; j<=indexMax[1]; ++j)
			  celle.push_back(i+j*div[0]);
		      break;
	      case(3):
		      // faccio un reserve
		      celle.reserve((indexMax[0]-indexMin[0])*(indexMax[1]-indexMin[1])*(indexMax[2]-indexMin[2]));
		      
		      // metto i valori 
		      for(UInt i=indexMin[0]; i<=indexMax[0]; ++i)
			  for(UInt j=indexMin[1]; j<=indexMax[1]; ++j)
			    for(UInt k=indexMin[2]; k<=indexMax[2]; ++k)
		  		  celle.push_back(i+j*div[0]+k*div[0]*div[1]);
		      break;
	}
	
	// faccio un reserve 
	neight->reserve(celle.size()*10);
	
	// Per ogni cella che ho trovato
	for(UInt j=0; j<celle.size(); ++j)
	    // ricavo gli elementi intorno a P
	    for(UInt i=0; i<grid[celle[j]].size(); ++i) neight->push_back(grid[celle[j]][i]);
	    
	// se neight è vuoto ritorno falso altrimenti vero
	if(neight->size()==0)     return(false);
	else			  return(true);
}

template<class MESH, UInt DIM>  
bool meshSearchStructured<MESH, DIM>::moveAroundGrid(point P, point dir, UInt passo, vector<UInt> * coor)
{
	// variabili temporanee
	point pNew;
	
	// faccio il resize di coor
	coor->resize(5);
	
	// mi assicuro che la direzione sia normalizzata
	dir = dir / dir.norm2();
	
	// trovo il punto spostandomi lungo la direzione 
	pNew = P + (dir*static_cast<Real>(passo))*fabs(dir*H);
	
	// trovo le coordinate di pNew
	getPointToGridCoor(pNew, coor);
	
	if(grid[coor->at(0)].size()==0)			    return(false);
	else						    return(true);
}

//
// Metodi per trovare l'intersezione 
//
template<class MESH, UInt DIM>
pair<bool, vector<UInt> > meshSearchStructured<MESH, DIM>::findIntersection(point boxMax, point boxMin)
{
	// varaibili in uso 
	pair<bool, vector<UInt> >	result;
	vector<UInt>     coorMin,coorMax,celle;
	UInt 	   indexMax[DIM],indexMin[DIM];
	point	                         p,tmp;
      
	// setto tmp 
	tmp.setX(1.0);	tmp.setY(1.0);	tmp.setZ(1.0);
      
	// do spessore agli elementi che hanno un bbox troppo piccolo
	if((boxMax-boxMin).norm2()<toll)
	{
		p = boxMax;
		boxMax = p + tmp*toll*1000.0;
		boxMin = p - tmp*toll*1000.0;
	}
	
	// prendo le coordinate del minimo e del massimo 
	getPointToGridCoor(boxMin, &coorMin);
	getPointToGridCoor(boxMax, &coorMax);
	
	// ciclo sulle dimensioni per prendere le dimensioni di ricerca 
	for(UInt i=0; i<DIM; ++i)
	{
	    indexMax[i] = min((static_cast<int>(coorMax[1+i])+1), (static_cast<int>(div[i])-1));
	    indexMin[i] = max((static_cast<int>(coorMin[1+i])-1), 0);
	}
	
	// prendo gli indici delle celle corrispondenti
	switch(DIM)
	{
	      case(1):
		      // faccio un reserve
		      celle.reserve(indexMax[0]-indexMin[0]);
		      
		      // metto i valori 
		      for(UInt i=indexMin[0]; i<=indexMax[0]; ++i)
			celle.push_back(i);
		      break;
	      case(2):
		      // faccio un reserve
		      celle.reserve((indexMax[0]-indexMin[0])*(indexMax[1]-indexMin[1]));
		      
		      // metto i valori 
		      for(UInt i=indexMin[0]; i<=indexMax[0]; ++i)
			for(UInt j=indexMin[1]; j<=indexMax[1]; ++j)
			  celle.push_back(i+j*div[0]);
		      break;
	      case(3):
		      // faccio un reserve
		      celle.reserve((indexMax[0]-indexMin[0])*(indexMax[1]-indexMin[1])*(indexMax[2]-indexMin[2]));
		      
		      // metto i valori 
		      for(UInt i=indexMin[0]; i<=indexMax[0]; ++i)
			  for(UInt j=indexMin[1]; j<=indexMax[1]; ++j)
			    for(UInt k=indexMin[2]; k<=indexMax[2]; ++k)
		  		  celle.push_back(i+j*div[0]+k*div[0]*div[1]);
		      break;
	}
	
	// faccio un reserve 
	result.second.reserve(celle.size()*10);
	
	// Per ogni cella che ho trovato
	for(UInt j=0; j<celle.size(); ++j)
	    // ricavo gli elementi intorno a P
	    for(UInt i=0; i<grid[celle[j]].size(); ++i) result.second.push_back(grid[celle[j]][i]);
	      
	// controllo se ho trovato elementi 
	if(result.second.size()==0)	result.first=false;
	else				result.first=true;
	
	// ritorno il risultato 
	return(result);
}

template<class MESH, UInt DIM>
pair<bool, vector<UInt> > meshSearchStructured<MESH, DIM>::isIn(point node)
{
      // varaibili in uso 
      point				       tmp;
      vector<point>			      elem;
      pair<bool, vector<UInt> >   result,resultTmp;
      
      // cerco i possibili elementi che intersecano
      resultTmp = findIntersection(node, node);
      
      // faccio un reserve 
      result.second.reserve(resultTmp.second.size());
      
      // per ogni elemento testo 
      for(UInt i=0; i<resultTmp.second.size(); ++i)
      {
	    // prendo i nodi
	    meshPointer->getNodeOfElement(resultTmp.second[i], &elem);
	    
	    switch(MESH::RefShape::Shape)
	    {
	      case(POINT):
			  // prendo il punto 
			  tmp = meshPointer->getNode(meshPointer->getElement(resultTmp.second[i]).getConnectedId(0));
			  
			  // controllo 
			  if((tmp-node).norm2()<toll)	result.second.push_back(resultTmp.second[i]);
			  
			  break;
	      case(LINE):
			  // faccio il test
			  if(inSeg.isIn(&elem, node))	result.second.push_back(resultTmp.second[i]);
			  
			  break;
	      case(TRIANGLE):
			  // faccio il test
			  if(inTria.isIn(&elem, node))	result.second.push_back(resultTmp.second[i]);
			  
			  break;
	    }
      }
      
      // controllo se ho riempito la seconda parte della lista 
      if(result.second.size()!=0)  result.first = true;
      else			   result.first = false;
      
      // ritorno il risultato 
      return(result);
}

//
// Metodi per "editare la grid"
//
template<class MESH, UInt DIM>
void meshSearchStructured<MESH, DIM>::addToGrid(UInt gridId, UInt id)
{
    // assert per essere sicuri
    assert(gridId<grid.size());      
    
    // sfrutto i metodi di graphItem
    grid[gridId].push_back(id);
}

template<class MESH, UInt DIM>
void meshSearchStructured<MESH, DIM>::removeToGrid(UInt gridId, UInt id)
{
    // assert per essere sicuri
    assert(gridId<grid.size());
    
    // variabili in uso 
    vector<UInt>::iterator    it;
    
    // faccio un sort per cercarli 
    sort(grid[gridId].begin(), grid[gridId].end());
    
    // lo cerco 
    it = find(grid[gridId].begin(), grid[gridId].end(), id);
    
    // lo rimuovo 
    if(it!=grid[gridId].end())   grid[gridId].erase(it);
    
}

template<class MESH, UInt DIM>
void meshSearchStructured<MESH, DIM>::insertElement(UInt elemId)
{
      // controllo che sia ok
      assert(elemId<meshPointer->getNumElements());
      
      // variabili in uso 
      vector<UInt>         coor;
      point bBoxMax,bBoxMin,tmp;
      
      // prendo il massimo delle coordinate dei punti per creare il bbox del triangolo 
      meshPointer->createBBox(elemId, bBoxMax, bBoxMin);
      
      // prendo le sue coordinate
      tmp.replace(bBoxMax,bBoxMin,0.5);
		  
      // ricavo le coordinate
      getPointToGridCoor(tmp, &coor);
      
      // controllo se è dentro 
      if(coor[4]==1)	addToGrid(coor[0], elemId);
      else		cout << "ATTENZIONE: L'elemento non è dentro alla struttura di ricerca" << endl;
}

template<class MESH, UInt DIM>
void meshSearchStructured<MESH, DIM>::eraseElement(UInt elemId)
{
      // controllo che sia ok
      assert(elemId<meshPointer->getNumElements());
      
      // variabili in uso 
      vector<UInt>         coor;
      point bBoxMax,bBoxMin,tmp;
      
      // prendo il massimo delle coordinate dei punti per creare il bbox del triangolo 
      meshPointer->createBBox(elemId, bBoxMax, bBoxMin);
      
      // prendo le sue coordinate
      tmp.replace(bBoxMax,bBoxMin,0.5);
		  
      // ricavo le coordinate
      getPointToGridCoor(tmp, &coor);
      
      // controllo se è dentro 
      if(coor[4]==1)	removeToGrid(coor[0], elemId);
      else		cout << "ATTENZIONE: L'elemento non è dentro alla struttura di ricerca" << endl;
      
}

//
// Print
//
template<class MESH, UInt DIM>
void meshSearchStructured<MESH, DIM>::print(string s, bool all)
{
	// Variabili sempre utilizzate
	UInt 			    cont=0;
	createFile 		      file;
	vector<UInt>                  coor;
	
	// gestisco in modo diverso i casi 
	if(DIM==1)
	{
	      // variabili 
	      mesh1d<Line>		mesh;
	      geoElement<Line>		 lin;
	      point  		   tmp,p1,p2;
		      
	      // faccio un reserve
	      mesh.getNodePointer()->clear();
	      mesh.getNodePointer()->reserve(div[0]*2);
		      
	      // faccio un reserve
	      mesh.getElementPointer()->clear();
	      mesh.getElementPointer()->reserve(div[0]);
		      
	      // ciclo 
	      for(UInt i=0; i<div[0]; ++i)
	      {
		  p1.setX(pMin.getX()+i*H.getX());	   p1.setY(0.0);     p1.setZ(0.0);
		  p2.setX(pMin.getX()+(i+1)*H.getX()); 	   p2.setY(0.0);     p2.setZ(0.0);
		  
		  // metto i nodi 
		  mesh.insertNode(p1);	mesh.insertNode(p2);
			  
		  // setto la linea 
		  lin.setConnectedId(0, cont);
		  lin.setConnectedId(1, cont+1);
			  
		  // trovo le coordinate del punto
		  tmp.replace(p1,p2,0.5);
		  getPointToGridCoor(tmp,&coor);
		      			
		  // se ci sono elementi 
		  if(grid[coor[0]].size()!=0)			mesh.insertElement(lin);
		  else if(all)					mesh.insertElement(lin);

		  // aggiorno cont
		  cont = cont + 2;
	      }
		      
	      // stampo su file 
	      file.fileForParaview(s,&mesh);
	}	      
	else if(DIM==2)
	{
	    // variabili 
	    mesh2d<Quad>		mesh;
	    geoElement<Quad>		quad;
	    point  	     tmp,p1,p2,p3,p4;
		      
	    // faccio un reserve
	    mesh.getNodePointer()->clear();
	    mesh.getNodePointer()->reserve(div[0]*div[1]*4);
		      
	    // faccio un reserve
	    mesh.getElementPointer()->clear();
	    mesh.getElementPointer()->reserve(div[0]*div[1]);
		      
	    // ciclo 
	    for(UInt i=0; i<div[0]; ++i)
	    {
		  for(UInt j=0; j<div[1]; ++j)
		  {
		      p1.setX(pMin.getX()+i*H.getX());	   p1.setY(pMin.getY()+j*H.getY());     p1.setZ(0.0);
		      p2.setX(pMin.getX()+(i+1)*H.getX()); p2.setY(pMin.getY()+j*H.getY());	p2.setZ(0.0);
		      p3.setX(pMin.getX()+(i+1)*H.getX()); p3.setY(pMin.getY()+(j+1)*H.getY()); p3.setZ(0.0);
		      p4.setX(pMin.getX()+i*H.getX());     p4.setY(pMin.getY()+(j+1)*H.getY()); p4.setZ(0.0);
			  
		      // metto i nodi 
		      mesh.insertNode(p1);	mesh.insertNode(p2);
		      mesh.insertNode(p3);	mesh.insertNode(p4);
			  
		      // setto la linea 
		      quad.setConnectedId(0, cont);
		      quad.setConnectedId(1, cont+1);
		      quad.setConnectedId(2, cont+2);
		      quad.setConnectedId(3, cont+3);
			      
		      // trovo le coordinate del punto
		      tmp.replace(p1,p3,0.5);
		      getPointToGridCoor(tmp,&coor);
		      
		      // se ci sono elementi 
		      if(grid[coor[0]].size()!=0)		mesh.insertElement(quad);
		      else if(all)				mesh.insertElement(quad);

		      // aggiorno cont
		      cont = cont + 4;
		  }
	      }
		      
	      // stampo su file 
	      file.fileForParaview(s,&mesh);
		  
	}
	else if(DIM==3)
	{
	      // Variabili in gioco
	      mesh3d<Hexa>                  mesh;
	      point  tmp,p1,p2,p3,p4,p5,p6,p7,p8;
	      geoElement<Hexa>               hex;
	
	      // setting delle veraibili
	      coor.resize(5);
		      
	      // faccio un reserve
	      mesh.getNodePointer()->clear();
	      mesh.getNodePointer()->reserve(div[0]*div[1]*div[2]*4);
		      
	      // faccio un reserve
	      mesh.getElementPointer()->clear();
	      mesh.getElementPointer()->reserve(div[0]*div[1]*div[2]);
		      
	      // ciclo 
	      for(UInt k=0; k<div[2]; ++k)
	      {
		  for(UInt j=0; j<div[1]; ++j)
		  {
		      for(UInt i=0; i<div[0]; ++i)
		      {
			  // creo i nodi 
			  p1.setX(pMin.getX()+i*H.getX());	   
			  p1.setY(pMin.getY()+j*H.getY());     
			  p1.setZ(pMin.getZ()+k*H.getZ());
		     
			  p2.setX(pMin.getX()+(i+1)*H.getX()); 
			  p2.setY(pMin.getY()+j*H.getY());
			  p2.setZ(pMin.getZ()+k*H.getZ());
			  
			  p3.setX(pMin.getX()+(i+1)*H.getX()); 
			  p3.setY(pMin.getY()+(j+1)*H.getY()); 
			  p3.setZ(pMin.getZ()+k*H.getZ());
			  
			  p4.setX(pMin.getX()+i*H.getX());
			  p4.setY(pMin.getY()+(j+1)*H.getY()); 
			  p4.setZ(pMin.getZ()+k*H.getZ());
		     
			  p5.setX(pMin.getX()+i*H.getX());
			  p5.setY(pMin.getY()+j*H.getY()); 
			  p5.setZ(pMin.getZ()+(k+1)*H.getZ());
			  
			  p6.setX(pMin.getX()+(i+1)*H.getX());
			  p6.setY(pMin.getY()+j*H.getY());     
			  p6.setZ(pMin.getZ()+(k+1)*H.getZ());
				  
			  p7.setX(pMin.getX()+(i+1)*H.getX());
			  p7.setY(pMin.getY()+(j+1)*H.getY()); 
			  p7.setZ(pMin.getZ()+(k+1)*H.getZ());
			  
			  p8.setX(pMin.getX()+i*H.getX());
			  p8.setY(pMin.getY()+(j+1)*H.getY()); 	
			  p8.setZ(pMin.getZ()+(k+1)*H.getZ());
		     
			  // inserisco i nodi 
			  mesh.insertNode(p1);	mesh.insertNode(p2);	mesh.insertNode(p3);	mesh.insertNode(p4);
			  mesh.insertNode(p5);	mesh.insertNode(p6);	mesh.insertNode(p7);	mesh.insertNode(p8);
		 
			  // creo l'elemento
			  hex.setConnectedId(0,cont);
			  hex.setConnectedId(1,cont+1);
			  hex.setConnectedId(2,cont+2);
			  hex.setConnectedId(3,cont+3);
		  
			  hex.setConnectedId(4,cont+4);
			  hex.setConnectedId(5,cont+5);
			  hex.setConnectedId(6,cont+6);
			  hex.setConnectedId(7,cont+7);
		      
			  // trovo le coordinate del punto
			  tmp.replace(p1,p7,0.5);
			  getPointToGridCoor(tmp,&coor);
		      			
			  // se ci sono elementi 
			  if(grid[coor[0]].size()!=0)			mesh.insertElement(hex);
			  else if(all)					mesh.insertElement(hex);
				  
			  // incremento cont
			  cont = cont + 8;
		      }
		  }
	      }
	
	      // stampo su file 
	      file.fileForParaview(s,&mesh);
	}
}


}

#endif
