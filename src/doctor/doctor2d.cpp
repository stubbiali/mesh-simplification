#include "doctor2d.h"

using namespace std;
using namespace geometry;

//
// Costruttore 
//
doctor2d<Triangle>::doctor2d() : tricky2d<Triangle>()
{
}

doctor2d<Triangle>::doctor2d(mesh2d<Triangle> * _meshPointer) : tricky2d<Triangle>(_meshPointer)
{
}

//
// refresh
//
void doctor2d<Triangle>::refresh()
{
      
      vector<point>                    tmpPt;
      vector<geoElement<Triangle> >    tmpTr;
      point                                p;
      geoElement<Triangle>              tria;
      vector<bool>    		      active;
      vector<UInt>       	       newId;
      UInt              tmp,cont,id1,id2,id3;
      Real                             x,y,z;
      vector<geoElement<Line> >  lineListTmp;
      
      // faccio un reserve
      active.reserve(meshPointer->getNumNodes());
      newId.reserve(meshPointer->getNumNodes());
      tmpPt.reserve(meshPointer->getNumNodes());
      tmpTr.reserve(meshPointer->getNumElements());
      
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
		  
		  // creo la mia nuova variabile triangolo
		  tria.setConnectedId(0,id1);
		  tria.setConnectedId(1,id2);
		  tria.setConnectedId(2,id3);
		  tria.setGeoId(tmp);
		  tria.setId(cont);
		  
		  // metto nella lista dei triangoli
		  tmpTr.push_back(tria);
	    }
      }
      
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
}

bool doctor2d<Triangle>::eraseBoundEdge(vector<UInt> * edge)
{
    // variabili in uso 
    bool 			     result;
    geoElement<Line>			lin;
    set<geoElement<Line> >::iterator	 it;
    
    // creo la linea
    lin.setConnectedId(0, edge->at(0));
    lin.setConnectedId(1, edge->at(1));
    
    // lo cerco
    it = bordo.find(lin);
    
    // prendo il risultato 
    result = (it!=bordo.end());
    
    // elimino 
    if(result)	bordo.erase(it);
    
    return(result);
}

void doctor2d<Triangle>::addBoundEdge(vector<UInt> * edge, UInt geoId)
{
    // variabili in uso 
    geoElement<Line>		lin;
    
    // creo la linea
    lin.setConnectedId(0, edge->at(0));
    lin.setConnectedId(1, edge->at(1));
    lin.setGeoId(geoId);
    
    // aggiungo
    bordo.insert(lin);
}

void doctor2d<Triangle>::changeNode(UInt id, point newPos)
{
      // cambio le coordinate
      meshPointer->getNodePointer(id)->setX(newPos.getX());
      meshPointer->getNodePointer(id)->setY(newPos.getY());
      meshPointer->getNodePointer(id)->setZ(newPos.getZ()); 
}

//
// Smoothing
//
bool doctor2d<Triangle>::controlPositionForMetricAda(UInt i, point newPos, Real geoLim)
{
     // controllo della variabile in input
     assert(i<meshPointer->getNumNodes());
     
     // se è degenere
     if((newPos-pNull).norm2()<toll) return(false);
     
     // variabili in uso
     point      coorOld,normalOld,normalNew;
     //UInt 	 	 id1,id2,id3; // not actually used
     UInt                idElem;
     Real			     angolo;
     vector<UInt>        ids,onEdge,edge(2);
     
     // calolo l'angolo 
     angolo = (geoLim/180.)*PGRECO;
     
     // metto le coordinate vecchie in una variabile point
     coorOld.setX(meshPointer->getNode(i).getX());
     coorOld.setY(meshPointer->getNode(i).getY());
     coorOld.setZ(meshPointer->getNode(i).getZ());
     
     // inserisco le nuove coordinate
     meshPointer->getNodePointer(i)->setX(newPos.getX());	
     meshPointer->getNodePointer(i)->setY(newPos.getY());
     meshPointer->getNodePointer(i)->setZ(newPos.getZ());
     
     // prendo la stellata del punto 
     createStellata(i, &ids);
     edge[0] = i;
     
     // ciclo sugli elementi connessi con i e cotrollo delle loro normali
     for(UInt j=0; j<conn.getNodeToElementPointer(i)->getNumConnected(); ++j)
     {
	  // id del triangolo
	  normalOld = getTriangleNormal(conn.getNodeToElementPointer(i)->getConnectedId(j));
		  
	  // inserisco le nuove coordinate
	  meshPointer->getNodePointer(i)->setX(newPos.getX());
	  meshPointer->getNodePointer(i)->setY(newPos.getY());
	  meshPointer->getNodePointer(i)->setZ(newPos.getZ());
		  
	  // calcolo la nuova normale e la nuova area, N.B.  uso la metrica standard
	  normalNew = getTriangleNormal(conn.getNodeToElementPointer(i)->getConnectedId(j));
		  		  
	  // prendo l'id dell'elemento e quelli dei nodi 
	  idElem = conn.getNodeToElementPointer(i)->getConnectedId(j);
	  //id1 = meshPointer->getElement(idElem).getConnectedId(0);
	  //id2 = meshPointer->getElement(idElem).getConnectedId(1);
	  //id3 = meshPointer->getElement(idElem).getConnectedId(2);
	  
	  if(((normalOld*normalNew)<(sqrt(3.)/2)))
	  {
		meshPointer->getNodePointer(i)->setX(coorOld.getX());
		meshPointer->getNodePointer(i)->setY(coorOld.getY());
		meshPointer->getNodePointer(i)->setZ(coorOld.getZ());
			    
		return(false);
	  }
		  
     }
     
     // per ogni lato controllo che il salto sia piccolo
     for(UInt j=0; j<ids.size(); ++j)
     {
	  // prendo l'altro id 
	  edge[1] = ids[j];
	  
	  // prendo gli elementi sull'edge
	  elementOnEdge(edge[0], edge[1], &onEdge);
	  
	  // se è 2 calcolo il salto 
	  if((onEdge.size()==2) && !normalVarr(onEdge[0],onEdge[1], angolo))
	  {
	      // metto le vecchie coordinate 
	      meshPointer->getNodePointer(i)->setX(coorOld.getX());
	      meshPointer->getNodePointer(i)->setY(coorOld.getY());
	      meshPointer->getNodePointer(i)->setZ(coorOld.getZ());
	      
	      return(false);
	    
	  }
	  
     }
     
     // se ho passato tutto ritorno vero 
     return(true);
}

