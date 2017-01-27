#include <sstream>

#include "meshDataSimplification.h"

using namespace std;
using namespace geometry;

//
// Costruttore 
//
meshDataSimplification<Triangle>::meshDataSimplification() : simplification2d<Triangle>()
{
    times              = 1.5;
    dontTouch          = false;
    elemDontTouch      = 0;
    colorElemDontTouch = 0;
    numGlobElem        = 0.0;
    numGlobPtAss       = 0.0;
    meanAssValue       = 0.0;
    maxGeoCost         = 0.0;
    maxAssCost         = 0.0;
    maxDistCost        = 0.0;
    pesoGeo            = 1./3.;
    pesoAss            = 1./3.;
    pesoDist           = 1./3.;
    listToUpdate       = false;
}

void meshDataSimplification<Triangle>::setMeshPointer(mesh2d<Triangle> * _meshPointer)
{
    // variabili in uso 
    vector<point>		nodi;
    
    // chiamo il setMeshPointer del dottore
    doctor2d<Triangle>::setMeshPointer(_meshPointer);
    
    // creo il vettore Q
    setUpQ();
    
    // riempi il vettore memento 
    mem.resize(meshPointer->getNumElements());
    
    // metto a posto gli oggetti
    for(UInt i=0; i<mem.size(); ++i)
    {
	// prendo i nodi 
	meshPointer->getNodeOfElement(i, &nodi);
	
	// metto a posti gli id
	mem[i].setElement(meshPointer->getElement(i));
	
	// setto i nodi e i connessi
	mem[i].setNodi(&nodi);
	mem[i].setPtAss(&nodi);
	mem[i].setToll(toll);
    }
    
    // sistemo le atre variabili 
    numGlobElem  = static_cast<Real>(meshPointer->getNumElements());
    numGlobPtAss = static_cast<Real>(meshPointer->getNumNodes());
    
    // sistemo il valore 
    meanAssValue  = 0.0;
    
    // sistemo la variabile finder 
    finder.setMeshPointer(_meshPointer);
}

void meshDataSimplification<Triangle>::refresh()
{
      
      // variabili in uso
      graphItem 		       tmpGraph;
      vector<point>                       tmpPt;
      vector<point>			tmpNode;
      vector<geoElement<Triangle> >       tmpTr;
      point                                   p;
      geoElement<Triangle>                 tria;
      mementoElement<Triangle>		memElem;
      vector<bool>    		         active;
      vector<UInt>         	          newId;
      UInt                 tmp,cont,id1,id2,id3;
      Real                                x,y,z;
      vector<geoElement<Line> >     lineListTmp;
      vector<mementoElement<Triangle> >  tmpMem;
      
      // faccio un reserve
      active.reserve(meshPointer->getNumNodes());
      newId.reserve(meshPointer->getNumNodes());
      tmpPt.reserve(meshPointer->getNumNodes());
      tmpTr.reserve(meshPointer->getNumElements());
      tmpMem.reserve(meshPointer->getNumElements());
      
      // setto il vettore active che mi dice quali nodi sono attivi
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
      {
	    active.push_back(false);
	    newId.push_back(-1);
      }
      
      // vedo quali sono gli elementi attivi
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	    if(!isTriangleDegenerate(i))
	    {
		  id1 = meshPointer->getElement(i).getConnectedId(0);
		  id2 = meshPointer->getElement(i).getConnectedId(1);
		  id3 = meshPointer->getElement(i).getConnectedId(2);
		  
		  active[id1] = true;
		  active[id2] = true;
		  active[id3] = true;
	    }
      }
            
      // ciclo sui nodi pre creare la nuova lista
      cont = 0;
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
      {
	    if(active[i])
	    {
		    // prendo le coordinate
		    x   = meshPointer->getNode(i).getX();
		    y   = meshPointer->getNode(i).getY();
		    z   = meshPointer->getNode(i).getZ();
		    tmp = meshPointer->getNode(i).getBoundary();
		    
		    // setto una variabile p temporanea
		    p.setX(x);	  p.setY(y);	p.setZ(z);
		    p.setBoundary(tmp);         p.setId(cont);
		    
		    // metto a posto gli id
		    newId[i] = cont;
		    
		    // aggiorno cont
		    ++cont;
		    
		    // inserisco nella lista temporanea dei nodi
		    tmpPt.push_back(p);
	    }
      }
      
      // ciclo sugli elementi per sistemare gli id dei nodi e metterli nella lista temporanea
      cont = 0;
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	    if(!isTriangleDegenerate(i))
	    {
		  id1 = newId[meshPointer->getElement(i).getConnectedId(0)];
		  id2 = newId[meshPointer->getElement(i).getConnectedId(1)];
		  id3 = newId[meshPointer->getElement(i).getConnectedId(2)];
		  tmp = meshPointer->getElement(i).getGeoId();
		  
		  // se è attiva la flag di non toccare lo coloro in modo diverso 
		  if(dontTouch && i==elemDontTouch)	tria.setGeoId(colorElemDontTouch);
		  else					tria.setGeoId(tmp);
		  
		  // creo la mia nuova variabile triangolo
		  tria.setConnectedId(0,id1);
		  tria.setConnectedId(1,id2);
		  tria.setConnectedId(2,id3);
		  tria.setId(cont);
		  ++cont;
		  
		  // metto nella lista dei triangoli
		  tmpTr.push_back(tria);
		  
		  // setto il memento 
		  memElem.setElement(tria);
		  
		  // prendo i punti 
		  tmpNode.clear();
		  tmpNode.resize(3);
		  tmpNode[0]= tmpPt[id1];	tmpNode[1]= tmpPt[id2];	  	tmpNode[2]= tmpPt[id3];
		  
		  // li setto nel memento element
		  memElem.setNodi(&tmpNode);
		  
		  // prendo gli associati e li setto nei nuovi memento 
		  tmpNode = mem[i].getPtAss();
		  memElem.setPtAss(&tmpNode);
		  
		  // lo metto nella lista 
		  tmpMem.push_back(memElem);
		  
		  // pulisco 
		  memElem.clear();
	    }
      }
      
      
      // metto i memento 
      mem.clear();
      mem.resize(tmpMem.size());
      copy(tmpMem.begin(), tmpMem.end(), mem.begin());
      
      // libero le liste
      meshPointer->clear();
      
      // riempio i nodi
      meshPointer->insertNode(&tmpPt);
      
      // riempio gli elementi
      meshPointer->insertElement(&tmpTr);
        
      // metto a posto gli id
      meshPointer->setUpIds();
      
      // faccio un setUp
      setUp();
      
      // creo il vettore Q
      setUpQ();
      
      // pulisco la variabile finder 
      finder.setMeshPointer(meshPointer);
      
      // ricostruisco la lista 
      createElementList();
}

//
// Setting e getting delle variabili 
//
void meshDataSimplification<Triangle>::setToll(Real _toll)
{
    // setto la variabile di ambiente 
    toll = _toll;
    
    // setto la tolleranza negli oggetti mementoElement
    for(UInt i=0; i<mem.size(); ++i)	mem[i].setToll(toll);
    
    // setto la variabile intersect
    intersec.setToll(toll);
    inTria.setToll(toll);
}

void meshDataSimplification<Triangle>::setFattMemento(Real _fatt)
{
    // lo setto per ognuno 
    for(UInt i=0; i<mem.size(); ++i)	mem[i].setFatt(_fatt);
}

Real meshDataSimplification<Triangle>::getFattMemento()
{
    // controllo ci siano elementi nella lista
    if(mem.size()==0)
    {
	cout << "Non ho salvato alcun elemento nella lista memento" << endl;
	return(-1.);
    }
    
    // ne ritorno uno tanto tutti sono uguali 
    return(mem[0].getFatt());
}

void meshDataSimplification<Triangle>::setPointToTrack(vector<point>  * _toTrack)
{
    // faccio un reszie e uso stl
    toTrack.resize(_toTrack->size());
    copy(_toTrack->begin(), _toTrack->end(), toTrack.begin());
}

Real meshDataSimplification<Triangle>::getMeanElemAss()
{
//       // variabili in uso
//       vector<Real>  			val,tmpVal;
//       pair<int,Real>			    result;
//       vector<point>		        ptAss,nodi;
// 	
//       // faccio un reserve 
//       val.reserve(meshPointer->getNumElements());
//       
//       // controllo le distanze delle associazioni 
//       for(UInt i=0; i<mem.size(); ++i)
//       {
// 	   // controllo se è degenere
// 	   if(!isTriangleDegenerate(i))
// 	   {
// 		// prendo gli associati 
// 		ptAss = mem[i].getPtAss();
// 	   
// 		// prendo i nodi 
// 		nodi = mem[i].getNodi();
// 	    
// 		// ciclo su ptAss
// 		tmpVal.assign(ptAss.size(),0.0);
// 	   
// 		// calcolo i pesi 
// 		for(UInt j=0; j<ptAss.size(); ++j) 	tmpVal[j] = evaluateWeight(i, &nodi, ptAss[j]);
// 	   
// 		// se ha nodi associati lo conto 
// 		if(tmpVal.size()!=0) 			val.push_back(accumulate(tmpVal.begin(), tmpVal.end(), 0.0));
// 	   }
//       }
//       
//       // devono esserci delle associazioni 
//       assert(val.size()!=0);
//       
//       // ritorno la media 
//       return((accumulate(val.begin(), val.end(), 0.0)/static_cast<Real>(val.size())));

      return(numGlobPtAss/numGlobElem);
      
}

//
// Metodi per il calcolo dello score relativo a la lunghezza del lato 
//
Real meshDataSimplification<Triangle>::getEdgeMetricCost(vector<UInt> * edge, point pNew)
{
    // variabili in uso     
    vector<Real>	Q;
    
    // creo la matrice dell'edge 
    Q = createQ(edge);
       
    // ritorno il costo 
    return(getEdgeCost(&Q, pNew));
}

