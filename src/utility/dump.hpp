#ifndef dump_HPP_
#define dump_HPP_

#include <algorithm>
#include <vector>
#include <set>
#include <ctime>

#include "../core/shapes.hpp"

#include "../utility/bin.hpp"

namespace geometry
{

/*! Questa classe permette di collezionare una serie di classi di tipo bin organizzandoli in un set della STL ordinandoli in base 
all'operazione "<" definita sul parametro TAG *namespace geometry*/

template<class ELEMENT, class TAG> class dump
{
      //
      // Variabili di classe 
      //
      public:
		/*! Set che contiene tutti gli elementi */
		set<bin<ELEMENT,TAG> >	        allBins;
      
      //
      // Costruttore e setting
      //
      public:
		/*! Costruttore vuoto */
		dump();
		
		/*! Pulisco l'insieme*/
		inline void clear();
		
		/*! Metodo che ritorna l'identificatore dell'elemento più piccolo*/
		UInt getMin();
		
		/*! Metodo che ritorna un elemento alla posizione pos partendo dal più piccolo 
		    \param id identificatore dell'elemento */
		UInt getMin(UInt pos);
		
		/*! Metodo che ritorna l'identificatore dell'elemento più piccolo*/
		UInt getMax();
		
		/*! Metodo che dice se il vettore contiene elementi */
		bool isEmpty();
		
		/*! Metodo che controlla se c'è un elemento 
		    \param id identificatore */
		bool isIn(UInt id);
		
		/*! Metodo che ritorna l'elemento con un id particolare
		    \param id identificatore */
		ELEMENT getElement(UInt id);
		
      //
      // Metodi che cambiano
      //
      public:	
		/*! Metodo che aggiunge un elemento 
		    \param elem elemento da inserire 
		    \param tag tag dell'elemento*/
		void add(ELEMENT elem, TAG tag);
		
		/*! Metodo che elimina un elemento 
		    \param elem elemento da inserire 
		    \param tag tag dell'elemento*/
		void remove(ELEMENT elem, TAG tag);
		
		/*! Metodo che elimina un elemento 
		    \param elem elemento da inserire 
		    \param oldtag vecchio tag dell'elemento
		    \param newtag nuovo tag dell'elemento*/
		void change(ELEMENT elem, TAG oldtag, TAG newtag);
      //
      // Metodi per il debug
      //
      public:
		/*! Stampa */
		void print();
};

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttore
//
template<class ELEMENT, class TAG>
dump<ELEMENT,TAG>::dump()
{
}

template<class ELEMENT, class TAG>
inline void dump<ELEMENT,TAG>::clear()
{
      allBins.clear();
}

template<class ELEMENT, class TAG>
UInt dump<ELEMENT,TAG>::getMin()
{
      // se è vuoto ritorno 0
      if(allBins.size()==0)	
      {
	cout << "L'insieme non contiene nessun elemento" << endl;
	return(0);
      }
      
      // passo in rassegna tutti gli elementi 
      for(typename set<bin<ELEMENT,TAG> >::iterator it=allBins.begin(); it!=allBins.end(); ++it)
	  if(it->getNumElements()!=0)
	      return(it->getFirstElement().getId());
      
      
      cout << "I vettori contenuti non hanno nessun elemento" << endl;	
      return(0);
}

template<class ELEMENT, class TAG>
UInt dump<ELEMENT,TAG>::getMin(UInt pos)
{
      // variabili in uso
      UInt 	     cont=0,tot=0;
      
      // se è vuoto ritorno 0
      if(allBins.size()==0)	
      {
	cout << "L'insieme non contiene nessun elemento" << endl;
	return(0);
      }
      
      // conto tutti gli elementi 
      for(typename set<bin<ELEMENT,TAG> >::iterator it=allBins.begin(); it!=allBins.end(); ++it)
	 tot+= it->getNumElements();
      
      // se prendo una posizione troppo alta ritonrno il massimo 
      if(pos>=tot)
      {
	  cout << "Vuoi un elemento a una posizione troppo alta" << endl;
	  return(getMax());
      }
      
      // passo in rassegna tutti gli elementi 
      for(typename set<bin<ELEMENT,TAG> >::iterator it=allBins.begin(); it!=allBins.end(); ++it)
      {
	  for(UInt i=0; i<it->getNumElements(); ++i)
	  {
	      ++cont;
	      if(cont==pos)	return(it->getPosition(i));
	  }
      }
      
      // ritorno
      return(getMax());
}


template<class ELEMENT, class TAG>
UInt dump<ELEMENT,TAG>::getMax()
{
      // se è vuoto ritorno 0
      if(allBins.size()==0)	
      {
	cout << "L'insieme non contiene nessun elemento" << endl;
	return(0);
      }
      
      // passo in rassegna tutti gli elementi 
      for(typename set<bin<ELEMENT,TAG> >::reverse_iterator it=allBins.rbegin(); it!=allBins.rend(); ++it)
	  if(it->getNumElements()!=0)
	      return(it->getFirstElement().getId());
      
      
      cout << "I vettori contenuti non hanno nessun elemento" << endl;	
      return(0);
}

template<class ELEMENT, class TAG>
bool dump<ELEMENT,TAG>::isEmpty()
{
      // controllo in ogni cestino 
      for(typename set<bin<ELEMENT,TAG> >::iterator it=allBins.begin(); it!=allBins.end(); ++it)
	  if(!it->isEmpty())
	      return(false);
      
      return(true);
}

template<class ELEMENT, class TAG>
bool dump<ELEMENT,TAG>::isIn(UInt id)
{
      // controllo in ogni cestino 
      for(typename set<bin<ELEMENT,TAG> >::iterator it=allBins.begin(); it!=allBins.end(); ++it)
	  if(it->isIn(id))	
	      return(true);
  
      // se non lo trovo ritorno falso
      return(false);
}

template<class ELEMENT, class TAG>
ELEMENT dump<ELEMENT,TAG>::getElement(UInt id)
{
      // controllo in ogni cestino 
      for(typename set<bin<ELEMENT,TAG> >::iterator it=allBins.begin(); it!=allBins.end(); ++it)
	  if(it->isIn(id))	
	      return(it->getElement(id));
  
      // se non lo trovo ritorno falso
      cout << "ATTENZIONE nel metodo getElement di dump non ho trovato l'elemento " << id << endl;
      ELEMENT elem;
      return(elem);
}

//
// Metodi che cambiano
//
template<class ELEMENT, class TAG>
void dump<ELEMENT,TAG>::add(ELEMENT elem, TAG tag)
{
      // variabile temporanea 
      typename set<bin<ELEMENT,TAG> >::iterator   it;
      bin<ELEMENT,TAG>                        binTmp;
      
      // setto il tag
      binTmp.setTag(tag);
      
      // la cerco 
      it = allBins.find(binTmp);
      
      // se lo trovo metto l'elemento in quel vettore 
      if(it==allBins.end())
      {
	  // metto l'elemento 
	  binTmp.insertElement(elem);
	  
	  // lo aggiungo al set
	  allBins.insert(binTmp);
      }
      else
      {
	  // cerco l'elemento 
	  if(it->isIn(elem))	  cout << "ATTENZIONE: l'elemento che cerco di aggiungere in dump è già presente " << endl;
	  else			  it->insertElement(elem);
      }
}

template<class ELEMENT, class TAG>
void dump<ELEMENT,TAG>::remove(ELEMENT elem, TAG tag)
{
      // variabile temporanea 
      typename set<bin<ELEMENT,TAG> >::iterator   it;
      bin<ELEMENT,TAG>                        binTmp;
      
      // setto il tag
      binTmp.setTag(tag);
      
      // la cerco 
      it = allBins.find(binTmp);
      
      // se lo trovo metto l'elemento in quel vettore 
      if(it!=allBins.end())
      {
	  // cerco l'elemento 
	  if(!it->isIn(elem))	  cout << "ATTENZIONE: l'elemento che cerco di togliere in dump non è presente " << endl;
	  else			  it->eraseElement(elem);
      }
      else
      {
	  // non trovo nemmeno il tag
	  cout << "ATTENZIONE: nel dump non c'è nemmeno questo tag" << endl;
      }
}

template<class ELEMENT, class TAG>
void dump<ELEMENT,TAG>::change(ELEMENT elem, TAG oldtag, TAG newtag)
{
      // variabile temporanea 
      typename set<bin<ELEMENT,TAG> >::iterator   it;
      bin<ELEMENT,TAG>                        binTmp;
      
      // setto il tag
      binTmp.setTag(oldtag);
      
      // la cerco 
      it = allBins.find(binTmp);
      
      // se lo trovo metto l'elemento in quel vettore 
      if(it!=allBins.end())
      {
	  // cerco l'elemento se lo trovo se i due tag sono diversi lo tolgo e lo rimetto 
 	  if(!it->isIn(elem))	  
	  {
	      cout << "ATTENZIONE: l'elemento che voglio cambiare non è presente" << endl;
	  }
	  else
	  {
	      // lo tolgo 
	      it->eraseElement(elem);
	      
	      // lo aggiungo 
	      add(elem, newtag);
	  }
      }
      else
      {
	  // non trovo nemmeno il tag
	  cout << "ATTENZIONE: nel dump non c'è nemmeno questo tag" << endl;
      }
}

//
// Metodi per il debug
//
template<class ELEMENT, class TAG>
void dump<ELEMENT,TAG>::print()
{
    if(allBins.size()==0)	
    {
	cout << "L'insieme di bin è vuoto" << endl;
	return;
    }
    
    cout << "L'insieme di bin contiene " << allBins.size() << " elementi:" <<endl;
    for(typename set<bin<ELEMENT,TAG> >::iterator it=allBins.begin(); it!=allBins.end(); ++it)	it->print();
}


  
}






#endif