bool doctor2d<Triangle>::controlPosition(UInt i, point newPos, Real angMin)
{
     // controllo della variabile in input
     assert(i<meshPointer->getNumNodes());
     
     // se è degenere
     if((newPos-pNull).norm2()<toll) return(false);
     
     
     // variabili in uso
     //UInt                         id1,id2,id3; // not actually used
     UInt                         idElem;
     point               normalOld,normalNew,coorOld;
     vector<UInt>		             tmpEdge;
     Real  			            ang,area;
     
     // metto le coordinate vecchie in una variabile point
     coorOld.setX(meshPointer->getNode(i).getX());
     coorOld.setY(meshPointer->getNode(i).getY());
     coorOld.setZ(meshPointer->getNode(i).getZ());
     
     // ciclo sugli elementi connessi con i e cotrollo delle loro normali
     for(UInt j=0; j<conn.getNodeToElementPointer(i)->getNumConnected(); ++j)
     {
		  // id del triangolo
		  normalOld = getTriangleNormal(conn.getNodeToElementPointer(i)->getConnectedId(j));
		  
		  // inserisco le nuove coordinate
		  meshPointer->getNodePointer(i)->setX(newPos.getX());
		  meshPointer->getNodePointer(i)->setY(newPos.getY());
		  meshPointer->getNodePointer(i)->setZ(newPos.getZ());
		  
		  // calcolo la nuova normale e la nuova area, N.B.  uso la metrica standard
		  normalNew = getTriangleNormal(conn.getNodeToElementPointer(i)->getConnectedId(j));
		  		  
		  // prendo l'id dell'elemento e quelli dei nodi 
		  idElem = conn.getNodeToElementPointer(i)->getConnectedId(j);
		  //id1 = meshPointer->getElement(idElem).getConnectedId(0);
		  //id2 = meshPointer->getElement(idElem).getConnectedId(1);
		  //id3 = meshPointer->getElement(idElem).getConnectedId(2);
		  
		  // trovo l'angolo minimo 
		  ang = getMinAngle(idElem);
		  
		  // calcolo la nuova area 
		  area      = getTriangleArea(conn.getNodeToElementPointer(i)->getConnectedId(j));
		  
		  // se la normale cambia molto la direzione (più di 60°) e l'area diminuisce molto l'inversione non è concessa
		  if(((normalOld*normalNew)<(sqrt(3.)/2)) || (ang<angMin) || (area<toll))
		  {
			    meshPointer->getNodePointer(i)->setX(coorOld.getX());
			    meshPointer->getNodePointer(i)->setY(coorOld.getY());
			    meshPointer->getNodePointer(i)->setZ(coorOld.getZ());
			    
			    return(false);
		  }
		  
		  meshPointer->getNodePointer(i)->setX(coorOld.getX());
		  meshPointer->getNodePointer(i)->setY(coorOld.getY());
		  meshPointer->getNodePointer(i)->setZ(coorOld.getZ());
		  
     }
     
     return(true);
}

//
// Swapping
//
bool doctor2d<Triangle>::controlSwap(vector<UInt> * edge, Real limite)
{                 
      // variabili temporanee
      bool  geoId,normali,angleCont;
      bool bound1,bound2,stillExist;
      UInt 	        last1,last2;
      Real   	   ang1,ang2,angLim;
      vector<UInt>  onEdge,altro(2);
      
      // prendo gli elementi sull'edge
      elementOnEdge(edge->at(0), edge->at(1), &onEdge);
      
      // prendo l'angolo in radianti
      angLim = (limite/360.0)*2.0*PGRECO;
      
      // se ci sono più di due elementi sull'edge o solo uno
      if((onEdge.size()<=1) || (onEdge.size()>2))	return(false);
      
      // controllo che lo swapping di questo edge non mi porti a un triangolo con tutti i vertici sul bordo 
      // (perdo il grado di libertà)
      last1 = lastNode(edge->at(0), edge->at(1), onEdge[0]);
      last2 = lastNode(edge->at(0), edge->at(1), onEdge[1]);
      
      // controllo il primo triangolo
      bound1 = (meshPointer->getNode(edge->at(0)).getBoundary()>=1);
      bound1 = bound1 && (meshPointer->getNode(last1).getBoundary()>=1);
      bound1 = bound1 && (meshPointer->getNode(last2).getBoundary()>=1);
      
      // controllo il secondo 
      bound2 = (meshPointer->getNode(edge->at(1)).getBoundary()>=1);
      bound2 = bound2 && (meshPointer->getNode(last1).getBoundary()>=1);
      bound2 = bound2 && (meshPointer->getNode(last2).getBoundary()>=1);
      
      // se almeno uno dei due è vera ritorno falso
      if(bound1 || bound2)	return(false);
      
      // geoId
      geoId   = sameGeoId(onEdge[0], onEdge[1]);
		  
      // normali
      normali = normalVarr(onEdge[0], onEdge[1], angLim);
		  
      // calcolo gli angoli
      ang1 = angolo(edge->at(0), onEdge[0]) + angolo(edge->at(0), onEdge[1]);
      ang2 = angolo(edge->at(1), onEdge[0]) + angolo(edge->at(1), onEdge[1]);
      		  
      // controllo
      angleCont = (max(ang1,ang2)<(PGRECO*(165./180.))+toll);
      
      // controllo che non esista di gia 
      altro[0] = last1;	altro[1] = last2;
      stillExist = (!isAnEdge(&altro));
            
      return(geoId && normali && angleCont && stillExist);
}

bool doctor2d<Triangle>::controlSwapForMetricAda(vector<UInt> * edge, Real limite)
{
      // variabili temporanee
      bool 	   geoId,angleCont;
      bool           bound1,bound2;
      UInt	       last1,last2;
      Real   	  ang1,ang2,angLim;
      vector<UInt> onEdge,altro(2);
      vector<UInt>	      star;
      
      // prendo gli elementi sull'edge
      elementOnEdge(edge->at(0), edge->at(1), &onEdge);
      
      // prendo l'angolo in radianti
      angLim = (limite/360.0)*2.0*PGRECO;
      
      // se ci sono più di due elementi sull'edge o solo uno
      if((onEdge.size()<=1) || (onEdge.size()>2))	return(false);
      
      // controllo che lo swapping di questo edge non mi porti a un triangolo con tutti i vertici sul bordo 
      // (perdo il grado di libertà)
      last1 = lastNode(edge->at(0), edge->at(1), onEdge[0]);
      last2 = lastNode(edge->at(0), edge->at(1), onEdge[1]);
      
      // creo l'altroedge
      altro[0] = last1; 	altro[1] = last2; 
      
      // controllo il primo triangolo
      bound1 = (meshPointer->getNode(edge->at(0)).getBoundary()>=1);
      bound1 = bound1 && (meshPointer->getNode(last1).getBoundary()>=1);
      bound1 = bound1 && (meshPointer->getNode(last2).getBoundary()>=1);
      
      // controllo il secondo 
      bound2 = (meshPointer->getNode(edge->at(1)).getBoundary()>=1);
      bound2 = bound2 && (meshPointer->getNode(last1).getBoundary()>=1);
      bound2 = bound2 && (meshPointer->getNode(last2).getBoundary()>=1);
      
      // se almeno uno dei due è vera ritorno falso
      if(bound1 || bound2)	return(false);
      
      // geoId
      geoId   = sameGeoId(onEdge[0], onEdge[1]);
      
      // se non passa il geoId ritorno falso
      if(!geoId)		return(false);
		    
      // calcolo gli angoli
      ang1 = angolo(edge->at(0), onEdge[0]) + angolo(edge->at(0), onEdge[1]);
      ang2 = angolo(edge->at(1), onEdge[0]) + angolo(edge->at(1), onEdge[1]);
      		  
      // controllo
      angleCont = (max(ang1,ang2)<(PGRECO*(165./180.))+toll);
      
      // se non passa l'angolo ritorno falso 
      if(!angleCont)		return(false);
      
      // per ogni elemento sull'edge controllo la variazione delle normali 
      // 
      // N.B. posso evitare questo controllo perché so necessariamente che la situazione migliorerà??? chiedi a Hang 
      // 
      for(UInt j=0; j<2; ++j)
      {
	  // creo la stellata 
	  stellataTria(onEdge[j], &star);
      
	  // controllo 
	  for(UInt i=0; i<star.size(); ++i) 
	    if(!normalVarr(onEdge[j], star[i], angLim))
	      return(false);
      }
      
      // lo swappo  rifaccio il controllo 
      swap(edge);
      
      // per ogni elemento sull'edge controllo la variazione delle normali 
      for(UInt j=0; j<2; ++j)
      {
	  // creo la stellata 
	  stellataTria(onEdge[j], &star);
      
	  // controllo 
	  for(UInt i=0; i<star.size(); ++i) 
	  {
	    if((onEdge[(j+1)%2]!=star[i]) && (!normalVarr(onEdge[j], star[i], angLim)))
	    {
		swap(&altro);
		return(false);
	    }
	  }
      }
      
      
      // lo swappo 
      swap(&altro);
      
      // se non sono uscito prima tutto è ok!
      return(true);
}

