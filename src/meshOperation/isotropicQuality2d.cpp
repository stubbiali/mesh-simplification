#include "isotropicQuality2d.h"

using namespace std;
using namespace geometry;

//
// Costruttore 
//
isotropicQuality2d<Triangle>::isotropicQuality2d() : doctor2d<Triangle>()
{
}

isotropicQuality2d<Triangle>::isotropicQuality2d(mesh2d<Triangle> * _meshPointer) : doctor2d<Triangle>(_meshPointer)
{
}

//
// Metodi per calcolare la qualità
//
Real isotropicQuality2d<Triangle>::triangleQual(UInt elemId)
{
    // variabili temporanee
    vector<point>	pt;
    
    // prendo le coordinate dei nodi 
    meshPointer->getNodeOfElement(elemId, &pt);
    
    // ritorno il valore
    return(triangleQual(pt[0], pt[1], pt[2]));
}

Real isotropicQuality2d<Triangle>::triangleQual(UInt id1, UInt id2, UInt id3)
{
      return(triangleQual(meshPointer->getNode(id1), meshPointer->getNode(id2), meshPointer->getNode(id3)));
}

Real isotropicQuality2d<Triangle>::triangleQual(point p1, point p2, point p3)
{
    // ricavo le lunghezze
    Real a = (p2-p1).norm2();
    Real b = (p3-p2).norm2();
    Real c = (p1-p3).norm2();
    Real p = (a+b+c)*0.5;

    // se è degenere ritorno 0.0
    if( (a<toll) || (b<toll) || (c<toll) )     return (0.0);
    
    // ritorno la qualità
    return ((8.0*(p-a)*(p-b)*(p-c))/(a*b*c));
}

Real isotropicQuality2d<Triangle>::qualityOnEdge(vector<UInt> * edge)
{
    // controllo che edge sia corretto 
    assert(edge->size()==2);
    
    // variabili temporanee
    Real       minQual=1.0;
    vector<point>	pt;
    vector<UInt>      elem;
    
    // prendo gli elementi sull'edge
    elementOnEdge(edge->at(0), edge->at(1), &elem);
    
    // per ogni elemento sull'edge calcolo la qualità
    for(UInt i=0; i<elem.size(); ++i)
    {
	// prendo le coordinate dei nodi 
	meshPointer->getNodeOfElement(elem[i], &pt);
	
	// calcolo la qualità e prendo la più piccola
	minQual = min(minQual, triangleQual(pt[0], pt[1], pt[2]));
    }
    
    // ritorno il valore 
    return(minQual);
}

Real isotropicQuality2d<Triangle>::qualityOnEdge(vector<UInt> * edge, point pt)
{
    // controllo che edge sia corretto 
    assert(edge->size()==2);
    
    // variabili temporanee
    Real                     minQual=1.0;
    vector<UInt>		    elem;  
    set<UInt>			  allTmp;
    set<UInt>::iterator               it;
    
    // prendo gli elementi sull'edge
    elementOnEdge(edge->at(0), edge->at(1), &elem);
    
    // prendo tutti gli elementi 
    for(UInt j=0; j<2; ++j)
	for(UInt i=0; i<conn.getNodeToElementPointer(edge->at(j))->getNumConnected(); ++i)	
	    allTmp.insert(conn.getNodeToElementPointer(edge->at(j))->getConnectedId(i));	
	
    // tolgo tutti gli elementi che collasseranno
    for(UInt j=0; j<elem.size(); ++j)
    {
	// cerco l'elemento
	it = allTmp.find(elem[j]);
	
	// lo devo trovare 
	assert(it!=allTmp.end());
	
	// lo elimino
	allTmp.erase(it);
    }
    
    // prendo le coordinat
    Real x0 = meshPointer->getNode(edge->at(0)).getX();
    Real y0 = meshPointer->getNode(edge->at(0)).getY();
    Real z0 = meshPointer->getNode(edge->at(0)).getZ();
    
    // prendo le coordinat
    Real x1 = meshPointer->getNode(edge->at(1)).getX();
    Real y1 = meshPointer->getNode(edge->at(1)).getY();
    Real z1 = meshPointer->getNode(edge->at(1)).getZ();
    
    // cambio le coordinate
    for(UInt i=0; i<2; ++i)
    {
	meshPointer->getNodePointer(edge->at(i))->setX(pt.getX());
	meshPointer->getNodePointer(edge->at(i))->setY(pt.getY());
	meshPointer->getNodePointer(edge->at(i))->setZ(pt.getZ());
    }
    
    // calcolo la qualità
    for(it=allTmp.begin(); it!=allTmp.end(); ++it) minQual = min(minQual, triangleQual(*it));
    
    // simetto le coordinate
    meshPointer->getNodePointer(edge->at(0))->setX(x0);
    meshPointer->getNodePointer(edge->at(0))->setY(y0);
    meshPointer->getNodePointer(edge->at(0))->setZ(z0);
    
    // simetto le coordinate
    meshPointer->getNodePointer(edge->at(1))->setX(x1);
    meshPointer->getNodePointer(edge->at(1))->setY(y1);
    meshPointer->getNodePointer(edge->at(1))->setZ(z1);
    
    // ritorno il valore 
    return(minQual);
}

Real isotropicQuality2d<Triangle>::qualityAroundEdge(vector<UInt> * edge)
{
    return(min(qualityOnNode(edge->at(0)),qualityOnNode(edge->at(1))));
}

