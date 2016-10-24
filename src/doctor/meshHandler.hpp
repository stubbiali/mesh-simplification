#ifndef MESHHANDLER_HPP_
#define MESHHANDLER_HPP_

#include <cassert>
#include <ctime>
#include <iostream>
#include <vector>
#include <cmath>
#include <map>

#include "../geometry/geoElement.hpp"
#include "../geometry/mesh1d.hpp"
#include "../geometry/mesh2d.hpp"
#include "../geometry/mesh3d.hpp"
#include "../geometry/connect1d.hpp"
#include "../geometry/connect2d.hpp"
#include "../geometry/connect3d.hpp"
#include "../geometry/meshSearch.hpp"

namespace geometry
{

/*! Classe che di fare delle semplici operazioni di gestione delle mesh, dipende da due parametri template 
<ol>
<li> DIM dimensione degli oggetti che sono salvati 
<li> MESH oggetto mesh che si sta analizzando 
<ol>
Tali funzionalità servono principalmente per 
<ol>
<li> unire due mesh differenti 
<li> estrarre una mesh con un particolare geoId
<li> preveire la presenza di punti doppi 
<ol>
N.B. la fusione della mesh non fa un'analisi della conformità si limita SOLAMENTE a fondere i punti identici 
*/

template<class MESH, UInt DIM=3> class meshHandler
{
	//
	// Variabili di classe 
	//	
	public:
		  /*! Puntatore all'oggetto Mesh*/
		  MESH        *                   meshPointer;
		  	  
		  /*! Valore della tolleranza */
		  Real                                   toll;
	//
	// Costruttore
	//
	public:
		  /*! Costruttore vuoto */
		  meshHandler();
		  
		  /*! Costruttore non vuoto 
		      \param _meshPointer puntatore ad un oggetto di tipo mesh */
		  meshHandler(MESH  * _meshPointer);
	//
	// Metodi di set/get
	//	  
	public: 
		  /*! Metodo che setta il puntatore 
		      \param _meshPointer puntatore ad un oggetto di tipo mesh */
		  inline void setMeshPointer(MESH * _meshPointer);
		  
		  /*! Metodo che restituisce il puntatore */
		  inline MESH * getMeshPointer();
		  
		  /*! Metodo che restituisce la tolleranza 
		      \param _toll valore della tolleranza */
		  inline void setToll(Real toll=1e-15);
		  
		  /*! Metodo che restituisce la tolleranza */
		  inline Real getToll();
	//
	// Metodi di classe per rendere unici i punti o eliminare quelli liberi 
	//
	public:
		  /*! Metodo che permette di fare una mappa fra i punti che sono nel vettore in input e quelli della mesh 
		      \param listPt lista di punti 
		      \param extToInt mappa che fa le associazioni di punti */
		  void createMap(vector<point> * listPt, map<UInt, UInt> * extToInt);
		  
		  /*! Metodo che permette di creare una mappa per capire se ci sono punti doppi
		      \param oldToNew mappa che da la numerazione */
		  void uniqueNodes(map<UInt, UInt> * oldToNew);
		  
		  /*! Metodo che modifica i nodi in input partedo dalla mappa costriuta nel metodo uniqueNodes 
		      \param oldToNew mappa che da la numerazione */
		  void changeNodes(map<UInt, UInt> * oldToNew);
		  
		  /*! Metodo che modifica gli identificatori degli elementi dalla mappa costriuta nel metodo uniqueNodes 
		      \param oldToNew mappa che da la numerazione */
		  void changeElements(map<UInt, UInt> * oldToNew);
		  
		  /*! Metodo che chiama i precedenti 3 metodi e sistema eventuali nodi doppi
		      N.B. non effettua alcun controllo sul intersezione di elementi */
		  void setUpMesh();
		  
		  /*! Metodo che permette di eliminare tutti i nodi della mesh puntata dalla classe che non sono connessi a nessun 
		      elemento con uno specifico geoId.		  
		      N.B. non effettua alcun controllo sul intersezione di elementi */
		  void deleteFreeNodes();
	//
	// Metodi di classe per aggiungere o estrarre elementi della mesh 
	//
	public:  
		  /*! Metodo per aggiungere elementi alla mesh puntata dalla classe 
		      \param _meshExt mesh esterna*/
		  void addMesh(MESH * _meshExt);
		  
		  /*! Metodo per aggiungere elementi alla mesh puntata dalla classe prendendo un particolare geoId da una mesh 
		      esterna
		      \param _meshExt mesh esterna
		      \param geoId geoId da prendere */
		  void addMesh(MESH * _meshExt, UInt geoId);
		  
		  /*! Metodo per aggiungere elementi alla mesh puntata dalla classe prendendo un particolare geoId da una mesh 
		      esterna
		      \param _meshExt mesh esterna
		      \param geoId vettore che contiene i geoId degli elementi che devono essere presi*/
		  void addMesh(MESH * _meshExt, vector<UInt> * geoIds);
		  
		  /*! Metodo per estrarre dalla mesh puntata dalla classe quella con particolari geoId
		      \param _meshOut mesh esterna
		      \param geoIds vettore che contiene i geoId degli elementi che devono essere estratti*/
		  void extractMesh(MESH * _meshOut, vector<UInt> * geoIds);
		  
		  /*! Metodo per estrarre dalla mesh puntata dalla classe quella con particolari geoId
		      \param _meshOut mesh esterna
		      \param geoId il geoId da estrarre*/
		  void extractMesh(MESH * _meshOut, UInt geoId);
};

}

