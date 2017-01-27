#ifndef MEMENTOELEMENT_HPP_
#define MEMENTOELEMENT_HPP_

#include <cassert>
#include <iostream>

#include "../geometry/geoElement.hpp"

#include "../utility/inSegment.h"
#include "../utility/inTriangle.h"

namespace geometry
{

using namespace std;

/*! Classe derivata da geoElement che permette di associare a un elemento geometrico una serie di punti.*/
	
template<typename GEOSHAPE> class mementoElement : public geoElement<GEOSHAPE> 
{
	//
	// Variabili di classe 
	//
	public:
		
		/*! Vettore con tutti i punti associati all'elemento*/
		vector<point>			 	   ptAss;
		
		/*! Tolleranza */
		Real 					    toll;
		
		/*! Punti associati all'emento */
		vector<point>				    nodi;
		
		/*! variabile che indica il fattore con cui rifiutare le proiezioni */
		Real 					    fatt;
	//
	// Costruttori
	//	
	public:
		/*! Costruttore */
		mementoElement();
		
		/*! Costruttore non vuoto
		    \param _ptAss vettore con i punti associati all'elemento 
		    \param _nodi  vettore con i nodi dell'elemento */
		mementoElement(vector<point> * _ptAss, vector<point> * _nodi);
			
		/*! Costruttore di copia */
		mementoElement<GEOSHAPE> (const mementoElement<GEOSHAPE> &E);
			
		/*! Operatore uguaglianza */
		mementoElement<GEOSHAPE> operator=(const mementoElement<GEOSHAPE> &E);
	//
	// Set/get
	//
	public:
		/*! Metodo che svuota l'elemento */
		void clear();
		
		/*! Metodo che dice se il memento elemento è vuoto */
		bool isEmpty();
		
		/*! Set delle variabili della classe da cui deriva mementoElement 
		    \param E geoElement da cui prendere le informazioni */
		void setElement(const geoElement<GEOSHAPE> &E);
		
		/*! Set, reserve, push_back, clear e get della variabile ptAss */
 		inline vector<point> getPtAss() 	{return(ptAss);};
		inline void          reserve(UInt num)	{ptAss.reserve(num);};
		inline void          pushBack(point pt)	{ptAss.push_back(pt);};
		inline void 	     clearPtAss()	{ptAss.clear();};
		inline UInt 	     ptAssSize()	{return(ptAss.size());};
		void setPtAss(vector<point> * _ptAss);
		
		/*! Set e get della variabile fatt */
		inline Real getFatt()			{return(fatt);};
		inline void setFatt(Real _fatt)		{fatt = _fatt;};
		
		/*! Set e get della variabile nodi */
		inline vector<point> getNodi() {return(nodi);};
		void setNodi(vector<point> * _nodi);
		
		/*! Set e get Toll */
		inline void setToll(Real _toll=1e-15) {toll=_toll;};
		inline Real getToll()	 	      {return(toll);};
		
		/*! Metodo che prende il nodo in input e lo proietta se possibile sull'elemento
		    \param pt punto da proiettare 
		    \param id identificatore del punto 
		    \param ptOriginal punto originale da cui si deve calcolare la distanza 
		    Il metodo restituisce una coppia con un intero che indica che tipo di associazione è stata fatta 		    e un reale che dice la distanza */
		pair<int, Real> putPoint(point pt, UInt id, point ptOriginal);
		
		/*! Metodo che prende il nodo e controlla se viene può essere associato alla classe 
		    \param pt punto da proiettare  
		    \param ptOriginal punto originale da cui si deve calcolare la distanza
		    \param ass vettore che conterrà le informazioni su che cosa ho associato avrà l'id dell'elemento se viene 
			       associato alla faccia, i due id dell'edge se è associato all'edge e l'id del nodo se associato 
			       al nodo
		    Il metodo restituisce una coppia con un booleano indica se è stato associato e 
		    un reale che dice la distanza */
		pair<int, Real> controlPoint(point pt, point ptOriginal, vector<UInt> * ass);
	//
	// Operatori
	//
	public:
		/*! Operatore non uguaglianza */
		bool operator!=(const mementoElement<GEOSHAPE> &E) const;
		
		/*! Operatore non uguaglianza */
		bool operator==(const mementoElement<GEOSHAPE> &E) const;
			
		/*! Operatore minore */
		bool operator<(const mementoElement<GEOSHAPE> &E) const;
	//
	// metodo di stampa
	//
	public:
		void print();
};

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttori
//
template<typename GEOSHAPE> 
mementoElement<GEOSHAPE>::mementoElement() : geoElement<GEOSHAPE>::geoElement() 
{
	toll = 1e-15;
 	fatt = 9e99;
}

template<typename GEOSHAPE> 
mementoElement<GEOSHAPE>::mementoElement(vector<point> * _ptAss, vector<point> * _nodi)
{
	toll = 1e-15;
	fatt = 9e99;
	ptAss.resize(_ptAss->size());
	nodi.resize(_nodi->size());
	copy(_ptAss->begin(), _ptAss->end(), ptAss.begin());
	copy(_nodi->begin(),  _nodi->end(),  nodi.begin());
	
	// assert per controllare che il numero di nodi sia corretto 
	assert(nodi.size()==GEOSHAPE::numVertices);
}

template<typename GEOSHAPE> 
mementoElement<GEOSHAPE>::mementoElement(const mementoElement<GEOSHAPE> &E) : geoElement<GEOSHAPE>(E)
{
	ptAss = E.ptAss;
	toll  = E.toll;
	fatt  = E.fatt;
	nodi  = E.nodi;
}

template<typename GEOSHAPE> 
mementoElement<GEOSHAPE> mementoElement<GEOSHAPE>::operator=(const mementoElement<GEOSHAPE> &E) 
{	
	this->geoElement<GEOSHAPE>::operator=(E);
	ptAss = E.ptAss;
	toll  = E.toll;
	fatt  = E.fatt;
	nodi  = E.nodi;
	return *this;
}

//
// Get/Set
//
template<typename GEOSHAPE> 
void mementoElement<GEOSHAPE>::clear()
{
	// pulisco le liste 
	ptAss.clear();
	nodi.clear();
	
	// setto a zero tutto 
	for(UInt i=0; i<GEOSHAPE::numVertices; ++i)	this->setConnectedId(i, 0);
	this->setId(0);
}

template<typename GEOSHAPE> 
bool mementoElement<GEOSHAPE>::isEmpty()
{
	if(ptAss.size()!=0)		return(false);
	else if(nodi.size()!=0)		return(false);
	else if(this->getId()!=0)	return(false);
	else
	{
	      for(UInt i=0; i<GEOSHAPE::numVertices; ++i)	
		if(this->getConnectedId(i)!=0)	
		   return(false);
	}	
	
	// ritorno vero 
	return(true);
}

template<typename GEOSHAPE> 
void mementoElement<GEOSHAPE>::setElement(const geoElement<GEOSHAPE> & E)
{
	this->geoElement<GEOSHAPE>::operator=(E);
}	


template<typename GEOSHAPE> 
void mementoElement<GEOSHAPE>::setPtAss(vector<point> * _ptAss)
{
	ptAss.clear();
	ptAss.resize(_ptAss->size());
	copy(_ptAss->begin(), _ptAss->end(), ptAss.begin());
}

template<typename GEOSHAPE> 
void mementoElement<GEOSHAPE>::setNodi(vector<point> * _nodi)
{
	nodi.clear();
	nodi.resize(_nodi->size());
	copy(_nodi->begin(), _nodi->end(), nodi.begin());
	
	// assert per controllare che il numero di nodi sia corretto 
	assert(nodi.size()==GEOSHAPE::numVertices);
}

template<typename GEOSHAPE> 
pair<int, Real> mementoElement<GEOSHAPE>::putPoint(point pt, UInt id, point ptOriginal)
{
	// variabili in uso 
	bool 	  	            found=false;
	Real   dist,distEdge,distNode,distLimite;
	point                   normal,piede,tmp;
	vector<point>                    edge(2);
	pair<int, Real>		          result;
	
	// setto le tolleranze
	inSegment 	    inSeg;
	inSeg.setToll(toll);
	
	// ----------------------------------
	// 	   cerco il diametro 
	// ----------------------------------
	switch(GEOSHAPE::Shape)
	{
	    case(TRIANGLE):
			   distLimite = min(min((nodi[0]-nodi[1]).norm2(),(nodi[1]-nodi[2]).norm2()),(nodi[2]-nodi[0]).norm2());
			   distLimite = distLimite*fatt;
			   break;
	}
	
	// ---------------------------------
	// controllo se coincide con un nodo 
	// ---------------------------------
	for(UInt i=0; i<nodi.size(); ++i)
	{
	      // se coincidono 
	      if((nodi[i]-ptOriginal).norm2()<toll)
	      {
		  // setto l'id e lo metto nella lista 
		  pt.setId(id);
		  ptAss.push_back(pt);
		  
		  // riempio l'output
		  result.first  = VERTEX;
		  result.second = 0.0;
		  return(result);
	      }
	}		
	
	// -----------------------------------
	// controllo se coincide con la faccia  
	// -----------------------------------
	switch(GEOSHAPE::Shape)
	{
	    case(TRIANGLE):
			  inTriangle		    inTria;
			  inTria.setToll(toll);
			  
			  // calcolo la normale
			  normal = (nodi[1]-nodi[0])^(nodi[2]-nodi[0]);
			  normal.normalize();

			  // faccio il piede 
			  piede.setX(pt.getX()+(normal*(nodi[0]-pt))*normal.getX());
			  piede.setY(pt.getY()+(normal*(nodi[0]-pt))*normal.getY());
			  piede.setZ(pt.getZ()+(normal*(nodi[0]-pt))*normal.getZ());
			  
			  // calcolo la distanza 
			  dist = (ptOriginal-piede).norm2();
      
			  // se è dentro lo salvo 
			  if((dist < distLimite) && (inTria.isIn(&nodi, piede)))
			  {
			      piede.setId(id);
			      ptAss.push_back(piede);
			      
			      // riempio l'output
			      result.first  = FACE;
			      result.second = dist;
			      return(result);
			  }
			  break;
	}
	
	// ------------------------------
	// controllo se coincide gli edge   
	// ------------------------------
	switch(GEOSHAPE::Shape)
	{
	    case(TRIANGLE):
			  // setto le variabili 
			  distEdge = (nodi[0]-nodi[1]).norm2();
			  
			  // controllo gli edge 
			  for(UInt i=0; i<3; ++i)
			  {
			      // setto gli edge 
			      edge[0] = nodi[i%3];	edge[1] = nodi[(i+1)%3];
			      
			      // prendo la direzione della retta 
			      normal = (edge[1]-edge[0]);
			      normal.normalize();

			      // prendo la proiezione di p3-p1
			      piede = normal*((pt-edge[0])*normal)+edge[0];
			      
			      // calcolo la distanza 
			      dist = (ptOriginal-piede).norm2();

			      // controllo 
			      if((dist < distLimite) && (dist < distEdge) && inSeg.isIn(&edge, piede))
			      {
				  found   = true;
				  tmp 	   = piede;
				  distEdge = dist;
			      }				
			  }
			  
			  // se l'ho trovato 
			  if(found)
			  {
			      tmp.setId(id);
			      ptAss.push_back(tmp);
			      
			      // riempio l'output
			      result.first  = EDGE;
			      result.second = distEdge;
			      return(result);
			  }
			  break;
	}
	
	// ----------------------------
	// 	controllo i nodi 
	// ----------------------------
	found   = false;
	distNode = (nodi[0]-nodi[1]).norm2();
	for(UInt i=0; i<nodi.size(); ++i)
	{
	      // calcolo la distanza 
	      dist = (nodi[i]-ptOriginal).norm2();
	      
	      // se sono vicini
	      if((dist<distLimite) && (dist<distNode))
	      {
		  found   = true;
		  tmp      = nodi[i];
		  distNode = dist;
	      }
	}
	
	// se l'ho trovato 
	if(found)
	{
	      tmp.setId(id);
	      ptAss.push_back(tmp);
	      
	      // riempio l'output
	      result.first  = VERTEX;
	      result.second = distNode;
	      return(result);
	}
	
	// non ho trovato nulla
	result.first  = NULLGEO;
	result.second = 0.0;
	return(result);
}

template<typename GEOSHAPE> 
pair<int, Real> mementoElement<GEOSHAPE>::controlPoint(point pt, point ptOriginal, vector<UInt> * ass)
{
	// variabili in uso 
	bool 	  	            found=false;
	Real   dist,distEdge,distNode,distLimite;
	point                   normal,piede,tmp;
	vector<point>                    edge(2);
	pair<int, Real>		          result;
	
	// faccio un clear 
	ass->reserve(3);
	
	// setto le tolleranze
	inSegment 	    inSeg;
	inSeg.setToll(toll);
	
	// ----------------------------------
	// 	   cerco il diametro 
	// ----------------------------------
	switch(GEOSHAPE::Shape)
	{
	    case(TRIANGLE):
			   distLimite = min(min((nodi[0]-nodi[1]).norm2(),(nodi[1]-nodi[2]).norm2()),(nodi[2]-nodi[0]).norm2());
			   distLimite = distLimite*fatt;
			   break;
	}	
	
	// ---------------------------------
	// controllo se coincide con un nodo 
	// ---------------------------------
	for(UInt i=0; i<nodi.size(); ++i)
	{
	      // se coincidono 
	      if((nodi[i]-ptOriginal).norm2()<toll)
	      {		  
		  // riempio l'output
		  result.first  = VERTEX;
		  result.second = 0.0;
		  ass->clear();		ass->reserve(1);
		  ass->push_back(nodi[i].getId());
		  return(result);
	      }
	}		
	
	// -----------------------------------
	// controllo se coincide con la faccia  
	// -----------------------------------
	switch(GEOSHAPE::Shape)
	{
	    case(TRIANGLE):
			  inTriangle		    inTria;
			  inTria.setToll(toll);
			  
			  // calcolo la normale
			  normal = (nodi[1]-nodi[0])^(nodi[2]-nodi[0]);
			  normal.normalize();
			  			  
			  // faccio il piede 
			  piede.setX(pt.getX()+(normal*(nodi[0]-pt))*normal.getX());
			  piede.setY(pt.getY()+(normal*(nodi[0]-pt))*normal.getY());
			  piede.setZ(pt.getZ()+(normal*(nodi[0]-pt))*normal.getZ());
			  
			  // calcolo la distanza 
			  dist = (ptOriginal-piede).norm2();
			 
			  // se è dentro lo salvo 
			  if((dist < distLimite) && (inTria.isIn(&nodi, piede)))
			  {
			      // riempio l'output
			      result.first  = FACE;
			      result.second = dist;
			      ass->push_back(this->getId());
			      return(result);
			  }
			  break;
	}
	
	// ------------------------------
	// controllo se coincide gli edge   
	// ------------------------------
	switch(GEOSHAPE::Shape)
	{
	    case(TRIANGLE):
			  // setto le variabili 
			  distEdge = (nodi[0]-nodi[1]).norm2();
			  
			  // controllo gli edge 
			  for(UInt i=0; i<3; ++i)
			  {
			      // setto gli edge 
			      edge[0] = nodi[i%3];	edge[1] = nodi[(i+1)%3];
			      
			      // prendo la direzione della retta 
			      normal = (edge[1]-edge[0]);
			      normal.normalize();
			      			      
			      // prendo la proiezione di p3-p1
			      piede = normal*((pt-edge[0])*normal)+edge[0];
			      
			      // calcolo la distanza 
			      dist = (ptOriginal-piede).norm2();

			      // controllo 
			      if((dist < distLimite) && (dist < distEdge) && inSeg.isIn(&edge, piede))
			      {
				  found   = true;
				  tmp 	   = piede;
				  distEdge = dist;
				  ass->clear();		ass->reserve(2);
				  ass->push_back(edge[0].getId());
				  ass->push_back(edge[1].getId());
				  
			      }				
			  }
			  
			  // se l'ho trovato 
			  if(found)
			  {
			      // riempio l'output
			      result.first  = EDGE;
			      result.second = distEdge;
			      return(result);
			  }
			  break;
	}
	
	// ----------------------------
	// 	controllo i nodi 
	// ----------------------------
	found   = false;
	distNode = (nodi[0]-nodi[1]).norm2();
	for(UInt i=0; i<nodi.size(); ++i)
	{
	      // calcolo la distanza 
	      dist = (nodi[i]-ptOriginal).norm2();
	      
	      // se sono vicini
	      if((dist<distLimite) && (dist<distNode))
	      {
		  found   = true;
		  tmp      = nodi[i];
		  distNode = dist;
		  ass->clear();		ass->reserve(1);
		  ass->push_back(nodi[i].getId());
	      }
	}
	
	// se l'ho trovato 
	if(found)
	{
	      // riempio l'output
	      result.first  = VERTEX;
	      result.second = distNode;
	      return(result);
	}
	
	// non ho trovato nulla
	result.first  = NULLGEO;
	result.second = 0.0;
	ass->clear();
	return(result);
}

//
// operatori
//
template<typename GEOSHAPE> 
bool mementoElement<GEOSHAPE>::operator!=(const mementoElement<GEOSHAPE> &E) const
{
	return(this->graphItem::operator!=(E));	
}

template<typename GEOSHAPE> 
bool mementoElement<GEOSHAPE>::operator==(const mementoElement<GEOSHAPE> &E) const
{
	return(this->geoElement<GEOSHAPE>::operator==(E));
}

template<typename GEOSHAPE>
bool mementoElement<GEOSHAPE>::operator<(const mementoElement<GEOSHAPE> &E) const
{
	return(this->graphItem::operator<(E));	
}

//
// Metodi di stampa 
//
template<typename GEOSHAPE>
void mementoElement<GEOSHAPE>::print()
{
	std::cout << "Id         : " << this->getId() 		<< std::endl;
	std::cout << "GeoId      : " << this->getGeoId() 	<< std::endl;
	std::cout << "Num Points : " << this->getNumConnected() << std::endl;
	std::cout << "Nodes Id's : ";
	
	for(UInt i=0; i<this->getNumConnected(); ++i)		std::cout << this->getConnectedId(i) << " ";
	std::cout << std::endl;
	
	std::cout << "Punti dell'elemento" << std::endl;
	for(UInt i=0; i<nodi.size(); ++i)		nodi[i].print();
	
	std::cout << "Punti associati" << std::endl;
	for(UInt i=0; i<ptAss.size(); ++i)		ptAss[i].print();
}


}

#endif