Real isotropicQuality2d<Triangle>::qualityAfterSwap(vector<UInt> * edge)
{
    // varaibli in uso 
    UInt             id3,id4;
    vector<UInt>	elem;
    
    // prendo gli elementi sull'edge 
    elementOnEdge(edge->at(0), edge->at(1), &elem);
    
    // assert che deve essere sempre vero 
    assert(elem.size()==2);
    
    // trovo gli altri nodi 
    id3 = lastNode(edge->at(0), edge->at(1), elem[0]);
    id4 = lastNode(edge->at(0), edge->at(1), elem[1]);
    
    // ritorno il più piccolo
    return(min(triangleQual(edge->at(0),id3,id4),triangleQual(edge->at(1),id3,id4)));
}

Real isotropicQuality2d<Triangle>::qualityOnNode(UInt nodeId)
{
    // variabili temporanee
    Real       minQual=1.0;
    vector<point>	pt;
    vector<UInt>      elem;
      
    // per ogni elemento sull'edge calcolo la qualità
    for(UInt i=0; i<conn.getNodeToElementPointer(nodeId)->getNumConnected(); ++i)
    {
	// prendo le coordinate dei nodi 
	meshPointer->getNodeOfElement(conn.getNodeToElementPointer(nodeId)->getConnectedId(i), &pt);
	
	// calcolo la qualità e prendo la più piccola
	minQual = min(minQual, triangleQual(pt[0], pt[1], pt[2]));
    }
    
    // ritorno il valore 
    return(minQual);
}

Real isotropicQuality2d<Triangle>::qualityOnNode(UInt nodeId, point pt)
{
    // prendo le vacchie coordinate
    Real x = meshPointer->getNode(nodeId).getX();
    Real y = meshPointer->getNode(nodeId).getY();
    Real z = meshPointer->getNode(nodeId).getZ();
    
    // cambio le coordinate
    meshPointer->getNodePointer(nodeId)->setX(pt.getX());
    meshPointer->getNodePointer(nodeId)->setY(pt.getY());
    meshPointer->getNodePointer(nodeId)->setZ(pt.getZ());
    
    // prendo la qualità
    Real minQual = qualityOnNode(nodeId);
    
    // cambio le coordinate
    meshPointer->getNodePointer(nodeId)->setX(x);
    meshPointer->getNodePointer(nodeId)->setY(y);
    meshPointer->getNodePointer(nodeId)->setZ(z);
    
    // ritorno la qualità
    return(minQual);
    
}

//
// Metodi per le routine greedy
//
void isotropicQuality2d<Triangle>::createNodeList(set<geoElementSize<simplePoint> > * lista)
{
    // varaibili in uso 
    geoElementSize<simplePoint>	 elem;
    
    // mi assicuro che la lista sia vuota 
    lista->clear();
    
    // ciclo sugli elementi e creazione della lista
    for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
    {
	  // setto la connessione 
	  elem.setConnectedId(0, i);
	  
	  // setto il geoSize
	  elem.setGeoSize(qualityOnNode(i));
	  
	  // lo aggiungo 
	  lista->insert(elem);
    }
}

void isotropicQuality2d<Triangle>::createElementList(set<geoElementSize<Triangle> > * lista)
{ 
    // varaibili in uso 
    geoElementSize<Triangle>	 elem;
    
    // mi assicuro che la lista sia vuota 
    lista->clear();
    
    // ciclo sugli elementi e creazione della lista
    for(UInt i=0; i<meshPointer->getNumElements(); ++i)
    {
	  // setto la connessione 
	  elem.setConnectedId(0, meshPointer->getElement(i).getConnectedId(0));
	  elem.setConnectedId(1, meshPointer->getElement(i).getConnectedId(1));
	  elem.setConnectedId(2, meshPointer->getElement(i).getConnectedId(2));
	  elem.setId(meshPointer->getElement(i).getId());
	  
	  // setto il geoSize
	  elem.setGeoSize(triangleQual(i));
	  
	  // lo aggiungo 
	  lista->insert(elem);
    }
}

void isotropicQuality2d<Triangle>::removeNodeList(set<geoElementSize<simplePoint> > * lista, vector<UInt> * ids)
{ 
    // varaibili in uso 
    geoElementSize<simplePoint>	 		   elem;
    set<geoElementSize<simplePoint> >::iterator  finder;
     
    // ciclo sugli elementi e creazione della lista
    for(UInt i=0; i<ids->size(); ++i)
    {
	  // setto la connessione e il size 
	  elem.setConnectedId(0, ids->at(i));
	  elem.setGeoSize(qualityOnNode(ids->at(i)));
	  
	  // lo cerco
	  finder = lista->find(elem);

	  // lo elimino 
	  if(finder!=lista->end()) 	lista->erase(finder);
    }
}

void isotropicQuality2d<Triangle>::removeElementList(set<geoElementSize<Triangle> > * lista, vector<UInt> * ids)
{
    // varaibili in uso 
    geoElementSize<Triangle>	 		   elem;
    set<geoElementSize<Triangle> >::iterator     finder;
     
    // ciclo sugli elementi e creazione della lista
    for(UInt i=0; i<ids->size(); ++i)
    {
	  // setto la connessione e il size 
	  elem.setConnectedId(0, meshPointer->getElement(ids->at(i)).getConnectedId(0));
	  elem.setConnectedId(1, meshPointer->getElement(ids->at(i)).getConnectedId(1));
	  elem.setConnectedId(2, meshPointer->getElement(ids->at(i)).getConnectedId(2));
	  elem.setGeoSize(triangleQual(ids->at(i)));
	  
	  // lo cerco
	  finder = lista->find(elem);
	  
	  // lo elimino 
	  if(finder!=lista->end()) 	lista->erase(finder);
    }
}