void doctor2d<Triangle>::swap(vector<UInt> * edge, bool setUpNormal)
{	      
      // Variabili temporanee
      UInt     elemId1,elemId2,id1,id2,id3,id4;
      vector<UInt>             unCommon,onEdge;
      point                             normal;
      
      // prendo gli elementi sull'edge
      elementOnEdge(edge->at(0), edge->at(1), &onEdge);
      
      // se ci sono più di due elementi sull'edge o solo uno
      if((onEdge.size()==0) || (onEdge.size()>2))	return;
      
      // Metto le informazioni degli elementi
      elemId1 = onEdge[0];
      elemId2 = onEdge[1];
		      
      // Metto a posto le connettività
      id1 = edge->at(0);
      id2 = edge->at(1);
      
      // prendo gli esterni	      
      meshPointer->getElement(elemId1).unCommon(meshPointer->getElement(elemId2), &unCommon);
      id3 = unCommon[0];
      meshPointer->getElement(elemId2).unCommon(meshPointer->getElement(elemId1), &unCommon);
      id4 = unCommon[0];
      
		      
      // in questo modo possiamo dire che
      // elemId1 avrà i seguenti connessi id1-id2-id3
      // elemId2 avrà i seguenti connessi id1-id2-id4
		      
      // calcolo la normale   // OLDSTYLE CON ORIENTAZIONE
      if(setUpNormal)
      {
	  // prendo la normale
	  normal.replace(getTriangleNormal(elemId1),getTriangleNormal(elemId2),0.5);   
			  
	  // swappo
	  meshPointer->getElementPointer(elemId1)->setConnectedId(0,id1);
	  meshPointer->getElementPointer(elemId1)->setConnectedId(1,id4);
	  meshPointer->getElementPointer(elemId1)->setConnectedId(2,id3);
	  orient(elemId1,normal);
			  
	  meshPointer->getElementPointer(elemId2)->setConnectedId(0,id2);
	  meshPointer->getElementPointer(elemId2)->setConnectedId(1,id3);
	  meshPointer->getElementPointer(elemId2)->setConnectedId(2,id4);
	  orient(elemId2,normal);
      }
      else
      {
	  // controllo il successivo a id1 
	  if(nextNode(id1,elemId1)==id2)
	  {
	      // swappo
	      meshPointer->getElementPointer(elemId1)->setConnectedId(0,id1);
	      meshPointer->getElementPointer(elemId1)->setConnectedId(1,id4);
	      meshPointer->getElementPointer(elemId1)->setConnectedId(2,id3);
			      
	      meshPointer->getElementPointer(elemId2)->setConnectedId(0,id2);
	      meshPointer->getElementPointer(elemId2)->setConnectedId(1,id3);
	      meshPointer->getElementPointer(elemId2)->setConnectedId(2,id4);
	  }
	  else if(nextNode(id1,elemId1)==id3)
	  {
	      // swappo
	      meshPointer->getElementPointer(elemId1)->setConnectedId(0,id1);
	      meshPointer->getElementPointer(elemId1)->setConnectedId(1,id3);
	      meshPointer->getElementPointer(elemId1)->setConnectedId(2,id4);
			      
	      meshPointer->getElementPointer(elemId2)->setConnectedId(0,id2);
	      meshPointer->getElementPointer(elemId2)->setConnectedId(1,id4);
	      meshPointer->getElementPointer(elemId2)->setConnectedId(2,id3);
	  }
	  else
	  {
	      cout << "ATTENZIONE: non ha orientato bene lo swapping" << endl;
	      assert(1==0);
	  }
      }
		      
      // metto a posto le connessioni
      conn.getNodeToElementPointer(id1)->remove(elemId2);
      conn.getNodeToElementPointer(id2)->remove(elemId1);
		      
      conn.getNodeToElementPointer(id3)->add(elemId2);
      conn.getNodeToElementPointer(id4)->add(elemId1);
}

// 
// Collapsing
// 
bool doctor2d<Triangle>::controlColl(vector<UInt> * edge, point pNew)
{
      // controllo il tipo di punti con cui stiamo lavorando
      Real 					     area;
      set<UInt>                                 coinvolti;
      set<UInt>::iterator                             it1;
      vector<UInt>                            common,elem;
      UInt                      		  id1,id2;
      point                  oldId1,oldId2,nPrima,nDopo,p;
      bool         			     inv,nullArea;
      
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
      // bound*   = controlla che collassando non ci siano tre punti che stanno sul bordo 
      // ang      = controllo sull'angolo minimo che si formerà a seguito del collasso 
      
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
	    // 	    inv  = ((nPrima*nDopo)>(sqrt(3.)/2.));
	    inv  = ((nPrima*nDopo)>0.9);
	    
	    // calcolo l'area
	    area = getTriangleArea(*it1);
	    
	    // controllo l'area 
	    nullArea = (area<toll);   
	    
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
	    if((!inv) || nullArea)		return(false);
      }
      
      // ritorno vero se passa tutti i test
      return(true);
}

