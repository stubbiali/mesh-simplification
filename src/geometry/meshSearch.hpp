#ifndef MESHSEARCH_HPP_
#define MESHSEARCH_HPP_

#include <cassert>
#include <ctime>
#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <stack>
#include <map>
#include <limits>

#include "../utility/tree.hpp"
#include "../utility/inSegment.h"
#include "../utility/inTriangle.h"

#include "../intersec/intervalIntersection.h"

#include "../geometry/mesh1d.hpp"
#include "../geometry/mesh2d.hpp"
#include "../geometry/mesh3d.hpp"
#include "../geometry/geoElementSearch.h"
#include "../geometry/geoElementSize.hpp"


namespace geometry
{

/*! Classe che permette di effettuare le ricerche su una mesh1d, 2d o 3d. La classe è templatizzata rispetti i seguenti parametri
<ol>
<li> DIM dimensione degli oggetti che sono salvati 
<li> MESH oggetto mesh che si sta analizzando 
<ol>
Le ricerche vengono effettuate seguendo la seguendo la strategia degli ADTtree e sfruttando il contenitore implementato da Kasper
Peeters*/

template<class MESH, UInt DIM=3> class meshSearch
{
	//
	// Variabili di classe 
	//	
	public:
		  /*! Puntatore all'oggetto Mesh*/
		  MESH        *                   meshPointer;
		  
		  /*! Struttura di ricerca 
		      N.B. più elementi possono avere lo stesso rappresentante e non essere necessariamente uguali*/
		  tree<vector<geoElementSearch> >      finder;
		  
		  /*! Valore della tolleranza */
		  Real                                   toll;
		  
		  /*! Valore degli estremi del BBox */
		  point                             pMax,pMin;
		  
		  /*! Classe che permette di fare le intersezioni degli intervalli */
		  intervalIntersection               intersec;
		  
		  /*! Classi che permettono di fare i test di appartenenza */
		  inSegment				inSeg;
		  inTriangle			       inTria;
	//
	// Costruttore
	//
	public:
		  /*! Costruttore vuoto */
		  meshSearch();
		  
		  /*! Costruttore non vuoto 
		      \param _meshPointer puntatore ad un oggetto di tipo mesh */
		  meshSearch(MESH  * _meshpointer);
		  
		  /*! Oggetto che permette di eliminare tutto quello che è stato creato*/
		  void clear();
	//
	// Metodi di set/get
	//	  
	public: 
		  /*! Metodo che setta il puntatore 
		      \param _meshPointer puntatore ad un oggetto di tipo mesh 
		      \param riordina parametro che dice se fare o meno il riordinamento */
		  void setMeshPointer(MESH * _meshPointer, bool riordina=true);
		  
		  /*! Metodo che restituisce il puntatore */
		  inline MESH * getMeshPointer();
		  
		  /*! Metodo che restituisce la tolleranza 
		      \param _toll valore della tolleranza */
		  void setToll(Real toll=1e-14);
		  
		  /*! Metodo che restituisce la tolleranza */
		  inline Real getToll();
	//
	// Metodi per la creazione della struttura di ricerca
	//
	public:
		  /*! Metodo che chiama in successione i vari metodo sottostanti e crea la struttura dati 
		      \param riordina parametro che dice se fare o meno il riordinamento*/
		  void buildDataStructure(bool riordina=true);
		  
		  /*! Metodo che crea un oggetto di tipo geoElementSearch partendo da un elemento della mesh  
		      \param elem puntatore a un oggetto di cui si voule creare un rappresentante*/
		  geoElementSearch createElement(geoElement<typename MESH::RefShape> * elem);
		  
		  /*! Metodo per ricavare la coordinata da analizzare in funzione della profondità dell'albero
		      \param dep intero che indica la profondità
		      \param elem oggetto di tipo geoElementSearch di cui deve essere presa la coordinata */
		  Real coorToAnalize(UInt dep, geoElementSearch & elem);
		  
		  /*! Metodo per ricavare la coordinata da analizzare in funzione della profondità dell'albero tale coordinata 
		      viene scelta per favorire la ricerca dell'elemento nell'albero quindi è alternata rispetto a quella del 
		      metodo precedente.
		      \param dep intero che indica la profondità
		      \param elem oggetto di tipo geoElementSearch di cui deve essere presa la coordinata 
		  N.B. se coorToAnalize(dep)-->XXmax allora coorToAnalizePrec(dep)-->XXmin*/
		  Real coorToAnalizePrec(UInt dep, geoElementSearch & elem);
		  
		  /*! Metodo che inserisce un elemento nella struttura dati
		      \param geoElementSearch oggetto che deve essere inserito */
		  void insertElement(geoElementSearch & elem);
		  
		  /*! Metodo che inserisce un elemento nella struttura dati
		      \param elemId identificatore dell'elemento
		      N.B. tale elemento deve essere nella mesh*/
		  void insertElement(UInt elemId);
		  
		  /*! Metodo che elimina un elemento 
		      \param geoElementSearch oggetto che deve essere eliminato 
		      \param fixedMesh parametro che fa il controllo sul bounding box viene settato a falso nel momento in cui 
		      la mesh evolvendo cambia il bounding box*/
		  void eraseElement(geoElementSearch & elem);
		  
		  /*! Metodo che toglie un elemento nella struttura dati
		      \param elemId identificatore dell'elemento
		      \param fixedMesh parametro che fa il controllo sul bounding box viene settato a falso nel momento in cui 
		      la mesh evolvendo cambia il bounding box
		      N.B. tale elemento deve essere nella mesh*/
		  void eraseElement(UInt elemId);
	//
	// Metodi per effettuare la ricerca
	//
	public:
		  /*! Metodo che interseca un boundig box con un punto 
		      \param p punto da considerare
		      \param boxMax punto massimo del box
		      \param boxMin punto minimo del box*/
		  bool pointBoxIntersecton(point p, point boxMax, point boxMin);
		  
		  /*! Metodo che interseca due boundingBox 
		      \param box1Max punto massimo del box
		      \param box1Min punto minimo del box
		      \param box2Max punto massimo del box
		      \param box2Min punto minimo del box*/
		  bool boxBoxIntersecton(point box1Max, point box1Min, point box2Max, point box2Min);
		  		  
		  /*! Metodo per i punti all'interno di un bbox
		      \param boxMax punto massimo del box
		      \param boxMin punto minimo del box
		      \param verb variabile che dice di stampare le informazioni
		  Il metodo ritorna una coppia che contiene un booleano che dice se ha trovato elementi e un vettore con quali 
		  elementi*/
		  pair<bool, vector<UInt> > findIntersection(point boxMax, point boxMin, bool verb=false);
		  
		  /*! Metodo che serve per stabilire se un punto è dentro alla mesh o comunque a un patch
		      \param node punto da testare
		  Il metodo ritorna una coppia che contiene un booleano che dice se ha trovato elementi e un vettore con quali 
		  elementi*/
		  pair<bool, vector<UInt> > isIn(point node);  
		  
		  /*! Metodo che serve trovare gli elementi più vicini a un bounding box quando il Box è esterno alla mesh
		      \param boxMax punto massimo del box
		      \param boxMin punto minimo del box
		  Il metodo ritorna una coppia che contiene un booleano che dice se ha trovato elementi e un vettore con quali 
		  elementi*/
		  pair<bool, vector<UInt> > nearElement(point boxMax, point boxMin);
		  
		  /*! Metodo che serve a trovare il nodo della mesh più vicino a un nodo interno
		      \param node nodo che si vuole trovare 
		  Il metodo ritorna una coppia che contiene la distanza fra i due nodi e l'id del nodo */
		  pair<Real, UInt> nearestNode(point node);
	//
	// Metodi per fare il riordinamento 
	//
	public:  
		  /*! Metodo che trova la mediana 
		      \param lista puntatore alla lista da cercare 
		      \param pos posizione da cercare 
		      \param dep parametro che dice la profondità
		      Il metodo ritorna l'identificatore */
		  UInt findKMedian(vector<geoElementSearch>  * listaTot, UInt pos, UInt dep);
		  
		  /*! Metodo che crea la lista 
		      \param listaTot puntatore a un contenitore delle liste*/
		  void createList(vector<geoElementSearch>  * listaTot);
		  
		  /*! Metodo che crea un vettore con l'ordine con cui inserire i gli elementi della classe
		      \param listIds puntatore a un vettore di dimensione pari al numero degli elementi che conterrà il 
				    nuovo ordine */
		  void riOrder(vector<UInt> * listIds);
};

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttori
//
template<class MESH, UInt DIM> 
meshSearch<MESH,DIM>::meshSearch()
{
      meshPointer = NULL;
      setToll();
}

template<class MESH, UInt DIM> 
meshSearch<MESH,DIM>::meshSearch(MESH * _meshPointer)
{
      // setto il puntatore
      meshPointer = _meshPointer;
      
      // creo la struttura dati 
      buildDataStructure();
      
      // setto la tolleranza
      setToll();
}

template<class MESH, UInt DIM> 
void meshSearch<MESH,DIM>::clear()
{
      // libero finder 
      finder.clear();
}
//
// Metodi di set/get
//
template<class MESH, UInt DIM> 
void meshSearch<MESH,DIM>::setMeshPointer(MESH * _meshPointer, bool riordina)
{
      // pulisco 
      finder.clear();
      
      // setto il puntatore
      meshPointer = _meshPointer;      
      
      // creo la struttura dati 
      buildDataStructure(riordina);
}
		  
template<class MESH, UInt DIM> 
inline MESH * meshSearch<MESH,DIM>::getMeshPointer()
{
      return(meshPointer);
}

template<class MESH, UInt DIM> 
void meshSearch<MESH,DIM>::setToll(Real _toll)
{
      toll = _toll;
      intersec.setToll(toll);
      inSeg.setToll(toll);
      inTria.setToll(toll);
}
		  
template<class MESH, UInt DIM> 
inline Real meshSearch<MESH,DIM>::getToll()
{
      return(toll);
}

//
// Metodi per la creazione della struttura di ricerca
//

template<class MESH, UInt DIM>
void meshSearch<MESH,DIM>::buildDataStructure(bool riordina)
{
      // variabili in uso 
      time_t                                     start,end;
      geoElementSearch                                elem;
      vector<UInt>  listIds(meshPointer->getNumElements());
      vector<geoElementSearch>           figlioDx,figlioSx;
      vector<geoElementSearch>                 iniziale(1);
      tree<vector<geoElementSearch> >::iterator         it;
      geoElement<typename MESH::RefShape>          elemTmp;
      Real                                             dif;
      set<UInt>                             elementiAlbero; // temporanee
      set<UInt>::iterator                           found; // temporanee
      
      // output
      cout << "Costruzione Struttura di ricerca..." << endl;
      time(&start);
      
      // riordino se lo voglio altrimenti creo una lista che va da 0 a meshPointer->getNumElements()-1
      if(riordina)		riOrder(&listIds);
      else			for(UInt i=0; i<meshPointer->getNumElements(); ++i)  listIds[i]=i;      
	  
      // prendo gli estremi della mesh
      meshPointer->createBBox(pMax, pMin);
      
      // ciclo per rendere nulle le coordinate non usate nella ricerca
      for(UInt i=DIM; i<3; ++i)
      {
	  pMax.setI(i, 0.0);
	  pMin.setI(i, 0.0);
      }
      	
      // creo la radice dell'albero
      elemTmp   = meshPointer->getElement(listIds[0]);     // copio l'elemento in un elemento fittizio
      elem      = createElement(&elemTmp);		   // creo l'elemento per la ricerca
      it        = finder.insert(finder.begin(), iniziale); // faccio la radice dell'albero
      it->at(0) = elem;					   // usiamo i vettori perché elementi diversi possono avere lo stesso 
							   // rappresentante
      // uso delle variabili temporanee per evitare il warning "taking address of temporary"
      
      // gli metto due figli 
      finder.append_child(it, figlioSx);
      finder.append_child(it, figlioDx);
      
      // ciclo sugli elementi
      for(UInt i=1; i<meshPointer->getNumElements(); ++i)
      {
	  // istanzio un elemento da inserire nell'albero 
	  geoElementSearch                toAdd;
	  
	  elemTmp = meshPointer->getElement(listIds[i]); // copio l'elemento in un elemento fittizio  
	  toAdd   = createElement(&elemTmp);		 // creo l'elemento per la ricerca
	  insertElement(toAdd);				 // lo metto nell'albero	  
	  // tutto sto casino per evitare il warning "taking address of temporary"
      }
      
      time(&end);
      dif = difftime(end,start);
      cout << "Profondità dell'albero: " << finder.max_depth() << "                          " << endl;
      cout << "Struttura di ricerca completata: " <<  dif << " sec.                          " << endl;
      
}

template<class MESH, UInt DIM>
geoElementSearch meshSearch<MESH,DIM>::createElement(geoElement<typename MESH::RefShape> * elem)
{
      // setto le variabili 
      geoElementSearch                       leaf;
      Real            	     MIN[3],MAX[3],MED[3];
      
      // inizializzo 
      MIN[0]=meshPointer->getNode(elem->getConnectedId(0)).getI(0);
      MIN[1]=meshPointer->getNode(elem->getConnectedId(0)).getI(1);
      MIN[2]=meshPointer->getNode(elem->getConnectedId(0)).getI(2);
      
      MAX[0]=meshPointer->getNode(elem->getConnectedId(0)).getI(0);
      MAX[1]=meshPointer->getNode(elem->getConnectedId(0)).getI(1);
      MAX[2]=meshPointer->getNode(elem->getConnectedId(0)).getI(2);
      
      MED[0]=0.0;
      MED[1]=0.0;
      MED[2]=0.0;
      
      // ciclo sull'elemento 
      for(UInt i=1; i<MESH::RefShape::numVertices; ++i)
      {
	  // ciclo sulle coordinate che mi interessano 
	  for(UInt j=0; j<DIM; ++j)
	  {
		MIN[j] = std::min(MIN[j],meshPointer->getNode(elem->getConnectedId(i)).getI(j));
		MAX[j] = std::max(MAX[j],meshPointer->getNode(elem->getConnectedId(i)).getI(j));
	  }
      }
      
      // calcolo i punti medi
      for(UInt j=0; j<DIM; ++j) MED[j] = (MAX[j]+MIN[j])*0.5;
      
      // setto le coordinate 
      for(UInt j=0; j<3; ++j)
      {
	leaf.setBoxMax(j, MAX[j]+1000.0*toll);
	leaf.setBoxMin(j, MIN[j]-1000.0*toll);
	leaf.setRapp(j, MED[j]);
      }
      
      // setto il suo id
      leaf.setId(elem->getId());
      
      // setto che è attivo perchè rappresenta un elemento che si sta cercando 
      leaf.setActive(true);
      
      // ritorno 
      return(leaf);
}

template<class MESH, UInt DIM> 
Real meshSearch<MESH,DIM>::coorToAnalize(UInt dep, geoElementSearch & elem)
{
      // variabile in uso 
      Real   valore=0.0;
      
      // switch per veder che cosa devo restiutire 
      switch(DIM)
      {
	 case(1):
		  if((dep%2)==0)	valore = elem.getBoxMin(0);
		  else if((dep%2)==1)	valore = elem.getBoxMax(0);
		  break;
	case(2):
		  if((dep%4)==0)	valore = elem.getBoxMin(0);
		  else if((dep%4)==1)	valore = elem.getBoxMax(0);
		  else if((dep%4)==2)	valore = elem.getBoxMin(1);
		  else if((dep%4)==3)	valore = elem.getBoxMax(1);
		  break;
	case(3):
		  if((dep%6)==0)	valore = elem.getBoxMin(0);
		  else if((dep%6)==1)	valore = elem.getBoxMax(0);
		  else if((dep%6)==2)	valore = elem.getBoxMin(1);
		  else if((dep%6)==3)	valore = elem.getBoxMax(1);
		  else if((dep%6)==4)	valore = elem.getBoxMin(2);
		  else if((dep%6)==5)	valore = elem.getBoxMax(2);
		  break;
      }
  
      // ritorno il valore 
      return(valore);
}

template<class MESH, UInt DIM> 
Real meshSearch<MESH,DIM>::coorToAnalizePrec(UInt dep, geoElementSearch & elem)
{
      // variabile in uso 
      Real   valore=0.0;
      
      // switch per veder che cosa devo restiutire 
      switch(DIM)
      {
	 case(1):
		  if((dep%2)==0)	valore = elem.getBoxMax(0);
		  else if((dep%2)==1)	valore = elem.getBoxMin(0);
		  break;
	case(2):
		  if((dep%4)==0)	valore = elem.getBoxMax(0);
		  else if((dep%4)==1)	valore = elem.getBoxMin(0);
		  else if((dep%4)==2)	valore = elem.getBoxMax(1);
		  else if((dep%4)==3)	valore = elem.getBoxMin(1);
		  break;
	case(3):
		  if((dep%6)==0)	valore = elem.getBoxMax(0);
		  else if((dep%6)==1)	valore = elem.getBoxMin(0);
		  else if((dep%6)==2)	valore = elem.getBoxMax(1);
		  else if((dep%6)==3)	valore = elem.getBoxMin(1);
		  else if((dep%6)==4)	valore = elem.getBoxMax(2);
		  else if((dep%6)==5)	valore = elem.getBoxMin(2);
		  break;
      }
  
      // ritorno il valore 
      return(valore);
}

template<class MESH, UInt DIM>  
void meshSearch<MESH,DIM>::insertElement(geoElementSearch & elem)
{
      // variabili in uso 
      UInt                                        where=0,dep=0;
      Real 					 valTree,valExt;
      bool                                  founded,added=false;
      tree<vector<geoElementSearch> >::iterator   itPadre,itTmp;
      
      // setto it all'inizio dell'albero
      itPadre = finder.begin();
	
      // ciclo per l'inserimento 
      while(!added)
      {	      
	      // caso in cui non c'è il padre 
	      if(itPadre->size()==0)
	      {
		  // lo inserisco alla posizone itPadre
		  itPadre->push_back(elem);
		  
		  // genero i figli 
		  vector<geoElementSearch>   figlioSx,figlioDx;
		  
		  // metto due figli nulli
		  finder.append_child(itPadre, figlioSx);
		  finder.append_child(itPadre, figlioDx);
		  
		  // setto added
		  added = true;
	      }
	      // caso in cui è lo stesso punto 
	      else if(((itPadre->at(0).getRapp()-elem.getRapp()).norm2())<toll)
	      {
		  // setto founded
		  founded = false;
		  
		  // controllo che non abbia lo stesso id di quelli già contenuti
		  for(UInt i=0; i<itPadre->size(); ++i) 
		    if((itPadre->at(i).getId()==elem.getId()) && itPadre->at(i).isActive())
		      founded = true;
		  
		  // lo aggiungo se non l'ho trovato 
		  if(!founded)	itPadre->push_back(elem);
		  
		  // setto added
		  added = true;
	      }
	      // caso in cui c'è il padre e non è lo stesso punto 
	      else
	      {  
		  // cerco la coordinata
		  valTree = coorToAnalize(dep, itPadre->at(0));
		  valExt  = coorToAnalize(dep, elem);
		  
		  // controllo quello di sinistra
		  if(valExt<valTree)    where = 0;
		  else 			where = 1;
	      }
	      
	      // incremento dep
	      ++dep;
	      
	      // setto il nuovo padre
	      itTmp   = itPadre;
	      itPadre = finder.child(itTmp, where);
      }
      
      // ciclo per sistemare il bBox
      for(UInt i=0; i<DIM; ++i)
      {
	    pMax.setI(i, std::max(pMax.getI(i), elem.getBoxMax(i)));
	    pMin.setI(i, std::min(pMin.getI(i), elem.getBoxMin(i)));
      }
}

template<class MESH, UInt DIM>  
void meshSearch<MESH,DIM>::insertElement(UInt elemId)
{
      assert(elemId<meshPointer->getNumElements());
      
      // varaibili in uso 
      geoElementSearch			    elemSearch;
      geoElement<typename MESH::RefShape >     elemTmp;
      
      // prendo l'elemento
      elemTmp = meshPointer->getElement(elemId);
      
      // creo l'elemento di ricerca 
      elemSearch = createElement(&elemTmp);
      
      // lo inserisco
      insertElement(elemSearch);
}

template<class MESH, UInt DIM>  
void meshSearch<MESH,DIM>::eraseElement(geoElementSearch & elem)
{
      // varaibili sicuramente utilizzate
      pair<bool, vector<UInt> >                    result;      
      point                           			p;
      
      // prendo il nodo 
      p = elem.getRapp();
      
      // controllo che sta nel bounding box
      if(!pointBoxIntersecton(p, pMax, pMin))
      {
	cout << "Il punto da eliminare non è nell'albero" << endl;
	return;
      }
      
      // Variabili in uso 
      UInt                                                                     where=0,dep=0;
      Real 			        				      valTree,valExt;
      point                                                                            ptTmp;
      tree<vector<geoElementSearch> >::iterator      		    itToRemove,itPadre,itTmp;
      vector<geoElementSearch>::iterator				                  it;
      
      // setto a null 
      itToRemove     = NULL;
      
      // setto it all'inizio dell'albero
      itPadre = finder.begin();
      
      // esploro l'albero cercando di fissare la variabile
      // itToRemove     -> conterrà l'indirizzo del nodo da eliminare
      while(1)
      {
	    // controllo 
	    if(itPadre->size()==0)	cout << "ERRORE: quando eliminio l'elemento non lo trovo!" << endl;
	
	    // prendo il rappresentante del nodo da eliminare 
	    ptTmp = itPadre->at(0).getRapp();
	    	   
	    // l'ho trovato salvo l'indirizzo
	    if((ptTmp-p).norm2()<toll)
	    {
		// setto chi devo rimuovere 
		itToRemove = itPadre;
		
		// mi fermo 
		break;	
	    }
	      
	    // cerco lfindera coordinata
	    valTree = coorToAnalize(dep, itPadre->at(0));
	    valExt  = coorToAnalize(dep, elem);
		  
	    // controllo quello di sinistra
	    if(valExt<valTree)    where = 0;
	    else 		  where = 1;
	    
	    // incremento dep
	    ++dep;
	      
	    // setto il nuovo padre
	    itTmp   = itPadre;
	    itPadre = finder.child(itTmp, where);
	    
      }
      
      // a questo livello DEVO aver trovato solamente quello da sostituire
      if(!(itToRemove!=NULL))
      {
	  cout << "Non ho trovato nell'albero il punto da eliminare" << endl;
	  return;
      }
      
      // lo metto inattivo 
      for(UInt i=0; i<itToRemove->size(); ++i)
	if(itToRemove->at(i).getId()==elem.getId())
	  itToRemove->at(i).setActive(0);
      
}

template<class MESH, UInt DIM>  
void meshSearch<MESH,DIM>::eraseElement(UInt elemId)
{
      assert(elemId<meshPointer->getNumElements());
      
      // varaibili in uso 
      geoElementSearch			    elemSearch;
      geoElement<typename MESH::RefShape >     elemTmp;
      
      // prendo l'elemento
      elemTmp = meshPointer->getElement(elemId);
      
      // creo l'elemento di ricerca 
      elemSearch = createElement(&elemTmp);
      
      // lo inserisco
      eraseElement(elemSearch);
}

//
// Metodi per effettuare la ricerca
//
template<class MESH, UInt DIM>  
bool meshSearch<MESH,DIM>::pointBoxIntersecton(point p, point boxMax, point boxMin)
{
      // controllo che sta nel bounding box
      if((!((p.getX()<=(boxMax.getX()+toll)) && (p.getX()>=(boxMin.getX()-toll)))) ||
	 (!((p.getY()<=(boxMax.getY()+toll)) && (p.getY()>=(boxMin.getY()-toll)))) ||
	 (!((p.getZ()<=(boxMax.getZ()+toll)) && (p.getZ()>=(boxMin.getZ()-toll)))) )
      {  
	    return(false);
      }
      
      return(true);
}

template<class MESH, UInt DIM>  
bool meshSearch<MESH,DIM>::boxBoxIntersecton(point box1Min, point box1Max, point box2Min, point box2Max)
{
      // variabli in uso 
      bool         toto=true;
      bool       result[DIM];
      
      // faccio i test 
      for(UInt j=0; j<DIM; ++j) 
	result[j] = intersec.doIntersec(box1Min.getI(j), box1Max.getI(j), box2Min.getI(j), box2Max.getI(j));
	
      // interseco le informazioni
      for(UInt j=0; j<DIM; ++j)
	toto = (toto && result[j]);
      
      
      // ritorno 
      return(toto);
}

template<class MESH, UInt DIM> 
pair<bool, vector<UInt> > meshSearch<MESH,DIM>::findIntersection(point boxMax, point boxMin, bool verb)
{
      // variabli in suo
      pair<bool, vector<UInt> >   result;
      point                        p,tmp;
      
      // setto tmp 
      tmp.setX(1.0);	tmp.setY(1.0);	tmp.setZ(1.0);
      
      // do spessore agli elementi che hanno un bbox troppo piccolo
      if((boxMax-boxMin).norm2()<toll)
      {
	      p = boxMax;
	      boxMax = p + tmp*toll*1000.0;
	      boxMin = p - tmp*toll*1000.0;
      }
     
      // controllo che sta nel bounding box
      // 
      // controllo contemporaneamente tutti gli estremi del bbox dell'oggetto e del box passato in input
      if(!(boxBoxIntersecton(boxMin, boxMax, pMin, pMax)))
      {
	    result.first  = false;
	    result.second.resize(0);
		  
	    return(result);
      }
            
      // Variabili in uso 
      UInt                                                     dep,cont;
      Real 		  			       coorTree,coorExt;
      point 					    boxMaxTmp,boxMinTmp;
      geoElementSearch				 	        elemExt;
      tree<vector<geoElementSearch> >::iterator itPadre,itTmpDx,itTmpSx;
      stack<tree<vector<geoElementSearch> >::iterator>        toAnalize;
      set<UInt>                                                papabili;
      
      // setto itPadre
      itPadre = finder.begin();
      
      // se ci sono 
      toAnalize.push(itPadre);
      
      // creo l'elemento per la ricerca 
      // 
      // per quest routine non serve settare la variabile id e nemmeno il rappresentante 
      // 
      elemExt.setBoxMax(boxMax);
      elemExt.setBoxMin(boxMin);
      
      // conto quanti ne visito
      cont = 0;
      
      // ciclo sulla dimensione dello stack
      while(!toAnalize.empty())
      {
	    // prendo l'iteratore
	    itPadre = toAnalize.top();
	    
	    // lo tolgo dallo stack
	    toAnalize.pop();
	    
	    // per ogni elemento nel nodo
	    for(UInt j=0; j<itPadre->size(); ++j)
	    {
		  // prendo il suo box
		  boxMaxTmp = itPadre->at(j).getBoxMax();
		  boxMinTmp = itPadre->at(j).getBoxMin();
		  
		  cont++;
	    
		  // se sono nel box 
		  if((itPadre->at(j).isActive()) && 
		     (boxBoxIntersecton(boxMinTmp, boxMaxTmp, boxMin, boxMax)))	papabili.insert(itPadre->at(j).getId());
	    }
	    
	    // prendo la profondità
	    dep = finder.depth(itPadre);
		
	    // prendo i due figli
	    itTmpDx = finder.child(itPadre, 1);
	    itTmpSx = finder.child(itPadre, 0);
	    
	    // prendo la coordinata da analizzare 
	    coorExt = coorToAnalizePrec(dep, elemExt);
	    
	    // passi pari ossia guardo il precedente che è un massimo 
	    if((dep%2)==0)
	    {
		// prendo la coordinata dell'albero 
		coorTree = coorToAnalize(dep, itPadre->at(0));
		   
		// prendo solo il ramo di destra se ci sono nodi e se è vera la disuguaglianza 
		if(coorExt<(coorTree-toll))
		{
		    if(itTmpSx->size()!=0)	toAnalize.push(itTmpSx);
		}
		else
		{
		    if(itTmpDx->size()!=0)	toAnalize.push(itTmpDx);
		    if(itTmpSx->size()!=0)	toAnalize.push(itTmpSx);
		}
	    }
	    // passi pari ossia guardo il precedente che è un minimo 
	    else
	    {
		// prendo la coordinata dell'albero 
		coorTree = coorToAnalize(dep, itPadre->at(0));
		   
		// prendo solo il ramo di destra se ci sono nodi e se è vera la disuguaglianza 
		if(coorExt>(coorTree+toll))
		{
		    if(itTmpDx->size()!=0)	toAnalize.push(itTmpDx);
		}
		else
		{
		    if(itTmpDx->size()!=0)	toAnalize.push(itTmpDx);
		    if(itTmpSx->size()!=0)	toAnalize.push(itTmpSx);
		}
	    }
      }
      
      if(verb) cout << "Elementi visitati " << cont << " (elementi della mesh " << meshPointer->getNumElements() << " )" << endl;
      
      // riempio il vettore
      result.second.resize(papabili.size());
      copy(papabili.begin(), papabili.end(), result.second.begin());
      
      // metto a posto il secondo 
      if(papabili.size()==0)	result.first = false;
      else			result.first = true;
      
      // ritorno 
      return(result);
}

template<class MESH, UInt DIM>  
pair<bool, vector<UInt> > meshSearch<MESH,DIM>::isIn(point node)
{
      // variabli in suo
      point				       tmp;
      vector<point>			      elem;
      pair<bool, vector<UInt> >   result,resultTmp;
      
      // cerco i possibili elementi che intersecano
      resultTmp = findIntersection(node, node);
      
      // faccio un reserve 
      result.second.reserve(resultTmp.second.size());
      
      // per ogni elemento testo 
      for(UInt i=0; i<resultTmp.second.size(); ++i)
      {
	    // prendo i nodi
	    meshPointer->getNodeOfElement(resultTmp.second[i], &elem);
	    
	    switch(MESH::RefShape::Shape)
	    {
	      case(POINT):
			  // prendo il punto 
			  tmp = meshPointer->getNode(meshPointer->getElement(resultTmp.second[i]).getConnectedId(0));
			  
			  // controllo 
			  if((tmp-node).norm2()<toll)	result.second.push_back(resultTmp.second[i]);
			  
			  break;
	      case(LINE):
			  // faccio il test
			  if(inSeg.isIn(&elem, node))	result.second.push_back(resultTmp.second[i]);
			  
			  break;
	      case(TRIANGLE):
			  // faccio il test
			  if(inTria.isIn(&elem, node))	result.second.push_back(resultTmp.second[i]);
			  
			  break;
	    }
      }
      
      // controllo se ho riempito la seconda parte della lista 
      if(result.second.size()!=0)  result.first = true;
      else			   result.first = false;
      
      // ritorno il risultato 
      return(result);
}

template<class MESH, UInt DIM>
pair<bool, vector<UInt> > meshSearch<MESH,DIM>::nearElement(point boxMax, point boxMin)
{
    // varaibili in uso 
    Real    fatt = max(max(fabs(boxMax.getX()-boxMin.getX())*0.5,
			   fabs(boxMax.getY()-boxMin.getY())*0.5), 
			   fabs(boxMax.getZ()-boxMin.getZ())*0.5);
    pair<bool, vector<UInt> > result;
    
    // setto result
    result.first = false;
    
    // cerco le intersezioni 
    result = findIntersection(boxMax, boxMin);
    
    // ciclo while che incrementa alla fine sempre il bbox
    while(!result.first)
    {	
	  // incremento il box 
	  for(UInt i=0; i<DIM; ++i)
	  {
	      boxMax.setI(i, boxMax.getI(i)+fatt);
	      boxMin.setI(i, boxMin.getI(i)-fatt);
	  }
	  
	  // cerco le intersezioni 
	  result = findIntersection(boxMax, boxMin);
    }
    
    return(result);
}

template<class MESH, UInt DIM>
pair<Real, UInt> meshSearch<MESH,DIM>::nearestNode(point node)
{
    // variabili in uso
    UInt 		       id,idTmp;
    Real                   dist,distTmp;
    point   boxMax,boxMin,tmp(1.,1.,1.);
    pair<Real, UInt>	         result;
    pair<bool, vector<UInt> > resultTmp;
    set<UInt>			vertici;
    
    // prendo un fattore decente
    Real    fatt = max(max(fabs(pMax.getX()-pMin.getX())*0.001,
			   fabs(pMax.getY()-pMin.getY())*0.001), 
			   fabs(pMax.getZ()-pMin.getZ())*0.001);
    
    // prendo i punti
    boxMax = node + tmp*fatt;
    boxMin = node - tmp*fatt;
    
    // prendo gli elementi più vicini 
    resultTmp = nearElement(boxMax, boxMin);
    
    // prendo i nodi 
    for(UInt i=0; i<resultTmp.second.size(); ++i)
      for(UInt j=0; j<MESH::RefShape::numVertices; ++j)
	vertici.insert(meshPointer->getElement(resultTmp.second[i]).getConnectedId(j));
    
    // calcolo le distanze e salvo il più alto 
    dist    = 9e+99;
    distTmp = 9e+99;
    id      = meshPointer->getNumNodes();
    idTmp   = meshPointer->getNumNodes();
    
    // ciclo sui vertici trovati
    for(set<UInt>::iterator it=vertici.begin(); it!=vertici.end(); ++it)
    {
	// prendo le quantità
	idTmp   = *it;
	distTmp = (meshPointer->getNode(idTmp)-node).norm2();
	
	// controllo i valori 
	if(distTmp<dist)
	{
	    id   = idTmp;
	    dist = distTmp;
	}
    }
    
    // aggiorno result 
    result.first  = dist;
    result.second = id;
      
    return(result);
}

//
// Metodi per la mediana 
//
template<class MESH, UInt DIM>
UInt meshSearch<MESH,DIM>::findKMedian(vector<geoElementSearch>  * lista, UInt pos, UInt dep)
{
    // variabili in uso 
    Real                              val=0.0;
    UInt 	                      i=0,j=0;
    UInt                l=0,m=lista->size()-1;
    geoElementSearch                        t;
    
    while(l<m)
    {
	// prendo il size 
	val = coorToAnalize(dep, lista->at(pos));
	
	// setto i valori 
	i=l;
	j=m;
	
	do
	{
	    while(coorToAnalize(dep, lista->at(i))<val)	++i;
	    while(coorToAnalize(dep, lista->at(j))>val)	--j;
	    
	    if(i<=j)
	    {
		t = lista->at(i);
		lista->at(i) = lista->at(j);
		lista->at(j) = t;
		++i;
		
		// se j è zero mi devo fermare 
		if(j==0)	break;
		
		--j;
	    }
	
	} while(i<=j);
	  
	if(j<pos)	l=i;
	if(pos<i) 	m=j;
    }
        
    return(lista->at(pos).getId());
}

template<class MESH, UInt DIM>
void meshSearch<MESH,DIM>::createList(vector<geoElementSearch>  * listaTot)
{
    // variabili in uso 
    listaTot->resize(meshPointer->getNumElements());
    
    // faccio la lista dei BBOX
    for(UInt i=0; i<meshPointer->getNumElements(); ++i)	listaTot->at(i) = createElement(meshPointer->getElementPointer(i));    
}

template<class MESH, UInt DIM>
void meshSearch<MESH,DIM>::riOrder(vector<UInt> * listIds)
{
    // variabili in uso 
    UInt 		              id=0,dep=0;
    UInt 			           div=1;
    vector<geoElementSearch>	        listaTot;
    vector<geoElementSearch>    	listaPar;
    vector<UInt>::iterator		      it;
    vector<UInt>                 ids,idsTmp,posV;
    set<UInt>				   added;
    
    // creo la lista 
    createList(&listaTot);
    
    // faccio un reserve 
    idsTmp.reserve(meshPointer->getNumElements());
    listIds->reserve(meshPointer->getNumElements());
    
    cout << "Riordinamento degli Elementi" << endl;
    
    if(meshPointer->getNumElements()<10)
    {
	listIds->resize(meshPointer->getNumElements());
	for(UInt i=0; i<meshPointer->getNumElements(); ++i)	listIds->at(i)=i;
	return;
    }
    
    // vedo fino a quanto voglio spingermi 
    while(true)
    {
	  // riempio pos 
	  posV.clear();
	  posV.reserve(div+1);
	  for(UInt i=0; i<=div; ++i) posV.push_back((static_cast<Real>(i)/static_cast<Real>(div))*listaTot.size());
	  
	  // creo le liste temporanee 
	  for(UInt i=0; i<div; ++i)
	  {
	      // faccio un reserve 
	      listaPar.clear();
	      listaPar.reserve(listaTot.size()/div);
	      
	      // riempio il vettore a seconda di dove sono lo riempio in modo differente 
	      // caso in cui di due numeri sono uguali 
	      if(posV[i]==posV[i+1])
	      {
		  if(added.find(listaTot[posV[i]].getId())==added.end())
		  {
		      // metto negli id temporanei 
		      idsTmp.push_back(listaTot[posV[i]].getId());
		      
		      // per vedere bene l'albero 
// 		      meshPointer->getElementPointer(listaTot[posV[i]].getId())->setGeoId(dep+1);
		      
		      // lo aggiungo in added 
		      added.insert(listaTot[posV[i]].getId());
		      
		      // lo metto in listaPar in modo da saltare l'if di sotto 
		      listaPar.push_back(listaTot[posV[i]]);
		  }
	      }
	      // il primo della lista
	      else if(div==1)
	      {
		  for(UInt j=posV[i]; j<posV[i+1]; ++j)		listaPar.push_back(listaTot[j]);
	      }
	      else if(i==0)
	      {
		  for(UInt j=posV[i]; j<(posV[i+1]-1); ++j)	listaPar.push_back(listaTot[j]);
	      }
	      // l'ultimo della lista
	      else if(i==(div-1))
	      {
		  for(UInt j=(posV[i]-1); j<posV[i+1]; ++j)	listaPar.push_back(listaTot[j]);
	      }
	      // quelli in mezzo 
	      else
	      {
		  for(UInt j=(posV[i]-1); j<(posV[i+1]-1); ++j)	listaPar.push_back(listaTot[j]);
	      }
	      
	      // se la lista non è vuota
 	      if(listaPar.size()>1)
	      {
		      // prendo gli id 
		      id = findKMedian(&listaPar, static_cast<UInt>(listaPar.size()/2.), dep);
		      
		      // vedo se ho già messo l'id 
		      if(added.find(id)==added.end())
		      {
			  // metto negli id temporanei 
			  idsTmp.push_back(id);
			  
			  // lo aggiungo in added 
			  added.insert(id);
			  
			  // per vedere bene l'albero 
// 			  meshPointer->getElementPointer(id)->setGeoId(dep+1);
		      }
		      
	      }
	       
	      // se ho trovato tutti gli elementi mi fermo 
	      if(idsTmp.size()==meshPointer->getNumElements())	break;
	  }  
	  
	  // se ho trovato tutti gli elementi mi fermo 
	  if(idsTmp.size()==meshPointer->getNumElements())	break;
	    	     
	  // setto div
	  div = div*2;
	  
	  // incremento il dep 
	  ++dep;
    }
    
    cout << "Ho messo in modo corretto " << idsTmp.size() << " elementi                                " << endl;

    
    // copio 
    listIds->resize(idsTmp.size());
    copy(idsTmp.begin(), idsTmp.end(), listIds->begin()); 
}


}



#endif