void meshDataSimplification<Triangle>::getElemEdgeMetricCost(UInt elemId, 
							     vector<vector<UInt> > * edges, 
							     vector<point> * ptMed, 
							     vector<Real> * cost)
{
     // variabili in uso
     UInt 		      count = 0;
     vector<UInt>	   tmpEdge(2,0);
     vector<point>		   nodi;
     pair<Real, point>		 result;
     geoElementSize<Line>	    lin;
     map<UInt, point>	    edgeToPoint;
     set<geoElementSize<Line> > tmpList;
     
     // ciclo sui lati 
     for(UInt i=0; i<3; ++i)
     {
	  // faccio l'edge 
	  tmpEdge[0] = meshPointer->getElement(elemId).getConnectedId(i%3);
	  tmpEdge[1] = meshPointer->getElement(elemId).getConnectedId((i+1)%3);
	    
	  // lo salvo in lin
	  lin.setConnectedId(0, tmpEdge[0]);
	  lin.setConnectedId(1, tmpEdge[1]);

	  // creo la lista dei nodi 
	  createPointList(&tmpEdge, &nodi);
	  
	  // salvo quello che ho trovato 
	  for(UInt j=0; j<nodi.size(); ++j)
	  {
	      // controllo che non sia coinvolto l'elemento da non toccare
	      if(controlDontTouch(&tmpEdge))
	      {
		  // faccio l'id 
		  lin.setId(count);
	      
		  // faccio l'associazione
		  edgeToPoint[count] = nodi[j];
		  ++count;
	      
		  // faccio il geoSize 
		  lin.setGeoSize(getEdgeMetricCost(&tmpEdge, nodi[j]));
	      
		  // lo metto nella lista 
		  tmpList.insert(lin);
	      }
	  }
     }
     
     // controllo che ci sia almeno un edge 
     if(tmpList.size()==0)
     {
	  edges->clear();
	  ptMed->clear();
	  cost->clear();
	  return;
     }
     
     // faccio un resize 
     count = 0;
     edges->clear();  edges->resize(tmpList.size());
     ptMed->clear();  ptMed->resize(tmpList.size());
     cost->clear();   cost->resize(tmpList.size());
     
     for(set<geoElementSize<Line> >::iterator it=tmpList.begin(); it!=tmpList.end(); ++it)
     {
	  // metto gli id 
	  edges->at(count).resize(2);
	  edges->at(count)[0] = it->getConnectedId(0);
	  edges->at(count)[1] = it->getConnectedId(1);
	  
	  // metto il punto 
	  ptMed->at(count) = edgeToPoint[it->getId()];
	  
	  // metto il costo 
	  cost->at(count) = it->getGeoSize();
	  
	  // incremento cont
	  ++count;
     }
}



//
// Metodi per il calcolo dello score relativo allo score delle associazioni
//
void meshDataSimplification<Triangle>::changeMemento(vector<UInt> * edge, 
						     point pNew, 
						     vector<mementoElement<Triangle> > * newMem)
{
      // controllo il tipo di punti con cui stiamo lavorando
      bool			       found=false;
      int	            typeAssociation=NULLGEO;
      UInt 	          id1,id2,numToAss,idMem,id;
      Real		 	  	       dist;
      vector<UInt>  elem,elemToCheck,tmp,ass,assTmp;
      vector<UInt>::iterator			it2;
      vector<point>		      newNodi,toAss;
      set<UInt>                           coinvolti;
      set<UInt>::iterator                       it1;
      pair<int,Real>		     result,result2;
      vector<mementoElement<Triangle> >	     oldMem;
      mesh2d<Triangle>			    tmpMesh;
      tricky2d<Triangle>		   tmpTrick;
      map<UInt,UInt>	          newToOld,oldToNew;
      
      // setto le variabili per comodità
      id1 = edge->at(0);
      id2 = edge->at(1);
      
      // trovo i triangoli adiacenti al lato
      elementOnEdge(id1, id2, &elem);
      
      // trovo TUTTI gli elementi coinvolti
      for(UInt s=0; s<edge->size(); ++s)
	    for(UInt j=0; j<conn.getNodeToElementPointer(edge->at(s))->getNumConnected(); ++j)
		  coinvolti.insert(conn.getNodeToElementPointer(edge->at(s))->getConnectedId(j));
            
      // elimino dalla lista dei coinvolti quelli in elem
      for(UInt s=0; s<elem.size(); ++s)
      {    
	    // li cerco 
	    it1 = coinvolti.find(elem[s]);
	    
	    // li DEVO TROVARE!!
	    assert(it1!=coinvolti.end());
	    
	    // li elimino
	    coinvolti.erase(it1);
      }
      
      // riempio elemToCheck
      elemToCheck.resize(coinvolti.size());
      copy(coinvolti.begin(), coinvolti.end(), elemToCheck.begin());
      
      // ------------------------------------------------------------------
      // 	prendo tutti i memento element coinvolti e li cambio  
      // ------------------------------------------------------------------
      
      // conto quanti saranno i nodi da associare 
      numToAss = 0;
      
      // prendo tutti i mementoElement coinvolti
      newMem->resize(elemToCheck.size());
      for(UInt i=0; i<elemToCheck.size(); ++i)
      {
	    // prendo l'elemento 
	    newMem->at(i) = mem[elemToCheck[i]];
	    newMem->at(i).setId(elemToCheck[i]);
	    
	    // sistemo il connesso 
	    for(UInt j=0; j<3; ++j)
	    {
		// setto l'identificatore del nodo ricordandomi che metterò il nuovo nodo alla fine della lista dei nodi 
		if((newMem->at(i).getConnectedId(j)==id1) || (newMem->at(i).getConnectedId(j)==id2))
		{
		      newMem->at(i).setConnectedId(j, meshPointer->getNumNodes());
		}
	    }
	    
	    // prendo i nodi
	    newNodi = newMem->at(i).getNodi();
	    
	    // cambio le coordinate
	    for(UInt j=0; j<3; ++j)
	    {
		// setto le corrette coordinate del nodo 
		if((newNodi[j].getId()==id1) || (newNodi[j].getId()==id2))
		{
		      newNodi[j]=pNew;
		      newNodi[j].setId(meshPointer->getNumNodes());
		}
	    }
	    
	    // metto in newMem
	    newMem->at(i).setNodi(&newNodi);
	    
	    // setto la tolleranza
	    newMem->at(i).setFatt(mem[0].getFatt());
	    
	    // incremento
	    numToAss+= newMem->at(i).ptAssSize();
      }
      
      // conto i nodi presenti nei due elementi che verranno collassati 
      for(UInt i=0; i<elem.size(); ++i)		numToAss = numToAss + mem[elem[i]].ptAssSize();
      
      // ----------------------------------------------
      // 	prendo tutti i nodi da associare 
      // ----------------------------------------------
      
      // in coinvolti metterò gli id dei nodi associati 
      coinvolti.clear();
      toAss.reserve(numToAss);
      
      // prendo tutti nodi associati all'elemento 
      for(UInt i=0; i<newMem->size(); ++i)
      {
	    // prendo i nodi
	    newNodi = newMem->at(i).getPtAss();
	    
	    // se non l'ho già associato lo metto 
	    for(UInt j=0; j<newNodi.size(); ++j)
	    {
		if(coinvolti.find(newNodi[j].getId())==coinvolti.end())
		{
		    // metto il suo id in coinvolti 
		    coinvolti.insert(newNodi[j].getId());
		    
		    // lo metto nella lista dei nodi da associare
		    toAss.push_back(newNodi[j]);
		}
	    }
	    
	    // tolgo gli associati
	    newMem->at(i).clearPtAss();
      }
      
      // ----------------------------------------------
      // 	prendo tutti i nodi da associare 
      // ----------------------------------------------
      
      // prendo anche i nodi che vengono dai due elementi collassati
      for(UInt i=0; i<elem.size(); ++i)
      {
	    // prendo i nodi
	    newNodi = mem[elem[i]].getPtAss();
	    
	    // se non l'ho già associato lo metto 
	    for(UInt j=0; j<newNodi.size(); ++j)
	    {	
		if(coinvolti.find(newNodi[j].getId())==coinvolti.end())
		{
		    // metto il suo id in coinvolti 
		    coinvolti.insert(newNodi[j].getId());
		    
		    // lo metto nella lista dei nodi da associare
		    toAss.push_back(newNodi[j]);
		}
	    }
      }
      
      // ----------------------------------------------
      //   	     creo la Mesh temporanea
      // ----------------------------------------------
      buildTmpMesh(newMem, pNew, &tmpMesh, newToOld, oldToNew);
      tmpTrick.setMeshPointer(&tmpMesh);
            
      //
      // N.B. la mesh è fatta in modo che gli id locali degli elementi in tmpMesh siano quelli della lista 
      //      newMem
      // 
            
      // ----------------------------------------------
      // 		li associo 
      // ----------------------------------------------
      for(UInt i=0; i<toAss.size(); ++i)
      {
	  // setto la distanza 
	  dist            = 9e99;
	  idMem 	  = newMem->size();
	  id   		  = newMem->size();
	  typeAssociation = NULLGEO;
	  
	  // prendo l'id del nodo 
	  id = toAss[i].getId();
	  
	  // passo in rassegna gli elementi 
	  for(UInt j=0; j<newMem->size(); ++j)
	  {	      
	      // lo associo
	      result = newMem->at(j).controlPoint(toAss[i], toTrack[id], &assTmp);
	      	      
	      // se lo associo e la distanza è più piccola 
	      if((result.first!=NULLGEO) && (result.second<dist))
	      {
		  // setto l'id
		  idMem           = j;
		  dist            = result.second;
		  typeAssociation = result.first;
		  ass.clear();	  ass.resize(assTmp.size());
		  copy(assTmp.begin(), assTmp.end(), ass.begin());
	      }
	  }
	  	  	    	  
	  // vedo che cosa fare
	  switch(typeAssociation)
	  {
	      case(FACE):
			  // caso della faccia lo associo a solo un elemento 
			  result2 = newMem->at(idMem).putPoint(toAss[i], id, toTrack[id]);
			  
			  // mi assicuro che sia associato 
			  assert(result2.first!=NULLGEO);
			  break;
	      case(EDGE):
			  // caso dell'edge lo assoco ai triangoli che condividono l'edge 
			  tmpTrick.elementOnEdge(oldToNew[ass[0]], oldToNew[ass[1]], &tmp);
			  	
			  // scorro quelli di newMem
			  for(UInt j=0; j<tmp.size(); ++j)	
			  {
			      // PERÒ sappiamo che almeno uno dei due lo associa 
			      if(tmp[j]==idMem)
			      {
				  result2 = newMem->at(tmp[j]).putPoint(toAss[i], id, toTrack[id]);
				  
				  // mi assicuro che sia associato 
				  assert(result2.first!=NULLGEO);
			      }
			  }
			  
			  break;			  
	      case(VERTEX):
			  // prendo tutti i triangoli 
			  tmpTrick.getElementAround(oldToNew[ass[0]], &tmp);
			  
			  // inizializzo found 
			  found = false;
				
			  // scorro quelli di newMem
			  for(UInt j=0; j<tmp.size(); ++j)
			  {
			      result2 = newMem->at(tmp[j]).putPoint(toAss[i], id, toTrack[id]);
			      
			      // non è assolutamente detto che tutti i nodi sono associati a questo vertice 
			      // ma ne devo trovare almeno uno!!
			      if(result2.first!=NULLGEO)	found=true;
			  }
			  
			  // mi assicuro di averlo trovato 
              (void)found;
			  assert(found);
			  
			  break;
	      default:
			  newMem->clear();
			  return;
	  }
      }

      //---------------------------------------------------------------------------------------
      // PEZZO DA AGGIUNGERE SE SI EFFETTUA ANCHE UN CONTROLLO SULLA DISTRIBUZIONE DEI DATI !!
      //---------------------------------------------------------------------------------------

      // ultimo controllo che vede se effettivamente tutti gli elementi hanno le associazioni 
      for(UInt i=0; i<newMem->size(); ++i)
	if(newMem->at(i).ptAssSize()<=1)
	  newMem->clear();
	  
      
      return;
}