bool doctor2d<Triangle>::controlCollForMetricAda(vector<UInt> * edge, point pNew, Real angLim)
{
      // controllo il tipo di punti con cui stiamo lavorando
      Real 			angolo/*,area1=0.0,area2=0.0*/;
      point 		          p1,p2,p3,p4,nPrima,nDopo;
      set<Real>			 	        areeOnEdge;
      vector<UInt>                       coinvolti,tmpVect;
      vector<vector<UInt> >	   	    stellateOnEdge;
      vector<set<UInt> >			  stellate;
      vector<UInt>             elem,connId1,connId2,eleExt;
      set<UInt>::iterator				it;
      UInt                       id1,id2,idTmp1=0,idTmp2=0;
      point                                  oldId1,oldId2;
      bool     boundExt,bound1=false,bound2=false,inv=true;
      
      // controllo che pNew non sia degenere 
      if((pNew-pNull).norm2()<toll) return(false);
      
      // calcolo l'angolo 
      angolo = (angLim/180.)*PGRECO;
     
      // setto le variabili per comodità
      id1 = edge->at(0);
      id2 = edge->at(1);
      
      // trovo i triangoli adiacenti al lato
      elementOnEdge(id1, id2, &elem);
      
      // controllo se unod ei due è di bordo
      boundExt = ((meshPointer->getNode(id1).getBoundary()!=0) || (meshPointer->getNode(id2).getBoundary()!=0));
      
      // trovo TUTTI gli elementi coinvolti
      createStellataEdgeWithoutOnEdge(edge, &coinvolti);
      
      // controllo che TUTTE le condizioni che ammettano il collasso siano verificate:
      // inv      = non devono esserci elementi invertiti
      // bound*   = controlla che collassando non ci siano tre punti che stanno sul bordo 
      
      // inserisco le coordinate di id1 e id2 in questi due punti
      // id1 ...
      oldId1.setX(meshPointer->getNode(id1).getX());
      oldId1.setY(meshPointer->getNode(id1).getY());
      oldId1.setZ(meshPointer->getNode(id1).getZ());
      // ... e id2
      oldId2.setX(meshPointer->getNode(id2).getX());
      oldId2.setY(meshPointer->getNode(id2).getY());
      oldId2.setZ(meshPointer->getNode(id2).getZ());
            
      // controllo tutti i triangoli in modo tale che non si invertano, non abbiano area nulla e che non formino un 
      // triangolo con tutti i nodi sul bordo 
      for(UInt i=0; i<coinvolti.size(); ++i)
      {    
	    // calcolo la normale prima del collasso 
	    nPrima = getTriangleNormal(coinvolti[i]);
	    
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
	    nDopo = getTriangleNormal(coinvolti[i]);
	    
	    // vedo i due controlli
// 	    inv  = ((nPrima*nDopo)>(sqrt(3.)/2.));
	    inv  = ((nPrima*nDopo)>0.9);
	    
	    // ripristino le vecchie coordinate
	    // per sicurezza sostituisco sia a id1 ...
	    meshPointer->getNodePointer(id1)->setX(oldId1.getX());
	    meshPointer->getNodePointer(id1)->setY(oldId1.getY());
	    meshPointer->getNodePointer(id1)->setZ(oldId1.getZ());
	    // ...che a id2, lo so che è sovrabbondante come cosa ma non costa nulla
	    meshPointer->getNodePointer(id2)->setX(oldId2.getX());
	    meshPointer->getNodePointer(id2)->setY(oldId2.getY());
	    meshPointer->getNodePointer(id2)->setZ(oldId2.getZ());
	    
	    // controllo il fatto che il nuovo triangolo sia tutto di bordo 
	    for(UInt j=0; j<3; ++j)
	    {
		  if((meshPointer->getElement(coinvolti[i]).getConnectedId(j)==id1) || 
		     (meshPointer->getElement(coinvolti[i]).getConnectedId(j)==id2))
		  {
			idTmp1 = meshPointer->getElement(coinvolti[i]).getConnectedId((j+1)%3);
			idTmp2 = meshPointer->getElement(coinvolti[i]).getConnectedId((j+2)%3);
			
			bound1 = meshPointer->getNode(idTmp1).getBoundary()!=0;
			bound2 = meshPointer->getNode(idTmp2).getBoundary()!=0;
		  }
	    }
	         	    
	    // controllo preventivo sull'inversione
	    if((!inv) || (boundExt && bound1 && bound2))
	    {
		// ripristino le vecchie coordinate
		// per sicurezza sostituisco sia a id1 ...
		meshPointer->getNodePointer(id1)->setX(oldId1.getX());
		meshPointer->getNodePointer(id1)->setY(oldId1.getY());
		meshPointer->getNodePointer(id1)->setZ(oldId1.getZ());
		// ...che a id2, lo so che è sovrabbondante come cosa ma non costa nulla
		meshPointer->getNodePointer(id2)->setX(oldId2.getX());
		meshPointer->getNodePointer(id2)->setY(oldId2.getY());
		meshPointer->getNodePointer(id2)->setZ(oldId2.getZ());
		
		return(false);
	    }
      }
      
      // sostituisco il punto
      // per sicurezza sostituisco sia a id1 ...
      meshPointer->getNodePointer(id1)->setX(pNew.getX());
      meshPointer->getNodePointer(id1)->setY(pNew.getY());
      meshPointer->getNodePointer(id1)->setZ(pNew.getZ());
      // ...che a id2, lo so che è sovrabbondante come cosa ma non costa nulla
      meshPointer->getNodePointer(id2)->setX(pNew.getX());
      meshPointer->getNodePointer(id2)->setY(pNew.getY());
      meshPointer->getNodePointer(id2)->setZ(pNew.getZ());
      
      // creo la stellata degli elementi sull'edge
      stellateOnEdge.resize(elem.size());
      for(UInt i=0; i<stellateOnEdge.size(); ++i)	stellataTria(elem[i], &stellateOnEdge[i]);
      
      // creo le stellate degli elementi coinvolti nel collasso 
      stellate.resize(coinvolti.size());
      for(UInt i=0; i<stellate.size(); ++i)
      {
	  // creo la stellata 
	  stellataTria(coinvolti[i], &tmpVect);
	  
	  // la metto nel grafo
	  for(UInt j=0; j<tmpVect.size(); ++j)	stellate[i].insert(tmpVect[j]);
      }
      
      // passo in rassegna le stellate 
      for(UInt i=0; i<stellate.size(); ++i)
      {
	  // controllo che ci sia l'elemento iesimo dell'edge
	  for(UInt j=0; j<elem.size(); ++j)
	  {
	      // se è dentro metto tutti 
	      if(stellate[i].find(elem[j])!=stellate[i].end())
	      {
		  // li metto tutti 
		  for(UInt s=0; s<stellateOnEdge[j].size(); ++s)	stellate[i].insert(stellateOnEdge[j][s]);
		  break;
	      }
	  }
      }
      
      // tolgo tutti gli elementi che verranno collassati 
      for(UInt i=0; i<stellate.size(); ++i)
      {
	  for(UInt j=0; j<elem.size(); ++j)
	  {
	      // lo cerco 
	      it =  stellate[i].find(elem[j]);
	      
	      // se lo ho trovato lo elimino 
	      if(it!=stellate[i].end())	stellate[i].erase(it);
	  }
      }
      
      //  per ognuno controllo il salto della normale
      for(UInt i=0; i<stellate.size(); ++i)
      {
	  // per ogni elemento 
	  for(it=stellate[i].begin(); it!=stellate[i].end(); ++it)
	  {
		if(!normalVarr(coinvolti[i], *it, angolo))
		{
		    // ripristino le vecchie coordinate
		    // per sicurezza sostituisco sia a id1 ...
		    meshPointer->getNodePointer(id1)->setX(oldId1.getX());
		    meshPointer->getNodePointer(id1)->setY(oldId1.getY());
		    meshPointer->getNodePointer(id1)->setZ(oldId1.getZ());
		    // ...che a id2, lo so che è sovrabbondante come cosa ma non costa nulla
		    meshPointer->getNodePointer(id2)->setX(oldId2.getX());
		    meshPointer->getNodePointer(id2)->setY(oldId2.getY());
		    meshPointer->getNodePointer(id2)->setZ(oldId2.getZ());
		
		    return(false);
		}
	  }
      }
      
      // ripristino le vecchie coordinate
      // per sicurezza sostituisco sia a id1 ...
      meshPointer->getNodePointer(id1)->setX(oldId1.getX());
      meshPointer->getNodePointer(id1)->setY(oldId1.getY());
      meshPointer->getNodePointer(id1)->setZ(oldId1.getZ());
      // ...che a id2, lo so che è sovrabbondante come cosa ma non costa nulla
      meshPointer->getNodePointer(id2)->setX(oldId2.getX());
      meshPointer->getNodePointer(id2)->setY(oldId2.getY());
      meshPointer->getNodePointer(id2)->setZ(oldId2.getZ());
      
      
//       // controllo che tutti e due i nodi siano di bordo di tipo 1 
//       if((meshPointer->getNode(id1).getBoundary()==1) && (meshPointer->getNode(id2).getBoundary()==1))
//       {
// 	  // prendo i connessi di id1
// 	  createStellataBor(id1, &connId1);
// 	  createStellataBor(id2, &connId2);
// 	  
// 	  // devono essere 2 i connessi 	  
// 	  assert(connId1.size()==2);
// 	  assert(connId2.size()==2);
// 	  
// 	  // faccio un resize e riempio 
// 	  eleExt.resize(2);
// 	  if(connId1[0]==id2)	eleExt[0] = connId1[1];
// 	  else			eleExt[0] = connId1[0];
// 	  
// 	  if(connId2[0]==id1)	eleExt[1] = connId2[1];
// 	  else			eleExt[1] = connId2[0];
// 	  
// 	  // salvo le coordinate
// 	  p1 = meshPointer->getNode(id1);
// 	  p2 = meshPointer->getNode(id2);
// 	  p3 = meshPointer->getNode(eleExt[0]);
// 	  p4 = meshPointer->getNode(eleExt[1]);
// 	  
// 	  // calcolo le aree
// 	  area1 = getTriangleArea(p1,p2,p3);
// 	  area2 = getTriangleArea(p1,p2,p4);
// 	  
// 	  // trovo i triangoli adiacenti al lato
// 	  elementOnEdge(id1, id2, &elem);
// 	  
// 	  // metto tutte le aree 
// 	  for(UInt i=0; i<elem.size(); ++i)	areeOnEdge.insert(getTriangleArea(elem[i]));
// 	  
// 	  // se le aree sono troppo grosse 
// 	  if((area1+area2)*0.5>((*areeOnEdge.rbegin())*fattRel))	return(false);	
//       }
      
      // ritorno vero se passa tutti i test
      return(true);
}

