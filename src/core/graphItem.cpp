#include "graphItem.h"

using namespace geometry;

//
// Costruttori
//

graphItem::graphItem()
{
	Id = 0;
}

graphItem::graphItem(UInt N)
{
	Id = 0;
	connected.resize(N);
}

graphItem::graphItem(const vector<UInt> & values, const int & id)
{
	Id = id;
	connected = values;
}

graphItem::graphItem(const graphItem &G)
{
	Id = G.Id;
	connected = G.connected;
}

graphItem & graphItem::operator=(const graphItem &E)
{
	connected = E.connected;
	Id = E.Id;
	
	return *this;
}

		
//
// Metodi di get/set
//

void graphItem::setConnectedId(vector<UInt> * ids)
{
      // libero la lista connected
      connected.clear();
      
      // faccio un resize
      connected.resize(ids->size());
      
      // copio il contenuto
      std::copy(ids->begin(), ids->end(), connected.begin());
}

//
// Ridefinizione degli operatori
//

bool graphItem::operator<(const graphItem &E) const
{
	// Variabili in uso 
	vector<UInt>         internalNodes, externalNodes;
	vector<UInt>::iterator internalIter, externalIter;
	
	// prendo i connessi 
	internalNodes = this->getConnectedIds();
	externalNodes = E.getConnectedIds();
	
	// faccio un sort
	sort(internalNodes.begin(), internalNodes.end());
	sort(externalNodes.begin(), externalNodes.end());
	
	// setto gli iteratori 
	internalIter = internalNodes.begin();
	externalIter = externalNodes.begin();
	
	// controllo 
	for(; internalIter!=internalNodes.end();)
	{
		// confonto 
		if(*internalIter < *externalIter) 	return(true);
		else if(*internalIter > *externalIter)  return(false);
	
		//incremento 
		internalIter++;
		externalIter++;
	}
	
	return(false);
}	
	
bool graphItem::operator!=(const graphItem &E) const
{
	assert(E.connected.size() == connected.size());
	
	// Variabili in uso 
	bool                                  flag = true;
	vector<UInt>         internalNodes, externalNodes;
	vector<UInt>::iterator internalIter, externalIter;
	
	// prendo i connessi 
	internalNodes = this->getConnectedIds();
	externalNodes = E.getConnectedIds();
	
	// faccio un sort
	sort(internalNodes.begin(), internalNodes.end());
	sort(externalNodes.begin(), externalNodes.end());
	
	// setto gli iteratori 
	internalIter = internalNodes.begin();
	externalIter = externalNodes.begin();
	
	// controllo 
	for(; internalIter!=internalNodes.end();)
	{
		// confonto 
		flag = flag && (*internalIter == *externalIter);
	
		//incremento 
		internalIter++;
		externalIter++;
	}
	
	return(!flag);
}

bool graphItem::operator==(const graphItem &E) const
{
	assert(E.connected.size() == connected.size());
	
	// Variabili in uso 
	bool                                  flag = true;
	vector<UInt>         internalNodes, externalNodes;
	vector<UInt>::iterator internalIter, externalIter;
	
	// prendo i connessi 
	internalNodes = this->getConnectedIds();
	externalNodes = E.getConnectedIds();
	
	// faccio un sort
	sort(internalNodes.begin(), internalNodes.end());
	sort(externalNodes.begin(), externalNodes.end());
	
	// setto gli iteratori 
	internalIter = internalNodes.begin();
	externalIter = externalNodes.begin();
	
	// controllo 
	for(; internalIter!=internalNodes.end();)
	{
		// confonto 
		flag = flag && (*internalIter == *externalIter);
	
		//incremento 
		internalIter++;
		externalIter++;
	}
	
	return(flag);
}

//
// Metodi di rimozione e sostituzione
//