using namespace geometry;

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttori
//
template<class MESH, UInt DIM> meshHandler<MESH,DIM>::meshHandler()
{
      meshPointer = NULL;
      setToll();
}

template<class MESH, UInt DIM> meshHandler<MESH,DIM>::meshHandler(MESH * _meshPointer)
{
      // setto il puntatore
      meshPointer = _meshPointer;
      
      // setto la tolleranza
      setToll();
}

//
// Metodi di set/get
//
template<class MESH, UInt DIM> inline void meshHandler<MESH,DIM>::setMeshPointer(MESH * _meshPointer)
{
      // setto il puntatore
      meshPointer = _meshPointer;
}
		  
template<class MESH, UInt DIM> inline MESH * meshHandler<MESH,DIM>::getMeshPointer()
{
      return(meshPointer);
}

template<class MESH, UInt DIM> inline void meshHandler<MESH,DIM>::setToll(Real _toll)
{
      toll = _toll;
}
		  
template<class MESH, UInt DIM> inline Real meshHandler<MESH,DIM>::getToll()
{
      return(toll);
}

//
// Metodi di classe per rendere unici i punti o eliminare quelli liberi
//
template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::createMap(vector<point> * listPt, map<UInt, UInt> * extToInt)
{
      // assert per essere sicuro
      assert(meshPointer!=NULL);
  
      // variabili temporanee 
      UInt 						cont;
      point                                              ext;
      meshSearch<mesh0d<simplePoint>, DIM>          finder0d;
      mesh0d<simplePoint>      		               cloud;
      geoElement<simplePoint>				elem;
      pair<bool, vector<UInt> >			      result;
      
      // inserico i nodi 
      cloud.insertNode(meshPointer->getNodePointer());
      
      // setto ext
      ext.setX(100.0*toll);		ext.setY(100.0*toll);		ext.setZ(100.0*toll);
 	  
      // inserisco gli elementi 
      cloud.getElementPointer()->reserve(cloud.getNumNodes());
      for(UInt i=0; i<cloud.getNumNodes(); ++i)
      {
	  elem.setConnectedId(0, i);
	  cloud.insertElement(elem);
      }
      
      // setto la tolleranza
      finder0d.setToll(toll);
      
      // setto il finder 
      finder0d.setMeshPointer(&cloud);
      
      // per ogni nodo nella lista in ingresso lo cerco e creo la mappa 
      for(UInt i=0; i<listPt->size(); ++i)
      {
	    // cerco il nodo 
	    result = finder0d.findIntersection(listPt->at(i)+ext, listPt->at(i)-ext);
	    
	    // se lo trovo 
	    if(result.first)
	    {
		  // controllo di aver trovato un solo elemento
		  if(result.second.size()!=1)
		  {
		      cont = 0;
		      for(UInt j=0; j<result.second.size(); ++j)
		      {
			  // se il punto coincide 
			  if((listPt->at(i)-meshPointer->getNode(result.second[j])).norm2()<toll)
			  {
			      // faccio la corrispondenza 
			      extToInt->insert(pair<UInt,UInt>(listPt->at(i).getId(),result.second[j]));
			      
			      // incremento cont 
			      ++cont;
			  }
		      }
		      
		      // messaggio di errore
		      if(cont>1)	cout << "Ho trovato più di una corrispondenza nel metodo \"createMap\" " << endl;
		  }
		  // qui va tutto bene e creo la mappa 
		  else
		  {
		      extToInt->insert(pair<UInt,UInt>(listPt->at(i).getId(),result.second[0]));
		  }
	    }
	    else
	    {
		  cout << "Non ho trovato le corrispondenze" << endl;
	    }
      }
}