void doctor2d<Triangle>::collEdge(vector<UInt> * edge)
{
      // variabili utilizzate
      bool				bound1,bound2;
      UInt 					   tmpGeo;
      geoElement<Line>				  tmpEdge;
      set<geoElement<Line> >::iterator		   finder;
      set<UInt>                                 coinvolti;
      set<UInt>::iterator                          it,it2;
      vector<UInt>            common,elem,newEdge,oldEdge;
      UInt                              tmp,id1,id2,newId;
      graphItem				 newNodeToElement;
      
      // setto le variabili per comodità
      id1 = edge->at(0);
      id2 = edge->at(1);
      
      // setto il nodo
      newId = meshPointer->getNumNodes()-1;
      
      // trovo i triangoli adiacenti al lato
      elementOnEdge(id1, id2, &elem);
      
      // prendo le flag del bordo 
      bound1 = meshPointer->getNode(id1).getBoundary();
      bound2 = meshPointer->getNode(id2).getBoundary();
		          
      // trovo TUTTI gli elementi coinvolti
      for(UInt s=0; s<2; ++s)
	    for(UInt j=0; j<conn.getNodeToElementPointer(edge->at(s))->getNumConnected(); ++j)
		coinvolti.insert(conn.getNodeToElementPointer(edge->at(s))->getConnectedId(j));
      
      // elimino dalla lista dei coinvolti quelli in elem
      for(UInt s=0; s<elem.size(); ++s)
      {
	    // li cerco 
	    it = coinvolti.find(elem[s]);
	   
	    // li DEVO TROVARE
	    assert(it!=coinvolti.end());
	    
	    // li elimino
	    coinvolti.erase(it);
			    
	    // metto a 0 TUTTI gli id degli elementi eliminati
	    setTriangleDegenerate(elem[s]);
      }
		    
      // cambio i connessi mettendo l'id dell'ulitimo nodo
      for(it=coinvolti.begin(); it!=coinvolti.end(); ++it)
      {
	    // almeno una di queste due deve essere vera
	    meshPointer->getElementPointer(*it)->change(id1, newId);
	    meshPointer->getElementPointer(*it)->change(id2, newId);
      }
		     
      // rimuovo dalla lista dei connessi ai coinvolti gli id dei due elementi eliminati 
      // e creo il grafo del nouvo punto
      for(it=coinvolti.begin(); it!=coinvolti.end(); ++it)	    newNodeToElement.connectedPushBack(*it);
		
      // creo le connessioni nodo-elemento del nuovo punto
      conn.getNodeToElementPointer()->push_back(newNodeToElement);
		    
      // sistemo le connessioni di nodi dei triangoli coinvolti
      for(UInt s=0; s<2; ++s)
      {
	    for(UInt j=0; j<conn.getNodeToElementPointer(edge->at(s))->getNumConnected(); ++j)
	    {
		// prendo in esame un triangolo della stellata dell'estremo
		tmp = conn.getNodeToElementPointer(edge->at(s))->getConnectedId(j);
				      
		// metto il nuovo id agli elementi coinvolti
		for(UInt t=0; t<3; ++t)
		      for(UInt k=0; k<elem.size(); ++k)
			conn.getNodeToElementPointer(meshPointer->getElement(tmp).getConnectedId(t))->remove(elem[k]);
	    }
      }
		    
      // setto a null le coordinate dei nodi appartenenti al segmento collassato
      setNodeDegenerate(id1);		
      setNodeDegenerate(id2); 
      
      // se è di bordo devo fare delle modifiche 
      if(bound1>0 || bound2>0)
      {
	  // elimino dalla lista se presente 
	  if(isBoundary(edge)) 		eraseBoundEdge(edge);
	  
	  // creo la stellata del nodo appena creato 
	  createStellata(newId, &elem);
	  
	  // faccio un resize
	  oldEdge.resize(2);
	  newEdge.resize(2);
	  
	  for(UInt j=0; j<2; ++j)
	  {
	      // cerco ogni edge che potrebbe essere di bordo 
	      for(UInt i=0; i<elem.size(); ++i)
	      {
		  // controllo
		  oldEdge[0] = edge->at(j);
		  oldEdge[1] = elem[i];
		  
		  // setto l'edge da cercare 
		  tmpEdge.setConnectedId(0, oldEdge[0]);
		  tmpEdge.setConnectedId(1, oldEdge[1]);
		  
		  // lo cerco 
		  finder = bordo.find(tmpEdge);
		 
		  // se lo trovo 
		  if(finder!=bordo.end())
		  {
		      // salvo il geoId 
		      tmpGeo = finder->getGeoId();
		      
		      // lo tolgo 
		      eraseBoundEdge(&oldEdge);
		      
		      // creo l'edge 
		      newEdge[0] = elem[i];
		      newEdge[1] = newId;
		      		  
		      // e lo aggiungo 
		      addBoundEdge(&newEdge, tmpGeo);
		  }
	      }
	  }
      }      
}

bool doctor2d<Triangle>::removeNode(UInt nodeId)
{
    // Questo metodo permette di eliminare un nodo dalla triangolazione. Viene fatto attraverso il collasso di un edge connesso 
    // con l'elemento 
    
    // variabili in uso 
    point 		        pNew;
    vector<UInt>       ids,edge(2,0);
    geoElementSize<Line> 	 seg;
    set<geoElementSize<Line> > lista;
    
    // distinguo i vari casi se il nodo è interno o di bordo 
    switch(meshPointer->getNode(nodeId).getBoundary())
    {
	// se è interno 
	case(0):
		  // prendo i nodi connessi 
		  createStellata(nodeId, &ids);
		  break;
	// se è sul bordo non lo tolgo 
	case(1):
		  // prendo i nodi connessi 
		  createStellataBor(nodeId, &ids);
		  break;
    }
    
    // sett il primo connesso 
    seg.setConnectedId(0, nodeId);
    edge[0] = nodeId;
    
    // ciclo per mettere in ordine i segmenti dal più corto al più lungo 
    for(UInt i=0; i<ids.size(); ++i)
    {
	// setto il secondo e calcolo la lunghezza 
	seg.setConnectedId(1, ids[i]);
	seg.setGeoSize((meshPointer->getNode(nodeId)-meshPointer->getNode(ids[i])).norm2());
	
	// lo metto nella lista 
	lista.insert(seg);
    }
    
    // parto dal più corto e controllo se si può collassare 
    for(set<geoElementSize<Line> >::iterator it=lista.begin(); it!=lista.end(); ++it)
    {
	// setto l'edge 
	edge[1] = it->getConnectedId(1);
	
	// prendo il punto 
	pNew = meshPointer->getNode(edge[1]);
	
	// se lo posso collassare 
	// 
	// N.B. dato che è un triangolo con connessione tripla me ne frego di fare molti controlli 
	// 
	if(controlColl(&edge, pNew))
	{
	      // metto alla fine il punto 
	      meshPointer->insertNode(pNew);
	      
	      // collasso l'edge 
	      collEdge(&edge);
	      return(true);
	}
    }
    
    return(false);
}