void meshDataSimplification<Triangle>::buildTmpMesh(vector<mementoElement<Triangle> > * newMem, 
						    point pNew, 
						    mesh2d<Triangle> * tmpMesh,
						    map<UInt,UInt> & newToOld,
						    map<UInt,UInt> & oldToNew)
{
      // variabili in uso 
      UInt               cont=0;
      set<UInt>		    ids;
      geoElement<Triangle> elem;
      
      // prendo tutti gli id trannne quello che verrà inserito
      for(UInt i=0; i<newMem->size(); ++i)
        for(UInt j=0; j<3; ++j)
	  if(newMem->at(i).getConnectedId(j)!=meshPointer->getNumNodes())
	      ids.insert(newMem->at(i).getConnectedId(j));
      
      // creo le mappe 
      for(set<UInt>::iterator it=ids.begin(); it!=ids.end(); ++it)
      {
	  newToOld.insert(pair<UInt,UInt>(cont,*it));
	  oldToNew.insert(pair<UInt,UInt>(*it,cont));
	  ++cont;
      }
      
      // faccio anche l'ultima 
      newToOld.insert(pair<UInt,UInt>(cont,meshPointer->getNumNodes()));
      oldToNew.insert(pair<UInt,UInt>(meshPointer->getNumNodes(),cont));
      
      // metto i nodi 
      tmpMesh->getNodePointer()->reserve(cont);
      for(UInt i=0; i<cont; ++i)	tmpMesh->insertNode(meshPointer->getNode(newToOld[i]));
      tmpMesh->insertNode(pNew);
      
      // faccio gli elementi 
      tmpMesh->getElementPointer()->reserve(newMem->size());
      for(UInt i=0; i<newMem->size(); ++i)
      {
	  elem.setId(i);
	  for(UInt j=0; j<3; ++j)  elem.setConnectedId(j, oldToNew[newMem->at(i).getConnectedId(j)]);
	  tmpMesh->insertElement(elem);
      }
}

Real meshDataSimplification<Triangle>::evaluateWeight(UInt elemId, vector<point> * nodiTria, point pt)
{
      // variabili in uso 
      pair<bool, vector<UInt> > result;
      vector<UInt>              ids(3);
      
      // controllo dove è il punto 
      result = inTria.intersec(nodiTria, pt);
      
      // deve essere interno 
      assert(result.first);
      
      // distinguo 
      switch(result.second.size())
      {
	  case(1):
			// setto gli id 
			ids[0] = meshPointer->getElement(elemId).getConnectedId(0);
			ids[1] = meshPointer->getElement(elemId).getConnectedId(1);
			ids[2] = meshPointer->getElement(elemId).getConnectedId(2);
			
			// cerco il punto 
			if((pt-meshPointer->getNode(ids[0])).norm2()<toll)	return(1./getNumElementAround(ids[0]));
			else if((pt-meshPointer->getNode(ids[1])).norm2()<toll)	return(1./getNumElementAround(ids[1]));
			else							return(1./getNumElementAround(ids[2]));
			
			break;
	  case(2):
			return(1./2.);
			break;
	  case(0):
			return(1.);
			break;
	  default:
			cout << "ATTENZIONE: Non ho fatto bene le associazioni" << endl;
			assert(1==0);
      }
      
      // dovrei essere già uscito 
      return(-1.);
}

Real meshDataSimplification<Triangle>::getElemAssCost(vector<mementoElement<Triangle> > * newMem)
{
  
      // variabili in uso 
      Real         valore,meanPatch,actualAssValue;
      vector<Real>                      val,tmpVal;
      pair<int,Real>			    result;
      vector<point>		        ptAss,nodi;
      
      // valuto il valore attuale 
      actualAssValue = getMeanElemAss();
      
      // faccio un reserve 
      val.reserve(newMem->size());
      
      // controllo sulla dimensione di newMem
      if(newMem->size()==0)	return(-1.);
      
      // controllo le distanze delle associazioni 
      for(UInt i=0; i<newMem->size(); ++i)
      {
	   // prendo gli associati 
	   ptAss = newMem->at(i).getPtAss();
	   
	   // prendo i nodi 
	   nodi = newMem->at(i).getNodi();
	    
	   // ciclo su ptAss
	   tmpVal.assign(ptAss.size(),0.0);
	   
	   // calcolo i pesi 
	   for(UInt j=0; j<ptAss.size(); ++j) 	tmpVal[j] = evaluateWeight(newMem->at(i).getId(), &nodi, ptAss[j]);
	   
	   // controllo il size di tmpVal se ho creato un elemento che non ha associazioni per il momento lo peso in negativo 
	   if(tmpVal.size()!=0) 	val.push_back(*max_element(tmpVal.begin(), tmpVal.end()));
      }
      
      // calcolo la media sul pathc
      meanPatch = accumulate(tmpVal.begin(), tmpVal.end(), 0.0)/static_cast<Real>(newMem->size());
      
      // calcolo la media 
      valore = 0.;
      for(UInt i=0; i<val.size(); ++i)	   
	valore += (val[i]-meanPatch)*(val[i]-meanPatch)+(actualAssValue-meanPatch)*(actualAssValue-meanPatch);
      

//       //------------------------------------------------------------------------------------------
//       // PEZZO DA AGGIUNGERE SE NON SI EFFETTUA ANCHE UN CONTROLLO SULLA DISTRIBUZIONE DEI DATI !!
//       //------------------------------------------------------------------------------------------
      // controllo che ho fatto tutte le associazioni
//       if(val.size()!=newMem->size())	   return(max(max(maxAssCost,maxDistCost),maxGeoCost)*1000.);
      
      
      //---------------------------------------------------------------------------------------
      // PEZZO DA AGGIUNGERE SE SI EFFETTUA ANCHE UN CONTROLLO SULLA DISTRIBUZIONE DEI DATI !!
      //---------------------------------------------------------------------------------------
      if(val.size()!=newMem->size())	   return(-1.);
      
      // ritorno il massimo 
      return(valore/static_cast<Real>(val.size()));
      
}

//
// Metodi per il calcolo dello score relativo allo score della distanza 
//
Real meshDataSimplification<Triangle>::getElemDistCost(vector<mementoElement<Triangle> > * newMem)
{
  
      // variabili in uso 
      vector<point>			        ptAss;
      vector<Real>     			   val,tmpVal;
      
      // faccio un reserve 
      val.reserve(newMem->size());
      
      // controllo sulla dimensione di newMem
      if(newMem->size()==0)	return(-1.);
      
      // passo in rassegna tutti i nodi e tutti i newMementoElements e vedo se li associo
      for(UInt i=0; i<newMem->size(); ++i)
      {
	   // prendo gli associati 
	   ptAss = newMem->at(i).getPtAss();
	      
	   // ciclo su ptAss
	   tmpVal.assign(ptAss.size(),0.0);
	   
	   // calcolo le distanze 
	   for(UInt j=0; j<ptAss.size(); ++j)	      tmpVal[j] = (ptAss[j]-toTrack[ptAss[j].getId()]).norm2();
	   
	   // controllo il size di tmpVal se ho creato un elemento che non ha associazioni per il momento lo peso in negativo 
	   if(tmpVal.size()!=0) 	val.push_back(*max_element(tmpVal.begin(), tmpVal.end()));
      }

//       //------------------------------------------------------------------------------------------
//       // PEZZO DA AGGIUNGERE SE NON SI EFFETTUA ANCHE UN CONTROLLO SULLA DISTRIBUZIONE DEI DATI !!
//       //------------------------------------------------------------------------------------------
//       // controllo che ho fatto tutte le associazioni
//       if(val.size()!=newMem->size())	   return(max(max(maxAssCost,maxDistCost),maxGeoCost)*1000.);

      //---------------------------------------------------------------------------------------
      // PEZZO DA AGGIUNGERE SE SI EFFETTUA ANCHE UN CONTROLLO SULLA DISTRIBUZIONE DEI DATI !!
      //---------------------------------------------------------------------------------------
      if(val.size()!=newMem->size())	   return(-1.);
      
      // ritorno il massimo 
      return(*max_element(val.begin(), val.end()));
}