void isotropicQuality2d<Triangle>::addNodeList(set<geoElementSize<simplePoint> > * lista, vector<UInt> * ids)
{ 
    // varaibili in uso 
    geoElementSize<simplePoint>	 		   elem;
     
    // ciclo sugli elementi e creazione della lista
    for(UInt i=0; i<ids->size(); ++i)
    {
	  // setto la connessione e il size 
	  elem.setConnectedId(0, ids->at(i));
	  elem.setGeoSize(qualityOnNode(ids->at(i)));
	  	  
	  // lo aggiungo
	  lista->insert(elem);
    }
}

void isotropicQuality2d<Triangle>::addElementList(set<geoElementSize<Triangle> > * lista, vector<UInt> * ids)
{
    // varaibili in uso 
    geoElementSize<Triangle>	 		   elem;
     
    // ciclo sugli elementi e creazione della lista
    for(UInt i=0; i<ids->size(); ++i)
    {
	  // setto la connessione e il size 
	  elem.setConnectedId(0, meshPointer->getElement(ids->at(i)).getConnectedId(0));
	  elem.setConnectedId(1, meshPointer->getElement(ids->at(i)).getConnectedId(1));
	  elem.setConnectedId(2, meshPointer->getElement(ids->at(i)).getConnectedId(2));
	  elem.setGeoSize(triangleQual(ids->at(i)));
	  
	  // lo aggiungo 
	  lista->insert(elem);
    }
}

//
//  Metodi per calcolare la nuova posizione dei punti e cercare cosa fare 
//
point isotropicQuality2d<Triangle>::newSmoothingPoint(UInt i)
{

     // controllo della variabile in input
     assert(i<meshPointer->getNumNodes());

     // Variabili utilizzate
     UInt                                tmpId;
     point  tmpPt1,tmpPt2,p0,pm,newPoint,toAdd;
     point                         delta,p1,p2;
     vector<UInt>              connessi,geoIds;
     
     // prendo le coordinate del punto in input
     p0 = meshPointer->getNode(i);
     
     // Setto a (0,0,0) la variabile point di output
     newPoint.setX(0.0);	newPoint.setY(0.0);	newPoint.setZ(0.0);
     delta.setX(0.0);	        delta.setY(0.0);	delta.setZ(0.0);
     
     //
     // Il calcolo dellle coordinate del nuovo punto è preso dall'articolo "Two techniques to improve mesh quality and preserve 
     // surface characteristics" di B.Semenova, V.Savchenko e Hagiwara
     // 
     
     // controllo che non sia un punto triplo
     geoIdAround(i, &geoIds);
     if(geoIds.size()>2)	return(pNull);
     
     // Ci sono tre casi
     // 0 -> il nodo preso in esame è interno
     // 1 -> il nodo preso in esame è di bordo
     switch(p0.getBoundary())
     {
	  // il nodo è iterno quindi mi limito a prendere la sua stellata così come è
	  case(0):
		  // creo la stellata a partire dalla connettività nodo elemento
		  createStellata(i, &connessi);  
		  
		  // prendi in esame la stellata del punto
		  for(UInt j=0; j<connessi.size(); ++j)
		  {
		      // prendo in esame un id della stellata
		      tmpId   = connessi[j];
		      
		      // prendo le sue coordinate
		      tmpPt1  = meshPointer->getNode(tmpId);
		    
		      // prendo le coordinate del punto della stellata che forma un angolo massimo con il segmento i-tmpId
		      tmpPt2  = meshPointer->getNode(getMaxAngle(i, tmpId));
		    
		      // prendo il punto medio
		      pm      = ((tmpPt1 + tmpPt2)*0.5);
		    
		      // calcolo la nuova posizione
		      toAdd   = p0 + (((tmpPt2-tmpPt1)/((tmpPt2-tmpPt1).norm2()))*((pm-p0)*(pm-tmpPt1)))/(pm-tmpPt1).norm2();
		      
		      // sommo
		      newPoint.add(toAdd);
		  }
	  
		  newPoint = newPoint / static_cast<Real>(connessi.size());
		  
		  break;
	  
	  // il nodo è di bordo quindi prendo solo i nodi di bordo connessi a lui
	  case(1):
		  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		  // da dei problemi per il caso di quadrati con bordo ben definito 
		  // per il momento spengo questa funzionalità
		  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 		  // prendo i connessi 
// 		  createStellataBor(i, &connessi);
// 		  
// 		  // nel caso di un punto sul bordo utilizzo la formula di matteo Longoni
// 		  for(UInt j=0; j<connessi.size(); ++j)
// 		  {
// 		      delta.add(meshPointer->getNode(connessi[j])-p0);
// 		  }
// 
// 		  newPoint = p0 + delta*(0.6*(1./static_cast<Real>(connessi.size())));
		  
		  
		  newPoint = p0;
		  

		  break;
     }
     return (newPoint);
}