// 
// Splitting
//
void doctor2d<Triangle>::splitTria(UInt elemId, bool setUpNormal)
{
      // variabili in uso 
      UInt 			  id1,id2,id3,newId,geoId;
      point 					   normal;
      geoElement<Triangle>                           tria;
      graphItem				 newNodeToElement;
      
      // prendo il geoId 
      geoId = meshPointer->getElement(elemId).getGeoId();
      
      // prendo gli id dei nodi 
      id1   = meshPointer->getElement(elemId).getConnectedId(0);
      id2   = meshPointer->getElement(elemId).getConnectedId(1);
      id3   = meshPointer->getElement(elemId).getConnectedId(2);
      newId = meshPointer->getNumNodes()-1;
      
      // prendo la normale 
      normal = getTriangleNormal(elemId);
      
      // creo i tre elementi 
      tria.setGeoId(geoId);
      tria.setConnectedId(0, id1);
      tria.setConnectedId(1, id2);
      tria.setConnectedId(2, newId);
      meshPointer->insertElement(tria);
      
      // sistemo l'orientazione
      if(setUpNormal)	orient(meshPointer->getNumElements()-1,normal);
      
      tria.setConnectedId(0, id2);
      tria.setConnectedId(1, id3);
      tria.setConnectedId(2, newId);
      meshPointer->insertElement(tria);
      
      // sistemo l'orientazione
      if(setUpNormal)	orient(meshPointer->getNumElements()-1,normal);
      
      tria.setConnectedId(0, id3);
      tria.setConnectedId(1, id1);
      tria.setConnectedId(2, newId);
      meshPointer->insertElement(tria);
      
      // sistemo l'orientazione
      if(setUpNormal)	orient(meshPointer->getNumElements()-1,normal);
      
      // sistemo le associazioni
      conn.getNodeToElementPointer(id1)->remove(elemId);
      conn.getNodeToElementPointer(id2)->remove(elemId);
      conn.getNodeToElementPointer(id3)->remove(elemId);
      
      // lo setto degenere 
      setTriangleDegenerate(elemId);
      
      
      // creo le associazioni dell'ultimo 
      newNodeToElement.connectedPushBack(meshPointer->getNumElements()-1);
      newNodeToElement.connectedPushBack(meshPointer->getNumElements()-2);
      newNodeToElement.connectedPushBack(meshPointer->getNumElements()-3);
      
      // inserisco la connettività del punto
      conn.getNodeToElementPointer()->push_back(newNodeToElement);
}

bool doctor2d<Triangle>::controlSplit(vector<UInt> * edge, point pNew)
{
    // variabili in uso
    UInt            id1,id2,idExt;
    point              p1,p2,pExt;
    point 	 normal,normalTmp;
    vector<UInt>           onEdge;
    
    // salvo gli id 
    id1 = edge->at(0);	id2 = edge->at(1);
    
    // prendo gli elementi sull'edge
    elementOnEdge(id1, id2, &onEdge);
    
    // prendo i punti 
    p1 = meshPointer->getNode(id1);
    p2 = meshPointer->getNode(id2);
    
    // ciclo su tutti i triangoli
    for(UInt i=0; i<onEdge.size(); ++i)
    {
	// prendo l'altro punto 
	idExt = lastNode(id1, id2, onEdge[i]);
	pExt  = meshPointer->getNode(idExt);
	
	// faccio la nomrale 
	normal = (p2-p1)^(pExt-p1);
	normal = normal / normal.norm2();
	
	// controllo il primo triangolo 
	normalTmp = (pNew-p1)^(pExt-p1);
	normalTmp = normalTmp / normalTmp.norm2();
	
	// verifico che non formano un angolo ottuso
	if(normalTmp*normal<0.0)	return(false);
	
	// controllo il primo triangolo 
	normalTmp = (p2-pNew)^(pExt-pNew);
	normalTmp = normalTmp / normalTmp.norm2();
	
	// verifico che non formano un angolo ottuso
	if(normalTmp*normal<0.0)	return(false);
      
    }
    
    return(true);  
}

void doctor2d<Triangle>::splitEdge(vector<UInt> * edge, bool setUpNormal)
{
      // variabili utilizzate
      UInt 					   tmpGeo;
      geoElement<Line>				  tmpEdge;
      set<geoElement<Line> >::iterator		   finder;
      set<UInt>                                 coinvolti;
      set<UInt>::iterator                          it,it2;
      vector<UInt>                             extId,elem;
      vector<point>                               normali;
      UInt                              tmp,id1,id2,newId;
      graphItem				 newNodeToElement;
      geoElement<Triangle>                           tria;
      
      // setto le variabili per comodità
      id1 = edge->at(0);
      id2 = edge->at(1);
		         
      // trovo i triangoli adiacenti al lato
      elementOnEdge(id1, id2, &elem);
      
      // setto il nuovo id per comodità
      newId = meshPointer->getNumNodes()-1;
		    		        
      for(UInt j=0; j<elem.size(); ++j)
      {
	    //pulisco tmp
	    tmp = lastNode(id1, id2, elem[j]);
		  
	    // lo metto nella lista dei nodi esterni
	    extId.push_back(tmp);
			  
	    // Calcolo anche la normale e la metto nel vettore normal
	    normali.push_back(getTriangleNormal(elem[j]));
      }
		    
      // Per ogni elem aggiungo i due elementi che nascono
      for(UInt j=0; j<elem.size(); ++j)
      {	
	    // metto il geoId corretto
	    tria.setGeoId(meshPointer->getElement(elem[j]).getGeoId());
			  
	    // creo i triangoli quello con id1
	    tria.setConnectedId(0,id1);
	    
	    if(setUpNormal)
	    {
		tria.setConnectedId(1,newId);	 	  // OLDSTYLE CON ORIENTAZIONE
		tria.setConnectedId(2,extId[j]);	  // OLDSTYLE CON ORIENTAZIONE
	    }
	    else
	    {
		if(nextNode(id1,elem[j])==id2)
		{
		  tria.setConnectedId(1,newId);
		  tria.setConnectedId(2,extId[j]);
		}
		else
		{
		  tria.setConnectedId(1,extId[j]);
		  tria.setConnectedId(2,newId);
		}
	    }
	    	  
	    // Lo metto nella lista e lo oriento
	    meshPointer->insertElement(tria);
 	    if(setUpNormal) 	orient(meshPointer->getNumElements()-1,normali[j]);   // OLDSTYLE CON ORIENTAZIONE
			  
	    // metto a posto le connessioni
	    conn.getNodeToElementPointer(id1)->remove(elem[j]);
	    conn.getNodeToElementPointer(id1)->add(meshPointer->getNumElements()-1);
	    conn.getNodeToElementPointer(extId[j])->add(meshPointer->getNumElements()-1);
			  
	    // Faccio la connettività del nuovo punto inserito
	    newNodeToElement.connectedPushBack(meshPointer->getNumElements()-1);
			  
	    // creo i triangoli quello con id2
	    tria.setConnectedId(0,id2);
	    if(setUpNormal)
	    {
	    	    tria.setConnectedId(1,newId);     // OLDSTYLE CON ORIENTAZIONE
		    tria.setConnectedId(2,extId[j]);  // OLDSTYLE CON ORIENTAZIONE  
	    }
	    else
	    {
		if(nextNode(id1,elem[j])==id2)
		{
		    tria.setConnectedId(1,extId[j]);
		    tria.setConnectedId(2,newId);
		}
		else
		{
		    tria.setConnectedId(1,newId);
		    tria.setConnectedId(2,extId[j]);
		}
	    }
	    
	    // Lo metto nella lista e lo oriento
	    meshPointer->insertElement(tria);
	    if(setUpNormal)	orient(meshPointer->getNumElements()-1,normali[j]);  // OLDSTYLE CON ORIENTAZIONE
			  
	    // metto a posto le connessioni
	    conn.getNodeToElementPointer(id2)->remove(elem[j]);
	    conn.getNodeToElementPointer(id2)->add(meshPointer->getNumElements()-1);
	    conn.getNodeToElementPointer(extId[j])->add(meshPointer->getNumElements()-1);
			  
	    // Faccio la connettività del nuovo punto inserito
	    newNodeToElement.connectedPushBack(meshPointer->getNumElements()-1);
			  
	    // rimuovo la connessione dell'elemento splittato dal "terzo vertice"
	    conn.getNodeToElementPointer(extId[j])->remove(elem[j]);
      }
      	    
      // inserisco la connettività del punto
      conn.getNodeToElementPointer()->push_back(newNodeToElement);
		    
      // setto a 0 gli elementi splittati
      for(UInt j=0; j<elem.size(); ++j)		setTriangleDegenerate(elem[j]);
      
      // sistemo la variabile bordo 
      if(isBoundary(edge))
      {
	  // setto la linea e la cerco 
	  tmpEdge.setConnectedId(0, edge->at(0));
	  tmpEdge.setConnectedId(1, edge->at(1));
	  
	  // so che ci saraprendo il suo geoId 
	  tmpGeo = bordo.find(tmpEdge)->getGeoId();
	  
	  // elimino dalla lista 
	  eraseBoundEdge(edge);
	  
	  // aggiungo i due che si sono formati
	  elem.clear();
	  elem.resize(2);
	  
	  // setto i due nuovi edge
	  elem[0] = edge->at(0);
	  elem[1] = newId;
	  
	  // aggiungo 
	  addBoundEdge(&elem, tmpGeo);
	  
	  // setto i due nuovi edge
	  elem[0] = edge->at(1);
	  elem[1] = newId;
	  
	  // aggiungo 
	  addBoundEdge(&elem, tmpGeo);
      }
}

