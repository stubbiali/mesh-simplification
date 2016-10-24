#ifndef GEOELEMENTSIZE_HPP_
#define GEOELEMENTSIZE_HPP_

#include <cassert>
#include <iostream>

#include "../geometry/geoElement.hpp"

namespace geometry
{

using namespace std;

/*! Classe derivata da geoElement che permette di ordinare gli elemneti in base alla loro "misura". Verrà utilizzata
principalmente per gli algoritmi di edge collapsing e splitting dove è necessario avere un ordinamento in base alle
caratteristiche dell'oggetto per effettuare le dovute semplificazioni.*/
	
template<typename GEOSHAPE> class geoElementSize : public geoElement<GEOSHAPE> 
{
	//
	// Variabili di classe 
	//
	public:
		
		/*! Questo tipo di quantità dipende fortemente dal tipo di GEOSHAPE con cui è templatizzata la classe sta all'utente calcolarla nel modo opportuno:
		- se GEOSHAPE -> (1D) ----> lunghezze  
		- se GEOSHAPE -> (2D) ----> aree 
		- se GEOSHAPE -> (3D) ----> volumi */
		Real size;
	//
	// Costruttori
	//	
	public:
		/*! Costruttore */
		geoElementSize();
			
		/*! Costruttore di copia */
		geoElementSize<GEOSHAPE> (const geoElementSize<GEOSHAPE> &E);
			
		/*! Operatore uguaglianza */
		geoElementSize<GEOSHAPE> operator=(const geoElementSize<GEOSHAPE> &E);
	//
	// Operatori
	//
	public:
		/*! Operatore non uguaglianza */
		bool operator!=(const geoElementSize<GEOSHAPE> &E) const;
		
		/*! Operatore non uguaglianza */
		bool operator==(const geoElementSize<GEOSHAPE> &E) const;
			
		/*! Operatore minore */
		bool operator<(const geoElementSize<GEOSHAPE> &E) const;
	
	//
	// Set/get
	//
	public:	
		/*! Funzione che permette di accedere al size dell'elemento */
		inline Real getGeoSize() const;
			
		/*! Funzione che permette di settare l'id geometrico */
		inline void setGeoSize(const Real & _size);

	//
	// Stampa
	//
	public:
		/*! Print to screen */
		void print();
};

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttori
//
template<typename GEOSHAPE> geoElementSize<GEOSHAPE>::geoElementSize() : geoElement<GEOSHAPE>::geoElement() 
{
	size = 0.0;
}

template<typename GEOSHAPE> geoElementSize<GEOSHAPE>::geoElementSize(const geoElementSize<GEOSHAPE> &E) : geoElement<GEOSHAPE>(E)
{
	size = E.size;
}

template<typename GEOSHAPE> geoElementSize<GEOSHAPE> geoElementSize<GEOSHAPE>::operator=(const geoElementSize<GEOSHAPE> &E) 
{	
	this->geoElement<GEOSHAPE>::operator=(E);
	size  = E.size;
	return *this;
}

//
// Get/Set
//	
template<typename GEOSHAPE> inline Real geoElementSize<GEOSHAPE>::getGeoSize() const
{
	return(size);
}

template<typename GEOSHAPE> inline void geoElementSize<GEOSHAPE>::setGeoSize(const Real & _size)
{
	size= _size;
}

//
// operatori
//
template<typename GEOSHAPE> bool geoElementSize<GEOSHAPE>::operator!=(const geoElementSize<GEOSHAPE> &E) const
{
	if(E.size==this->size) 	return(this->graphItem::operator!=(E));
	else			return(true);
	
}

template<typename GEOSHAPE> bool geoElementSize<GEOSHAPE>::operator==(const geoElementSize<GEOSHAPE> &E) const
{
	return(this->geoElement<GEOSHAPE>::operator==(E));
	
}

template<typename GEOSHAPE> bool geoElementSize<GEOSHAPE>::operator<(const geoElementSize<GEOSHAPE> &E) const
{
	if(this->size==E.size) 	return(this->graphItem::operator<(E));	
	else			return(this->size<E.size);
}

//
// Stampa
//
template<typename GEOSHAPE> void geoElementSize<GEOSHAPE>::print()
{
	cout << "Id         : " << this->getId() << endl;
	cout << "GeoId      : " << this->getGeoId() << endl;
	cout << "Size       : " << size << endl;
	cout << "Num Points : " << this->getNumConnected() << endl;
	cout << "Nodes Id's : ";
	
	for(UInt i=0; i<this->getNumConnected(); ++i)
	{
		cout << geoElement<GEOSHAPE>::getConnectedId(i) << " ";
	}
	cout << endl;
}

}

#endif