//
// Metodo che crea le liste
//
void meshDataSimplification<Triangle>::findElemDontTouch()
{
      // variabili in uso 
      point              bar,barMesh(0.0,0.0,0.0);
      geoElementSize<Triangle>		 elem;
      set<geoElementSize<Triangle> >         dist;
	  
      // calcolo il baricentro
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)	barMesh.add(meshPointer->getNode(i));
	  
      // setto 
      barMesh.setX(barMesh.getX()/static_cast<Real>(meshPointer->getNumNodes()));
      barMesh.setY(barMesh.getY()/static_cast<Real>(meshPointer->getNumNodes()));
      barMesh.setZ(barMesh.getZ()/static_cast<Real>(meshPointer->getNumNodes()));
	  
      // ciclo sugli elementi 
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	  // setto l'elemento
	  elem.setId(i);
	  elem.setConnectedId(0, meshPointer->getElement(i).getConnectedId(0));
	  elem.setConnectedId(1, meshPointer->getElement(i).getConnectedId(1));
	  elem.setConnectedId(2, meshPointer->getElement(i).getConnectedId(2));
	      
	  // trovo il baricentro del triangolo 
	  bar.setX(0.0);	bar.setY(0.0);		bar.setZ(0.0);
	  bar.add(meshPointer->getNode(meshPointer->getElement(i).getConnectedId(0)));
	  bar.add(meshPointer->getNode(meshPointer->getElement(i).getConnectedId(1)));
	  bar.add(meshPointer->getNode(meshPointer->getElement(i).getConnectedId(2)));
	      
	  // divido
	  bar.setX(bar.getX()/3.);
	  bar.setY(bar.getY()/3.);
	  bar.setZ(bar.getZ()/3.);
	      
	  // setto il size 
	  elem.setGeoSize((bar-barMesh).norm2());
	      
	  // lo metto nella lista 
	  dist.insert(elem);
      }
	  
      // salvo 
      elemDontTouch = dist.begin()->getId();
}

void meshDataSimplification<Triangle>::controlUpdatingList(vector<Real> * costi)
{
    // varaibili booleane con i test per i costi 
    bool hightGeoCost  = ((costi->at(0)/maxGeoCost)>1.1);	// forse converrebbe guardare che il rapporto fra le due sia 
    bool hightAssCost  = ((costi->at(1)/maxAssCost)>1.1);	// maggiore di 1.3 ? così la aggiorna troppo!!
    bool hightDistCost = ((costi->at(2)/maxDistCost)>1.1);
    
    // varaibili booleane con il test per il valor medio delle associazioni  
    bool assUpdate     = ((getMeanElemAss()/meanAssValue)>1.3);
    
    // controllo se ha un valore troppo alto 
    if(hightGeoCost || hightAssCost || hightDistCost || assUpdate)	listToUpdate=true;


}

void meshDataSimplification<Triangle>::createElementList()
{
      // variabili in uso 
      Real 				 costo = 0.0;
      point 					   p;
      vector<UInt>		        edge,toStore;
      vector<Real>                             costi;
      vector<Real>          costGeo,costDist,costAss;
      map<UInt,Real>           mapGeo,mapDist,mapAss;
      geoElementSize<Triangle> 			elem;
      set<geoElementSize<Triangle> >	    listaTmp;
      
      // pulisco le mappe 
      elemIdToEdge.clear();
      elemIdToPoint.clear();
      
      // faccio un reserve
      costGeo.reserve(meshPointer->getNumElements());
      costDist.reserve(meshPointer->getNumElements());
      costAss.reserve(meshPointer->getNumElements());
      toStore.reserve(meshPointer->getNumElements());
      
      // calcolo i costi
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	  // setto il suo costo 
	  costo = getElementCost(i, &costi, &edge, &p);
	  
	  // controllo che tutto sia ok
	  if(costo!=-1.) 
	  {	 
		 // sistemo le mappe 
		 elemIdToEdge[meshPointer->getElement(i).getId()]  = edge;
		 elemIdToPoint[meshPointer->getElement(i).getId()] = p;
		 mapGeo[meshPointer->getElement(i).getId()]        = costi[0];
		 mapAss[meshPointer->getElement(i).getId()]        = costi[1];
		 mapDist[meshPointer->getElement(i).getId()]       = costi[2];
		 
 		 // li metto nei vettori da cui poi prendero il massimo se non ci sono i valrori troppo alti 
		 costGeo.push_back(costi[0]);
		 costAss.push_back(costi[1]);
		 costDist.push_back(costi[2]);
		 
		 // salvo gli identificatori di quelli da calcolare 
		 toStore.push_back(i);
	  }
      }
      
      // setto i massimi 
      maxGeoCost  = *max_element(costGeo.begin(),  costGeo.end());
      maxAssCost  = *max_element(costAss.begin(),  costAss.end());
      maxDistCost = *max_element(costDist.begin(), costDist.end());
      
      // creo la lista 
      for(UInt i=0; i<toStore.size(); ++i)
      {
	  // metto gli id dei connessi
	  for(UInt j=0; j<3; ++j)  elem.setConnectedId(j, meshPointer->getElement(toStore[i]).getConnectedId(j));
	  
	  // metto gli id dell'elemento 
	  elem.setId(meshPointer->getElement(toStore[i]).getId());
	  
	  // controllo che sia stato associato 
	  if(mapGeo.find(meshPointer->getElement(toStore[i]).getId())!=mapGeo.end()) 
	  {	 		 
		 // prendo il costo 
		 elem.setGeoSize(pesoGeo*mapGeo[meshPointer->getElement(toStore[i]).getId()]/maxGeoCost+
		                 pesoAss*mapAss[meshPointer->getElement(toStore[i]).getId()]/maxAssCost+
		                 pesoDist*mapDist[meshPointer->getElement(toStore[i]).getId()]/maxDistCost);
				 
		 // lo metto nella lista
		 listaTmp.insert(elem);		 
	  }
      }
      
      // metto nella lista
      sortedList.clear();
      sortedList.setElementVector(&listaTmp);
}

void meshDataSimplification<Triangle>::deleteElementList(vector<UInt> * edge, vector<UInt> * toUpDate)
{
    // variabili in gioco 
    vector<UInt>   	     onEdge,edgeTmp,vicini;
    
    // prendo gli elementi sull'edge
    elementOnEdge(edge->at(0), edge->at(1), &onEdge);
    
    // prendo gli elementi attorno 
    // 
    // N.B. la stellata che è influenzata dal collasso dell'edge non è semplicemente quella dei triangoli che condividono i nodi
    //      dell'edge ma anche quelli a loro connessi 
    // 
    createBigStellataEdge(edge, &vicini);
    
    // elimino dalla lista gl elementi sull'edge
    for(UInt i=0; i<onEdge.size(); ++i)
      if(sortedList.isIn(onEdge[i]))
	sortedList.remove(onEdge[i]);
    
    // elimino quelli della lista che stanno sull'edge e che verranno quindi collassati 
    sort(vicini.begin(), vicini.end());
    for(UInt i=0; i<onEdge.size(); ++i)		vicini.erase(find(vicini.begin(), vicini.end(), onEdge[i]));

    // li elimino dalla lista 
    for(UInt i=0; i<vicini.size(); ++i)
    {
	  if(elemIdToEdge.find(vicini[i])!=elemIdToEdge.end())
	  {
	  	  elemIdToEdge.erase(vicini[i]);
		  elemIdToPoint.erase(vicini[i]);
	  }
    }
    
    // copio 
    toUpDate->clear();
    toUpDate->resize(vicini.size());
    copy(vicini.begin(), vicini.end(), toUpDate->begin());
}

void meshDataSimplification<Triangle>::upDate(vector<UInt> * vicini)
{
    // variabili in uso 
    Real 				 costo = 0.0;
    point 					   p;
    vector<UInt>				edge;
    vector<Real>                               costi;
    geoElementSize<Triangle> 			elem;
    set<geoElementSize<Triangle> >	    listaTmp;
      
    // cambio gli eleemtnidi della lista 
    for(UInt i=0; i<vicini->size(); ++i)
    {
	  // metto gli id dei connessi
	  for(UInt j=0; j<3; ++j)  elem.setConnectedId(j, meshPointer->getElement(vicini->at(i)).getConnectedId(j));
	  
	  // metto gli id dell'elemento 
	  elem.setId(vicini->at(i));
	  
	  // setto il suo costo 
	  costo = getElementCost(vicini->at(i), &costi, &edge, &p);
	  
	  // controllo che tutto sia ok
	  if(costo!=-1.) 
	  {
		 // controllo se deve essere aggiornata la lista 
		 controlUpdatingList(&costi);
	    
		 // prendo il costo 
		 elem.setGeoSize(pesoGeo*(costi[0]/maxGeoCost)+pesoAss*(costi[1]/maxAssCost)+pesoDist*(costi[2]/maxDistCost));
		 
		 // setto il punto 
		 elemIdToPoint[vicini->at(i)] = p;
		 elemIdToEdge[vicini->at(i)]  = edge;
		 
		 // se è dentro 
		 if(sortedList.isIn(vicini->at(i)))
		 {		   
		      // prendo il costo 
		      sortedList.change(vicini->at(i), costo);
		 }
		 else
		 {
		      // metto il costo 
		      elem.setGeoSize(costo);
		 
		      // prendo il costo 
		      sortedList.add(&elem);
		 }
	  }
	  // se ha null ed è presente lo elimino dalla lista 
	  else if(sortedList.isIn(vicini->at(i)))
	  {
		 sortedList.remove(vicini->at(i));
	  }
      }
}