void doctor2d<Triangle>::splitRed(UInt elemId, vector<UInt> * ids, vector<geoElement<Triangle> > * tmpElem)
{
    // controllo che ci sia l'elemento
    assert(elemId<meshPointer->getNumElements());
    assert(ids->size()==3);
  
    // variabili in uso 
    vector<UInt>      elemNode;
    geoElement<Triangle>  elem;
    
    // setto il suo geoId
    elem.setGeoId(meshPointer->getElement(elemId).getGeoId());
    
    // metto tutti gli elmeneti in elemNode
    elemNode = meshPointer->getElement(elemId).getConnectedIds();
    
    // ------------------------------------------------
    //                metto gli elementi 
    // ------------------------------------------------
    
    // il primo 
    elem.setConnectedId(0, elemNode[0]);
    elem.setConnectedId(1, ids->at(0));
    elem.setConnectedId(2, ids->at(2));
    elem.setId(tmpElem->size());
    tmpElem->push_back(elem);
    
    // il secondo
    elem.setConnectedId(0, ids->at(0));
    elem.setConnectedId(1, elemNode[1]);
    elem.setConnectedId(2, ids->at(1));
    elem.setId(tmpElem->size());
    tmpElem->push_back(elem);
    
    // il terzo
    elem.setConnectedId(0, ids->at(1));
    elem.setConnectedId(1, elemNode[2]);
    elem.setConnectedId(2, ids->at(2));
    elem.setId(tmpElem->size());
    tmpElem->push_back(elem);
    
    // il quarto
    elem.setConnectedId(0, ids->at(0));
    elem.setConnectedId(1, ids->at(1));
    elem.setConnectedId(2, ids->at(2));
    elem.setId(tmpElem->size());
    tmpElem->push_back(elem);
}

void doctor2d<Triangle>::splitBlue(UInt elemId, vector<UInt> * ids, vector<bool> * edge, vector<geoElement<Triangle> > * tmpElem)
{
    // controllo che ci sia l'elemento
    assert(elemId<meshPointer->getNumElements());
    assert(ids->size()==3);
    assert(edge->size()==3);
  
    // variabili in uso 
    vector<UInt>      elemNode;
    vector<UInt>     tmpLin(2);
    geoElement<Triangle>  elem;
    UInt       edgeId=4,last=0;
    
    // setto il suo geoId
    elem.setGeoId(meshPointer->getElement(elemId).getGeoId());
    
    // metto tutti gli elmeneti in elemNode
    elemNode = meshPointer->getElement(elemId).getConnectedIds();
    
    // guardo quale è l'edge da non splittare
    for(UInt i=0; i<3; ++i)
    {
	  if(!edge->at(i))
	  {
	      // prendo gli id dell'edge 
	      tmpLin[0] = meshPointer->getElement(elemId).getConnectedId((i%3));
	      tmpLin[1] = meshPointer->getElement(elemId).getConnectedId(((i+1)%3));
	      
	      // prendo l'id dell'edge che mi servirà poi per fare meglio l'orientazione 
	      edgeId=i;
	     	      
	      // prendo il terzo 
	      last = lastNode(tmpLin[1], tmpLin[0], elemId);
	      
	      
	      break;
	  }
    }
    
    // ------------------------------------------------
    //                metto gli elementi 
    // ------------------------------------------------
    
    switch(edgeId)
    {
      case(0):
	      // il primo 
	      elem.setConnectedId(0, tmpLin[0]);
	      elem.setConnectedId(1, tmpLin[1]);
	      elem.setConnectedId(2, ids->at(1));
	      elem.setId(tmpElem->size());
	      tmpElem->push_back(elem);
	      
	      // il secondo
	      elem.setConnectedId(0, ids->at(1));
	      elem.setConnectedId(1, last);
	      elem.setConnectedId(2, ids->at(2));
	      elem.setId(tmpElem->size());
	      tmpElem->push_back(elem);
	      
	      // il terzo
	      elem.setConnectedId(0, ids->at(2));
	      elem.setConnectedId(1, tmpLin[0]);
	      elem.setConnectedId(2, ids->at(1));
	      elem.setId(tmpElem->size());
	      tmpElem->push_back(elem);
	      break;
      case(1):
	      // il primo 
	      elem.setConnectedId(0, last);
	      elem.setConnectedId(1, ids->at(0));
	      elem.setConnectedId(2, ids->at(2));
	      elem.setId(tmpElem->size());
	      tmpElem->push_back(elem);
	      
	      // il secondo
	      elem.setConnectedId(0, ids->at(0));
	      elem.setConnectedId(1, tmpLin[0]);
	      elem.setConnectedId(2, tmpLin[1]);
	      elem.setId(tmpElem->size());
	      tmpElem->push_back(elem);
	      
	      // il terzo
	      elem.setConnectedId(0, tmpLin[1]);
	      elem.setConnectedId(1, ids->at(2));
	      elem.setConnectedId(2, ids->at(0));
	      elem.setId(tmpElem->size());
	      tmpElem->push_back(elem);
	      break;
      case(2):
	      // il primo 
	      elem.setConnectedId(0, tmpLin[1]);
	      elem.setConnectedId(1, ids->at(0));
	      elem.setConnectedId(2, ids->at(1));
	      elem.setId(tmpElem->size());
	      tmpElem->push_back(elem);
	      
	      // il secondo
	      elem.setConnectedId(0, ids->at(0));
	      elem.setConnectedId(1, last);
	      elem.setConnectedId(2, ids->at(1));
	      elem.setId(tmpElem->size());
	      tmpElem->push_back(elem);
	      
	      // il terzo
	      elem.setConnectedId(0, tmpLin[0]);
	      elem.setConnectedId(1, tmpLin[1]);
	      elem.setConnectedId(2, ids->at(1));
	      elem.setId(tmpElem->size());
	      tmpElem->push_back(elem);
	      break;
    }
}