template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::uniqueNodes(map<UInt, UInt> * oldToNew)
{
      // assert per essere sicuro
      assert(meshPointer!=NULL);
  
      // variabili temporanee 
      meshSearch<mesh0d<simplePoint>, DIM>          finder0d;
      mesh0d<simplePoint>      		               cloud;
      geoElement<simplePoint>				elem;
      pair<bool, vector<UInt> >			      result;
      tree<vector<geoElementSearch> >::iterator	          it;			
            
      // inserico i nodi 
      cloud.insertNode(meshPointer->getNodePointer());
 	  
      // inserisco gli elementi 
      cloud.getElementPointer()->reserve(cloud.getNumNodes());
      for(UInt i=0; i<cloud.getNumNodes(); ++i)
      {
	  elem.setConnectedId(0, i);
	  elem.setId(i);
	  cloud.insertElement(elem);
      }
      
      // setto la tolleranza
      finder0d.setToll(toll);
      
      // setto il finder 
      finder0d.setMeshPointer(&cloud);
      
      // per ogni nodo della mesh lo cerco e faccio l'associazione 
      for(it=finder0d.finder.begin(); it!=finder0d.finder.end(); ++it)	
      {
	      if(it->size()!=0)
	      {
		    // faccio la mappa 
		    for(UInt j=0; j<it->size(); ++j)	oldToNew->insert(pair<UInt,UInt>(it->at(j).getId(), it->at(0).getId()));
	      }
      }
}

template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::changeNodes(map<UInt, UInt> * oldToNew)
{	
	// assert per essere sicuro
	assert(meshPointer!=NULL);
    
	// variabili in uso
	vector<point>       ptTmp;
	set<UInt>	      ids;
	
	// ciclo sulla mappa e metto gli indici dei nodi 
	for(UInt i=0; i<meshPointer->getNumNodes(); ++i)	ids.insert(oldToNew->find(i)->second);
	
	// faccio un resize
	ptTmp.resize(ids.size());
	
	// per ogni nodo metto il suo rappresentante in ids
	for(UInt i=0; i<meshPointer->getNumNodes(); ++i)	ptTmp[oldToNew->find(i)->second] = meshPointer->getNode(i);
	
	// faccio il clear
	meshPointer->getNodePointer()->clear();
	
	// faccio un resize 
	meshPointer->getNodePointer()->resize(ptTmp.size());
	
	// copio
	copy(ptTmp.begin(), ptTmp.end(), meshPointer->getNodePointer()->begin());
	
	// metto a posto gli id
	meshPointer->setUpIds();
}