void meshDataSimplification<Triangle>::upDateMemento(vector<mementoElement<Triangle> > * newMem)
{
      // rimetto in oridne gli elementi 
      for(UInt i=0; i<newMem->size(); ++i)		mem[newMem->at(i).getId()] = newMem->at(i);
}

void meshDataSimplification<Triangle>::upDateFinder(vector<UInt> * edge, point pNew)
{
      // variabili in uso 
      UInt  	                        id1,id2;
      point		                  p1,p2;
      vector<UInt> tmp,elem,toChange,notToCheck;
      vector<UInt>::iterator                 it;
      
      // setto le variabili per comodità
      id1 = edge->at(0);
      id2 = edge->at(1);
      
      // prendo le coordinate di p1 e p2 
      p1 = meshPointer->getNode(id1);
      p2 = meshPointer->getNode(id2);
      
      // trovo i triangoli adiacenti al lato
      elementOnEdge(id1, id2, &elem);
      sort(elem.begin(), elem.end());
   
      // trovo i triangoli a cui dovrà essere cambiata la coordinata di almeno un punto 
      createStellataEdge(edge, &tmp);
      toChange.resize(tmp.size());
      it = set_difference(tmp.begin(), tmp.end(), elem.begin(), elem.end(), toChange.begin());
      toChange.resize(it-toChange.begin());
      
      // rimuovo gli elementi 
      for(UInt i=0; i<elem.size(); ++i)		finder.eraseElement(elem[i]);
      for(UInt i=0; i<toChange.size(); ++i)	finder.eraseElement(toChange[i]);
      
      // cambio le coordinate dei punti 
      for(UInt i=0; i<3; ++i)
      {
	    meshPointer->getNodePointer(id1)->setI(i, pNew.getI(i));
	    meshPointer->getNodePointer(id2)->setI(i, pNew.getI(i));
      }
      
      // aggiungo solamente quelli coinvolti 
      for(UInt i=0; i<toChange.size(); ++i)	finder.insertElement(toChange[i]);
}


//
// Processi che effettuano i controlli geometrici 
//
bool meshDataSimplification<Triangle>::controlInt(vector<UInt> * edge, point pNew)
{
      // variabili in uso 
      UInt  	                  id1,id2,idTmp;
      point		        p1,p2,pMax,pMin;
      vector<UInt>            tmp,elem,toChange;
      vector<UInt>::iterator                 it;
      vector<Real>			   coor;
      vector<point>		           nodi;
      set<UInt>			     notToCheck;
      vector<vector<point> >	        newCoor;
      pair<bool, vector<UInt> >          result;
      pair<bool, vector<point> >      resultInt;
      pair<bool, vector<UInt> >	     resultTria;
       
      // setto le variabili per comodità
      id1 = edge->at(0);
      id2 = edge->at(1);
      
      // prendo le coordinate di p1 e p2 
      p1 = meshPointer->getNode(id1);
      p2 = meshPointer->getNode(id2);
      
      // trovo i triangoli adiacenti al lato
      elementOnEdge(id1, id2, &elem);
      sort(elem.begin(), elem.end());
      
      // li metto in una lista da non controllare 
      for(UInt i=0; i<elem.size(); ++i)		notToCheck.insert(elem[i]);
   
      // trovo i triangoli a cui dovrà essere cambiata la coordinata di almeno un punto 
      createStellataEdge(edge, &tmp);
      toChange.resize(tmp.size());
      it = set_difference(tmp.begin(), tmp.end(), elem.begin(), elem.end(), toChange.begin());
      toChange.resize(it-toChange.begin());
      
      // metto anche quelli che ho cambiato nella lista di quelli da non controllare
      for(UInt i=0; i<toChange.size(); ++i)		notToCheck.insert(toChange[i]);
      
      // metto i punti nella lista i nuovi punti che mi serviranno per trovare le intersezioni 
      newCoor.resize(toChange.size());
      for(UInt i=0; i<toChange.size(); ++i)
      {
	    // faccio un resize 
	    newCoor[i].resize(3,0);
	    
	    // prendo le coordinate corrette e se trovo uno dei due punti prendo quelle in input
	    for(UInt j=0; j<3; ++j)
	    {
		idTmp = meshPointer->getElement(toChange[i]).getConnectedId(j);
		
		// se sono uguali agli estremi metto il nodo in input
		if((idTmp==id1) || (idTmp==id2))	newCoor[i][j] = pNew;
		else					newCoor[i][j] = meshPointer->getNode(idTmp);
	      
	    }
      }     
      
      // controllo che ogni nuovo triangolo ottenuto non interseca con quelli già esistenti 
      for(UInt i=0; i<newCoor.size(); ++i)
      {
	   // faccio il bounding box 
	   for(UInt j=0; j<3; ++j)
	   {
		pMax.setI(j, max(max(newCoor[i][0].getI(j),newCoor[i][1].getI(j)),newCoor[i][1].getI(j)));
		pMin.setI(j, min(min(newCoor[i][0].getI(j),newCoor[i][1].getI(j)),newCoor[i][1].getI(j)));
	   }
	   
	   // lo cerco nella struttura dati 
	   result = finder.findIntersection(pMax,pMin);
	   
	   // controllo le intersezioni solamente di quelli che non sono connessi 
	   for(UInt j=0; j<result.second.size(); ++j)
	   {		
		// se non è un triangolo adiavente al lato che viene collassato
		if(notToCheck.find(result.second[j])==notToCheck.end())
		{
		      // prendo i connessi al nodo 
		      meshPointer->getNodeOfElement(result.second[j], &nodi);
		      
		      // faccio le intersezioni 
		      resultInt = intersec.intersec(&nodi, &newCoor[i]);
		      
		      // l'intersezione potrebbe essere un vertice del triangolo 
		      if(resultInt.second.size()==1)
		      {
			    // calcolo le coordinate baricentriche e controllo che la somma faccia 1
			    resultTria = inTria.intersec(&nodi, resultInt.second[0]);
			    
			    // se cono interno e se è un vertice 
			    if(resultTria.first && resultTria.second.size()!=1)	      return(false);
		      }
		      else if(resultInt.second.size()==2)
		      {
			    // calcolo le coordinate baricentriche e controllo che la somma faccia 1
			    resultTria = inTria.intersec(&nodi, resultInt.second[0]);
			    
			    // se cono interno e se è un vertice 
			    if(resultTria.first && resultTria.second.size()!=1)	      return(false);
								      
			    // calcolo le coordinate baricentriche e controllo che la somma faccia 1
			    resultTria = inTria.intersec(&nodi, resultInt.second[1]);
			    
			    // se cono interno e se è un vertice 
			    if(resultTria.first && resultTria.second.size()!=1)	      return(false);
		      }
		}
	   }	
      }
      
      // se non sono uscito prima vuol dire che i nuovi elementi non intersecano quindi li devo togliere dalla struttura dati 
      // e rimettere con le nuove coordinate 
            
      // ritorno vero 
      return(true);
}

bool meshDataSimplification<Triangle>::controlDontTouch(vector<UInt> * edge)
{
      // varaibili in uso 
      vector<UInt>		elem;
      
      // prendo i connessi all'edge 
      createStellataEdge(edge, &elem);
      
      // se non devo toccare un elemento 
      if(dontTouch)
      {
	  // metto in ordine la lista per trovare l'elemento 
	  sort(elem.begin(), elem.end());
	  
	  // se lo trovo ritorno falso 
	  if(find(elem.begin(), elem.end(), elemDontTouch)!=elem.end())	return(false);
      }
      
      // altrimenti ritorno vero 
      return(true);
}

