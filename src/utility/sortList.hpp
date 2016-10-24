#ifndef sortList_HPP_
#define sortList_HPP_

#include "../core/shapes.hpp"

#include <vector>
#include <map>
#include <set>
#include <iterator>

namespace geometry
{

/*! Questa classe permette di creare una lista di elementi ordinata secondo un criterio di ordine definito dall'operatore "<".
Inoltre include le seguenti funzionalità
<ol>
<li> aggiungere elementi;
<li> togliere elementi;
<li> modificare elementi;
<li> estrarre l'elemento alla posizione k-esima
</ol>

La classe è templatizzata in base alla classe ELEMENT che deve contenere al suo interno 
<ol>
<li> il metodo "getId()" che contiene l'id dell'elemento;
<li> la ridefinizione dell'operatore "<";
</ol>

*/

template<class ELEMENT> class sortList
{
      //
      // Variabili di classe 
      //
      public:	
		/*! Vettore che contiene gli elementi */
		set<ELEMENT>			  lista;
		
		/*! Set che contiene gli id */
		map<UInt,ELEMENT>		 id2Ele;
      //
      // Costruttore e setting
      //
      public:
		/*! Costruttore vuoto */
		sortList();
		
		/*! Metodo che pulisce le liste */
		void clear();
		
		/*! Metodo setta le variabili della classe partendo da liste  
		    \param lista puntatore alla lista che si vuole analizzare */
		void setElementVector(vector<ELEMENT> * _lista);
		void setElementVector(set<ELEMENT>    * _lista);	
      //
      // Metodo che prende i mediani e gli altri elementi 
      //
      public:
		/*! Trovo il primo */
		inline UInt findMin();
		
		/*! Metodo che ritorna la mediana k-esima
		    \param k posizione */
		UInt findKMedian(UInt k);
		
		/*! Metodo che ritorna la mediana */
		inline UInt findMedian();
		
		/*! Trovo il l'ulitmo */
		inline UInt findMax();
		
		/*! Controllo se un elemento è presente 
		    \param elemId identificatore dell'elemento */
		inline bool isIn(UInt elemId);
		
		/*! Metodo che stabilisce se la lista è vuota */
		inline bool isEmpty();
		
		/*! Metodo per prendere un elemento */
		inline ELEMENT getElement(UInt elemId);
		
		/*! Ottengo il suo geoSize 
		    \param elemId identificatore dell'elemento */
		inline Real getGeoSize(UInt elemId);
      //
      // Metodi per modificare il vettore 
      //
      public:		
		/*! Metodo che modifica il geoSize dell'elemento 
		    \param elemId identificatore dell'elemento 
		    \param val valore da sostituire */
		void change(UInt elemId, Real val);
		
		/*! Metodo che aggiunge un elemento
		    \param toAdd puntatore a un elemento */
		inline void add(ELEMENT * toAdd);
		
		/*! Metodo che rimuove un elemento 
		    \param toDelete elemento */
		void remove(UInt elemId);
      //
      // Metodo di stampa 
      //
      public:
		/*! Metodo che stampa il contenuto del vettore */
		void print();
};

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttore e setting
//
template<class ELEMENT>
sortList<ELEMENT>::sortList()
{
}

template<class ELEMENT>
void sortList<ELEMENT>::clear()
{
    // pulisco le lista e le mappe 
    lista.clear();
    id2Ele.clear();
}

template<class ELEMENT>
void sortList<ELEMENT>::setElementVector(vector<ELEMENT> * _lista)
{
    // pulisco 
    clear();
    
    // se è vuota do un messaggio di errore
    if(_lista->size()==0)
    {
	cout << "Il vettore che hai passato a sortList è vuoto" << endl;
	return;
    }
    
    // per ogni elemento della lista creo la mappa e lo metto nel vettore 
    for(UInt i=0; i<_lista->size();  ++i)
    {	  
	  // lo metto nella lista 
	  lista.insert(_lista->at(i));
	  
	  // faccio le associazioni 
	  id2Ele[_lista->at(i).getId()] = _lista->at(i);
    }
}

template<class ELEMENT>
void sortList<ELEMENT>::setElementVector(set<ELEMENT> * _lista)
{    
    // se è vuota do un messaggio di errore
    if(_lista->size()==0)
    {
	cout << "Il set che hai passato a sortList è vuoto" << endl;
	return;
    }
    
    // creo un vettore 
    vector<ELEMENT>	 tmp(_lista->size());
    copy(_lista->begin(),_lista->end(), tmp.begin());
    
    // ciamo l'altro metodo 
    setElementVector(&tmp);
}

//
// Metodo che prende i mediani 
//
template<class ELEMENT>
inline UInt sortList<ELEMENT>::findMin()
{
    return(lista.begin()->getId());
}

template<class ELEMENT>
UInt sortList<ELEMENT>::findKMedian(UInt k)
{
    // variabili 
    typename set<ELEMENT>::iterator it=lista.begin();
    
    // se k è troppo alto stampo errore
    if(k>=lista.size())		
    {
	cout << "Mi hai dato un k troppo alto" << endl;
	return(-1);
    }
    
    // prendo l'iteratore 
    advance(it, k);
    
    // ritorno l'id
    return(it->getId());
}

template<class ELEMENT>
inline UInt sortList<ELEMENT>::findMedian()
{
    return(findKMedian(static_cast<UInt>(lista.size()*0.5)));
}

template<class ELEMENT>
inline UInt sortList<ELEMENT>::findMax()
{
    return(lista.rbegin()->getId());
}

template<class ELEMENT>
inline bool sortList<ELEMENT>::isIn(UInt elemId)
{
    return(id2Ele.find(elemId)!=id2Ele.end());
}

template<class ELEMENT>
inline bool sortList<ELEMENT>::isEmpty()
{
    return(lista.size()==0);
}

template<class ELEMENT>
inline Real sortList<ELEMENT>::getGeoSize(UInt elemId)
{
    // controllo che il punto ci sia
    if(!isIn(elemId))
    {
	cout << "L'Elemento non è presente" << endl;
	return(-1.0);
    }
  
    return(id2Ele[elemId].getGeoSize());
}

template<class ELEMENT>
inline ELEMENT sortList<ELEMENT>::getElement(UInt elemId)
{
    // controllo che il punto ci sia
    if(!isIn(elemId))
    {
	cout << "L'Elemento non è presente" << endl;
	return(*lista.begin());
    }
  
    return(id2Ele[elemId]);
}

//
// Metodi per modificare il vettore 
//
template<class ELEMENT>
void sortList<ELEMENT>::change(UInt elemId, Real val)
{
    // varaibili in uso 
    ELEMENT    nuovo;
    
    // controllo che il punto ci sia
    if(!isIn(elemId))
    {
	cout << "L'Elemento NON è presente e non può essere cambiato!" << endl;
	return;
    }
    
    // setto la mappa
    nuovo = id2Ele[elemId];
    
    // rimuovo l'elemento 
    remove(elemId);
    
    // setto il size
    nuovo.setGeoSize(val);
    
    // lo aggiungo 
    add(&nuovo);
}

template<class ELEMENT>
void sortList<ELEMENT>::add(ELEMENT * toAdd)
{
    // variabili in uso 
    ELEMENT         nuovo;
    
    // creo un nuovo elemento 
    nuovo = *toAdd;
    
    // controllo che il punto ci sia
    if(isIn(nuovo.getId()))
    {
	cout << "L'Elemento è già presente" << endl;
	return;
    }
    
    // lo metto nella lista 
    lista.insert(nuovo);
    
    // setto la mappa
    id2Ele[nuovo.getId()]=nuovo;
}

template<class ELEMENT>
void sortList<ELEMENT>::remove(UInt elemId)
{
    // variabili in uso 
    ELEMENT         toRemove;
    
    // controllo che il punto ci sia
    if(!isIn(elemId))
    {
	cout << "L'Elemento NON è presente e non può essere rimosso!" << endl;
	return;
    }
    
    // tolgo dalla mappa 
    toRemove = id2Ele[elemId];
    
    // rimuovo dalla lista 
    lista.erase(toRemove);
    
    // rimuovo l'associazione 
    id2Ele.erase(elemId);
}

//
// Metodi di stampa
//
template<class ELEMENT>
void sortList<ELEMENT>::print()
{    
    // variabili in uso 
    UInt cont=0;
    
    // stampo il contenuto
    cout << "CONTENUTO" << endl;
    for(typename set<ELEMENT>::iterator it=lista.begin(); it!=lista.end(); ++it)
    {
	cout << "Posizione: "      << cont << "         ";
	cout << "Identificatore: " << it->getId() << "         ";
	cout << "Valore: "         << it->getGeoSize() << endl;
	
	// incremento cont
	++cont;
    }
}

}

#endif