void graphItem::add(UInt value)
{
      // variabili in uso 
      vector<UInt>           tmp;
      vector<UInt>::iterator  it;
      
      // gli metto tutti i numeri 
      tmp.assign(connected.size()+1, 0);
      
      // metto il valore da aggiungere 
      std::copy(connected.begin(), connected.end(), tmp.begin());
      
      // aggiungo all'ultima uscita
      tmp[tmp.size()-1] = value;
      
      // faccio un sort e poi unique
      std::sort(tmp.begin(), tmp.end());
      it = unique(tmp.begin(), tmp.end());
      
      // metto a posto connected
      connected.assign(it-tmp.begin(), 0);
      std::copy(tmp.begin(), it, connected.begin());
}

void graphItem::add(vector<UInt> & value)
{
      // variabili in uso 
      vector<UInt>           tmp;
      vector<UInt>::iterator  it;
      
      // gli metto tutti i numeri 
      tmp.assign(connected.size()+value.size(), 0);
      
      // metto il valore da aggiungere 
      std::copy(connected.begin(), connected.end(), tmp.begin());
      
      // aggiungo all'ultima uscita
      std::copy(value.begin(), value.end(), tmp.begin()+connected.size());
      
      // faccio un sort e poi unique
      std::sort(tmp.begin(), tmp.end());
      it = unique(tmp.begin(), tmp.end());
      
      // metto a posto connected
      connected.assign(it-tmp.begin(), 0);
      std::copy(tmp.begin(), it, connected.begin());
}

void graphItem::remove(UInt value)
{
	// variabili in uso 
	vector<UInt>          tmp;
	vector<UInt>::iterator it;
	
	// salvo tutto in tmp
	tmp = connected;
	
	// lo cerco 
	std::sort(tmp.begin(), tmp.end());
	
	// lo cerco 
	it = std::find(tmp.begin(), tmp.end(), value);
	
	// lo elimino 
	if(it!=tmp.end()) tmp.erase(it);
	
	// salvo 
	connected.assign(tmp.size(), 0);
	std::copy(tmp.begin(), tmp.end(), connected.begin());
}

void graphItem::change(UInt oldValue, UInt newValue)
{
	replace(connected.begin(), connected.end(), oldValue, newValue);
}

void graphItem::invert()
{
	reverse(connected.begin(), connected.end());
}

//
// Metodi per trovare gli id comuni e no
//

void graphItem::common(const graphItem& E, vector<UInt> * com)
{
	  // Mi accerto che com sia vuota
	  com->clear();

	  // Variabili in uso 
	  set<UInt>     	external;
	  set<UInt>::iterator         it;
	  
	  // Riempio in un set i connessi di quello esterno
	  for(UInt i=0; i<E.getNumConnected(); ++i)	  external.insert(E.getConnectedId(i));
	  
	  // Per ogni connesso dell'elenco di classe controllo se sono presenti in external
	  for(UInt i=0; i<connected.size(); ++i)
	  {
		  // cerco l'elemento
		  it = external.find(connected[i]);
		  
		  // se lo trovo lo inserisco nel vettore con
		  if(it!=external.end())	com->push_back(connected[i]);
	  }	  
}
		
void graphItem::unCommon(const graphItem& E, vector<UInt> * unCom)
{
	  // Mi accerto che com sia vuota
	  unCom->clear();

	  // Variabili in uso 
	  set<UInt>    internal,external;
	  set<UInt>::iterator         it;
	  
	  // Riempio in un set i connessi di quello esterno
	  for(UInt i=0; i<E.getNumConnected(); ++i)	  external.insert(E.getConnectedId(i));
	  
	  // Per ogni connesso dell'elenco di classe controllo se sono presenti in external
	  for(UInt i=0; i<connected.size(); ++i)
	  {
		  // cerco l'elemento
		  it = external.find(connected[i]);
		  
		  // se non lo trovo lo inserisco nel vettore unCom
		  if(it==external.end())     unCom->push_back(connected[i]);
	  }
}

//
// Print
//

void graphItem::print()
{
	cout << "Id         : " << this->getId() << endl;
	cout << "Num Points : " << this->getNumConnected() << endl;
	cout << "Nodes Id's : ";
	
	for(UInt i=0; i<connected.size(); ++i)
	{
		cout << this->getConnectedId(i) << " ";
	}
	cout << endl;
}