bool meshDataSimplification<Triangle>::controlInv(vector<UInt> * edge, point pNew)
{
      // controllo il tipo di punti con cui stiamo lavorando
      graphItem			      grafTmp,grafTmpPnew;
      set<UInt>                                 coinvolti;
      set<UInt>::iterator                             it1;
      vector<UInt>            common,elem,stellata,tmpEle;
      UInt                                        id1,id2;
      point                  oldId1,oldId2,nPrima,nDopo,p;
      bool                                            inv;
      
      // controllo che pNew non sia degenere 
      if((pNew-pNull).norm2()<toll) return(false);
     
      // setto le variabili per comodità
      id1 = edge->at(0);
      id2 = edge->at(1);
      
      // trovo i triangoli adiacenti al lato
      elementOnEdge(id1, id2, &elem);
            
      // trovo TUTTI gli elementi coinvolti
      for(UInt s=0; s<edge->size(); ++s)
	    for(UInt j=0; j<conn.getNodeToElementPointer(edge->at(s))->getNumConnected(); ++j)
		  coinvolti.insert(conn.getNodeToElementPointer(edge->at(s))->getConnectedId(j));
      
      // elimino dalla lista dei coinvolti quelli in elem
      for(UInt s=0; s<elem.size(); ++s)
      {    
	    // li cerco 
	    it1 = coinvolti.find(elem[s]);
	    
	    // li DEVO TROVARE!!
	    assert(it1!=coinvolti.end());
	    
	    // li elimino
	    coinvolti.erase(it1);
      }
      
      // controllo che TUTTE le condizioni che ammettano il collasso siano verificate:
      // inv      = non devono esserci elementi invertiti
      
      // inserisco le coordinate di id1 e id2 in questi due punti
      // id1 ...
      oldId1.setX(meshPointer->getNode(id1).getX());
      oldId1.setY(meshPointer->getNode(id1).getY());
      oldId1.setZ(meshPointer->getNode(id1).getZ());
      // ... e id2
      oldId2.setX(meshPointer->getNode(id2).getX());
      oldId2.setY(meshPointer->getNode(id2).getY());
      oldId2.setZ(meshPointer->getNode(id2).getZ());
      
      // setto queste due variabili vere e poi nel ciclo le aggiorno
      inv  = true;
      
      for(it1 = coinvolti.begin(); it1!=coinvolti.end(); ++it1)
      {
	    // calcolo la normale prima del collasso 
	    nPrima = getTriangleNormal(*it1);
	    
	    // sostituisco il punto
	    // per sicurezza sostituisco sia a id1 ...
	    meshPointer->getNodePointer(id1)->setX(pNew.getX());
	    meshPointer->getNodePointer(id1)->setY(pNew.getY());
	    meshPointer->getNodePointer(id1)->setZ(pNew.getZ());
	    // ...che a id2, lo so che è sovrabbondante come cosa ma non costa nulla
	    meshPointer->getNodePointer(id2)->setX(pNew.getX());
	    meshPointer->getNodePointer(id2)->setY(pNew.getY());
	    meshPointer->getNodePointer(id2)->setZ(pNew.getZ());
	            
	    // calcolo la normale dopo il collasso 
	    nDopo = getTriangleNormal(*it1);
	    
	    // vedo i due controlli
	    inv  = ((nPrima*nDopo)>(sqrt(3.)/2.));
	    
	    // ripristino le vecchie coordinate
	    // per sicurezza sostituisco sia a id1 ...
	    meshPointer->getNodePointer(id1)->setX(oldId1.getX());
	    meshPointer->getNodePointer(id1)->setY(oldId1.getY());
	    meshPointer->getNodePointer(id1)->setZ(oldId1.getZ());
	    // ...che a id2, lo so che è sovrabbondante come cosa ma non costa nulla
	    meshPointer->getNodePointer(id2)->setX(oldId2.getX());
	    meshPointer->getNodePointer(id2)->setY(oldId2.getY());
	    meshPointer->getNodePointer(id2)->setZ(oldId2.getZ());
	    
	    // controllo preventivo sull'inversione
	    if(!inv)		  return(false);
      }
      
      // prendo la stellata degli estremi 
      createStellataEdgeNode(edge, &stellata);
      
      // creo il grafo con i connessi del punto in cui ho collassato tutto 
      for(it1=coinvolti.begin(); it1!=coinvolti.end(); ++it1)		grafTmpPnew.connectedPushBack(*it1);
      
      // controllo che dopo il collasso ogni edge abbia solamente due triangolo adiacenti 
      for(UInt i=0; i<stellata.size(); ++i)
      {
	  // lo azzero 
	  grafTmp.clear();
	  tmpEle.clear();
	  
	  // creo il grafo 
	  for(UInt j=0; j<conn.getNodeToElementPointer(stellata[i])->getNumConnected(); ++j)	
	    grafTmp.connectedPushBack(conn.getNodeToElementPointer(stellata[i])->getConnectedId(j));
	  
	  // rimuovo quelli che saranno eliminati dal collasso 
	  for(UInt s=0; s<elem.size(); ++s)		  
	    grafTmp.remove(elem[s]);
	  
	  // guardo i comuni
	  grafTmp.common(grafTmpPnew, &tmpEle);
	  
	  // se dopo non hanno in comune due triangoli mi fermo 
	  if(tmpEle.size()!=2) 	return(false);
      }
      
      // ritorno vero se passa tutti i test
      return(true);
}

void meshDataSimplification<Triangle>::controlAutoIntersection()
{
      // variabili in uso 
      UInt 		   numTot=meshPointer->getNumElements();
      barCoordinates 					    bar;
      vector<UInt>					   elem;
      vector<point>					   nodi;
      vector<Real>			  		   coor;
      set<UInt>					       toDelete;
      mesh1d<Line>					   line;
      mesh2d<Triangle>		                 tmpTria,totale;
      meshIntersec<mesh2d<Triangle>, mesh2d<Triangle> > meshInt;
      tricky2d<Triangle>				  trick;
      createFile					   file;
      
      // setto la tolleranza 
      meshInt.setToll(toll);
      bar.setToll(toll);
      
      // metto i nodi 
      totale.insertNode(meshPointer->getNodePointer());
      
      // faccio la mesh di triangoli 
      tmpTria.getNodePointer()->resize(3);
      tmpTria.getElementPointer()->resize(1);
      tmpTria.getElementPointer(0)->setConnectedId(0,0);
      tmpTria.getElementPointer(0)->setConnectedId(1,1);
      tmpTria.getElementPointer(0)->setConnectedId(2,2);
      
      // setto il puntatore 
      trick.setMeshPointer(meshPointer);
      
      // numero totale di elementi 
      for(UInt i=0; i<numTot; ++i)
      {
	    // stampa 
	    cout << "----------------------------------------------"   << endl;
	    cout << "  " << i << " elemento controllato su " << numTot << endl;
	    cout << "----------------------------------------------"   << endl;
	    
	    // faccio un resize 
	    totale.insertElement(meshPointer->getElementPointer());
	    
	    // metto i nodi 
	    tmpTria.getNodePointer()->at(0) = meshPointer->getNode(meshPointer->getElement(i).getConnectedId(0));
	    tmpTria.getNodePointer()->at(1) = meshPointer->getNode(meshPointer->getElement(i).getConnectedId(1));
	    tmpTria.getNodePointer()->at(2) = meshPointer->getNode(meshPointer->getElement(i).getConnectedId(2));
	    
	    // prendo tutti gli elementi vicini al triangolo 
	    trick.stellataTria(i, &elem);
	    
	    // riempio toDelete 
	    toDelete.clear();
	    for(UInt j=0; j<elem.size(); ++j)	toDelete.insert(elem[j]);
	    toDelete.insert(i);
	    
	    // li tolgo dalla lista 
	    totale.getElementPointer()->clear();
	    totale.getElementPointer()->reserve(meshPointer->getNumElements());
	    for(UInt j=0; j<meshPointer->getNumElements(); ++j)
	      if(toDelete.find(j)==toDelete.end())
		totale.insertElement(meshPointer->getElement(j));
	    
	    // setto le variabili
	    meshInt.setMeshPointer1(&tmpTria);
	    meshInt.setMeshPointer2(&totale);
	    
	    // faccio le intersezioni 
	    meshInt.createIntersection(&line);
	
	    // se non sono vuoti le stampo 
	    for(UInt j=0; j<line.getNumNodes(); ++j)
	    {
		  // prendo i nodi del triangolo 
		  tmpTria.getNodeOfElement(0, &nodi);
		  
		  // prendo le coordinate del punto 
		  bar.ofTriangle(&nodi, line.getNode(j), &coor);
		  
		  // controllo dove è il punto 
		  if(bar.whereIsNode(&coor)!=VERTEX)
		  {
			char command [150];
			sprintf(command,"INTERSEZIONE%d.inp",i);
			file.fileForParaview(command, &line);
			break;
		  }	      
	    }
      }
}

//
// Processo di semplificazione
//
point meshDataSimplification<Triangle>::getEdgeToSimplificate(vector<UInt> * edge)
{
      // controllo che la lista non sia vuota 
      if(sortedList.isEmpty())		return(pNull);
	
      // prendo l'id dell'elemento 
      UInt elemId = sortedList.findMin();
      vector<UInt>		     tmp;
      
      vector<Real>  costi;
      point		p;
      
      // faccio un resize 
      edge->assign(2,0);
      tmp = elemIdToEdge[elemId];
      edge->at(0) = tmp[0];
      edge->at(1) = tmp[1];
      
      // ritorno il punto
      return(elemIdToPoint[elemId]);
}

Real meshDataSimplification<Triangle>::getElementCost(UInt elemId, vector<Real> * costi, vector<UInt> * edge, point * p)
{
      // variabili in uso
      Real    costMetrica=0.,costElemAss=0.,costDist=0.;
      vector<vector<UInt> >                       edges;
      vector<UInt>				tmpEdge;
      vector<point>                               ptMed;
      vector<Real>                                 cost;
      vector<mementoElement<Triangle> >          newMem;
      
      // faccio un resize 
      costi->clear(); costi->reserve(3);
      edge->clear();
      
      // se è degenere
      if(isTriangleDegenerate(elemId))	return(-1.);
      
      // trovo l'edge 
      getElemEdgeMetricCost(elemId, &edges, &ptMed, &cost);
      
      // ciclo sulle scelte
      for(UInt i=0; i<ptMed.size(); ++i)
      {
	    // prendo il costo 
	    costMetrica = cost[i];
	    
	    // prendo quelli da controllare 
	    changeMemento(&edges[i], ptMed[i], &newMem);
	    
	    // calcolo il costo delle associazioni 
	    costElemAss = getElemAssCost(&newMem);
	        
	    // calcolo il costo delle distanze 
	    costDist = getElemDistCost(&newMem);
	    
	    // se uno dei due non va bene mi fermo 
	    if((costElemAss!=-1.) && (costDist!=-1.) && // controllo per Distanza e Associazione 
	        controlDontTouch(&edges[i]) && 		// controllo per DontTouch
	        controlInv(&edges[i], ptMed[i]) && 	// controllo per inversione dei triangoli
 	        controlInt(&edges[i], ptMed[i])) 	// controllo per le intersezioni 
	    {	     
		// salvo l'edge 
		edge->clear();
		edge->resize(2); 
		edge->at(0)=edges[i][0];
		edge->at(1)=edges[i][1];
		
		// salvo le coordinate 
		p->setX(ptMed[i].getX());
		p->setY(ptMed[i].getY());
		p->setZ(ptMed[i].getZ());
		  
		// setto i costi 
		costi->push_back(costMetrica);
		costi->push_back(costElemAss);
		costi->push_back(costDist);
		
		// solo geometria
  		return(costi->at(0));
	
		break;
	    }
      }
      
      // se non sono uscito prima ritorno un null
      costi->push_back(-1.);
      costi->push_back(-1.);
      costi->push_back(-1.);
      return(-1.);      
}

