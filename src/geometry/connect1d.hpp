#ifndef CONNECT1D_HPP_
#define CONNECT1D_HPP_

#include <ctime>
#include <cassert>	
#include <iostream>
#include <vector>
#include <set>

#include "../geometry/mesh1d.hpp"

namespace geometry{

using namespace std;

/*! Classe che implementa le connessioni per le mesh1d. Restituisce principalmente le seguenti informazioni:
    
    <ol>
    <li> connettivit\'a nodo-nodo;
    <li> connettivit\'a elemento-elemento;
    <li> connettivit\'a nodo-elemento;
    <li> genera la mesh di bordo.
    </ol>
    
    La modifica e la lettura dei grafi contenuti in questa classe viene affidata ai metodi sviluppati in graphItem. */

template<typename GEOSHAPE> class connect1d
{
		// Un typedef per semplificarci la vita!!
		typedef typename GEOSHAPE::BShape BSHAPE;
	
	public:
		/*! Puntatore all'oggetto mesh1d di cui si dorv\'a creare la connettivit\'a */
		mesh1d<GEOSHAPE>           *  meshPointer;
			  
		/*! Vettore che contiene la connettività nodo-nodo*/
		vector<graphItem> 	       nodeToNode;
		
		/*! Vettore che contiene la connettività elemento-elemento*/
		vector<graphItem> 	 elementToElement;
		
		/*! Vettore che contiene la connettività nodo-elemento*/
		vector<graphItem>           nodeToElement;
	//
	// Costruttori
	//
	public:
		/*! Costruttore */
		connect1d();
		
		/*! Costruttore con il puntatore*/
		connect1d(mesh1d<GEOSHAPE>  *_meshPointer);
		
		/*! Metodo che cambia il puntatore alla mesh1d e resetta tutte le variabili */
		void setMeshPointer(mesh1d<GEOSHAPE>  *_meshPointer);
		
		/*! Metodo che cambia il puntatore alla mesh1d e resetta tutte le variabili */
		mesh1d<GEOSHAPE>  * getMeshPointer();
		
		/*! Metodo che pulisce le liste*/
		void clear();
	
	//
	// Metodi che restituiscono i puntatori alle strutture dati:
	// N.B. questi metodi sevono per avere una maggiore flessibilità nell'editing delle liste
	//
	public:
		/*! Puntatore alla lista delle connettività nodo-nodo*/
		vector<graphItem> * getNodeToNodePointer();
		
		/*! Puntatore alla lista delle connettività elemento-elemento*/
		vector<graphItem> * getElementToElementPointer();
		
		/*! Puntatore alla lista delle connettività nodo-elemento*/
		vector<graphItem> * getNodeToElementPointer();
		
		/*! Puntatore alla connettività nodo-nodo del nodo nodeId
		    \param nodeId identificatore del nodo*/
		graphItem * getNodeToNodePointer(UInt nodeId);
		
		/*! Puntatore alla connettività elemento-elemento dell'elemento elemId
		    \param elemId identificatore dell'elemento */
		graphItem * getElementToElementPointer(UInt elemId);
		
		/*! Puntatore alla connettività nodo-elemento del nodo nodeId
		    \param nodeId identificatore del nodo*/
		graphItem * getNodeToElementPointer(UInt nodeId);
	//
	// Metodi che creano le connessioni partendo solo dall'informazioni della mesh1d
	//
	public:
		/*! Connessioni nodo-nodo*/
		void buildNodeToNode();
		
		/*! Connessioni nodo-elemento */
		void buildNodeToElement();
		
		/*! Connessioni elemento-elemento sfrutta SOLO la struttura dati nodeToElement */
		void buildElementToElement();
		
		/*! TUTTE LE CONNESSIONI */
		void buildConnectivity();
		
		/*! crea la mesh di bordo, sfrutta anche la struttura element-element per le superfici con più di un geoId 
		    \param node puntatore a un'oggetto che conterrà la mesh di bordo 
		    \param nodeToBor mappa che associa gli id della mesh di bordo con quella puntata dalla classe
		    \param allBoundary booleano che indica se cercare o meno i bordi dovuti alla presenza di elementi con
		                       differenti geoId */
 		void buildBoundaryConnectivity(mesh0d<BSHAPE> * node, map<UInt, UInt>  * nodeToBor, bool allBoundary=true);
		
		/*! Settaggio del bordo 
		    \param allBoundary booleano che indica se cercare o meno i bordi dovuti alla presenza di elementi con
		                       differenti geoId */
		void setUpBoundary(bool allBoundary=true);
		
	//
	// Metodo che stampa le connessioni
	//
	public:
		/*! Riempie le liste di connettivit\'a*/
		void print();
};

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttori
//

template<typename GEOSHAPE> connect1d<GEOSHAPE>::connect1d()
{
	meshPointer    = NULL;
}
		
template<typename GEOSHAPE> connect1d<GEOSHAPE>::connect1d(mesh1d<GEOSHAPE>  *_meshPointer)
{
	meshPointer    = _meshPointer;
}

template<typename GEOSHAPE> void connect1d<GEOSHAPE>::setMeshPointer(mesh1d<GEOSHAPE>  *_meshPointer)
{
	clear();
	meshPointer    = _meshPointer;
}

template<typename GEOSHAPE> mesh1d<GEOSHAPE>  * connect1d<GEOSHAPE>::getMeshPointer()
{
	return(meshPointer);
}
		
template<typename GEOSHAPE> void connect1d<GEOSHAPE>::clear()
{
	meshPointer     = NULL;
	nodeToNode.clear();
	elementToElement.clear();
	nodeToElement.clear();
}

//
// Metodi che restituiscono i puntatori alle strutture dati:
//
template<typename GEOSHAPE> vector<graphItem> * connect1d<GEOSHAPE>::getNodeToNodePointer()
{
	return(&nodeToNode);
}
		
template<typename GEOSHAPE> vector<graphItem> * connect1d<GEOSHAPE>::getElementToElementPointer()
{
	return(&elementToElement);
}
		
template<typename GEOSHAPE> vector<graphItem> * connect1d<GEOSHAPE>::getNodeToElementPointer()
{
	return(&nodeToElement);
}

template<typename GEOSHAPE> graphItem * connect1d<GEOSHAPE>::getNodeToNodePointer(UInt nodeId)
{
	assert(nodeId<meshPointer->getNumNodes());
	return(&nodeToNode[nodeId]);
}
		
template<typename GEOSHAPE> graphItem * connect1d<GEOSHAPE>::getElementToElementPointer(UInt elemId)
{
	assert(elemId<meshPointer->getNumElements());
	return(&elementToElement[elemId]);
}
		
template<typename GEOSHAPE> graphItem * connect1d<GEOSHAPE>::getNodeToElementPointer(UInt nodeId)
{
	assert(nodeId<meshPointer->getNumNodes());
	return(&nodeToElement[nodeId]);
}

//
// Metodi che creano le connessioni partendo solo dall'informazioni della mesh1d 
//
template<typename GEOSHAPE> void connect1d<GEOSHAPE>::buildNodeToNode()
{
	  // Libero la lista
	  nodeToNode.clear();
	  nodeToNode.resize(meshPointer->getNumNodes());
	  
	  // Variabli temporanee
	  UInt 					       id1,id2;
	  Real						   dif;
	  time_t                                     start,end;
	  
	  // output
	  cout << "Costruzione connettività 1d nodoNodo..." << endl;
	  time(&start);
	  
	  // Mi ricavo le informazioni sull'elemento preso in esame per avere una migliore descrizione delle 
	  // sue connettività
	  UInt num = GEOSHAPE::numEdges;
	  
	  // Ciclo sugli elementi
	  for(UInt i=0; i<meshPointer->getNumElements(); ++i)
	  {
		  // metto a posto le connettività 
		  for(UInt j=0; j<num*2; j=j+2)
		  {
		      // prendo i nodi 
		      id1 = meshPointer->getElement(i).getConnectedId(GEOSHAPE::edgeConn[j]);
		      id2 = meshPointer->getElement(i).getConnectedId(GEOSHAPE::edgeConn[j+1]);
		      
		      // setto tutto 
		      nodeToNode[id1].add(id2);
		      nodeToNode[id2].add(id1);
		  }
	  }
	  
	  time(&end);
	  dif = difftime(end,start);
	  cout << "Connettività 1d nodoNodo completata: " <<  dif << " sec." << endl;
}
		
template<typename GEOSHAPE> void connect1d<GEOSHAPE>::buildNodeToElement()
{
	  // Libero la lista
	  nodeToElement.clear();
	  nodeToElement.resize(meshPointer->getNumNodes());
	  
	  // Mi ricavo le informazioni sull'elemento preso in esame per avere una migliore descrizione delle 
	  // sue connettività
	  UInt num = GEOSHAPE::numVertices;
	  Real						   dif;
	  time_t                                     start,end;
	  
	  // output
	  cout << "Costruzione connettività 1d nodoElemento..." << endl;
	  time(&start);
	  
	  // Ciclo sugli elementi
	  for(UInt i=0; i<meshPointer->getNumElements(); ++i)
		  for(UInt j=0; j<num; ++j)	
			  nodeToElement[meshPointer->getElement(i).getConnectedId(j)].connectedPushBack(i);
		  
	  time(&end);
	  dif = difftime(end,start);
	  cout << "Connettività 1d nodoElemento completata: " <<  dif << " sec." << endl;
}

template<typename GEOSHAPE> void connect1d<GEOSHAPE>::buildElementToElement()
{
	  // Libero la lista
	  elementToElement.clear();
	  elementToElement.resize(meshPointer->getNumElements());
	  
	  // Sfrutto la lista nodeToElement
	  if(nodeToElement.size()==0)	  buildNodeToElement();
	  
	  // Variabile temporanea
	  UInt          			        elemId;
	  vector<UInt>	         			  conn;
	  vector<UInt>::iterator   			    it;
	  
	  Real						   dif;
	  time_t                                     start,end;
	  
	  // output
	  cout << "Costruzione connettività 1d elementoElemento..." << endl;
	  time(&start);
	  
	  // Ciclo sugli elementi
	  for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
	  {
		  // li metto nella lista 
		  conn = nodeToElement[i].getConnectedIds();
		
		  // reimpio le connessioni 
		  for(UInt k=0; k<conn.size(); ++k)
		  {
			// ordino conn
			sort(conn.begin(), conn.end());
			  
			// cerco quello che sto analizzando 
			it = find(conn.begin(), conn.end(), conn[k]);
			  
			// lo devo sempre trovare!!
			assert(it!=conn.end());
			  
			// lo salvo 
			elemId = *it;
			  
			// lo elimino 
			conn.erase(it);
			  
			// li metto 
			elementToElement[elemId].add(conn);
			  
			// lo reinserisco 
			conn.push_back(elemId);
		   }
	}
	
	time(&end);
	dif = difftime(end,start);
	cout << "Connettività 1d elementoElemento completata: " <<  dif << " sec." << endl;
}
		
template<typename GEOSHAPE> void connect1d<GEOSHAPE>::buildConnectivity()
{
	// Chiama gli altri metodi
	buildNodeToNode();
	buildNodeToElement();
	buildElementToElement();
}

template<typename GEOSHAPE> void connect1d<GEOSHAPE>::buildBoundaryConnectivity(mesh0d<BSHAPE> * node, 
										map<UInt, UInt> * nodeToBor, 
										bool allBoundary)
{
	// variabili in uso
	UInt                        id1,cont;
	vector<UInt>		        conn;
	set<UInt>                         id;
	mesh0d<BSHAPE>                 bordo;
	vector<geoElement<BSHAPE> >    lista;
	vector<bool>		      active;
	map<UInt,UInt>             borToNode;
	Real				 dif;
	time_t                     start,end;
	  
	// output
	cout << "Costruzione mesh di Bordo in connect1d..." << endl;
	time(&start);
	
	// pulisco bor
	node->clear();
	nodeToBor->clear();
	
	// creo lo scheletro
	meshPointer->createMesh0d(&bordo);
	
	// faccio un reserve
	lista.reserve(bordo.getNumElements());
	
	// faccio un resize
	active.assign(bordo.getNumNodes(), false);
	
	// controllo che sia stata creata la connessione nodoElemento
	if(nodeToElement.empty())	buildNodeToElement();
	
	// cilco sugli elementi di bordo per cercare quelli che condividono più di un triangolo
	for(UInt i=0; i<bordo.getNumElements(); ++i)
	{
		// prendo i nodi 
		id1 = bordo.getElement(i).getConnectedId(0);
		
		// trovo chi hanno in comune
		conn = nodeToElement[id1].getConnectedIds();
		
		// se hanno solo un elemento in comune
		if(conn.size()==1)
		{
		    // setto active
		    active[id1] = true;
		
		    lista.push_back(bordo.getElement(i));
		}
		else if(allBoundary)
		{
		    // prendo tutti gli id
		    id.clear();
		    for(UInt j=0; j<conn.size(); ++j)	id.insert(meshPointer->getElement(conn[j]).getGeoId());
			
		    // se ho trovato più elementi metto tutto nella lista
		    if(id.size()>1)
		    {
			// setto active
			active[id1] = true;
			
			// la metto nella lista
			lista.push_back(bordo.getElement(i));
		    }
		}
	}
	
	// faccio un reserve
	node->getNodePointer()->reserve(active.size());
	
	// creo la mappa 
	cont = 0;
	for(UInt i=0; i<active.size(); ++i)	
	{
	      if(active[i])
	      {
		  // se il nodo è attivo creo la mappa 
		  nodeToBor->insert(pair<UInt,UInt>(cont, i));
		  borToNode.insert(pair<UInt,UInt>(i, cont));
	      
		  // lo inserisco 
		  node->insertNode(meshPointer->getNode(i));
	      
		  // incremento cont
		  ++cont;
	      }
	}
	
	// ciclo sulla lista per cambiare gli id
	for(UInt i=0; i<lista.size(); ++i)
	{
	      // prendo gli id 
	      id1 = lista[i].getConnectedId(0);
	      
	      // cambio gli id 
	      lista[i].setConnectedId(0, borToNode[id1]);
	}
	
	// la metto in bor
	node->insertElement(&lista);
	
	// faccio un setup
	node->setUpIds();
	
	time(&end);
	dif = difftime(end,start);
	cout << "Costruzione mesh di bordo completata: " <<  dif << " sec." << endl;
}

template<typename GEOSHAPE> void connect1d<GEOSHAPE>::setUpBoundary(bool allBoundary)
{
	// variabli in uso 
	mesh0d<typename GEOSHAPE::BShape>    bor;
	map<UInt, UInt>		      borToCloud;
	
	// creo la mesh di bordo
 	buildBoundaryConnectivity(&bor, &borToCloud);
	
	// setto a tutte le variabili boundary 
	for(UInt i=0; i<meshPointer->getNumNodes(); ++i)	meshPointer->getNodePointer(i)->setBoundary(0);
	
	// inserisco le informazioni di bordo 
	for(UInt i=0; i<bor.getNumNodes(); ++i) meshPointer->getNodePointer(borToCloud[i]).setBoundary(1);
}

//
// Metodo che stampa le connessioni

template<typename GEOSHAPE> void connect1d<GEOSHAPE>::print()
{
	cout << "Lista delle connessioni nodo-nodo" << endl;
	for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
	{
	      cout << "Node " << meshPointer->nodes[i].getId() << ": ";
	      for(UInt j=0; j<nodeToNode[i].getNumConnected(); ++j)
	      {
		    cout << nodeToNode[i].getConnectedId(j) << "  ";
	      }
	      cout << endl;
	}
	cout << endl;
	
	cout << "Lista delle connessioni elemento-elemento" << endl;
	for(UInt i=0; i<meshPointer->getNumElements(); ++i)
	{
	      cout << "Element " << meshPointer->elements[i].getId() << ": ";
	      for(UInt j=0; j<elementToElement[i].getNumConnected(); ++j)
	      {
		    cout << elementToElement[i].getConnectedId(j) << "  ";
	      }
	      cout << endl;
	}
	cout << endl;
	
	cout << "Lista delle connessioni nodo-elemento" << endl;
	for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
	{
	      cout << "Node " << meshPointer->nodes[i].getId() << ": ";
	      for(UInt j=0; j<nodeToElement[i].getNumConnected(); ++j)
	      {
		    cout << nodeToElement[i].getConnectedId(j) << "  ";
	      }
	      cout << endl;
	}
	cout << endl;
}



}

#endif