point isotropicQuality2d<Triangle>::newCollapsingPoint(vector<UInt> * edge)
{
    // varaibili in uso 
    point     		             pNew = pNull;
    point p1 =	meshPointer->getNode(edge->at(0));
    point p2 =	meshPointer->getNode(edge->at(1));
    UInt bound1 = meshPointer->getNode(edge->at(0)).getBoundary();
    UInt bound2 = meshPointer->getNode(edge->at(1)).getBoundary();
    vector<UInt>		geoIds1,geoIds2;
    
    switch(bound1)
    {
      case(0):
	      switch(bound2)
	      {	
		// tutti e due interni
		case(0):
			pNew.replace(p1,p2,0.5);
			pNew.setBoundary(0);
			break;
		// p2 è sul bordo
		case(1):
			pNew = p2;
			pNew.setBoundary(1);
			break;
	      }
	      break;
      case(1):
	      switch(bound2)
	      {	
		// p1 è sul bordo
		case(0):
			pNew = p1;
			pNew.setBoundary(1);
			break;
		// p1 e p2 sono sul bordo ma si deve controllare se sono effettivamente un edge
		case(1):
			if(isBoundary(edge))
			{
			    // controllo i geoId che hanno attorno perché se no riscio di non conservare gli spigoli
			    geoIdAround(edge->at(0), &geoIds1);
			    geoIdAround(edge->at(1), &geoIds2);
			    
			    // modo per conservare gli spigoli 
			    if((geoIds1.size()>2)       && (geoIds2.size()==2))	pNew = p1;		
			    else if((geoIds1.size()==2) && (geoIds2.size()>2))	pNew = p2;
			    else
			    {
			      pNew.replace(p1,p2,0.5);
			      pNew.setBoundary(1);
			    }
			}
			else			pNew = pNull;
			break;
	      }
	      break;
    }
    
    // ritorno il valore 
    return(pNew);
}

point isotropicQuality2d<Triangle>::newSplittingPoint(vector<UInt> * edge)
{
    // varaibili in uso 
    point     				     pNew;
    point p1 =	meshPointer->getNode(edge->at(0));
    point p2 =	meshPointer->getNode(edge->at(1));
    UInt bound1 = meshPointer->getNode(edge->at(0)).getBoundary();
    UInt bound2 = meshPointer->getNode(edge->at(1)).getBoundary();
    
    switch(bound1)
    {
      case(0):
	      switch(bound2)
	      {	
		// tutti e due interni
		case(0):
			pNew.replace(p1,p2,0.5);
			pNew.setBoundary(0);
			break;
		// p2 è sul bordo
		case(1):
			pNew.replace(p1,p2,0.5);
			pNew.setBoundary(0);
			break;
	      }
	      break;
      case(1):
	      switch(bound2)
	      {	
		// p1 è sul bordo
		case(0):
			pNew.replace(p1,p2,0.5);
			pNew.setBoundary(0);
			break;
		// p1 e p2 sono sul bordo ma si deve controllare se sono effettivamente un edge
		case(1):
			if(isBoundary(edge))
			{
			    pNew.replace(p1,p2,0.5);
			    pNew.setBoundary(1);
			}
			else
			{
			    pNew.replace(p1,p2,0.5);
			    pNew.setBoundary(0);
			}
			break;
	      }
	      break;
    }
    
    // ritorno il valore 
    return(pNew);
}

//
//  Metodi per cercare l'edge da modificare 
//
pair<bool, vector<UInt> > isotropicQuality2d<Triangle>::findEdgeToSwap(UInt elemId, Real limite)
{
    // variabili temporanee
    Real               oldQual,newQual;
    vector<UInt>		tmpLin;
    geoElementSize<Line>	   lin;
    set<geoElementSize<Line> >   lista;
    pair<bool, vector<UInt> >	result;
    
    // faccio un resize
    tmpLin.resize(2);
    
    // ciclo sui lati
    for(UInt i=0; i<3; ++i)
    {
	  // setto la linea
	  tmpLin[0] = meshPointer->getElement(elemId).getConnectedId((i%3));
	  tmpLin[1] = meshPointer->getElement(elemId).getConnectedId(((i+1)%3));
	  
	  // se lo posso swappare procedo con l'analisi 
	  if(controlSwap(&tmpLin, limite))
	  {
	      // calcolo le qualità
	      oldQual = qualityOnEdge(&tmpLin);
	      newQual = qualityAfterSwap(&tmpLin);
	      
	      // se incrementa la qualità lo salvo 
	      if(oldQual<newQual)	
	      {
		  // setto lin
		  lin.setConnectedId(0, tmpLin[0]);
		  lin.setConnectedId(1, tmpLin[1]);
		  lin.setGeoSize(newQual);
		  
		  // lo metto nella lista
		  lista.insert(lin);
	      }
	  }
    }
    
    // controllo come è andata l'analisi 
    if(lista.size()==0)
    {
	result.first = false;
    }
    else
    {
	result.first = true;
	result.second.resize(2);
	result.second[0] = lista.rbegin()->getConnectedId(0);
	result.second[1] = lista.rbegin()->getConnectedId(1);
    }
    
    // ritorno il risultato 
    return(result);
}

pair<bool, vector<UInt> > isotropicQuality2d<Triangle>::findEdgeToColl(UInt elemId, Real lung)
{
    // variabili temporanee
    Real               oldQual,newQual;
    point                   pNew,p1,p2;
    vector<UInt>		tmpLin;
    geoElementSize<Line>	   lin;
    set<geoElementSize<Line> >   lista;
    pair<bool, vector<UInt> >	result;
    
    // faccio un resize
    tmpLin.resize(2);
    
    // ciclo sui lati
    for(UInt i=0; i<3; ++i)
    {
	  // setto la linea
	  tmpLin[0] = meshPointer->getElement(elemId).getConnectedId((i%3));
	  tmpLin[1] = meshPointer->getElement(elemId).getConnectedId(((i+1)%3));
	  
	  // prendo i punti 
	  p1 = meshPointer->getNode(tmpLin[0]);
	  p2 = meshPointer->getNode(tmpLin[1]);
	  
	  // calcolo il punto 
	  pNew = newCollapsingPoint(&tmpLin);
	  
	  // controllo la lunghezza e se lo posso collassare 
	  if(((p1-p2).norm2()<(lung-toll))  && controlColl(&tmpLin, pNew))
	  {
	      // calcolo le qualità
	      oldQual = qualityAroundEdge(&tmpLin)*2./3.;
	      newQual = qualityOnEdge(&tmpLin, pNew);
	      
	      // se incrementa la qualità lo salvo 
	      if(oldQual<newQual)	
	      {
		  // setto lin
		  lin.setConnectedId(0, tmpLin[0]);
		  lin.setConnectedId(1, tmpLin[1]);
		  lin.setGeoSize(newQual);
		  
		  // lo metto nella lista
		  lista.insert(lin);
	      }
	  }
    }
    
    // controllo come è andata l'analisi 
    if(lista.size()==0)
    {
	result.first = false;
    }
    else
    {
	result.first = true;
	result.second.resize(2);
	result.second[0] = lista.begin()->getConnectedId(0);
	result.second[1] = lista.begin()->getConnectedId(1);
    }
    
    // ritorno il risultato 
    return(result);
}