void meshDataSimplification<Triangle>::simplificationProcess(UInt numNodesMax, UInt _elemDontTouch)
{
      // variabili in uso
      time_t 			       start,end;
      Real          			     dif;
      UInt iniziali = meshPointer->getNumNodes();
      vector<point>			    nodi;
      
      
      // controllo che i punti non siano già sotto 
      if(numNodesMax>=meshPointer->getNumNodes())
      {
	  cout << "I punti della mesh sono " << meshPointer->getNumNodes();
	  cout << " e sono già sotto la soglia " << numNodesMax << endl;
	  return;
      }
      
      cout << endl;
      cout << "Inizializzo i dati" << endl;
      time(&start);
      
      // creo la lista dei nodi da associare 
      setPointToTrack(meshPointer->getNodePointer());
      
      // creo il valor medio delle associazioni
      meanAssValue  = getMeanElemAss();
      
      // setto quello da non toccare 
      if(dontTouch)
      {
	  if(_elemDontTouch==numeric_limits<UInt>::max())	findElemDontTouch();
	  else							setElemDontTouch(_elemDontTouch);
      }
      
      // creo la lista 
      createElementList();
      
      // stampa 
      time(&end);
      dif = difftime(end,start);
      cout << "Inizializzazione dei dati completata in: " <<  dif << " sec." << endl << endl;
      
      // stampe 
      cout << "Processo di collasso..." << endl;
      time(&start);
      
      // processo
      simplificate(numNodesMax);
 
      // stampa 
      time(&end);
      dif = difftime(end,start);
      cout << "Processo di collasso completato in: " <<  dif << " sec." << endl << endl;
       
      // stampe 
      cout << "Processo di setUp delle variabili di classe..." << endl;
      time(&start);
      
      refresh();
      
      time(&end);
      dif = difftime(end,start);
      cout << "Processo di setUp delle variabili di classe completato in: " <<  dif << " sec." << endl << endl;      
      
      cout << "Il processo è partito da " << iniziali << " ed è arrivato a " << meshPointer->getNumNodes() << " nodi" << endl;
      
}

void meshDataSimplification<Triangle>::simplificate(UInt numNodesMax)
{
      // variabili in uso
      UInt 	 	       			counter=0,updateCounter=0;
      UInt			       numNode=meshPointer->getNumNodes();
      UInt			  numNodeStart=meshPointer->getNumNodes();
      UInt	             limite=static_cast<UInt>(numNodeStart*times);
      point								p;
      vector<UInt>					       edge,toAdd;
      vector<mementoElement<Triangle> >                            newMem;
      
      // fino a che i nodi sono più grandi di quanto voglio proseguo con la decimazione 
      while(numNode>numNodesMax && counter<limite)
      {	  
	  // prendo l'id
	  p = getEdgeToSimplificate(&edge);
	  
	  if(p!=pNull)
	  {	   	    
	      // metto i dati in Q
	      Q.push_back(createQ(&edge));
	      	      
	      // creo la struttura dati 
	      changeMemento(&edge, p, &newMem);
	      
	      // metto a posto la lista 
	      deleteElementList(&edge, &toAdd);
	      
	      // faccio gli update delle liste
	      upDateMemento(&newMem);
	      upDateFinder(&edge, p);
	      
	      // inserisco il nodo 
	      meshPointer->insertNode(p);
		  
	      // collasso tenendo buono l'id del secondo 
	      collEdge(&edge);
		
	      // diminuisco i punto 
	      --numNode;
	      
	      cout << numNode << "  " << numNodesMax << endl;
	  
	      // aggiungo gli elementi modificati 
	      upDate(&toAdd);
	      
	      //
	      // SE NON DEVO FARE IL CONTROLLO SULL'ASSICIAZIONE DEI DATI SISTEMO NON DEVO FARE I 
	      // CONTROLLI SULLA VARIABILE "listToUpdate"
	      //
	      // se devo aggiornare la lista lo faccio 
	      if(listToUpdate)	
	      {
		  // aggiorno il contatore 
		  ++updateCounter;
		  
		  // stampo un messaggio 
		  cout << "Aggiorno per la " << updateCounter << " volta" << endl;
		  
		  // aggiorno il valore medio 
		  meanAssValue = getMeanElemAss();
		  
		  // creo la lista 
		  createElementList();
		  
		  listToUpdate = false;
	      }
	      
	      // aggiorno i numero di elementi 
	      numGlobElem = numGlobElem-2.;
	  }
	  
	  // incremento il contatore 
	  ++counter;
      }
      cout << endl;
      cout << "Il metodo ha aggiornato la lista " << updateCounter  << " volte" << endl;
      
      // marchio l'elmento che non deve essere toccato 
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	  if(i==elemDontTouch)
	  {
	      meshPointer->getElementPointer(i)->setGeoId(colorElemDontTouch);
	      break;
	  }
      }
}


//
// Metodi di stampa 
//
void meshDataSimplification<Triangle>::printElementCost(string s)
{
      // variabili in uso
      ostringstream 				   sNum;
      string                             sBase,s1,s2,s3;
      point 					      p;
      vector<UInt>			      dummyEdge;
      vector<Real>	 		      dummyCost;
      vector<Real>   val(meshPointer->getNumElements());
      vector<Real>  val1(meshPointer->getNumElements());
      vector<Real>  val2(meshPointer->getNumElements());
      vector<Real>  val3(meshPointer->getNumElements());
      createFile				   file;
    
      // riempio il vettore 
      for(UInt i=0; i<val.size(); ++i)
      {
	  // valuto il peso 
	  getElementCost(i, &dummyCost, &dummyEdge, &p);
	  
	  // valuto il costo 
	  val1[i] = dummyCost[0]/maxGeoCost;
	  val2[i] = dummyCost[1]/maxAssCost;
	  val3[i] = dummyCost[2]/maxDistCost;
	  val[i]  = pesoGeo*val1[i]+pesoAss*val2[i]+pesoDist*val3[i];
	  
      }
      
      // faccio la stringa con il numero 
      sNum << meshPointer->getNumNodes();
      
      // faccio le stringhe 
      sBase = s+sNum.str().c_str()+".inp";
      s1    = s+"EDGE"+sNum.str().c_str()+".inp";
      s2    = s+"ASS"+sNum.str().c_str()+".inp";
      s3    = s+"DIST"+sNum.str().c_str()+".inp";
      
      // faccio il file 
      file.fileForParaviewElementPropriety(sBase, meshPointer, &val);
      file.fileForParaviewElementPropriety(s1,    meshPointer, &val1); 
      file.fileForParaviewElementPropriety(s2,    meshPointer, &val2); 
      file.fileForParaviewElementPropriety(s3,    meshPointer, &val3); 
}

void meshDataSimplification<Triangle>::printEdgeMetricCost(string s)
{
      // variabili in uso
      point 					     p;
      vector<UInt>			     dummyEdge;
      vector<Real>			     dummyCost;
      vector<Real>  val(meshPointer->getNumElements());
      createFile				  file;
    
      // riempio il vettore 
      for(UInt i=0; i<val.size(); ++i)
      {
	  // valuto il costo 
	  getElementCost(i, &dummyCost, &dummyEdge, &p);
	  val[i] = dummyCost[0]/maxGeoCost;
	  
      }
      
      // faccio il file 
      file.fileForParaviewElementPropriety(s, meshPointer, &val); 
}
		  
void meshDataSimplification<Triangle>::printAssCost(string s)
{
      // variabili in uso
      point					     p;
      vector<UInt>			     dummyEdge;
      vector<Real>			     dummyCost;
      vector<Real>  val(meshPointer->getNumElements());
      createFile				  file;
    
      // riempio il vettore 
      for(UInt i=0; i<val.size(); ++i)
      {
	  // valuto il costo 
	  getElementCost(i, &dummyCost, &dummyEdge, &p);
	  val[i] = dummyCost[1]/maxAssCost;
	  
      }
      
      // faccio il file 
      file.fileForParaviewElementPropriety(s, meshPointer, &val); 
}
		  
void meshDataSimplification<Triangle>::printDistCost(string s)
{
      // variabili in uso
      point					     p;
      vector<UInt>			     dummyEdge;
      vector<Real>			     dummyCost;
      vector<Real>  val(meshPointer->getNumElements());
      createFile				  file;
    
      // riempio il vettore 
      for(UInt i=0; i<val.size(); ++i)
      {
	  // valuto il costo 
	  getElementCost(i, &dummyCost, &dummyEdge, &p);
	  val[i] = dummyCost[2]/maxDistCost;
	  
      }
      
      // faccio il file 
      file.fileForParaviewElementPropriety(s, meshPointer, &val); 
}

