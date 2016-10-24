#ifndef GRAPHITEM_H_
#define GRAPHITEM_H_

#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

#include "../core/shapes.hpp"
#include "../core/point.h"

namespace geometry
{

using namespace std;

/*! Classe che implementa la connessione di un generico elemento con un set di elementi. L'esempio più semplice è un elemento di griglia connesso al suo set di nodi. La classe contiene un Id che identifica l'elemento e una lista di interi che identifica gli id degli elementi connessi. E' una classe generale in grado di implementare un grafo generico. */

class graphItem
{ 
	public:
		/*! Lista Id di elementi connessi */
		vector<UInt> connected;
		
		/*! Id identificativo dell'elemento */
		UInt Id;
	//
	// Costruttori
	//
	public:
		/*! Costruttore */
		graphItem();
		
		/*! Costruttore
		  \param N il numero di elementi connessi */
		graphItem(UInt N);
		
		/*! Costruttore
		  \param values vettore che contiene gli Id degli elementi connessi
		  \param id id dell'elemento*/
		graphItem(const vector<UInt> & values, const int & id);
		
		/*! Costruttore di copia 
		  \param G referenza di un altro grafo */
		graphItem(const graphItem &G);
		
		/*! Operatore uguaglianza 
		  \param G referenza di un altro grafo */
		graphItem & operator=(const graphItem &E);
		
	//
	// Metodi di get
	//
	public:
		/*! Return the Id */
		inline UInt getId() const;
			
		/*! Return the id i-th node 
		    \param i nodo che si vuole ottenere */
		inline UInt getConnectedId(const UInt & i) const;
		
		/*! Restituzione della lista degli Id connessi */
		inline const vector<UInt> & getConnectedIds() const;
			
		/*! Return the number of the points */
		inline UInt getNumConnected() const;
		
		/*! Metodo che libera le variabili */
		inline void clear();
	
	//
	// Metodi di set
	//
		/*! Set the element id */
		inline void setId(const UInt & id);
			
		/*! Set the id of the i-th node */
		inline void setConnectedId(const UInt & i, const UInt & value);
		
		/*! Aggiunge un elemento connesso */
		inline void connectedPushBack(const UInt & value);
		
		/*! Ridimensionamento del numero di elementi connessi */
		inline void connectedResize(const UInt & dim);
		
		/*! Setto tutti i connessi */
		void setConnectedId(vector<UInt> * ids);
		
	//
	// Metodi di rimozione e sostituzione
	//
		/*! Aggiunge al grafo un particolare valore controllando che non sia gà presente */
		void add(UInt value);
		
		/*! Aggiunge al grafo un particolare valore controllando che non sia gà presente */
		void add(vector<UInt> & value);
		
		/*! Cerca e rimuove l'elemento con un particolare valore */
		void remove(UInt value);
		
		/*! Cerca e cambia l'elemento con un particolare valore */
		void change(UInt oldValue, UInt newValue);
		
		/*! Metodo che inverte */
		void invert();
	//
	// Metodi per trovare gli id comuni e no
	//
		/*! Riempie il vettore puntato con gli id comuni */
		void common(const graphItem& E, vector<UInt> * com);
		
		/*! Riempie il vettore puntato con gli id comuni */
		void unCommon(const graphItem& E, vector<UInt> * com);
	
	
	//
	// Ridefinizione opertatori
	//
	public:
		/*! The "less" operator: the nodes are ascending-ordered. The first "less" index wins */
		bool operator<(const graphItem &E) const;
			
		/*! The inequality operator*/
		bool operator!=(const graphItem &E) const;
		
		/*! The equality operator*/
		bool operator==(const graphItem &E) const;
			
	//
	// Metodo di stampa 
	//
	public:
		/*! Print to screen */
		void print();
};

//-----------------------------------------------------------------------------
// INLINE FUNCTIONS
//-----------------------------------------------------------------------------

// ------------
// get function
// ------------
inline UInt graphItem::getId() const
{
	return(Id);
}

inline UInt graphItem::getConnectedId(const UInt & i) const
{
	assert(i < connected.size());
	return(connected[i]);
}

inline UInt graphItem::getNumConnected() const
{
	return(connected.size());
}

inline const vector<UInt> & graphItem::getConnectedIds() const
{
	return(connected);
}

inline void graphItem::clear()
{
	Id = 0;
	connected.clear();
}

// ------------
// set function
// ------------

inline void graphItem::setId(const UInt & id)
{
	Id = id;
}

inline void graphItem::setConnectedId(const UInt & i, const UInt & value)
{
	assert(i < connected.size());
	connected[i] = value;
}

inline void graphItem::connectedPushBack(const UInt & value)
{
	connected.push_back(value);
}
	
inline void graphItem::connectedResize(const UInt & dim)
{
	connected.resize(dim);
}




}

#endif