template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::changeElements(map<UInt, UInt> * oldToNew)
{
	// assert per essere sicuro
	assert(meshPointer!=NULL);
	
	// variabili in uso     
	UInt                                 numConn,tmp,tmp2;
	set<geoElement<typename MESH::RefShape> >   tmpEleSet;
	
	// numero di nodi 
	numConn = MESH::RefShape::numVertices;
	
	// inizializzo tmp2
	tmp2 = 0;
	
	// ciclo sugli elementi di quella esterna 
	for(UInt i=0; i<meshPointer->getNumElements(); ++i)
	{
	      // ciclo sui connessi 
	      for(UInt j=0; j<numConn; ++j)
	      {
		  // prendo l'id 
		  tmp = meshPointer->getElementPointer(i)->getConnectedId(j);
		  
		  // controllo
		  if(oldToNew->find(tmp)!=oldToNew->end())	tmp2 = oldToNew->find(tmp)->second;
		  else						cout << "Non c'è corrispondenza!!" << endl;
		  
		  // lo cambio 
		  meshPointer->getElementPointer(i)->setConnectedId(j, tmp2);
	      }
	}
	
	// li rendo unici 
	for(UInt i=0; i<meshPointer->getNumElements(); ++i)	tmpEleSet.insert(meshPointer->getElement(i));
	
	// controllo se sono cambiati il numero di nodi 
	if(tmpEleSet.size()!=meshPointer->getNumElements())
	{
	    cout << "ATTENZIONE: nel processo di \"changeElements\" ho fuso elemeti: da ";
	    cout << meshPointer->getNumElements() << " a " << tmpEleSet.size() << endl;
	    
	    // li metto nella mesh puntata dalla classe 
	    meshPointer->insertElement(&tmpEleSet);
	}
}

template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::setUpMesh()
{
	// assert per essere sicuro
	assert(meshPointer!=NULL);
	
	// variabili in uso     
	map<UInt, UInt>          oldToNew;
	time_t              start,end,dif;
	
	// stampa 
	cout << "Inizio processo di setUp della Mesh" << endl;
	time(&start);
	
	// unifico i punti 
	uniqueNodes(&oldToNew);
	
	// cambio gli elementi 
	changeElements(&oldToNew);
	
	// li cancello 
	deleteFreeNodes();
		
	// prendo il tempo e stampo 
	time(&end);
	dif = difftime(end,start);
	cout << "Processo di setUp completato: " <<  dif << " sec." << endl;
}

template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::deleteFreeNodes()
{
      // assert per essere sicuro
      assert(meshPointer!=NULL);
      
      // variabili in uso
      map<UInt, UInt>               oldToNew;
      set<UInt>                       finder;
      UInt                idTmp,cont,numConn;
      vector<point>              newListNode;
      
      // numero di nodi 
      numConn = MESH::RefShape::numVertices;
      
      // faccio un resize
      newListNode.reserve(meshPointer->getNumNodes());
      
      // ciclo sugli elementi della mesh per vedere quali sono i nodi attivi
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	  // ciclo sui connessi 
	  for(UInt j=0; j<numConn; ++j)
	  {
		  // prendo l'id 
		  idTmp = meshPointer->getElementPointer(i)->getConnectedId(j);
		  
		  // setto attivo 
		  finder.insert(idTmp);
	  }
      }
      
      // setto un contatore che conterà i nodi che ci sono nella mesh e sono connessi a elementi
      cont = 0;
      
      // numero di nodi 
      numConn = MESH::RefShape::numVertices;
	  
      // creo una nuova mappa 
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
      {	    
	  // cerco l'id e se lo trovo creo la mappa 
	  if(finder.find(i)!=finder.end())
	  {
		// lo metto nella lista dei nodi
		newListNode.push_back(meshPointer->getNode(i));
		  
		// faccio l'associazione
		oldToNew[i]=cont;
		   
		// incremento cont
		++cont;
	   }
      }
	    
      // metto a posto i nodi 
      meshPointer->insertNode(&newListNode);
      
      // ciclo sugli elementi di quella esterna 
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	  // ciclo sui connessi 
	  for(UInt j=0; j<numConn; ++j)
	  {
		  // prendo l'id 
		  idTmp = meshPointer->getElementPointer(i)->getConnectedId(j);
		  
		  // lo cambio 
		  meshPointer->getElementPointer(i)->setConnectedId(j, oldToNew[idTmp]);
	   }	
      }
}