void meshDataSimplification<Triangle>::saveAss(string s)
{
      // varaibili in uso
      bool			found;
      UInt 	              idTmp,id;
      Real 		  distTmp,dist;
      vector<UInt>           numberAss;
      vector<point>            allNode;
      vector<vector<UInt> > associated;
      vector<point>        	assTmp;
      set<UInt>  		nodiId;
      ofstream          out(s.c_str());
      
      // riempio conto quanti sono i nodi 
      for(UInt i=0; i<mem.size(); ++i)
      {	
	    // prendo i punti da associare 
	    assTmp = mem[i].getPtAss();
	    
	    // prendo gli id 
	    for(UInt j=0; j<assTmp.size(); ++j)		nodiId.insert(assTmp[j].getId());
      }
      
      // faccio un resize 
      numberAss.assign(nodiId.size(),0);
      allNode.resize(nodiId.size());
      
      // faccio un reserve 
      associated.resize(nodiId.size());
      for(UInt i=0; i<associated.size(); ++i)	associated[i].reserve(10);
      
      // riempio 
      for(UInt i=0; i<mem.size(); ++i)
      {
	    // prendo i punti da associare 
	    assTmp = mem[i].getPtAss();
	    
	    // incremento e riempio il vettore che contiene tutti i nodi 
	    for(UInt j=0; j<assTmp.size(); ++j)
	    {
		// salvo le informazioni 
		associated[assTmp[j].getId()].push_back(i);
		numberAss[assTmp[j].getId()] = numberAss[assTmp[j].getId()]+1;
		allNode[assTmp[j].getId()]   = assTmp[j];
	    }
      }
      
      // faccio la prima linea in cui metto il numero di nodi da associare 
      out << "// node Id, typeAss, node Id or element Id, X-projection, Y-projection,  Z-projection " << endl;
      out << "// typeAss = 0 associated to a  VERTEX " << endl;
      out << "// typeAss = 1 associated to an EDGE" << endl;
      out << "// typeAss = 2 associated to a  FACE " << endl;
      out << nodiId.size() << endl;
      
      // ciclo per mettere le informazioni ricavate prima 
      for(UInt i=0; i<nodiId.size(); ++i)
      {
	  // identificatore 
	  out << i+1 << " ";
	  
	  // controllo le associazioni 
	  switch(numberAss[i])
	  {
	      // se è 0 ci sono problemo 
	      case(0):
			cout << "ATTENZIONE in saveAss il nodo non associato a niente" << endl;
			assert(1==2);
			break;
	      // siamo di fronte a un'associazione di tipo faccia
	      case(1):
			out << 2 << " " << associated[i][0]+1 << " ";
			out << allNode[i].getX() << " " << allNode[i].getY() << " " << allNode[i].getZ() << endl;
			break;
			
	      // è associato a un edge 
	      case(2):
			out << 1 << " " << associated[i][0]+1 << " ";
			out << allNode[i].getX() << " " << allNode[i].getY() << " " << allNode[i].getZ() << endl;
			break;
	      // è associato a un nodo
	      default:
			out << 0 << " ";
			
			// faccio un set delle variavili 
			dist   = 9e99;
			id     = 0;
			found = false;
			
			// controllo il nodo 
			for(UInt j=0; j<3; ++j)
			{
			    // prendo l'idTmp
			    idTmp = meshPointer->getElement(associated[i][0]).getConnectedId(j);
			    
			    // faccio la distanza 
			    distTmp = (meshPointer->getNode(idTmp)-allNode[i]).norm2();
			    
			    // se la distanza è più piccola
			    if(distTmp<dist)
			    {
				found = true;
				id     = idTmp;
				dist   = distTmp;
			    }
			}
			
			// devo essere entrato 
            (void)found;
			assert(found);
			out << id+1 << " " << allNode[i].getX() << " " << allNode[i].getY() << " " << allNode[i].getZ() << endl;  
			break;
	  }	  
      }
      
      out.close();
      
      
}

void meshDataSimplification<Triangle>::saveAss(string s, string link)
{
      // varaibili in uso
      bool			found;
      UInt 	              idTmp,id;
      Real 		  distTmp,dist;
      vector<UInt>           numberAss;
      vector<point>            allNode;
      vector<vector<UInt> > associated;
      vector<point>        	assTmp;
      set<UInt>  		nodiId;
      ofstream          out(s.c_str());
      
//       cout << "Inizio " << endl;
      
      // riempio conto quanti sono i nodi 
      for(UInt i=0; i<mem.size(); ++i)
      {	
	    // prendo i punti da associare 
	    assTmp = mem[i].getPtAss();
	    
// 	    cout << i << endl;
	    
	    // prendo gli id 
	    for(UInt j=0; j<assTmp.size(); ++j)		nodiId.insert(assTmp[j].getId());
      }
      
//       cout << "Fine prima" << endl;
      
      // faccio un altro controllo se non fosse vero questo vuol dire che ho perso dei nodi 
//       assert(nodiId.size()==toTrack.size());
      
      // faccio un resize 
      numberAss.assign(nodiId.size(),0);
      allNode.resize(nodiId.size());
      
      // faccio un reserve 
      associated.resize(nodiId.size());
      for(UInt i=0; i<associated.size(); ++i)	associated[i].reserve(10);
      
      // riempio 
      for(UInt i=0; i<mem.size(); ++i)
      {
	    // prendo i punti da associare 
	    assTmp = mem[i].getPtAss();
	    
// 	    cout << i << endl;
	    
	    // incremento e riempio il vettore che contiene tutti i nodi 
	    for(UInt j=0; j<assTmp.size(); ++j)
	    {
		// controllo che effettivamente sia un id corretto 
// 		assert(assTmp[j].getId()<toTrack.size());
		
		// faccio le associazioni 
		associated[assTmp[j].getId()].push_back(i);
		numberAss[assTmp[j].getId()] = numberAss[assTmp[j].getId()]+1;
		allNode[assTmp[j].getId()]   = assTmp[j];
	    }
      }
      
      // faccio la prima linea in cui metto il numero di nodi da associare 
      out << "// node Id, typeAss, node Id or element Id, X-projection, Y-projection,  Z-projection " << endl;
      out << "// typeAss = 0 associated to a  VERTEX " << endl;
      out << "// typeAss = 1 associated to an EDGE" << endl;
      out << "// typeAss = 2 associated to a  FACE " << endl;
      out << nodiId.size() << endl;
      
      // ciclo per mettere le informazioni ricavate prima 
      for(UInt i=0; i<nodiId.size(); ++i)
      {
	  // identificatore 
	  out << i+1 << " ";
	  
	  // controllo le associazioni 
	  switch(numberAss[i])
	  {
	      // se è 0 ci sono problemo 
	      case(0):
			cout << "ATTENZIONE in saveAss il nodo non associato a niente" << endl;
			assert(1==2);
			break;
	      // siamo di fronte a un'associazione di tipo faccia
	      case(1):
			out << 2 << " " << associated[i][0]+1 << " ";
			out << allNode[i].getX() << " " << allNode[i].getY() << " " << allNode[i].getZ() << endl;
			break;
			
	      // è associato a un edge 
	      case(2):
			out << 1 << " " << associated[i][0]+1 << " ";
			out << allNode[i].getX() << " " << allNode[i].getY() << " " << allNode[i].getZ() << endl;
			break;
	      // è associato a un nodo
	      default:
			out << 0 << " ";
			
			// faccio un set delle variavili 
			dist   = 9e99;
			id     = 0;
			found = false;
			
			// controllo il nodo 
			for(UInt j=0; j<3; ++j)
			{
			    // prendo l'idTmp
			    idTmp = meshPointer->getElement(associated[i][0]).getConnectedId(j);
			    
			    // faccio la distanza 
			    distTmp = (meshPointer->getNode(idTmp)-allNode[i]).norm2();
			    
			    // se la distanza è più piccola
			    if(distTmp<dist)
			    {
				found = true;
				id     = idTmp;
				dist   = distTmp;
			    }
			}
			
			// devo essere entrato
            (void)found;
			assert(found);
			out << id+1 << " " << allNode[i].getX() << " " << allNode[i].getY() << " " << allNode[i].getZ() << endl;  
			break;
	  }	  
      }
      out.close();
      
      // ---------------------
      // creo il file dei link
      // ---------------------
      vector<Real>		  lung;
      mesh1d<Line>	          line;
      vector<point>	      nodiLink;
      createFile	 	  file;
      
      // creo i nodi 
      nodiLink.resize(allNode.size()+toTrack.size());
      copy(toTrack.begin(), toTrack.end(), nodiLink.begin());
      copy(allNode.begin(), allNode.end(), nodiLink.begin()+toTrack.size());
      line.insertNode(&nodiLink);
      
      // faccio un resize degli elementi 
      lung.resize(allNode.size());
      line.getElementPointer()->resize(allNode.size());
      for(UInt i=0; i<allNode.size(); ++i)
      {
	  // setto le connessioni dell'elemento 
	  line.getElementPointer(i)->setConnectedId(0,i);
	  line.getElementPointer(i)->setConnectedId(1,i+toTrack.size());
	  
	  // setto il vettore della lunghezza 
	  lung[i] = (line.getNode(i)-line.getNode(i+toTrack.size())).norm2();
	  
      }
      
      // creo il file 
      file.fileForParaviewElementPropriety(link, &line, &lung);
}

void meshDataSimplification<Triangle>::printAngle(string s)
{
      // varaibili in uso
      createFile				      file;
      vector<Real>	ang(meshPointer->getNumElements());
      
      // ciclo per il calcolo 
      for(UInt i=0; i<ang.size(); ++i)	ang[i] = getMaxAngle(i);
      
      // faccio il file 
      file.fileForParaviewElementPropriety(s, meshPointer, &ang);
}

