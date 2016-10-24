#ifndef MESH1D_HPP_
#define MESH1D_HPP_

#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

#include "../geometry/geoElement.hpp"
#include "../geometry/mesh0d.hpp"

namespace geometry{

/*! Classe che rapprensenta una mesh1d geometrica. La mesh1d è rappresentata come un insieme di punti che vengono connessi da
"elementi" che nella versione più semplice sono delle linee. 

N.B. Questa classe è semplicemente un CONTENITORE ed ha le caratteristiche di una semplice Lista Lineare Sequenziale, non è stato
fatto alcun algoritmo per verificare la conformità della griglia contenuta. Al suo interno sono presenti però delle utiliy molto
semplici per fare alcune operazioni che danno delle informazioni sulla griglia. */

template<typename GEOSHAPE> class mesh1d
{
	//
	// Variabili 
	//
	public:
		/*! Elemento di riferimento */
		typedef GEOSHAPE	                   RefShape;
		
		/*! tipo di mesh di bordo */
		typedef mesh0d<typename GEOSHAPE::BShape> BoundMesh;
		
		/*! Massima capacità delle liste */
		UInt                                    maxNumNodes;
		UInt                                 maxNumElements;
	  
		/*! Lista dei nodi */
		vector<point>  		          	     nodes;
		
		/*! Lista degli elementi */
		vector<geoElement<GEOSHAPE> >             elements;
		
	//
	// Costruttori
	//
	public:
		/*! Costruttore Vuoto*/
		mesh1d();
		
		/*! Costruttore 
		    \param nV intero per settare la capacità della lista dei nodi 
		    \param nF intero per settare la capacità della lista degli elementi */
		mesh1d(UInt _nV, UInt _nF);
		
		/*! Costruttore con delle liste 
		    \param Nodes    lista dei nodi;
		    \param elements lista degli elementi;
		    \param nV intero per settare la capacità della lista dei nodi 
		    \param nF intero per settare la capacità della lista degli elementi */
		mesh1d(vector<point> _nodes, vector<geoElement<GEOSHAPE> >  _elements);
		
		/*! Costruttore di copia 
		 \param _mesh1d  mesh 2d da copiare*/
		mesh1d(const mesh1d &_mesh1d);
		
		/*! Operatore ugauglianza 
		 \param _mesh1d  mesh 2d da copiare*/
		mesh1d & operator=(const mesh1d &_mesh1d);
		
		/*! Metodo che pulisce le liste */
		void clear();
	
	//
	// Get generic information
	//
	public:
		/*! Numero di nodi */
		inline UInt getNumNodes() const;
		
		/*! Numero degli elementi */
		inline UInt getNumElements() const;
		
	//
	// Get node, element ecc
	// N.B. le prime tre funzioni restituiscono i puntatori alle liste per permettere una migliore possibilità di editarle
	//
	public:
		/*! Puntatore alla lista di nodi */
		inline vector<point> * getNodePointer();
		
		/*! Puntatore alla lista degli elementi */
		inline vector<geoElement<GEOSHAPE> > * getElementPointer();
		
		/*! Puntatore al nodo della lista di nodi 
		    \param nodeId indentificatore del nodo */
		inline point * getNodePointer(UInt nodeId);
		
		/*! Puntatore all'elemento della lista degli elementi 
		    \param elemId indentificatore dell'elemento */
		inline geoElement<GEOSHAPE> * getElementPointer(UInt elemId);
		
		/*! Punto i-esimo 
		   \param i  id del nodo*/
		inline point getNode(UInt i) const;
		
		/*! Elemento i-esimo 
		   \param i  id dell'elemento */
		inline geoElement<GEOSHAPE> getElement(UInt i) const;
		
		/*! Metodo che restituisce i nodi connessi all'elemento i-esimo 
		   \param i  id dell'elemento 
		   \param connessi puntatore a un vettore che conterrà i nodi */
		void getNodeOfElement(UInt i, vector<point> * connessi) const;
		
	//
	// Insert node e elements
	//
	public:
		/*! Inserimento di un nodo nella coda 
		     \param p punto */
		void insertNode(point p);
		
		/*! Inserimento di una lista di punti 
		    \param _nodes puntatore a una lista di nodi */
		void insertNode(vector<point> * _nodes);
		
		/*! Inserimento di un elemento 
		    \param element elemento da inserire */
		void insertElement(geoElement<GEOSHAPE> element);
		
		/*! Inserimento di una lista di elementi
		    \param _element puntatore alla lista da inserire */
		void insertElement(vector<geoElement<GEOSHAPE> > * _element);
		
		/*! Inserimento di un set di elementi da inserire
		    \param _element puntatore alla lista da inserire */
		void insertElement(set<geoElement<GEOSHAPE> > * _element);
	//
	// Settings
	//
	public:
		/*! Settaggio degli Id dei nodi e degli elementi in modo conforme all'ordinamento nei vettori nodes e elements*/
		void setUpIds();
		
		/*! Settaggio dei geoId con un valore fissato
		    \param geo valore del geoId*/
		void setUpGeoId(UInt geo);
		
		/*! Settaggio dei geoId con una mappa
		    \param oldToNew mappa che dice come cambiare i geoId */
		void setUpGeoId(map<UInt,UInt> oldToNew);
	//
	// metodi che fanno delle semplici analisi 
	//
	public:
		/*! Metodo che calcola il massimo e il minimo del bbox 
		    \param pMax punto che da il massimo 
		    \param pMin punto che da il minimo */
		void createBBox(point & pMax, point & pMin);
		
		/*! Metodo che calcola il massimo e il minimo del bbox di un elemento specifico 
		    \param elemId identificatore dell'elemento 
		    \param pMax punto che da il massimo 
		    \param pMin punto che da il minimo */
		void createBBox(UInt elemId, point & pMax, point & pMin);
		
		/*! Metodo che calcola l'edge più corto*/
		Real minH();
		
		/*! Metodo che calcola l'edge più lungo*/
		Real maxH();
	//
	// metodo che genera mesh1d
	//
	public:
		/*! Metodo che si limita a creare la mesh0d con la nuvola di punti  
		    \param reticolo puntatore alla mesh id che conterrà la mesh0d */
		void createMesh0d(mesh0d<typename GEOSHAPE::BShape> * cloud);
	//
	// print
	//
	public:
		/*! stampa a schermo delle informazioni*/
		void print();
	
};
 
//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------
 
//
// Costruttori
//
template<typename GEOSHAPE> mesh1d<GEOSHAPE>::mesh1d()
{
      // setto le variabili
      maxNumNodes       = 0;
      maxNumElements    = 0;
      
      // faccio un reserve
      nodes.reserve(maxNumNodes);
      elements.reserve(maxNumElements);
}
 
template<typename GEOSHAPE> mesh1d<GEOSHAPE>::mesh1d(UInt nV, UInt nF)
{
      // setto le variabli 
      maxNumNodes       = nV;
      maxNumElements    = nF;
      
      // faccio un reserve
      nodes.reserve(maxNumNodes);
      elements.reserve(maxNumElements);
}
 
template<typename GEOSHAPE> mesh1d<GEOSHAPE>::mesh1d(vector<point> _nodes, vector<geoElement<GEOSHAPE> >  _elements)
{
      // setto le variabili
      maxNumNodes       = 0;
      maxNumElements    = 0;
  
      // faccio un reserve
      nodes.reserve(std::max(maxNumNodes, static_cast<UInt>(_nodes.size())));
      elements.reserve(std::max(maxNumElements, static_cast<UInt>(_elements.size())));
      
      // copio 
      insertNode(_nodes.begin());
      insertElements(_elements.begin());  
}

template<typename GEOSHAPE> mesh1d<GEOSHAPE>::mesh1d(const mesh1d &_mesh1d)
{
      // setto le variabili
      maxNumNodes       = _mesh1d.maxNumNodes;
      maxNumElements    = _mesh1d.maxNumElements;
  
      // faccio un reserve
      nodes.reserve(maxNumNodes);
      elements.reserve(maxNumElements);
      
      // copio
      nodes = _mesh1d.nodes;
      elements = _mesh1d.elements;
}

template<typename GEOSHAPE> mesh1d<GEOSHAPE> & mesh1d<GEOSHAPE>::operator=(const mesh1d &_mesh1d)
{
      // setto le variabili
      maxNumNodes       = _mesh1d.maxNumNodes;
      maxNumElements    = _mesh1d.maxNumElements;
  
      // faccio un reserve
      nodes.reserve(maxNumNodes);
      elements.reserve(maxNumElements);
      
      // copio
      nodes = _mesh1d.nodes;
      elements = _mesh1d.elements;
      
      return *this;
}

template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::clear()
{
      // libero le liste
      nodes.clear();
      elements.clear();
}

//
// Get generic information
//

template<typename GEOSHAPE> inline UInt mesh1d<GEOSHAPE>::getNumNodes() const
{
	return(nodes.size());
}

template<typename GEOSHAPE> inline UInt mesh1d<GEOSHAPE>::getNumElements() const
{
	return(elements.size());
}

//
// Get node, element ecc
//

template<typename GEOSHAPE> inline vector<point> * mesh1d<GEOSHAPE>::getNodePointer()
{
	return(&nodes);
}

template<typename GEOSHAPE> inline vector<geoElement<GEOSHAPE> > * mesh1d<GEOSHAPE>::getElementPointer()
{
	return(&elements);
}

template<typename GEOSHAPE> inline point * mesh1d<GEOSHAPE>::getNodePointer(UInt nodeId)
{
	assert(nodeId<nodes.size());
	return(&nodes[nodeId]);
}

template<typename GEOSHAPE> inline geoElement<GEOSHAPE> * mesh1d<GEOSHAPE>::getElementPointer(UInt elemId)
{
	assert(elemId<elements.size());
	return(&elements[elemId]);
}

template<typename GEOSHAPE> inline point mesh1d<GEOSHAPE>::getNode(UInt i) const
{
	assert(i<nodes.size());
	return(nodes[i]);
}
		
template<typename GEOSHAPE> inline geoElement<GEOSHAPE> mesh1d<GEOSHAPE>::getElement(UInt i) const
{
	assert(i<elements.size());
	return(elements[i]);
}

template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::getNodeOfElement(UInt i, vector<point> * connessi) const
{
	assert(i<elements.size());
	
	// faccio un resize di connessi 
	connessi->clear();
	connessi->resize(GEOSHAPE::numVertices);
	
	for(UInt j=0; j<GEOSHAPE::numVertices; ++j) connessi->at(j) = nodes[elements[i].getConnectedId(j)];
}

//
// Insert node e elements
//

template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::insertNode(point p)
{
	p.setId(nodes.size());
	nodes.push_back(p);
}

template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::insertNode(vector<point> * _nodes)
{
        // pulisco la lista dei nodi
	nodes.clear();
	
	// faccio un resize
	nodes.resize(_nodes->size());
	
	// copio le informazioni
	copy(_nodes->begin(), _nodes->end(), nodes.begin());
	
	// setto gli id 
	for(UInt i=0; i<nodes.size(); ++i)  nodes[i].setId(i);
}
		
template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::insertElement(geoElement<GEOSHAPE> element)
{
	element.setId(elements.size());
	elements.push_back(element);
}

template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::insertElement(vector<geoElement<GEOSHAPE> > * _elements)
{
	// libero la lista 
	elements.clear();
	
	// faccio un resize
	elements.resize(_elements->size());
	
	// copio il contenuto
	copy(_elements->begin(), _elements->end(), elements.begin());
	
	// setto gli id 
	for(UInt i=0; i<elements.size(); ++i)  elements[i].setId(i);
}

template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::insertElement(set<geoElement<GEOSHAPE> > * _elements)
{
	// libero la lista 
	elements.clear();
	
	// faccio un resize
	elements.resize(_elements->size());
	
	// copio il contenuto
	copy(_elements->begin(), _elements->end(), elements.begin());
	
	// setto gli id 
	for(UInt i=0; i<elements.size(); ++i)  elements[i].setId(i);
}

//
// Settings
//

template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::setUpIds()
{
	// setto gli id dei nodi e degli elementi in modo che siano concordi con quelli della loro posizione nel vettore
	for(UInt i=0; i<nodes.size(); ++i)  	  nodes[i].setId(i);
	for(UInt i=0; i<elements.size(); ++i)	  elements[i].setId(i);
}

template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::setUpGeoId(UInt geo)
{
	for(UInt i=0; i<elements.size(); ++i)     elements[i].setGeoId(geo);
}

template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::setUpGeoId(map<UInt,UInt> oldToNew)
{
	UInt geo;
      
	for(UInt i=0; i<elements.size(); ++i)
	{
	      // prendo il suo vecchio geoId
	      geo = elements[i].getGeoId();
	      
	      // lo cambio
	      elements[i].setGeoId(oldToNew[geo]);
	}
}

//
// metodi che fanno delle semplici analisi 
//
template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::createBBox(point & pMax, point & pMin)
{
      // controllo che la mesh contenga punti 
      assert(nodes.size()!=0);
    
      // setto le variabili
      pMax = nodes[0];
      pMin = nodes[0];
      
      // prendo i minimi e massimi
      for(UInt i=1; i<nodes.size(); ++i)
      {
	  // setto il massimo 
	  pMax.setX(std::max(nodes[i].getX(), pMax.getX()));
	  pMax.setY(std::max(nodes[i].getY(), pMax.getY()));
	  pMax.setZ(std::max(nodes[i].getZ(), pMax.getZ()));
	  
	  // setto il minimo 
	  pMin.setX(std::min(nodes[i].getX(), pMin.getX()));
	  pMin.setY(std::min(nodes[i].getY(), pMin.getY()));
	  pMin.setZ(std::min(nodes[i].getZ(), pMin.getZ()));
      }
}

template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::createBBox(UInt elemId, point & pMax, point & pMin)
{
      // assert per essere sicuro
      assert(elemId<elements.size());
     
      // setto le variabili 
      Real            	     MIN[3],MAX[3];
      
      // inizializzo 
      MIN[0]=nodes[elements[elemId].getConnectedId(0)].getI(0);
      MIN[1]=nodes[elements[elemId].getConnectedId(0)].getI(1);
      MIN[2]=nodes[elements[elemId].getConnectedId(0)].getI(2);
      
      // inizializzo 
      MAX[0]=nodes[elements[elemId].getConnectedId(0)].getI(0);
      MAX[1]=nodes[elements[elemId].getConnectedId(0)].getI(1);
      MAX[2]=nodes[elements[elemId].getConnectedId(0)].getI(2);
      
      // ciclo sulle coordinate 
      for(UInt j=0; j<3; ++j)
      {
	 // ciclo sull'elemento 
	 for(UInt i=1; i<RefShape::numVertices; ++i)
	 {
	    MIN[j] = std::min(MIN[j],nodes[elements[elemId].getConnectedId(i)].getI(j));
	    MAX[j] = std::max(MAX[j],nodes[elements[elemId].getConnectedId(i)].getI(j));
	 }
      }
      
      // setto massimi e minini
      for(UInt j=0; j<3; ++j)
      {
	 pMax.setI(j, MAX[j]);
	 pMin.setI(j, MIN[j]);
      }
}
		
template<typename GEOSHAPE> Real mesh1d<GEOSHAPE>::minH()
{
      // mi assicuro che ci siano elementi 
      assert(elements.size()!=0);
      
      // variabili
      Real	minH(1e99);
      Real 	     delta;
      
      // ciclo sugli elementi
      for(UInt i=0; i<elements.size(); ++i)
      {
	    // calcolo la lunghezza
	    delta = (nodes[elements[i].getConnectedId(0)]-nodes[elements[i].getConnectedId(1)]).norm2();
	    
	    // calcolo il minimo 
	    minH  = std::min(delta, minH);
	
      }
      
      // ritorno 
      return(minH);
}
		
template<typename GEOSHAPE> Real mesh1d<GEOSHAPE>::maxH()
{
      // mi assicuro che ci siano elementi 
      assert(elements.size()!=0);
      
      // variabili
      Real	 maxH(0.0);
      Real 	     delta;
      
      // ciclo sugli elementi
      for(UInt i=0; i<elements.size(); ++i)
      {
	    // calcolo la lunghezza
	    delta = (nodes[elements[i].getConnectedId(0)]-nodes[elements[i].getConnectedId(1)]).norm2();
	    
	    // calcolo il minimo 
	    maxH  = std::max(delta, maxH);
      }
      
      // ritorno 
      return(maxH);
}

//
// metodo che genera mesh1d
//
template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::createMesh0d(mesh0d<typename GEOSHAPE::BShape> * cloud)
{
      // variabili in uso
      UInt                                               geoTmp;
      vector<UInt>                  listaGeoId(nodes.size(), 0);
      vector<geoElement<typename GEOSHAPE::BShape> >      lista;
      geoElement<typename GEOSHAPE::BShape>                elem;
      
      // APPUNTO SU COME È STATO IMPLEMENTATO IL METODO:
      // per convenzione mettiano ai nodi il massimo geoId degli elementi a cui appartengono 
      
      // pulisco l'input
      cloud->clear();
      
      // creo la lista degli id 
      for(UInt i=0; i<elements.size(); ++i)
      {
	  // prendo il geoId
	  geoTmp = elements[i].getGeoId();
	  
	  // lo assegno a tutti i nodi dell'elemento
	  for(UInt j=0; j<RefShape::numVertices; ++j)
	  {
	      listaGeoId[elements[i].getConnectedId(j)] = std::max(geoTmp, listaGeoId[elements[i].getConnectedId(j)]);
	  }
      }
      
      // faccio un resize
      cloud->getElementPointer()->resize(nodes.size());
      
      // inserisco i nodi 
      cloud->insertNode(&nodes);
        
      // creo gli elementi 
      for(UInt i=0; i<nodes.size(); ++i)
      {
	   // creo l'elemento 
	   elem.setConnectedId(0, i);
	   elem.setGeoId(listaGeoId[i]);
	   
	   // metto l'elemento 
	   cloud->getElementPointer()->at(i) = elem;
      }
      
      //  metto a posto gli id
      cloud->setUpIds();
}

//
// Stampa
//
template<typename GEOSHAPE> void mesh1d<GEOSHAPE>::print()
{
	cout << "Informazioni della Mesh1d" << endl;
	cout << "Numero di nodi:       " << this->getNumNodes() << endl;
	cout << "Numero di elements:   " << this->getNumElements() << endl;
	cout << endl;
	cout << endl;
	
	// stampo i nodi 
	if(this->getNumNodes()!=0)
	{
	      cout << "Lista dei nodi" << endl;
	      for(UInt i=0; i<nodes.size(); ++i)
	      {
		  cout << this->getNode(i).getId() << "  ";
		  this->getNode(i).print();
	      }
	      cout << endl;
	}
	
	// stampo i gli elementi 
	if(this->getNumElements()!=0)
	{
	      cout << "Lista degli elementi" << endl;
	      cout << endl;
	      for(UInt i=0; i<elements.size(); ++i)
	      {
		  this->getElement(i).print();
		  cout << endl;
	      }
	      cout << endl;
	}
}

}


#endif