//
// Metodi di classe per aggiungere o estrarre elementi della mesh 
//
template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::addMesh(MESH * _meshExt)
{
	// assert per essere sicuro
	assert(meshPointer!=NULL);
	
	// variabili in uso     
	UInt        intNode,extNode,intEle,extEle,tmp,numConn;
	MESH				              tmpMesh;
	vector<point>                                   tmpPt;
	vector<geoElement<typename MESH::RefShape> >   tmpEle;
	time_t              			start,end,dif;
	
	// stampa 
	cout << "Inizio processo di unione della Mesh" << endl;
	time(&start);
	
	// controllo se devo effettivamente aggiungere qualcosa 
	if(_meshExt->getNumNodes()==0 && _meshExt->getNumElements()==0)	
	{
	    // prendo il tempo e stampo 
	    time(&end);
	    dif = difftime(end,start);
	    cout << "ATTENZIONE: non ho aggiunto alcun elemento perché la mesh in ingresso era vuota " << endl;
	    cout << "Processo di unione completato: " <<  dif << " sec." << endl;
	    return;
	}
	else if(_meshExt->getNumNodes()==0 && _meshExt->getNumElements()!=0)
	{
	    // prendo il tempo e stampo 
	    time(&end);
	    dif = difftime(end,start);
	    cout << "ATTENZIONE: mesh con elementi ma non con nodi" << endl;
	    cout << "Processo di unione completato: " <<  dif << " sec." << endl;
	    return;
	}
	else if(_meshExt->getNumNodes()!=0 && _meshExt->getNumElements()==0)
	{
	    // prendo il tempo e stampo 
	    time(&end);
	    dif = difftime(end,start);
	    cout << "ATTENZIONE: mesh con nodi ma non con elementi" << endl;
	    cout << "Processo di unione completato: " <<  dif << " sec." << endl;
	    return;
	}
	
	// salvo il numero dei nodi 
	intNode = meshPointer->getNumNodes();
	extNode = _meshExt->getNumNodes();
	intEle  = meshPointer->getNumElements();
	extEle  = _meshExt->getNumElements();
	
	// numero di nodi 
	numConn = MESH::RefShape::numVertices;
	  
	// metto tutti i punti 
	tmpMesh.getNodePointer()->resize(intNode+extNode);
	copy(meshPointer->getNodePointer()->begin(), meshPointer->getNodePointer()->end(), tmpMesh.getNodePointer()->begin());
	copy(_meshExt->getNodePointer()->begin(), _meshExt->getNodePointer()->end(), tmpMesh.getNodePointer()->begin()+intNode);
	
	// faccio un resize
	tmpMesh.getElementPointer()->resize(intEle+extEle);
	
	// copio i nodi della mesh  
	copy(meshPointer->getElementPointer()->begin(), 
	     meshPointer->getElementPointer()->end(), 
	     tmpMesh.getElementPointer()->begin());
	
	// ciclo sugli elementi di quella esterna 
	for(UInt i=0; i<extEle; ++i)
	{
	      // ciclo sui connessi 
	      for(UInt j=0; j<numConn; ++j)
	      {
		  // prendo l'id 
		  tmp = _meshExt->getElementPointer(i)->getConnectedId(j);
		  
		  // lo cambio 
		  tmpMesh.getElementPointer(intEle+i)->setConnectedId(j, tmp+intNode);
	      }
	      tmpMesh.getElementPointer(intEle+i)->setGeoId(_meshExt->getElement(i).getGeoId());
	}
	
	// faccio un setUpIds
	tmpMesh.setUpIds();
	
	// salvo tutto nella mesh puntata dalla classe 
	meshPointer->insertNode(tmpMesh.getNodePointer());
	meshPointer->insertElement(tmpMesh.getElementPointer());
	
	// rendo sistemo le connettività
	setUpMesh();
	
	// prendo il tempo e stampo 
	time(&end);
	dif = difftime(end,start);
	cout << "Processo di unione completato: " <<  dif << " sec." << endl;
}

