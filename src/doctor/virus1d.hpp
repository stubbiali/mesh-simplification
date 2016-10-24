#ifndef VIRUS1D_HPP_
#define VIRUS1D_HPP_

#include <cassert>	
#include <iostream>
#include <vector>
#include <stack>
#include <utility>

#include "../geometry/mesh1d.hpp"
#include "../geometry/connect1d.hpp"

#include "../doctor/meshHandler.hpp"

namespace geometry
{

using namespace std;

/*! Classe che implementa l'algoritmo per diffondere la "malattia" in una mesh1d. È stato deciso di rendere questa funzionalità
una classe in modo tale da poter rendere più flessibile il suo utilizzo*/
    
template<typename GEOSHAPE, UInt DIM=3> class virus1d : public connect1d<GEOSHAPE>
{  
	// Un typedef per semplificarci la vita!!
		typedef typename GEOSHAPE::BShape BSHAPE;	
	//
	// Variabili di classe 
	//
	public:
		/*! Tolleranza */
		Real                            	                    		     toll;
	//
	// Costruttori
	//
	public:
		/*! Costruttore */
		virus1d();
		
		/*! Costruttore con il puntatore*/
		virus1d(mesh1d<GEOSHAPE>  * _meshPointer);
		
		/*! Metodo che cambia il puntatore alla mesh1d e resetta tutte le variabili */
		void setMeshPointer(mesh1d<GEOSHAPE>  *_meshPointer);
	//
	// Metodi di get/set 
	//
	public:
		/*! Metodo che restituisce la tolleranza 
		    \param _toll valore della tolleranza */
		inline void setToll(Real toll=1e-14);
		  
		/*! Metodo che restituisce la tolleranza */
		inline Real getToll();
	//
	// Metodi per la diffusione della malattia
	//
	public:
		/*! Metodo che diffonde la malattia partendo da un elemento nel caso in cui non ci sono bariere
		    \param idStart identificatore dell'elemento da cui partire 
		   Il metodo restutisce quante regioni ha trovato */
		UInt virusDiffusion(UInt idStart=0);
		
		/*! Metodo che diffonde la malattia partendo da un elemento
		    \param barrier barriera che non può essere valicata
		    \param idStart identificatore dell'elemento da cui partire 
		   Il metodo restutisce quante regioni ha trovato */
		UInt virusDiffusion(mesh0d<BSHAPE> * barrier, UInt idStart=0);
		
		/*! Metodo che riempie un vettore con le barriere con gli id dei nodi che sono quelle della mesh puntata dalla
		    classe
		    \param barrier barriera invalicabile 
		    \param lista set che rende più veloci le ricerche dei bordi invalicabili*/
		void createSet(mesh0d<BSHAPE> * barrier, set<geoElement<BSHAPE> > * finder);
		
		/*! Metodo che permette di trovare gli elementi adiacenti a uno dato 
		    \param elemId identificatore dell'elemento
		    \param adiacenti puntatore a un vettore che sarà riempito con gli adiacenti*/
		void findAdiacenti(UInt elemId, vector<UInt> * adiacenti);
		
		/*! Metodo che restituisce il primo id disponibile per la diffusione della malattia partendo da un vettore di 
		    booleani
		    \param toExplore puntatore a un vettore di booleni che deve essere analizzato*/
		pair<bool, UInt> newStart(vector<bool> * visited);
		
};

}

using namespace geometry;

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttori
//
template<typename GEOSHAPE, UInt DIM> virus1d<GEOSHAPE, DIM>::virus1d()
{
	typename connect1d<GEOSHAPE>::connect1d();
	
	// setto la tolleranza
	toll = 1e-14;
}
		
template<typename GEOSHAPE, UInt DIM> virus1d<GEOSHAPE, DIM>::virus1d(mesh1d<GEOSHAPE>  *_meshPointer)
{
	// chiamo il costruttore di connect
	connect1d<GEOSHAPE>::connect1d(_meshPointer);
	
	// creo la connettività che mi serve 
	connect1d<GEOSHAPE>::buildElementToElement();
	
	// setto la tolleranza
	toll = 1e-14;
}

template<typename GEOSHAPE, UInt DIM> void virus1d<GEOSHAPE, DIM>::setMeshPointer(mesh1d<GEOSHAPE>  *_meshPointer)
{		
	// chiamo il setMeshPointer della classe madre
	connect1d<GEOSHAPE>::setMeshPointer(_meshPointer);
	
	// creo la connettività che mi serve 
	connect1d<GEOSHAPE>::buildElementToElement();
}