pair<bool, vector<UInt> > isotropicQuality2d<Triangle>::findEdgeToSplit(UInt elemId, Real lung)
{
    // variabili temporanee
    point                   pNew,p1,p2;
    vector<UInt>		tmpLin;
    geoElementSize<Line>	   lin;
    set<geoElementSize<Line> >   lista;
    pair<bool, vector<UInt> >	result;
    
    // faccio un resize
    tmpLin.resize(2);
    
    // ciclo sui lati
    for(UInt i=0; i<3; ++i)
    {
	  // setto la linea
	  tmpLin[0] = meshPointer->getElement(elemId).getConnectedId((i%3));
	  tmpLin[1] = meshPointer->getElement(elemId).getConnectedId(((i+1)%3));
	  
	  // prendo i punti 
	  p1 = meshPointer->getNode(tmpLin[0]);
	  p2 = meshPointer->getNode(tmpLin[1]);
	  
	  // controllo la lunghezza e se lo posso collassare 
	  if((p1-p2).norm2()>(lung+toll))
	  {    
	      // setto lin
	      lin.setConnectedId(0, tmpLin[0]);
	      lin.setConnectedId(1, tmpLin[1]);
	      lin.setGeoSize((p1-p2).norm2());
		  
	      // lo metto nella lista
	      lista.insert(lin);
	  }
    }
    
    // controllo come è andata l'analisi 
    if(lista.size()==0)
    {
	result.first = false;
    }
    else
    {
	result.first = true;
	result.second.resize(2);
	result.second[0] = lista.rbegin()->getConnectedId(0);
	result.second[1] = lista.rbegin()->getConnectedId(1);
    }
    
    // ritorno il risultato 
    return(result);
}

//
// Metodo che fa un ciclo di smooth
//
void isotropicQuality2d<Triangle>::smoothing(UInt iter)
{
    // Variabili temporanee
    point   	       newPos;
    Real      	      oldQual;
    UInt                 cont;
    
    cout << "Node Smoothing process:" << endl;
    
    //	ciclo sui nodi 
    for(UInt k=0; k<iter; ++k)
    {
      cont = 0;
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
      {	
	  // se non è degenere 
	  if(!isNodeDegenerate(i))
	  {
	      // calcolo la qualità
	      oldQual = qualityOnNode(i);
	  
	      // calcolo il nuovo punto 
	      newPos  = newSmoothingPoint(i);
	      
	      // controllo se è ok e cambio le coordinate
	      if(controlPosition(i, newPos) && (qualityOnNode(i, newPos)>oldQual))
	      {
		  changeNode(i, newPos);
		  ++cont;
	      }
	  }
       }
       cout << "Iterazione-"  << k << " nodi cambiati: " << cont << endl;
    }
}

void isotropicQuality2d<Triangle>::smoothingOnlyInternal(UInt iter)
{
    // Variabili temporanee
    point   	       newPos;
    Real      	      oldQual;
    UInt             cont,bor;
    
    cout << "Node Smoothing process:" << endl;
    
    //	ciclo sui nodi 
    for(UInt k=0; k<iter; ++k)
    {
      cont = 0;
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
      {	
	  // prendo il brdo 
	  bor = meshPointer->getNode(i).getBoundary();
	  
	  // se non è degenere 
	  if((bor==0) && (!isNodeDegenerate(i)))
	  {
	      // calcolo la qualità
	      oldQual = qualityOnNode(i);
	  
	      // calcolo il nuovo punto 
	      newPos  = newSmoothingPoint(i);
	      
	      // controllo se è ok e cambio le coordinate
	      if(controlPosition(i, newPos) && (qualityOnNode(i, newPos)>oldQual))
	      {
		  changeNode(i, newPos);
		  ++cont;
	      }
	  }
       }
       cout << "Iterazione-"  << k << " nodi cambiati: " << cont << endl;
    }
}

void isotropicQuality2d<Triangle>::swapping(Real limite)
{
    // Variabili temporanee
    UInt                        cont;
    pair<bool, vector<UInt> > result;
    
    cout << "Swapping Process: ";
    
    cont = 0;
    for(UInt i=0; i<meshPointer->getNumElements(); ++i)
    {
	// se non è degenere 
	if(!isTriangleDegenerate(i))
	{
	      // cerco il lato 
	      result = findEdgeToSwap(i, limite);
	      
	      // se posso 
	      if(result.first)
	      {
		  // swappo 
		  swap(&result.second);
		  
		  // conto 
		  ++cont;
	      }
	}
    }
    cout << "edge swappati " << cont << endl;
}

