#ifndef INTERSEC_HPP_
#define INTERSEC_HPP_

#include <cassert>	
#include <cmath>	
#include <iostream>
#include <utility>
#include <vector>
#include <ctime>

#include "../core/shapes.hpp"

#include "../geometry/mesh0d.hpp"
#include "../geometry/mesh1d.hpp"
#include "../geometry/mesh2d.hpp"
#include "../geometry/mesh3d.hpp"
#include "../geometry/geoElementSearch.h"
#include "../geometry/meshSearch.hpp"
#include "../geometry/meshSearchStructured.hpp"

#include "../intersec/lineIntersection.h"
#include "../intersec/triangleLineIntersection.h"
#include "../intersec/triangleIntersection.h"
#include "../intersec/intersecHandler.hpp"

#include "../doctor/meshHandler.hpp"

// TODO la classe è predisposta anche per fare le intersezioni con elementi quadratici ma mancano alcune implementazioni 
// 	- intersezione quadrato quadrato 
// 	- intersezione linea quadrato 

namespace geometry
{

/*! Classe template che permette di cercare le intersezioni fra due mesh. Tali intersezioni possono essere di vario tipo
<ol>
<li> mesh1d e mesh1d composte da LINEE
<li> mesh2d e mesh1d composte da TRIANGOLI e LINEE
<li> mesh2d e mesh1d composte da TRIANGOLI
<ol>
La struttura dati di ricerca viene costriuta sulla mesh puntata dalla variabile meshPointer1. La ricerca può essere fatta in tre
modi
<ol>
<li> NORMAL si sfrutta solamente la struttura ad albero 
<li> FAST comboina la struttura ad albero e la ricerca strutturata 
<li> STRUCTURED utilizza solamente la struttura dati strutturata
<ol>
*/

enum howIntersec{NORMAL=0, FAST=1, STRUCTURED=2};

template<class MESH1, class MESH2, UInt DIM=3> class meshIntersec
{
	//
	// Variabili di classe 
	//
	public:
		/*! Tolleranza */
		Real                            	                    		     toll;
		
		/*! Puntatori alla mesh  */
		MESH1   *	  					              meshPointer1;
		MESH2   *						              meshPointer2;
		
		/*! Oggetto che analizza il tipo di intersezione */
		intersecHandler<typename MESH1::RefShape, typename MESH2::RefShape>        handler;
		
		/*! Oggetto che contiene la struttura di ricerca */
		meshSearch<MESH1,DIM>							    finder;
		
		/*! Oggetti che implementano le intersezioni */
		lineIntersection             	 		   		     lineLineInter;
		triangleLineIntersection  		     	   		     triaLineInter;
		triangleIntersection          	 		  		     triaTriaInter;
		
		/*! variabile che attiva o meno il riordiamento */
		bool								     activeReorder;
		
	//
	// Costruttore
	//
	public:
		/*! Costruttore vuoto */
		meshIntersec();
		
		/*! Costruttore con i puntatori alla mesh 
		    \param meshPointer1 puntatore alla prima mesh 
		    \param meshPointer2 puntatore alla seconda mesh */
		meshIntersec(MESH1 * _meshPointer1, MESH2 * _meshPointer2);
	//
	// Metodi di get/set 
	//
	public:
		/*! Metodo che setta il puntatore 
		    \param _meshPointer1 puntatore ad un oggetto di tipo mesh */
		void setMeshPointer1(MESH1 * _meshPointer1);
		  
		/*! Metodo che restituisce il puntatore */
		inline MESH1 * getMeshPointer1();
		
		/*! Metodo che setta il puntatore 
		    \param _meshPointer2 puntatore ad un oggetto di tipo mesh */
		void setMeshPointer2(MESH2 * _meshPointer2);
		  
		/*! Metodo che restituisce il puntatore */
		inline MESH2 * getMeshPointer2();
		  
		/*! Metodo che restituisce la tolleranza 
		    \param _toll valore della tolleranza */
		inline void setToll(Real toll=1e-14);
		  
		/*! Metodo che restituisce la tolleranza */
		inline Real getToll();
		
		/*! Metodo per attivare o disattivare il riordinamento */
		inline void onReorder()		{activeReorder=true;};
		inline void offReorder()	{activeReorder=false;};
	//
	// Metodi che fanno l'intersezione 
	//
	public:
		/*! Metodo che fa l'intersezione 
		    \param primo puntatore a un vettore che contiene il primo elemento con cui fare l'intersezione
		    \param secondo puntatore a un vettore che contiene il secondo elemento con cui fare l'intersezione*/
		pair<bool, vector<point> > intersecElement(vector<point> * primo, vector<point> * secondo);
		
		/*! Metodo che trova i possibili elementi che intersecano
		    \param elemento puntatore all'elemento che si vuole analizzare */
		pair<bool, vector<UInt> >  findElement(vector<point> * elemento);
		
		/*! Metodo che aggiorna le liste partendo dalle informazioni fornite da result
		    \param result coppia che permette di stabilire come agire 
		    \param nodiInt lista dei nodi che verrà riempita
		    \param elementiInt lista degli elementi che verrà riempita */
		void upDateList(pair<bool, vector<point> > * result, 
				vector<point> * nodiInt, 
				vector<vector<UInt> > * elementiInt);
		
		/*! Metodo che crea la stuttura dati di ricerca 
		    N.B. la struttura dati viene costriuta sulla mesh puntata da meshPointer1 */
		void createDataStructure();
		
		/*! Metodo generale che crea l'intersezione generica 
		    \param nodiInt puntatore a un vettore di nodi che conterrà i punti di intersezione 
		    \param elementiInt puntatore a un vettore che contiene in ogni componente le connessioni che generano 
				      l'elemento di intersezione */
		void createIntersection(vector<point> * nodiInt, vector<vector<UInt> > * elementiInt);
		
		/*! Metodo che crea l'intesezione filtrando prima gli elementi della mesh che si intersecano 
		    \param nodiInt puntatore a un vettore di nodi che conterrà i punti di intersezione 
		    \param elementiInt puntatore a un vettore che contiene in ogni componente le connessioni che generano 
				      l'elemento di intersezione */
		void createIntersectionFast(vector<point> * nodiInt, vector<vector<UInt> > * elementiInt);
		
		/*! Metodo che crea l'intesezione filtrando prima gli elementi della mesh che si intersecano 
		    \param nodiInt puntatore a un vettore di nodi che conterrà i punti di intersezione 
		    \param elementiInt puntatore a un vettore che contiene in ogni componente le connessioni che generano 
				      l'elemento di intersezione */
		void createIntersectionStructured(vector<point> * nodiInt, vector<vector<UInt> > * elementiInt);
		
		/*! Metodo che fa l'intersezione fra due mesh il cui risultato è una mesh 1d
		    \param inter puntatore a una mesh che fa l'intersezione 
		    \param fast booleano che dice se fare l'intersezione con il metodo che filtra i dati */
		void createIntersection(mesh1d<Line> * inter, howIntersec type=NORMAL);
		
		/*! Metodo che fa l'intersezione fra due mesh il cui risultato è una mesh 0d
		    \param inter puntatore a una mesh che fa l'intersezione */
		void createIntersection(mesh0d<simplePoint> * inter, howIntersec type=NORMAL);
	//
	// Metodi che controllano se c'è l'intersezione 
	//
	public:
		/*! Metodo che controlla l'intersezione 
		    \param primo puntatore a un vettore che contiene il primo elemento con cui fare l'intersezione
		    \param secondo puntatore a un vettore che contiene il secondo elemento con cui fare l'intersezione*/
		bool doElementIntersect(vector<point> * primo, vector<point> * secondo);
		
		/*! Metodo che analizza le intersezioni usando gli alberi di ricerca 
		    N.B. il metodo ritorna il numero di triangoli intersecanti */
		UInt analyzeIntersection();
		
		/*! Metodo che analizza le intersezioni usando filtrando l'albero con una pre analisi fatta da una mesh 
		    strutturata 
		    N.B. il metodo ritorna il numero di triangoli intersecanti */
		UInt analyzeIntersectionFast();
		
		/*! Metodo che analizza le intersezioni usando una mesh  strutturata 
		    N.B. il metodo ritorna il numero di triangoli intersecanti */
		UInt analyzeIntersectionStructured();
		
		/*! Metodo che fa l'intersezione fra due mesh il cui risultato è una mesh 1d
		    \param inter puntatore a una mesh che fa l'intersezione 
		    \param fast booleano che dice se fare l'intersezione con il metodo che filtra i dati */
		UInt doIntersect(howIntersec type=NORMAL);	
};

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttori
//
template<class MESH1, class MESH2, UInt DIM> meshIntersec<MESH1, MESH2, DIM>::meshIntersec()
{
      meshPointer1 = NULL;
      meshPointer2 = NULL;
      toll         = 1e-14;
      lineLineInter.setToll(toll);
      triaLineInter.setToll(toll);
      triaTriaInter.setToll(toll);
      finder.setToll(toll);
      activeReorder = true;
      
}

//
// Metodi di set/get
//
template<class MESH1, class MESH2, UInt DIM> void meshIntersec<MESH1, MESH2, DIM>::setMeshPointer1(MESH1 * _meshPointer1)
{
      meshPointer1 = _meshPointer1;
}	
		  
template<class MESH1, class MESH2, UInt DIM> MESH1 * meshIntersec<MESH1, MESH2, DIM>::getMeshPointer1()
{
      return(meshPointer1);
}

template<class MESH1, class MESH2, UInt DIM> void meshIntersec<MESH1, MESH2, DIM>::setMeshPointer2(MESH2 * _meshPointer2)
{
      meshPointer2 = _meshPointer2;
}	
		  
template<class MESH1, class MESH2, UInt DIM> MESH2 * meshIntersec<MESH1, MESH2, DIM>::getMeshPointer2()
{
      return(meshPointer2);
}

template<class MESH1, class MESH2, UInt DIM> inline void meshIntersec<MESH1, MESH2, DIM>::setToll(Real _toll)
{
      toll = _toll;
      lineLineInter.setToll(toll);
      triaLineInter.setToll(toll);
      triaTriaInter.setToll(toll);
      finder.setToll(toll);
}

template<class MESH1, class MESH2, UInt DIM> inline Real meshIntersec<MESH1, MESH2, DIM>::getToll()
{
      return(toll);
}

//
// Metodi che fanno l'intersezione 
//
template<class MESH1, class MESH2, UInt DIM> 
pair<bool, vector<point> > meshIntersec<MESH1, MESH2, DIM>::intersecElement(vector<point> * primo, vector<point> * secondo)
{
	// varaibli in uso
	bool 			  found=false;
	UInt 				 index;
	point				   tmp;
	pair<int,  vector<point> >   resultTmp;
	pair<bool, vector<point> > 	result;
	
	// controllo che cosa sarà il risultato dell'intersezione 
	if(handler.getShapeInter()==POINT)
	{
	      if((handler.getShape1()==TRIANGLE) && (handler.getShape2()==LINE))
	      {
		  resultTmp = triaLineInter.intersec(primo, secondo);
		  found    = true;
	      }
	      else if((handler.getShape1()==LINE) && (handler.getShape2()==TRIANGLE))
	      {
		  resultTmp = triaLineInter.intersec(secondo, primo);
		  found    = true;
	      }
	      else if((handler.getShape1()==LINE) && (handler.getShape2()==LINE))
	      {
		  resultTmp = lineLineInter.intersec(primo, secondo);
		  found    = true;
	      }
	}
	else if(handler.getShapeInter()==LINE)
	{
	      if((handler.getShape1()==TRIANGLE) && (handler.getShape2()==TRIANGLE))
	      {
		  resultTmp = triaTriaInter.intersec(primo, secondo);
		  found    = true;
	      }
	}
	
	// assert per essere sicuro di aver riempito result
	assert(found);
    (void)found;
	
	// controllo che le intersezioni siano corrette 
	if(resultTmp.first==NOTINTERSEC)
	{
	    result.first = false;
	    return(result);
	}
	else if((resultTmp.first!=INTERSEC) && (resultTmp.first!=NOTINTERSEC))
	{
	    cout << "L'intersezione fra le Mesh non è corretta c'è il rischio che siamo in casi particolari" << endl;
	}
	
	// a questo livello sono sicuro che intersecano
	result.first = true;
	result.second.reserve(resultTmp.second.size());
	
	// metto il primo 
	result.second.push_back(resultTmp.second[0]);
	
	// setto l'indice
	index = 0;
	
	// ciclo sugli elementi di result sono relativamente pochi 
	// 
	// ATTENTO perché gli elementi di resultTmp.second sono raddoppiati ma se sono doppi sono uno di seguito all'altro 
	// quindi li metto con circospezione 
	// 
	for(UInt i=1; i<resultTmp.second.size(); ++i)
	{
	      // prendo il precendente
	      tmp = result.second.at(index);
	      
	      // controllo se è uguale 
	      if((tmp-resultTmp.second[i]).norm2()>toll)
	      {
		  // metto il punto perché non è uguale 
		  result.second.push_back(resultTmp.second[i]);
		  
		  // incremento index in modo tale che punti al nuovo punto 
		  ++index;
	      }
	}
	
	// filtro i risultati in modo tale che vengano come output solo linee per facilitare la ricerca
	if(handler.getShapeInter()==LINE)
	{
	      if((handler.getShape1()==TRIANGLE) && (handler.getShape2()==TRIANGLE) && result.second.size()!=2)  
		result.first = false;
	}
	
	// ritorno 
	return(result);
}

template<class MESH1, class MESH2, UInt DIM> 
pair<bool, vector<UInt> > meshIntersec<MESH1, MESH2, DIM>::findElement(vector<point> * elemento)
{
	// varaibli in uso
	point                         pMax,pMin;
	pair<bool, vector<UInt>  >       result;
	
	// inizializzo 
	for(UInt j=0; j<DIM; ++j)
	{
	    pMin.setI(j, elemento->at(0).getI(j));
	    pMax.setI(j, elemento->at(0).getI(j));
	}
	
	// ciclo sull'elemento 
	for(UInt i=1; i<elemento->size(); ++i)
	{
	    // ciclo sulle coordinate che mi interessano 
	    for(UInt j=0; j<DIM; ++j)
	    {
		  pMin.setI(j, std::min(pMin.getI(j), elemento->at(i).getI(j)));
		  pMax.setI(j, std::max(pMax.getI(j), elemento->at(i).getI(j)));
	    }	
	}
	
	// provo a vedere se trovo 
	result = finder.findIntersection(pMax, pMin);
	
	// ritorno il risultato 
	return(result);
}

template<class MESH1, class MESH2, UInt DIM> 
void meshIntersec<MESH1, MESH2, DIM>::upDateList(pair<bool, vector<point> > * result, 
					         vector<point> * nodiInt, vector<vector<UInt> > * elementiInt)
{
	 // variabili in uso 
	 UInt 			id;
	 vector<UInt>         conn;
	 
	 // controllo se intersecano 
	 if(!result->first) return;
	 
	 // faccio un resize 
	 conn.resize(result->second.size());
	 
	 // prendo l'id
	 id = nodiInt->size();
	 
	 // per ogni elementi faccio il push_back
	 for(UInt i=0; i<result->second.size(); ++i)
	 {
	      // lo metto nella lista dei nodi 
	      nodiInt->push_back(result->second.at(i));
	      
	      // creo l'elemento 
	      conn[i] = id;
	      
	      // incremento id 
	      ++id;
	 }
	 
	 // inserisco l'elemento 
	 elementiInt->push_back(conn);
}

template<class MESH1, class MESH2, UInt DIM> 
void meshIntersec<MESH1, MESH2, DIM>::createDataStructure()
{
	// controllo che sia diverso da null
	assert(meshPointer1!=NULL);
	
	// pulisco quello che potrebbe esserci dentro 
	finder.clear();
	
	// setto la variabile se voglio o meno riordinare 
	if(activeReorder)	finder.setMeshPointer(meshPointer1);
	else			finder.setMeshPointer(meshPointer1, false);   
	
}

template<class MESH1, class MESH2, UInt DIM> 
void meshIntersec<MESH1, MESH2, DIM>::createIntersection(vector<point> * nodiInt, vector<vector<UInt> > * elementiInt)
{
	// controllo che sia diverso da null
	assert(meshPointer1!=NULL);
	assert(meshPointer2!=NULL);
	
	// varaibili in uso 
	vector<point>             primo,secondo;
	pair<bool, vector<UInt>  >       result;
	pair<bool, vector<point> >	  punti;

	// faccio un reserve
	nodiInt->reserve(max(meshPointer1->getNumNodes(),meshPointer2->getNumNodes()));
	elementiInt->reserve(max(meshPointer1->getNumElements(),meshPointer2->getNumElements()));
	
	// creo la stuttura dati 
	createDataStructure();
		
	// per ogni elemento della seconda mesh trovo l'intersezione 
 	for(UInt i=0; i<meshPointer2->getNumElements(); ++i)
	{
		// prendo i nodi connessi 
		meshPointer2->getNodeOfElement(i, &secondo);
		
		// faccio l'intersezione 
		result = findElement(&secondo);
		
		// se interseca cerco i punti 
		if(result.first)
		{		  		  
		      // vedo tutti i possibili elementi che intersecano e trovo quello che effettivamente interseca
		      for(UInt j=0; j<result.second.size(); ++j)
		      {
			  // prendo i nodi connessi 
			  meshPointer1->getNodeOfElement(result.second[j], &primo);
			  
			  // interseco 
			  punti = intersecElement(&primo, &secondo);
			  
			  // a seconda di quello che dice punti aggiorno i vettori in input
			  upDateList(&punti, nodiInt, elementiInt);
		      }
		}
	}
}

template<class MESH1, class MESH2, UInt DIM> 
void meshIntersec<MESH1, MESH2, DIM>::createIntersectionFast(vector<point> * nodiInt, vector<vector<UInt> > * elementiInt)
{
	// controllo che sia diverso da null
	assert(meshPointer1!=NULL);
	assert(meshPointer2!=NULL);
	
	// varaibili in uso 
	vector<point>             primo,secondo;
	pair<bool, vector<UInt>  >       result;
	pair<bool, vector<point> >	  punti;

	// faccio un reserve
	nodiInt->reserve(max(meshPointer1->getNumNodes(),meshPointer2->getNumNodes()));
	elementiInt->reserve(max(meshPointer1->getNumElements(),meshPointer2->getNumElements()));
	
	// creo la struttura dati 
	point 		  rapp,elemMax,elemMin;
	meshSearchStructured<MESH1,DIM> filter;
	MESH1		   	      tmpMesh1;
	MESH2		   	      tmpMesh2;
	MESH1	*	       tmpMesh1Pointer;
	MESH2	*	       tmpMesh2Pointer;
	vector<UInt>                    neight;
	set<UInt>	              meshBase;
	vector<UInt>		       meshExt;
	meshHandler<MESH1,DIM>		 hand1;
	meshHandler<MESH2,DIM>		 hand2;

	// creo la struttura di ricerca statica per filtrare gli elementi della mesh
	filter.setMeshPointer(meshPointer1);
	
	// ciclo sugli elementi per trovare quelli che intersecano 
	for(UInt i=0; i<meshPointer2->getNumElements(); ++i)
	{
		// prendo il massimo e il minimo 
		meshPointer2->createBBox(i, elemMax, elemMin);
		
		// setto rapp
		rapp.replace(elemMax, elemMin, 0.5);
		
		// salvo
		if(filter.getElementAroundPoint(rapp, 2, &neight))
		{
		    // metto quelli della mesh di base 
		    for(UInt j=0; j<neight.size(); ++j)	meshBase.insert(neight[j]);
		    
		    // metto l'elemento 
		    meshExt.push_back(i);
		}
		
	}
	
	// setto a 0 tutti gli id degli elementi	
	meshPointer1->setUpGeoId(0);
	meshPointer2->setUpGeoId(0);
	
	// setto i geoId che voglio eliminare 
	for(UInt i=0; i<meshExt.size(); ++i)					
	  meshPointer2->getElementPointer(meshExt[i])->setGeoId(1);
	for(set<UInt>::iterator it=meshBase.begin(); it!=meshBase.end(); ++it)	
	  meshPointer1->getElementPointer(*it)->setGeoId(1);
	
	// estraggo le superfici 
	hand1.setMeshPointer(meshPointer1);
	hand1.extractMesh(&tmpMesh1, 1);
	hand2.setMeshPointer(meshPointer2);
	hand2.extractMesh(&tmpMesh2, 1);
	
	// salvo i puntatori
	tmpMesh1Pointer = meshPointer1;
	tmpMesh2Pointer = meshPointer2;
	
	// setto le variabili di classe 
	meshPointer1 = &tmpMesh1;
	meshPointer2 = &tmpMesh2;
	
// 	createFile file;
// 	file.fileForParaview("prima.inp",   &tmpMesh1);
// 	file.fileForParaview("seconda.inp", &tmpMesh2);
	
	// creo la stuttura dati 
	createDataStructure();
	
	// creo l'intersezione
	createIntersection(nodiInt, elementiInt);
	
	// setto le variabili di classe 
	meshPointer1 = tmpMesh1Pointer;
	meshPointer2 = tmpMesh2Pointer;
}

template<class MESH1, class MESH2, UInt DIM> 
void meshIntersec<MESH1, MESH2, DIM>::createIntersectionStructured(vector<point> * nodiInt, vector<vector<UInt> > * elementiInt)
{
	// creo la struttura dati 
	meshSearchStructured<MESH1,DIM>  filter;
	vector<point>             primo,secondo;
	vector<UInt>			 neight;
	point 		   rapp,elemMax,elemMin;
// 	pair<bool, vector<UInt>  >       result;
	pair<bool, vector<point> >	  punti;
	
	// creo la struttura di ricerca statica per filtrare gli elementi della mesh
	filter.setMeshPointer(meshPointer1);
	
	// per ogni elemento della seconda mesh trovo l'intersezione 
 	for(UInt i=0; i<meshPointer2->getNumElements(); ++i)
	{	
		// prendo il massimo e il minimo 
		meshPointer2->createBBox(i, elemMax, elemMin);
		
		// setto rapp
		rapp.replace(elemMax, elemMin, 0.5);
		
		// cerco se intersecano 
		if(filter.getElementAroundPoint(rapp, 2, &neight))
		{
		    // prendo i nodi connessi 
		    meshPointer2->getNodeOfElement(i, &secondo);
		    
		    // metto quelli della mesh di base 
		    for(UInt j=0; j<neight.size(); ++j)
		    {
			// prendo i nodi connessi 
			meshPointer1->getNodeOfElement(neight[j], &primo);
			
			// interseco 
			punti = intersecElement(&primo, &secondo);
			  
			// a seconda di quello che dice punti aggiorno i vettori in input
			upDateList(&punti, nodiInt, elementiInt);
		    }  		  
		}
		
		
// 		// se interseca cerco i punti 
// 		if(result.first)
// 		{		  		  
// 		      // vedo tutti i possibili elementi che intersecano e trovo quello che effettivamente interseca
// 		      for(UInt j=0; j<result.second.size(); ++j)
// 		      {
// 			  // prendo i nodi connessi 
// 			  meshPointer1->getNodeOfElement(result.second[j], &primo);
// 			  
// 			  // interseco 
// 			  punti = intersecElement(&primo, &secondo);
// 			  
// 			  // a seconda di quello che dice punti aggiorno i vettori in input
// 			  upDateList(&punti, nodiInt, elementiInt);
// 		      }
// 		}
	}
	
	
}

template<class MESH1, class MESH2, UInt DIM> 
void meshIntersec<MESH1, MESH2, DIM>::createIntersection(mesh1d<Line> * inter, howIntersec type)
{
	// varaibli in uso 
	vector<point>                           nodiInt;
	vector<vector<UInt> >               elementiInt;
	geoElement<Line>                            lin;
	map<UInt, UInt>                        oldToNew;
	meshHandler<mesh1d<Line> , DIM>        finder0d;
	time_t                            start,end,dif;
	
	// stampe per vedere la procedura 
	cout << "Inizio processo di ricerca delle Intersezioni" << endl;
	time(&start);
	
	// faccio un clear
	inter->clear();
	
	// chiamo la funzione ch efa un'intersezione generica 
	switch(type)
	{
	  case(0):
		  createIntersection(&nodiInt, &elementiInt);
		  break;
	  case(1):
		  createIntersectionFast(&nodiInt, &elementiInt);
		  break;
	  case(2):
		  createIntersectionStructured(&nodiInt, &elementiInt);
		  break;
	}
		
	// se non ci sono elementi di intersezione ritorno
	if(nodiInt.size()==0)
	{
	    time(&end);
	    dif = difftime(end,start);
	    cout << "Intersezione completata: " <<  dif << " sec." << endl;
	    return;
	}
	
	// metto tutto in inter
	inter->insertNode(&nodiInt);
	
	// faccio un reserve e riempio il nuovo vettore con gli elementi 
	inter->getElementPointer()->reserve(elementiInt.size());
	for(UInt i=0; i<elementiInt.size(); ++i)
	{
	      // se il size di elementi è 2 vuol dire che è un edge corretto 
	      if(elementiInt[i].size()==2)
	      {
		    lin.setConnectedId(0, elementiInt[i][0]);
		    lin.setConnectedId(1, elementiInt[i][1]);
		    
		    // metto le informazioni in inter
		    inter->insertElement(lin);
	      }
	}
	
	// setto l'handler
	finder0d.setToll(toll);
	finder0d.setMeshPointer(inter);
	
	// rendo unici i punti 
	finder0d.setUpMesh();	
	
	time(&end);
	dif = difftime(end,start);
	cout << "Intersezione completata: " <<  dif << " sec." << endl;
}

template<class MESH1, class MESH2, UInt DIM> 
void meshIntersec<MESH1, MESH2, DIM>::createIntersection(mesh0d<simplePoint> * inter, howIntersec type)
{
	// varaibli in uso 
	vector<point>                           nodiInt;
	vector<vector<UInt> >               elementiInt;
	geoElement<simplePoint>                     lin;
	map<UInt, UInt>                        oldToNew;
	mesh1d<Line>                           interTmp;
	meshHandler<mesh1d<Line> , DIM>        finder0d;
	time_t                            start,end,dif;
	
	// stampe per vedere la procedura 
	cout << "Inizio processo di ricerca delle Intersezioni" << endl;
	time(&start);
	
	// faccio un clear
	inter->clear();
	
	// chiamo la funzione che fa un'intersezione generica 
	switch(type)
	{
	  case(0):
		  createIntersection(&nodiInt, &elementiInt);
		  break;
	  case(1):	    
		  createIntersectionFast(&nodiInt, &elementiInt);
		  break;
	  case(2):
		  createIntersectionStructured(&nodiInt, &elementiInt);
		  break;
	}
	
	// se non ci sono elementi di intersezione ritorno
	if(nodiInt.size()==0)
	{
	    time(&end);
	    dif = difftime(end,start);
	    cout << "Intersezione completata: " <<  dif << " sec." << endl;
	    return;
	}
	
	// metto tutto in inter
	interTmp.insertNode(&nodiInt);
	
	// setto l'handler
	finder0d.setToll(toll);
	finder0d.setMeshPointer(&interTmp);
	
	// genero la mappa 
	finder0d.uniqueNodes(&oldToNew);
	
	// cambio i nodi nella mesh
	finder0d.changeNodes(&oldToNew);
	
	// metto i nodi in iter
	inter->insertNode(interTmp.getNodePointer());

	// genero gli elementi 
	for(UInt i=0; i<elementiInt.size(); ++i)
	{
	      // se il size di elementi è 1 vuol dire che è un punto
	      if(elementiInt[i].size()==1)
	      {
		  // setto i connessi cambiando loro le coordinate prendendo le informazioni dalla mappa 
		  lin.setConnectedId(0, oldToNew[elementiInt[i][0]]);
		  
		  // metto le informazioni in inter
		  inter->insertElement(lin);
	      }
	}
	
	time(&end);
	dif = difftime(end,start);
	cout << "Intersezione completata: " <<  dif << " sec." << endl;
}

//
// Metodi che controllano se c'è l'intersezione 
//
template<class MESH1, class MESH2, UInt DIM> 
bool meshIntersec<MESH1, MESH2, DIM>::doElementIntersect(vector<point> * primo, vector<point> * secondo)
{
	// varaibli in uso
	bool 	     found=false,result=false;
	
	// controllo che cosa sarà il risultato dell'intersezione 
	if(handler.getShapeInter()==POINT)
	{
	      if((handler.getShape1()==TRIANGLE) && (handler.getShape2()==LINE))
	      {
		  result    = triaLineInter.doIntersec(primo, secondo);
		  found    = true;
	      }
	      else if((handler.getShape1()==LINE) && (handler.getShape2()==TRIANGLE))
	      {
		  result    = triaLineInter.doIntersec(secondo, primo);
		  found    = true;
	      }
	      else if((handler.getShape1()==LINE) && (handler.getShape2()==LINE))
	      {
		  result    = lineLineInter.doIntersec(primo, secondo);
		  found    = true;
	      }
	}
	else if(handler.getShapeInter()==LINE)
	{
	      if((handler.getShape1()==TRIANGLE) && (handler.getShape2()==TRIANGLE))
	      {
		  result    = triaTriaInter.doIntersec(primo, secondo);
		  found    = true;
	      }
	}
	
	// assert per essere sicuro di aver riempito result
    (void)found;
	assert(found);
	
	// ritorno il risultato 
	return(result);
}

template<class MESH1, class MESH2, UInt DIM> 
UInt meshIntersec<MESH1, MESH2, DIM>::analyzeIntersection()
{
	// controllo che sia diverso da null
	assert(meshPointer1!=NULL);
	assert(meshPointer2!=NULL);
	
	// varaibili in uso 
	UInt				 cont=0;
	pair<bool, vector<UInt>  >       result;
	vector<point>             primo,secondo;
	
	// creo la stuttura dati 
	createDataStructure();
		
	// per ogni elemento della seconda mesh trovo l'intersezione 
 	for(UInt i=0; i<meshPointer2->getNumElements(); ++i)
	{
		// prendo i nodi connessi 
		meshPointer2->getNodeOfElement(i, &secondo);
		
		// faccio l'intersezione 
		result = findElement(&secondo);
		
		// se interseca cerco i punti 
		if(result.first)
		{		  		  
		      // vedo tutti i possibili elementi che intersecano e trovo quello che effettivamente interseca
		      for(UInt j=0; j<result.second.size(); ++j)
		      {
			  // prendo i nodi connessi 
			  meshPointer1->getNodeOfElement(result.second[j], &primo);
			  
			  // controllo che intersecano 
			  if(doElementIntersect(&primo, &secondo))	++cont;
		      }
		}
	}
	
	// ritorno il numero di elementi che intersecano 
	return(cont);
}

template<class MESH1, class MESH2, UInt DIM> 
UInt meshIntersec<MESH1, MESH2, DIM>::analyzeIntersectionFast()
{
	// controllo che sia diverso da null
	assert(meshPointer1!=NULL);
	assert(meshPointer2!=NULL);

	// variabili in uso 
	UInt 				result;
	
	// creo la struttura dati 
	point 		  rapp,elemMax,elemMin;
	meshSearchStructured<MESH1,DIM> filter;
	MESH1		   	      tmpMesh1;
	MESH2		   	      tmpMesh2;
	MESH1	*	       tmpMesh1Pointer;
	MESH2	*	       tmpMesh2Pointer;
	vector<UInt>                    neight;
	set<UInt>	              meshBase;
	vector<UInt>		       meshExt;
	meshHandler<MESH1,DIM>		 hand1;
	meshHandler<MESH2,DIM>		 hand2;

	// creo la struttura di ricerca statica per filtrare gli elementi della mesh
	filter.setMeshPointer(meshPointer1);
	
	// ciclo sugli elementi per trovare quelli che intersecano 
	for(UInt i=0; i<meshPointer2->getNumElements(); ++i)
	{
		// prendo il massimo e il minimo 
		meshPointer2->createBBox(i, elemMax, elemMin);
		
		// setto rapp
		rapp.replace(elemMax, elemMin, 0.5);
		
		// salvo
		if(filter.getElementAroundPoint(rapp, 2, &neight))
		{
		    // metto quelli della mesh di base 
		    for(UInt j=0; j<neight.size(); ++j)	meshBase.insert(neight[j]);
		    
		    // metto l'elemento 
		    meshExt.push_back(i);
		}
		
	}
	
	// setto a 0 tutti gli id degli elementi	
	meshPointer1->setUpGeoId(0);
	meshPointer2->setUpGeoId(0);
	
	// setto i geoId che voglio eliminare 
	for(UInt i=0; i<meshExt.size(); ++i)					
	  meshPointer2->getElementPointer(meshExt[i])->setGeoId(1);
	for(set<UInt>::iterator it=meshBase.begin(); it!=meshBase.end(); ++it)	
	  meshPointer1->getElementPointer(*it)->setGeoId(1);
	
	// estraggo le superfici 
	hand1.setMeshPointer(meshPointer1);
	hand1.extractMesh(&tmpMesh1, 1);
	hand2.setMeshPointer(meshPointer2);
	hand2.extractMesh(&tmpMesh2, 1);
	
	// salvo i puntatori
	tmpMesh1Pointer = meshPointer1;
	tmpMesh2Pointer = meshPointer2;
	
	// setto le variabili di classe 
	meshPointer1 = &tmpMesh1;
	meshPointer2 = &tmpMesh2;
	
	// creo la stuttura dati 
	createDataStructure();
	
	// controllo le intersezioni 
	result = analyzeIntersection();
	
	// setto le variabili di classe 
	meshPointer1 = tmpMesh1Pointer;
	meshPointer2 = tmpMesh2Pointer;
	
	// ritorno il numrero di potenziali intersezioni 
	return(result);
}

template<class MESH1, class MESH2, UInt DIM> 
UInt meshIntersec<MESH1, MESH2, DIM>::analyzeIntersectionStructured()
{
	// creo la struttura dati 
	UInt 				 cont=0;
	meshSearchStructured<MESH1,DIM>  filter;
	vector<point>             primo,secondo;
	vector<UInt>			 neight;
	point 		   rapp,elemMax,elemMin;
	pair<bool, vector<UInt>  >       result;
	pair<bool, vector<point> >	  punti;
	
	// creo la struttura di ricerca statica per filtrare gli elementi della mesh
	filter.setMeshPointer(meshPointer1);
	
	// per ogni elemento della seconda mesh trovo l'intersezione 
 	for(UInt i=0; i<meshPointer2->getNumElements(); ++i)
	{	
		// prendo il massimo e il minimo 
		meshPointer2->createBBox(i, elemMax, elemMin);
		
		// setto rapp
		rapp.replace(elemMax, elemMin, 0.5);
		
		// cerco se intersecano 
		if(filter.getElementAroundPoint(rapp, 2, &neight))
		{
		    // prendo i nodi connessi 
		    meshPointer2->getNodeOfElement(i, &secondo);
		    
		    // metto quelli della mesh di base 
		    for(UInt j=0; j<neight.size(); ++j)
		    {
			// prendo i nodi connessi 
			meshPointer1->getNodeOfElement(neight[j], &primo);
			
			// controllo che intersecano 
			if(doElementIntersect(&primo, &secondo)) ++cont;
		    }  		  
		}
	}
	
	// ritorno il numero di intersezioni 
	return(cont);
}

template<class MESH1, class MESH2, UInt DIM> 
UInt meshIntersec<MESH1, MESH2, DIM>::doIntersect(howIntersec type)
{
	// varaibli in uso 
	UInt 					  num=0;
	time_t                            start,end,dif;
	
	// stampe per vedere la procedura 
	cout << "Inizio processo di analisi delle Intersezioni" << endl;
	time(&start);
	
	// chiamo la funzione ch efa un'intersezione generica 
	switch(type)
	{
	  case(0):
		  num = analyzeIntersection();
		  break;
	  case(1):
		  num = analyzeIntersectionFast();
		  break;
	  case(2):
		  num = analyzeIntersectionStructured();
		  break;
	}
	
	time(&end);
	dif = difftime(end,start);
	cout << "Analisi di intersezione completata: " <<  dif << " sec." << endl;
	
	// ritorno il numero 
	return(num);
}

}


#endif