template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::addMesh(MESH * _meshExt, UInt geoId)
{
	// varaibli in uso 
	vector<UInt>    geoIds(1,geoId);
      
	// chiamo il metodo dei vettori
	addMesh(_meshExt, &geoIds);
}

template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::addMesh(MESH * _meshExt, vector<UInt> * geoIds)
{
	// varaibili in uso 
	MESH 			    tmp;
	meshHandler<MESH,DIM>	tmpHand;
	
	// setto il puntatore 
	tmpHand.setMeshPointer(_meshExt);
	
	// estraggo la mesh che devo aggiungere 
	tmpHand.extractMesh(&tmp, geoIds);
	
	// chiamo il metodo della classe 
	this->addMesh(&tmp);
}

template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::extractMesh(MESH * _meshExt, vector<UInt> * geoIds)
{
	// assert per essere sicuro
	assert(meshPointer!=NULL);
	
	// varaibli in uso 
	MESH * 					   tmpPointer;
	MESH					      tmpMesh; 
	vector<geoElement<typename MESH::RefShape> >  tmpElem;
	time_t              			start,end,dif;
	
	// stampa 
	cout << "Inizio processo di estrazione della Mesh" << endl;
	time(&start);
	
	// metto tutto nella mesh temporanea
	tmpMesh.insertNode(meshPointer->getNodePointer());
	
	// faccio un reserve
	tmpElem.reserve(meshPointer->getNumElements());
	
	// faccio un sort per fare le ricerche sul vettore 
	sort(geoIds->begin(), geoIds->end());
	
	// creo i suoi elementi 
	for(UInt i=0; i<meshPointer->getNumElements(); ++i)
	{
	      // se trovo il suo geoId lo metto nella lista
	      if(find(geoIds->begin(), geoIds->end(), meshPointer->getElement(i).getGeoId())!=geoIds->end())
	      {
		    tmpElem.push_back(meshPointer->getElement(i));
	      }
	}
	
	// li salvo nella mesh 
	tmpMesh.insertElement(&tmpElem);
	
	// salvo il puntatore
	tmpPointer = meshPointer;
	
	// setto il puntatore di classe 
	setMeshPointer(&tmpMesh);
	
	// metto a posto i nodi liberi 
	deleteFreeNodes();
	
	// rimetto il vecchio puntatore di classe
	setMeshPointer(tmpPointer);
	
	// salvo nella mesh puntata dalla classe
	_meshExt->insertNode(tmpMesh.getNodePointer());
	_meshExt->insertElement(tmpMesh.getElementPointer());
	_meshExt->setUpIds();	
	
	// prendo il tempo e stampo 
	time(&end);
	dif = difftime(end,start);
	cout << "Processo di separazione completato: " <<  dif << " sec." << endl;
}

template<class MESH, UInt DIM> void meshHandler<MESH,DIM>::extractMesh(MESH * _meshExt, UInt geoId)
{
	// varaibli in uso 
	vector<UInt>    geoIds(1,geoId);
	
	// chiamo il metodo dei vettori 
	extractMesh(_meshExt, &geoIds);
}

#endif