void isotropicQuality2d<Triangle>::collapsing(Real lung)
{
    // Variabili temporanee
    UInt                        cont;
    point 			pNew;
    pair<bool, vector<UInt> > result;
    
    cout << "Collapsing Process: ";
    
    cont = 0;
    for(UInt i=0; i<meshPointer->getNumElements(); ++i)
    {
	// se non è degenere 
	if(!isTriangleDegenerate(i))
	{
	      // cerco il lato 
	      result = findEdgeToColl(i, lung);
	      
	      // se posso 
	      if(result.first)
	      {
		  // trovo il nodo 
		  pNew = newCollapsingPoint(&result.second);
		  
		  // lo metto nella lista 
		  meshPointer->insertNode(pNew);
		  
		  // collasso tenendo buono l'id del secondo 
		  collEdge(&result.second);
		  
		  // conto 
		  ++cont;
	      }
	}
    }
    cout << "edge collassati " << cont << endl;    
    cout << "Eseguo un refresh delle variabili" << endl;
    refresh();
}

void isotropicQuality2d<Triangle>::collapsingOnlyInternal(Real lung)
{
    // Variabili temporanee
    UInt                        cont;
    point 			pNew;
    pair<bool, vector<UInt> > result;
    
    cout << "Collapsing Process: ";
    
    cont = 0;
    for(UInt i=0; i<meshPointer->getNumElements(); ++i)
    {
	// se non è degenere 
	if(!isTriangleDegenerate(i))
	{
	      // cerco il lato 
	      result = findEdgeToColl(i, lung);
	      
	      // se posso e il lato non è di bordo 
	      if((result.first) && (!isBoundary(&result.second)))
	      {
		  // trovo il nodo 
		  pNew = newCollapsingPoint(&result.second);
		  
		  // lo metto nella lista 
		  meshPointer->insertNode(pNew);
		  
		  // collasso tenendo buono l'id del secondo 
		  collEdge(&result.second);
		  
		  // conto 
		  ++cont;
	      }
	}
    }
    cout << "edge collassati " << cont << endl;    
    cout << "Eseguo un refresh delle variabili" << endl;
    refresh();
}

void isotropicQuality2d<Triangle>::splitting(Real lung)
{
    // Variabili temporanee
    UInt                        cont;
    point 			pNew;
    pair<bool, vector<UInt> > result;
    
    cout << "Splitting Process: ";
    
    cont = 0;
    for(UInt i=0; i<meshPointer->getNumElements(); ++i)
    {
	// se non è degenere 
	if(!isTriangleDegenerate(i))
	{
	      // cerco il lato 
	      result = findEdgeToSplit(i, lung);
	      
	      // se posso 
	      if(result.first)
	      {
		  // trovo il nodo 
		  pNew = newSplittingPoint(&result.second);
		  
		  // lo metto nella lista 
		  meshPointer->insertNode(pNew);
		  
		  // collasso tenendo buono l'id del secondo 
		  splitEdge(&result.second);
		  
		  // conto 
		  ++cont;
	      }
	}
    }
    cout << "edge splittati " << cont << endl;
    cout << "Eseguo un refresh delle variabili" << endl;
    refresh();
}

void isotropicQuality2d<Triangle>::refineUniform()
{
      // varaibili in uso
      point 			   p1,p2;
      mesh1d<Line>		reticolo;
      vector<point>		    medi;
      
      // creo la mesh1d
      meshPointer->createMesh1d(&reticolo);
      
      // creo i punti medi
      medi.resize(reticolo.getNumElements());
      for(UInt i=0; i<medi.size(); ++i)
      {
	  // prendo i punti 
	  p1 = reticolo.getNode(reticolo.getElement(i).getConnectedId(0));
	  p2 = reticolo.getNode(reticolo.getElement(i).getConnectedId(1));
	  
	  // calcolo il punto medio 
	  medi[i].replace(p1,p2,0.5);
      }
      
      // faccio lo splitting
      splitRGB(&medi, reticolo.getElementPointer());
      
}

void isotropicQuality2d<Triangle>::removeNodes(UInt num)
{
      // variabili in uso 
      bool      isBound,isToColl;
      UInt    cont=0,tentativi=0;
      
      // stampa 
      cout << "Node contraction Process: ";
      
      // ciclo sui nodi 
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
      {
	     // aggiorno le variabil 
	     isBound  = (meshPointer->getNode(i).getBoundary()!=0);
	     isToColl = ((getNumElementAround(i)>0) && (getNumElementAround(i)<=num));
	     
	     // se passa tutti i test lo collasso e conto 
	     if((!isBound) && isToColl)
	     {
	       if(removeNode(i)) ++cont;
	        ++tentativi;
	     }
	     
      }
      
      cout << "nodi contratti " << cont << " su " << tentativi << endl;    
      cout << "Eseguo un refresh delle variabili" << endl;
      refresh();
}

void isotropicQuality2d<Triangle>::standardImprove(UInt iter, UInt iterSmooth, Real lungMax, Real lungMin, Real angolo)
{
    // variabili in uso
    time_t  start,end;
    Real          dif;
    set<Real>    lung;
    vector<UInt> edge;
    
    // output
    cout << "Processo di miglioramento isotropo..." << endl;
    time(&start);
    
    for(UInt passi=0; passi<iter; ++passi)
    {
	// prendo la lunghezza più piccola
	for(UInt i=0; i<meshPointer->getNumElements(); ++i)		lung.insert(getMinEdge(i, &edge));	
   
	// elimino gli edge più corti
	collapsing((*lung.begin())*lungMin+(*lung.rbegin())*lungMax);
	
	// faccio lo swap 
    	swapping(angolo);
	          
	// riparo la mesh
	for(UInt i=0; i<iterSmooth; ++i)
	{
      		smoothing(5);
		swapping(angolo);
	}
	
    }
    
    time(&end);
    dif = difftime(end,start);
    cout << "Processo di miglioramento isotropo completato: " <<  dif << " sec." << endl;
}