void doctor2d<Triangle>::splitGreen(UInt elemId, vector<UInt> * ids, vector<bool> * edge, vector<geoElement<Triangle> > * tmpElem)
{
    // controllo che ci sia l'elemento
    assert(elemId<meshPointer->getNumElements());
    assert(ids->size()==3);
    assert(edge->size()==3);
  
    // variabili in uso 
    UInt        last=0,nuovo=0;
    vector<UInt>     tmpLin(2);
    geoElement<Triangle>  elem;
    
    // setto il suo geoId
    elem.setGeoId(meshPointer->getElement(elemId).getGeoId());
    
    for(UInt i=0; i<3; ++i)
    {
	  if(edge->at(i))
	  {
	      // setto la linea 
	      tmpLin[0] = meshPointer->getElement(elemId).getConnectedId((i%3));
	      tmpLin[1] = meshPointer->getElement(elemId).getConnectedId(((i+1)%3));
	      
	      // prendo il terzo 
	      last = lastNode(tmpLin[1], tmpLin[0], elemId);
	      
	      // salvo l'id
	      nuovo = ids->at(i);
	      
	      // mi fermo 
	      break;
	  }
    }
    
    // ------------------------------------------------
    //                metto gli elementi 
    // ------------------------------------------------
    
    // il primo 
    elem.setConnectedId(0, tmpLin[0]);
    elem.setConnectedId(1, nuovo);
    elem.setConnectedId(2, last);
    elem.setId(tmpElem->size());
    tmpElem->push_back(elem);
    
    // il secondo
    elem.setConnectedId(0, nuovo);
    elem.setConnectedId(1, tmpLin[1]);
    elem.setConnectedId(2, last);
    elem.setId(tmpElem->size());
    tmpElem->push_back(elem);

}

void doctor2d<Triangle>::splitRGB(vector<point> * nodi, vector<geoElement<Line> > * edges)
{
      // varaibili in uso 
      vector<bool>	     edge(3,false);
      vector<UInt>           onEdge,ids(3);
      vector<vector<UInt> >	elemToEdge;
      vector<point>		  nodesNew;
      vector<geoElement<Triangle> >  lista;
      geoElement<Line>		       lin;
      
      // faccio un resize 
      elemToEdge.resize(meshPointer->getNumElements());
      
      // faccio una serie di reserve 
      for(UInt i=0; i<elemToEdge.size(); ++i)	elemToEdge[i].reserve(5);
      
      // per ogni elemento nella lista vedo a che elementi corrisponde
      for(UInt i=0; i<edges->size(); ++i)
      {
	  // prendo gli elementi sull'edge
	  elementOnEdge(edges->at(i).getConnectedId(0), edges->at(i).getConnectedId(1), &onEdge);
	  
	  // per ognuno lo metto sulla lista
	  for(UInt j=0; j<onEdge.size(); ++j) elemToEdge[onEdge[j]].push_back(i);
      }
      
      // faccio un reserve
      lista.reserve(meshPointer->getNumElements()*4);
      
      // creo i triangoli 
      for(UInt i=0; i<elemToEdge.size(); ++i)
      {
	  switch(elemToEdge[i].size())
	  {
	     case(0):
		      // se non ci sono elementi mi limito ad aggiungere l'elemento alla lista
		      lista.push_back(meshPointer->getElement(i));
		      break;
	     case(3):
		      // prendo gli id dei nuovi nodi 
		      for(UInt k=0; k<3; ++k)
		      {
			  // creo una linea 
			  lin.setConnectedId(0, meshPointer->getElement(i).getConnectedId((k%3)));
			  lin.setConnectedId(1, meshPointer->getElement(i).getConnectedId(((k+1)%3)));
			  
			  // se trovo l'edge setto a vero quell'edge
			  if(lin==edges->at(elemToEdge[i][0]))
			  {			      
			      // assegno il valore 
			      ids[k]=meshPointer->getNumNodes()+elemToEdge[i][0];
			      
			      // setto l'edge
			      edge[k]=true;
			  }
			  else if(lin==edges->at(elemToEdge[i][1]))
			  {
			      // assegno il valore 
			      ids[k]=meshPointer->getNumNodes()+elemToEdge[i][1];
			      
			      // setto l'edge
			      edge[k]=true;
			  }
			  else if(lin==edges->at(elemToEdge[i][2]))
			  {
			      // assegno il valore 
			      ids[k]=meshPointer->getNumNodes()+elemToEdge[i][2];
			      
			      // setto l'edge
			      edge[k]=true;
			  }
		      }
		
		      // chiamo il metodo che fa il raffinamento rosso 
		      splitRed(i, &ids, &lista);
		      break;
	     case(2):
		      // resetto edge
		      edge.assign(3,false);
		      
		      for(UInt k=0; k<3; ++k)
		      {
			  // creo una linea 
			  lin.setConnectedId(0, meshPointer->getElement(i).getConnectedId((k%3)));
			  lin.setConnectedId(1, meshPointer->getElement(i).getConnectedId(((k+1)%3)));
			  
			  // se trovo l'edge setto a vero quell'edge
			  if(lin==edges->at(elemToEdge[i][0]))
			  {
			      // assegno il valore 
			      ids[k]=meshPointer->getNumNodes()+elemToEdge[i][0];
			      
			      // setto l'edge
			      edge[k]=true;
			  }
			  else if(lin==edges->at(elemToEdge[i][1]))
			  {
			      // assegno il valore 
			      ids[k]=meshPointer->getNumNodes()+elemToEdge[i][1];
			      
			      // setto l'edge
			      edge[k]=true;
			  }
		      }
		      
		      // chiamo il metodo che fa il raffinamento blue
		      splitBlue(i, &ids, &edge, &lista);
		      break;
	     case(1):
		      // resetto edge
		      edge.assign(3,false);
		      
		      for(UInt k=0; k<3; ++k)
		      {
			  // creo una linea 
			  lin.setConnectedId(0, meshPointer->getElement(i).getConnectedId((k%3)));
			  lin.setConnectedId(1, meshPointer->getElement(i).getConnectedId(((k+1)%3)));
			  
			  // se trovo l'edge setto a vero quell'edge
			  if(lin==edges->at(elemToEdge[i][0]))
			  {
			      // assegno il valore 
			      ids[k]=meshPointer->getNumNodes()+elemToEdge[i][0];
			      
			      // setto l'edge
			      edge[k]=true;
			      
			      break;
			  }
		      }
		      
		      // chiamo il metodo che fa il raffinamento blue
		      splitGreen(i, &ids, &edge, &lista);
		      break;
	     default:
		      cout << "Errore: non so quale raffinamento fare!!" << endl;
		      return;
	  }
      }
      
      // metto tutto in nodesNew
      nodesNew.resize(meshPointer->getNumNodes()+nodi->size());
      copy(meshPointer->getNodePointer()->begin(), meshPointer->getNodePointer()->end(), nodesNew.begin());
      copy(nodi->begin(), nodi->end(), nodesNew.begin()+meshPointer->getNumNodes());
      
      // metto i nodi e gli elementi 
      meshPointer->insertNode(&nodesNew);
      meshPointer->insertElement(&lista);
      
      // metto a posto le connettività
      setUp();
      
}