//
// Metodi di get/set 
//
template<typename GEOSHAPE, UInt DIM> inline void virus1d<GEOSHAPE, DIM>::setToll(Real _toll)
{
      toll = _toll;
}

template<typename GEOSHAPE, UInt DIM> inline Real virus1d<GEOSHAPE, DIM>::getToll()
{
      return(toll);
}


//
// Metodi per la diffusione della malattia
//
template<typename GEOSHAPE, UInt DIM> UInt virus1d<GEOSHAPE, DIM>::virusDiffusion(UInt idStart)
{      
	// variabili in uso 
	mesh1d<GEOSHAPE>  * meshP = connect1d<GEOSHAPE>::meshPointer;
  
	// comtrollo che sia giusto il l'id dell'emento
	assert(meshP!=NULL);
	assert(idStart<meshP->getNumElements());
  
	// variabli in uso 
	UInt 						          color,cont,tmpId;
	vector<UInt>						     adiacenti,com;
	stack<UInt>							 toExplore;
	time_t                                   	             start,end,dif;
	typename set<geoElement<BSHAPE> >::iterator                             it;
	vector<bool>							   visited;
	pair<bool, UInt>						    result;
	
	// stampa 
	cout << "Inizio processo di diffusione della malattia" << endl;
	time(&start);
	
	// setto il colore e cont
	color = 1;
	cont  = 0;
	
	// setto lo stack
	toExplore.push(idStart);
	
	// setto visited
	visited.resize(meshP->getNumElements(), false);
	visited[idStart] = true;
	
	// inizializzo resu
	result.first  = false;
	result.second = 1;
	
	// fino a che non ho visitato tutti gli elementi 
	while(!result.first)
	{
	      // fino a che lo stack non è vuoto 
	      while(toExplore.size()!=0)
	      {
		      // prendo l'id dell'elemento
		      tmpId = toExplore.top();
		      
		      // lo tolgo dallo stack
		      toExplore.pop();
		      
		      // coloro l'elemento 
		      meshP->getElementPointer(tmpId)->setGeoId(color);
		      
		      // incremalento cont
		      ++cont;
		      
		      // segnalo che l'ho visitato
		      visited[tmpId] = true;
		          
		      // prendo tutti gli adiacenti
		      findAdiacenti(tmpId, &adiacenti);
		      
		      // per ogni adiacente vedo se l'ho visitato 
		      for(UInt i=0; i<adiacenti.size(); ++i)
		      {
			    // se non l'ho visitato
			    if(!visited[adiacenti[i]])	toExplore.push(adiacenti[i]);
		      }
	      }
	      
	      // cambio colore 
	      ++color;
	      
	      // metto il nuovo elemento 
	      result = newStart(&visited);
	      
	      // se result restituisce vero dobbiamo procedere 
	      if(!result.first)		toExplore.push(result.second);
	}
	
	// prendo il tempo e stampo 
	time(&end);
	dif = difftime(end,start);
	cout << "Regioni trovate: " << color-1 << endl;
	cout << "Processo di diffusione della malattia: " <<  dif << " sec." << endl;
	
	// ritorno quante regioni ho trovato 
	return(color-1);
}