void isotropicQuality2d<Triangle>::standardImproveOnlyInternal(UInt iter, UInt iterSmooth, Real lungMax, Real lungMin, Real angolo)
{
    // variabili in uso
    time_t  start,end;
    Real          dif;
    set<Real>    lung;
    vector<UInt> edge;
    
    // output
    cout << "Processo di miglioramento isotropo..." << endl;
    time(&start);
    
    for(UInt passi=0; passi<iter; ++passi)
    {
	// prendo la lunghezza più piccola
	for(UInt i=0; i<meshPointer->getNumElements(); ++i)		lung.insert(getMinEdge(i, &edge));	
   
	// elimino gli edge più corti
	collapsingOnlyInternal((*lung.begin())*lungMin+(*lung.rbegin())*lungMax);
	
	// faccio lo swap 
    	swapping(angolo);
	          
	// riparo la mesh
	for(UInt i=0; i<iterSmooth; ++i)
	{
      		smoothingOnlyInternal(5);
		swapping(angolo);
	}
	
    }
    
    time(&end);
    dif = difftime(end,start);
    cout << "Processo di miglioramento isotropo completato: " <<  dif << " sec." << endl;
}

//
// Processi che ciclano sulla mesh e fanno delle operazioni partendo sempre da quello con qualità più bassa
//
void isotropicQuality2d<Triangle>::smoothingGreedy(Real minQual)
{
    // Variabili temporanee
    UInt                               cont,nodeId;
    Real      	      	 	           oldQual;
    set<geoElementSize<simplePoint> >        lista;
    set<geoElementSize<simplePoint> >::iterator it;
    vector<UInt>	   	       ids,howMany;
    point   	      		            newPos;
    
    // creo il vettore che conterà quante volte 
    howMany.resize(meshPointer->getNumNodes(), 0);
    
    // stampa
    cout << "Greedy Node Smoothing process: ";
    
    // setto il contatore 
    cont = 0;
    
    // creo la lista 
    createNodeList(&lista);
    
    // ciclo sui nodi
    while(true)
    {	
	// controllo su quante volte l'ho visitato 
	for(it=lista.begin(); it!=lista.end(); ++it)
	    if(howMany[it->getConnectedId(0)]<3)
	      break;
	  
	// controllo se la qualità è già abbastanza alta 
	if((it==lista.end()) || (minQual<it->getGeoSize()))		break;
	
	// prendo la qualità
	oldQual = it->getGeoSize();
	
	// prendo l'id 
	nodeId = it->getConnectedId(0);
	
	// aggiorno howMany
	howMany[nodeId] = howMany[nodeId]+1;
	
	// elimino lui e tutti i nodi adiacenti a lui 
	createStellata(nodeId, &ids);
	
	// metto anche il nodo in esame
	ids.push_back(nodeId);
	
	// elimino dalla lista
	removeNodeList(&lista, &ids);
	
	// calcolo il nuovo punto 
	newPos  = newSmoothingPoint(nodeId);
	      
	// controllo se è ok e cambio le coordinate
	if(controlPosition(nodeId, newPos) && (qualityOnNode(nodeId, newPos)>oldQual))
	{	
	    // cambio le coordinate
	    changeNode(nodeId, newPos);
	    
	    // aggiorno cont
	    ++cont;
	    
	    // aggiungo alla lista
	    addNodeList(&lista, &ids);
	}
	else
	{    
	    // elimino dalla lista l'id
	    sort(ids.begin(), ids.end());
	    ids.erase(find(ids.begin(), ids.end(), nodeId));
	    
	    // aggiungo alla lista
	    addNodeList(&lista, &ids);	    
	}
    }
    
    cout << " nodi cambiati " << cont << endl;
}

void isotropicQuality2d<Triangle>::collapsingGreedy(Real lung, Real minQual)
{
    // Variabili temporanee
    bool 				   founded;
    UInt                       		      cont;
    pair<bool, vector<UInt> >		    result;
    vector<UInt>	   	        ids,onEdge;
    set<geoElementSize<Triangle> >           lista;
    set<geoElementSize<Triangle> >::iterator    it;
    point   	      		            newPos;
    
    // stampa
    cout << "Greedy Collapsing Edge process: ";
    
    // setto il contatore 
    cont = 0;
    
    // creo la lista 
    createElementList(&lista);
    
    // ciclo sui nodi
    while(true)
    {	  
	// controllo se la qualità è già abbastanza alta 
	if(minQual<lista.begin()->getGeoSize())		break;
	
	// setto founded
	founded = false;
	
	// ciclo sugli elementi
	for(set<geoElementSize<Triangle> >::iterator it2=lista.begin(); it2!=lista.end(); ++it2)
	{
	    // cerco il lato 
	    if((!isTriangleDegenerate(it2->getId())))
	    {
		// cerco il lato da collassare 
		result = findEdgeToColl(it2->getId(), lung);
		
		// lo trovo 
		if((result.first) && (it2->getGeoSize()<minQual))
		{
		    founded = true;
		    it = it2;
		}
	    }
	    	    
	    // se lo trovo mi fermo 
	    if(founded) break;
	}
	
	// se non ho trovato nessun elemento che soddisfa le condizioni mi fermo 
	if(!founded)	break;            
	
	// prendo gli id 
	createStellataEdge(&result.second, &ids);
	
	// salvo gli elementi sull'edge
	elementOnEdge(result.second[0], result.second[1], &onEdge);
	
	// li elimino dalla lista
	removeElementList(&lista, &ids);

	// trovo l'edge
	newPos = newCollapsingPoint(&result.second);
	
	// lo metto nella lista 
	meshPointer->insertNode(newPos);
		  
	// collasso tenendo buono l'id del secondo 
	collEdge(&result.second);
	
	// aggiorno la lista 
	sort(ids.begin(), ids.end());
	for(UInt i=0; i<onEdge.size(); ++i)	ids.erase(find(ids.begin(), ids.end(), onEdge[i]));
	
	// li elimino dalla lista
	addElementList(&lista, &ids);
		  
	// conto 
	++cont;	
	
    }
    
    cout << " edge collassate " << cont << endl;
    cout << "Eseguo un refresh delle variabili" << endl;
    refresh();
}

