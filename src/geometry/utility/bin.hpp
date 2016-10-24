#ifndef bin_HPP_
#define bin_HPP_

#include <algorithm>
#include <vector>

#include "../core/shapes.hpp"

namespace geometry
{

/*! Questa classe permette di creare un cestino che contiene un tipo di elementi. Gli elementi contenuti "rispettano" l'etichetta 
del cestino. L'etichetta del cestino e gli oggetti contenuti nel cestino sono parametri template della classe. L'etichetta deve 
aver definito l'operazione di "<" gli elementi in ingresso devono aver definito l'operazione di "==" */

template<class ELEMENT, class TAG> class bin
{
      //
      // Variabili di classe 
      //
      public:
		/*! Etichetta*/
		TAG			 	           tag;
		
		/*! Vettore che contiene gli elementi */
		// lo setto a mutable perchè è possibile che cambi durante la procedura
		mutable vector<ELEMENT>	      elements;
      
      
      //
      // Costruttore e setting
      //
      public:
		/*! Costruttore vuoto */
		bin();
		
		/*! Metodo che setta il tag 
		    \param _tag parametro */
		inline void setTag(TAG _tag);
		
		/*! Metodo che prende il tag */
		inline TAG getTag() const;
		
		/*! Metodo che ritorna quanti elementi sono contenuti*/
		inline UInt getNumElements() const;
		
		/*! Metodo che permette di sapere se l'insieme è vuoto*/
		inline bool isEmpty() const;
		
		
      //
      // Metodi per modificare la griglia
      //
      public:
		/*! Metodo che ritorna vero se l'elemento è contenuto 
		    \param id identificatore dell'elemento da cercare*/
		bool isIn(UInt id) const;
		
		/*! Metodo che ritorna vero se l'elemento è contenuto 
		    \param elem elemento che devo inserire*/
		bool isIn(ELEMENT elem) const;
		
		/*! Metodo che inserisce un elemento 
		    \param elem elemento che devo inserire */
		void insertElement(ELEMENT elem) const;
		
		/*! Metodo che permette di ottenere un elemento 
		    \param id identificatore dell'elemento da cercare*/
		ELEMENT getElement(UInt id) const;
		
		/*! Metodo che permette di ottenere un elemento 
		    \param id identificatore dell'elemento da cercare*/
		UInt getPosition(UInt id) const;
		
		/*! Metodo che permette di ottenere il primo elemento del vettore */
		inline ELEMENT getFirstElement() const;
		
		/*! Metodo che permette di eliminare un elemento 
		    \param id identificatore dell'elemento da eliminare*/
		void eraseElement(UInt id) const;
		
		/*! Metodo che permette di eliminare un elemento 
		    \param elem elemento da eliminare*/
		void eraseElement(ELEMENT elem) const;
      
      //
      // Definizione degli operatori 
      //
      public:
		/*! Meotodo che fa il confronto */
		bool operator<(const bin<ELEMENT,TAG> &binExt) const;
      //
      // Metodo per debug
      //
      public:
		/*! Stampa */
		void print() const;
};

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttore e setting
//
template<class ELEMENT, class TAG>
bin<ELEMENT,TAG>::bin()
{
}

template<class ELEMENT, class TAG>
inline void bin<ELEMENT,TAG>::setTag(TAG _tag)
{
    tag = _tag;
}

template<class ELEMENT, class TAG>
inline TAG bin<ELEMENT,TAG>::getTag() const
{
    return(tag);
}

template<class ELEMENT, class TAG>
inline UInt bin<ELEMENT,TAG>::getNumElements() const
{
    return(elements.size());
}

template<class ELEMENT, class TAG>
inline bool bin<ELEMENT,TAG>::isEmpty() const
{
    return(elements.size()==0);
}

//
// Metodi per modificare la griglia
//
template<class ELEMENT, class TAG>
inline bool bin<ELEMENT,TAG>::isIn(ELEMENT elem) const
{
    return(find(elements.begin(), elements.end(), elem)!=elements.end());
}

template<class ELEMENT, class TAG>
bool bin<ELEMENT,TAG>::isIn(UInt id) const
{
    // varaibili in uso 
    typename vector<ELEMENT>::iterator it;
    ELEMENT 			     elem;
    
    // setto il suo id 
    elem.setId(id);
    
    // lo cerco
    it = find(elements.begin(), elements.end(), elem);
    
    // lo trovo errore elimino 
    if(it!=elements.end())	return(true);
    
    return(false);
}

template<class ELEMENT, class TAG>
void bin<ELEMENT,TAG>::insertElement(ELEMENT elem) const
{
     if(isIn(elem))	cout << "ATTENZIONE: elemento già contenuto" << endl;
     else		elements.push_back(elem);
}

template<class ELEMENT, class TAG>
ELEMENT bin<ELEMENT,TAG>::getElement(UInt id) const
{
    // varaibili in uso 
    typename vector<ELEMENT>::iterator it;
    ELEMENT 			     elem;
    
    // setto il suo id 
    elem.setId(id);
    
    // lo cerco
    it = find(elements.begin(), elements.end(), elem);
    
    // lo trovo errore elimino 
    if(it!=elements.end())	return(*it);
    
    cout << "ATTENZIONE: non ho trovato l'elemento" << endl;
    return(elem);
}

template<class ELEMENT, class TAG>
UInt bin<ELEMENT,TAG>::getPosition(UInt id) const
{
    return(elements[id].getId());
}

template<class ELEMENT, class TAG>
inline ELEMENT bin<ELEMENT,TAG>::getFirstElement() const
{
    return(elements[0]);
}

template<class ELEMENT, class TAG>
void bin<ELEMENT,TAG>::eraseElement(UInt id) const
{
    // variabili in uso 
    typename vector<ELEMENT>::iterator it;
    ELEMENT 			     elem;
    
    // setto il suo id 
    elem.setId(id);
    
    // lo cerco
    it = find(elements.begin(), elements.end(), elem);
    
    // lo trovo errore elimino 
    if(it!=elements.end())	elements.erase(it);
    else			cout << "ATTENZIONE: l'elemento " << id << " non è presente" << endl;
}

template<class ELEMENT, class TAG>
void bin<ELEMENT,TAG>::eraseElement(ELEMENT elem) const
{
    eraseElement(elem.getId());
}
  
//
// Definizione degli operatori 
//
template<class ELEMENT, class TAG>
bool bin<ELEMENT,TAG>::operator<(const bin<ELEMENT,TAG> &binExt) const
{
    return(tag<binExt.getTag());
}

//
// Metodo per debug
//
template<class ELEMENT, class TAG>
void bin<ELEMENT,TAG>::print() const
{
    if(elements.size()==0)	
    {
	cout << "Il cestino " << tag << " è vuoto" << endl;
	return;
    }
    
    cout << "Il cestino " << tag << " contiene:" <<endl;
    for(UInt i=0; i<elements.size(); ++i)	elements[i].print();
}





}





#endif