template<typename GEOSHAPE, UInt DIM> UInt virus1d<GEOSHAPE, DIM>::virusDiffusion(mesh0d<BSHAPE> * barrier, UInt idStart)
{      
	// variabili in uso 
	mesh1d<GEOSHAPE>  * meshP = connect1d<GEOSHAPE>::meshPointer;
  
	// comtrollo che sia giusto il l'id dell'emento
	assert(meshP!=NULL);
	assert(idStart<meshP->getNumElements());
  
	// variabli in uso 
	UInt 						          color,cont,tmpId;
	vector<UInt>						     adiacenti,com;
	stack<UInt>							 toExplore;
	time_t                                   	             start,end,dif;
	geoElement<BSHAPE>						   elemTmp;
	set<geoElement<BSHAPE> >                                            finder;
	typename set<geoElement<BSHAPE> >::iterator                             it;
	vector<bool>							   visited;
	pair<bool, UInt>						    result;
	
	// stampa 
	cout << "Inizio processo di diffusione della malattia" << endl;
	time(&start);
	
	// creo il set di elementi con gli id della mesh puntata dalla classe 
	createSet(barrier, &finder);
	
	// setto il colore e cont
	color = 1;
	cont  = 0;
	
	// setto lo stack
	toExplore.push(idStart);
	
	// setto visited
	visited.resize(meshP->getNumElements(), false);
	visited[idStart] = true;
	
	// inizializzo resu
	result.first  = false;
	result.second = 1;
	
	// fino a che non ho visitato tutti gli elementi 
	while(!result.first)
	{
	      // fino a che lo stack non è vuoto 
	      while(toExplore.size()!=0)
	      {
		      // prendo l'id dell'elemento
		      tmpId = toExplore.top();
		      
		      // lo tolgo dallo stack
		      toExplore.pop();
		      
		      // coloro l'elemento 
		      meshP->getElementPointer(tmpId)->setGeoId(color);
		      
		      // incremalento cont
		      ++cont;
		      
		      // segnalo che l'ho visitato
		      visited[tmpId] = true;
		          
		      // prendo tutti gli adiacenti
		      findAdiacenti(tmpId, &adiacenti);
		      
		      // per ogni adiacente vedo se l'ho visitato 
		      for(UInt i=0; i<adiacenti.size(); ++i)
		      {
			    // se non l'ho visitato
			    if(!visited[adiacenti[i]])
			    {
				  // controllo i comuni 
				  meshP->getElement(tmpId).common(meshP->getElement(adiacenti[i]), &com);
				  
				  // setto l'elemento 
				  elemTmp.setConnectedId(&com);
				  
				  // lo cerco dalla lista
				  it = finder.find(elemTmp);
				  
				  // se non lo trovo lo metto nello stack
				  if(!(it!=finder.end())) 	toExplore.push(adiacenti[i]);
			    }
		      }
	      }
	      
	      // cambio colore 
	      ++color;
	      
	      // metto il nuovo elemento 
	      result = newStart(&visited);
	      
	      // se result restituisce vero dobbiamo procedere 
	      if(!result.first)		toExplore.push(result.second);
	}
	
	// prendo il tempo e stampo 
	time(&end);
	dif = difftime(end,start);
	cout << "Regioni trovate: " << color-1 << endl;
	cout << "Processo di diffusione della malattia: " <<  dif << " sec." << endl;
	
	// ritorno quante regioni ho trovato 
	return(color-1);
}

template<typename GEOSHAPE, UInt DIM> void virus1d<GEOSHAPE, DIM>::createSet(mesh0d<BSHAPE> * barrier, 
								             set<geoElement<BSHAPE> > * finder)
{
	// variabili in uso 
	UInt 			               tmpId;
	meshHandler<mesh1d<GEOSHAPE> , DIM>  handler;
	map<UInt, UInt>                     extToInt;
	geoElement<BSHAPE>	                elem;
	
	// setto la tolleranza 
	handler.setToll(toll);
	
	// setto i puntatori della mesh
	handler.setMeshPointer(connect1d<GEOSHAPE>::meshPointer);
	
	// creo la mappa 
	handler.createMap(barrier->getNodePointer(), &extToInt);
	
	// per ogni elemento della mesh ridefinisco i nodi con la numerazione della mesh puntata dalla classe
	for(UInt i=0; i<barrier->getNumElements(); ++i)
	{
	      // per ogni connesso 
	      for(UInt j=0; j<BSHAPE::numVertices; ++j)
	      {
		    // prendo l'id
		    tmpId = barrier->getElement(i).getConnectedId(j);
		    
		    // faccio l'associazione
		    elem.setConnectedId(j, extToInt[tmpId]);
	      }
	      
	      // lo metto nella lista
	      finder->insert(elem);
	}
}

template<typename GEOSHAPE, UInt DIM> pair<bool, UInt>  virus1d<GEOSHAPE, DIM>::newStart(vector<bool> * visited)
{
	// variabile in uso 
	pair<bool, UInt>	result;
	
	// percorro tutto il vettore e se è falso ritorno la posizione 
	for(UInt i=0; i<visited->size(); ++i)
	{
	    if(!visited->at(i))
	    {
		  result.first  = false;
		  result.second = i;
		  
		  return(result);
	    }
	}
	
	result.first  = true;
	result.second = 0;
	 
	
	return(result);
}

template<typename GEOSHAPE, UInt DIM> void virus1d<GEOSHAPE, DIM>::findAdiacenti(UInt elemId, vector<UInt> * adiacenti)
{
	// faccio un clear e un reserve
	adiacenti->clear();
	adiacenti->reserve(connect1d<GEOSHAPE>::getElementToElementPointer(elemId)->getNumConnected());
  
	// ciclo su tutti i connessi 
	for(UInt i=0; i<connect1d<GEOSHAPE>::getElementToElementPointer(elemId)->getNumConnected(); ++i)
	{
	      adiacenti->push_back(connect1d<GEOSHAPE>::getElementToElementPointer(elemId)->getConnectedId(i));
	}
}

#endif