void isotropicQuality2d<Triangle>::swappingGreedy(Real limite, Real minQual)
{
    // Variabili temporanee
    bool 				   founded;
    UInt                       		      cont;
    pair<bool, vector<UInt> >		    result;
    vector<UInt>	   	            onEdge;
    set<geoElementSize<Triangle> >           lista;
    set<geoElementSize<Triangle> >::iterator    it;
    
    // stampa
    cout << "Greedy Swapping Edge process: ";
    
    // setto il contatore 
    cont = 0;
    
    // creo la lista 
    createElementList(&lista);
    
    // ciclo sui nodi
    while(true)
    {	  
	// controllo se la qualità è già abbastanza alta 
	if(minQual<lista.begin()->getGeoSize())		break;
	
	// setto founded
	founded = false;
	
	// ciclo sugli elementi
	for(set<geoElementSize<Triangle> >::iterator it2=lista.begin(); it2!=lista.end(); ++it2)
	{
	    // cerco il lato 
	    if((!isTriangleDegenerate(it2->getId())))
	    {
		// cerco il lato da collassare 
		result = findEdgeToSwap(it2->getId(), limite);
		
		// lo trovo 
		if((result.first) && (it2->getGeoSize()<minQual))
		{
		    founded = true;
		    it = it2;
		}
	    }
	    	    
	    // se lo trovo mi fermo 
	    if(founded) break;
	}
	
	// se non ho trovato nessun elemento che soddisfa le condizioni mi fermo 
	if(!founded)	break;            
	
	// salvo gli elementi sull'edge
	elementOnEdge(result.second[0], result.second[1], &onEdge);
	
	// li elimino dalla lista
	removeElementList(&lista, &onEdge);
		  
	// collasso tenendo buono l'id del secondo 
	swap(&result.second);
	
	// li elimino dalla lista
	addElementList(&lista, &onEdge);
		  
	// conto 
	++cont;	
	
    }
    
    cout << " edge swappate " << cont << endl;
}

void isotropicQuality2d<Triangle>::standardImproveGreedy(Real lungMax, Real lungMin, UInt iter, Real qualMin, Real angolo)
{
    // variabili in uso
    time_t  start,end;
    Real          dif;
    set<Real>    lung;
    vector<UInt> edge;
    
    // output
    cout << "Processo di miglioramento isotropo GREEDY..." << endl;
    time(&start);
    
    for(UInt passi=0; passi<iter; ++passi)
    {
	// prendo la lunghezza più piccola
	for(UInt i=0; i<meshPointer->getNumElements(); ++i)		lung.insert(getMinEdge(i, &edge));	
   
	// elimino gli edge più corti
	collapsing((*lung.begin())*lungMin+(*lung.rbegin())*lungMax);	
	
	// faccio lo swap 
    	swappingGreedy(angolo);
	          
	// faccio lo smoothing
	smoothingGreedy(qualMin);
	
    }
    
    time(&end);
    dif = difftime(end,start);
    cout << "Processo di miglioramento isotropo GREEDY completato: " <<  dif << " sec." << endl;
}

//
// Metodi che stampano la griglia
//
void isotropicQuality2d<Triangle>::printNodeQuality(string s)
{
    // varaibili in uso
    createFile 		file;
    vector<Real>	qual;
    
    // faccio un resize
    qual.resize(meshPointer->getNumNodes());
    
    // ciclo sui nodi
    for(UInt i=0; i<meshPointer->getNumNodes(); ++i)	qual[i] = qualityOnNode(i);
    
    // faccio la stampa
    file.fileForParaviewNodePropriety(s, meshPointer, &qual);
}

void isotropicQuality2d<Triangle>::printElementQuality(string s)
{
    // varaibili in uso
    createFile 		file;
    vector<Real>	qual;
    
    // faccio un resize
    qual.resize(meshPointer->getNumElements());
    
    // ciclo sui nodi
    for(UInt i=0; i<meshPointer->getNumElements(); ++i)	qual[i] = triangleQual(i);
    
    // faccio la stampa
    file.fileForParaviewElementPropriety(s, meshPointer, &qual);
}

void isotropicQuality2d<Triangle>::printNodeDegree(string s)
{
    // varaibili in uso
    createFile 		file;
    vector<Real>	qual;
    
    // faccio un resize
    qual.resize(meshPointer->getNumNodes());
    
    // ciclo sui nodi
    for(UInt i=0; i<meshPointer->getNumNodes(); ++i)	qual[i] = getNumElementAround(i);
    
    // faccio la stampa
    file.fileForParaviewNodePropriety(s, meshPointer, &qual);
}



