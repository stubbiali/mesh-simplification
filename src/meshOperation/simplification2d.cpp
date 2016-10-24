#include "simplification2d.h"

using namespace std;
using namespace geometry;

//
// Costruttore 
//
simplification2d<Triangle>::simplification2d() : doctor2d<Triangle>()
{
}

simplification2d<Triangle>::simplification2d(mesh2d<Triangle> * _meshPointer) : doctor2d<Triangle>(_meshPointer)
{  
    // creo il vettore Q
    setUpQ();
}

void simplification2d<Triangle>::setMeshPointer(mesh2d<Triangle> * _meshPointer)
{
    // chiamo il setMeshPointer del dottore
    doctor2d<Triangle>::setMeshPointer(_meshPointer);
    
    // creo il vettore Q
    setUpQ();
}

void simplification2d<Triangle>::refresh()
{
      
      // variabili in uso
      graphItem 		    tmpGraph;
      vector<point>                    tmpPt;
      vector<geoElement<Triangle> >    tmpTr;
      point                                p;
      geoElement<Triangle>              tria;
      vector<bool>    		      active;
      vector<UInt>       	       newId;
      UInt              tmp,cont,id1,id2,id3;
      Real                             x,y,z;
      vector<geoElement<Line> >  lineListTmp;
      vector<graphItem>		tmpTrackList;
      
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


//
// Processo che crea la lista e i suoi elementi  
//
vector<Real> simplification2d<Triangle>::createK_p(UInt nodeId, UInt elemId)
{
    assert(nodeId<meshPointer->getNumNodes());
    assert(elemId<meshPointer->getNumElements());
    
    // varabile in uso 
    Real 			   noto;
    point			 normal;
    vector<Real>		    K_p;
    
    //	prendo la normale 
    normal = getTriangleNormal(elemId);
    
    // setto il termine noto 
    noto = (-1.0)*(meshPointer->getNode(nodeId)*normal);
    
    // creo la matrice 
    K_p.assign(16,0.0);
    
    // faccio gli elementi 
    K_p[0]  = normal.getX()*normal.getX();	
    K_p[1]  = normal.getX()*normal.getY();	
    K_p[2]  = normal.getX()*normal.getZ();
    K_p[3]  = normal.getX()*noto;
    
    K_p[4]  = normal.getY()*normal.getX();	
    K_p[5]  = normal.getY()*normal.getY();	
    K_p[6]  = normal.getY()*normal.getZ();
    K_p[7]  = normal.getY()*noto;
    
    K_p[8]  = normal.getZ()*normal.getX();	
    K_p[9]  = normal.getZ()*normal.getY();	
    K_p[10] = normal.getZ()*normal.getZ();
    K_p[11] = normal.getZ()*noto;
    
    K_p[12] = noto*normal.getX();	
    K_p[13] = noto*normal.getY();	
    K_p[14] = noto*normal.getZ();
    K_p[15] = noto*noto;
    
    // ritorno la matrice 
    return(K_p);
}

vector<Real> simplification2d<Triangle>::createQ(UInt nodeId)
{
    assert(nodeId<meshPointer->getNumNodes());
    
    // varaibili in uso
    vector<Real>	    QTmp,K_p;
    
    // faccio un resize
    QTmp.assign(16,0.0);
    
    // prendo tutti i connessi 
    for(UInt i=0; i<conn.getNodeToElementPointer(nodeId)->getNumConnected(); ++i)
    {
	  // prendo la matrice 
	  K_p = createK_p(nodeId, conn.getNodeToElementPointer(nodeId)->getConnectedId(i));
	  
	  // aggiorno la matrice Q
	  for(UInt k=0; k<16; ++k)	QTmp[k] = QTmp[k]+K_p[k];
    }
    
    // ritono la matrice 
    return(QTmp);
}

void simplification2d<Triangle>::setUpQ()
{
    // faccio un resize 
    Q.clear();
    Q.resize(meshPointer->getNumNodes());
    
    // ciclo sui nodi per creare le Q
    for(UInt i=0; i<meshPointer->getNumNodes(); ++i)	Q[i] = createQ(i);
}

//
// Metodo per trovare i punti 
//
vector<Real> simplification2d<Triangle>::createQ(vector<UInt> * edge)
{
    // variabili in uso 
    vector<Real> QEdge;
    
    // faccio il resize
    QEdge.assign(16,0.0);
    
    // faccio la somma 
    for(UInt i=0; i<16; ++i)	QEdge[i] = Q[edge->at(0)][i]+Q[edge->at(1)][i];
      
    return(QEdge);
}

void simplification2d<Triangle>::createPointList(vector<UInt> * edge, vector<point> * newNodes)
{
    // varaibili in uso 
    point     						pMid,pOpt;
    point p1		 =	meshPointer->getNode(edge->at(0));
    point p2 		 =	meshPointer->getNode(edge->at(1));
    UInt bound1 = meshPointer->getNode(edge->at(0)).getBoundary();
    UInt bound2 = meshPointer->getNode(edge->at(1)).getBoundary();
    vector<UInt>				  geoIds1,geoIds2;
    vector<point>				       newNodeTmp;
    
    // faccio un reserve 
    newNodeTmp.clear();
    newNodeTmp.reserve(5);
    
    switch(bound1)
    {
      case(0):
	      switch(bound2)
	      {	
		// tutti e due interni
		case(0):
			// calcolo il punto medio 
			pMid.replace(p1,p2,0.5);
			pMid.setBoundary(0);
						
			// metto gli altri 
			newNodeTmp.push_back(pMid);
			newNodeTmp.push_back(p1);
			newNodeTmp.push_back(p2);
			
			break;
		// p2 è sul bordo
		case(1):
			newNodeTmp.push_back(p2);
			break;
	      }
	      break;
      case(1):
	      switch(bound2)
	      {	
		// p1 è sul bordo
		case(0):
			newNodeTmp.push_back(p1);
			break;
		// p1 e p2 sono sul bordo ma si deve controllare se sono effettivamente un edge
		case(1):
			if(isBoundary(edge))
			{
			    // controllo i geoId che hanno attorno perché se no riscio di non conservare gli spigoli
			    geoIdAround(edge->at(0), &geoIds1);
			    geoIdAround(edge->at(1), &geoIds2);
			    
			    // modo per conservare gli spigoli 
			    if((geoIds1.size()>2)       && (geoIds2.size()==2))	newNodeTmp.push_back(p1);
			    else if((geoIds1.size()==2) && (geoIds2.size()>2))	newNodeTmp.push_back(p2);
			    else
			    {
			      // calcolo il punto medio 
			      pMid.replace(p1,p2,0.5);
			      pMid.setBoundary(1);
						
			      // metto gli altri 
			      newNodeTmp.push_back(pMid);
			      newNodeTmp.push_back(p1);
			      newNodeTmp.push_back(p2);
			    }
			}
			break;
	      }
	      break;
    }
    
    // faccio un reserve e metto solo i nodi che sono ok per evitare problemi di inversione 
    newNodes->clear();
    newNodes->reserve(5);
    
    // controllo tutta la lista e metto solo i nodi che non mi danno il problema di inversione 
    for(UInt i=0; i<newNodeTmp.size(); ++i)	
      if(controlColl(edge, newNodeTmp[i]))
	newNodes->push_back(newNodeTmp[i]);
}

Real simplification2d<Triangle>::getEdgeCost(vector<Real> * Q, point pNew)
{     
      // variabili in uso 
      vector<Real>	          v1,vTmp;
      
      // faccio un resize di v1
      v1.assign(4,1.0);
      for(UInt i=0; i<3; ++i)	v1[i] = pNew.getI(i);
     
      // faccio un resize e la moltiplicazione
      vTmp.assign(4, 0.0);
      
      // riempio il vettore
      vTmp[0] = Q->at(0)*v1[0] + Q->at(4)*v1[1] + Q->at(8)*v1[2]  + Q->at(12)*v1[3];
      vTmp[1] = Q->at(1)*v1[0] + Q->at(5)*v1[1] + Q->at(9)*v1[2]  + Q->at(13)*v1[3];  
      vTmp[2] = Q->at(2)*v1[0] + Q->at(6)*v1[1] + Q->at(10)*v1[2] + Q->at(14)*v1[3];  
      vTmp[3] = Q->at(3)*v1[0] + Q->at(7)*v1[1] + Q->at(11)*v1[2] + Q->at(15)*v1[3];  
            
      // ritorno il valore 
      return(inner_product(v1.begin(), v1.end(), vTmp.begin(), 0.0));
}

pair<point, vector<Real> > simplification2d<Triangle>::getEdgeCost(vector<UInt> * edge)
{
      // varaibili in uso 
      Real 	  	             costo,costoTmp;
      point 	  			       pNew;
      vector<point>			   newNodes;
      vector<Real>				  Q;
      pair<point, vector<Real> >	     result;
      
      // creo la matrice dell'edge 
      Q = createQ(edge);
      
      // la salvo in result
      result.second = Q;
      
      // creo la lista dei possibili nodi 
      createPointList(edge, &newNodes);
      
      // se non ho trovato punti ritorno il punto null
      if(newNodes.size()==0)
      {
	  result.first = pNull;
	  return(result);
      }
      
      // metto il primo costo 
      costo = getEdgeCost(&Q, newNodes[0]);
      result.first = newNodes[0];
      
      // controllo gli alri 
      for(UInt i=1; i<newNodes.size(); ++i)
      {
	  // calcolo il costo 
	  costoTmp = getEdgeCost(&Q, newNodes[i]);
	  
	  // se il costo è inferiore salvo
	  if(costoTmp<costo)	
	  {
	      costo = costoTmp;
	      result.first = newNodes[i];
	  }
      }
      
      // ritorno 
      return(result);
}

pair<point, vector<Real> > simplification2d<Triangle>::getMinEdgeCost(UInt elemId, vector<UInt> * edge)
{
    // controllo 
    assert(elemId<meshPointer->getNumElements());
    
    // variabili in uso
    Real 		     costo=9.9e99,costoTmp=0.0;
    vector<UInt>	 		     tmpLin(2);
    pair<point, vector<Real> >			result;
    pair<point, vector<Real> >		     resultTmp;
    
    // setto result
    result.first = pNull;
    edge->clear();
    edge->resize(2);
 
    // faccio un ciclo sugli edge 
    for(UInt i=0; i<3; ++i)
    {
	  // setto la linea
	  tmpLin[0] = meshPointer->getElement(elemId).getConnectedId((i%3));
	  tmpLin[1] = meshPointer->getElement(elemId).getConnectedId(((i+1)%3));
	  
	  // prendo la coppia 
	  resultTmp = getEdgeCost(&tmpLin);
	  
	  // se trova il punto 
	  if(resultTmp.first!=pNull)
	  {
	      // calcolo il costo 
 	      costoTmp = getEdgeCost(&resultTmp.second, resultTmp.first);
	      
	      // se il costo è inferiore allora sostituisco 
	      if(costoTmp<costo)
	      {
		  // sostituisco 
		  costo = costoTmp;
		  
		  // creo l'output
		  result = resultTmp;
		  edge->at(0) = tmpLin[0];
		  edge->at(1) = tmpLin[1];
		  
	      }
	      
	  }
    }
    // ritorno il valore 
    return(result);
}

//
// Metodo per creare le liste
//
void simplification2d<Triangle>::createElementList(sortList<geoElementSize<Triangle> > * lista)
{
      // variabili in uso 
      Real 				 costo = 0.0;
      vector<UInt>				edge;
      geoElementSize<Triangle> 			elem;
      pair<point, vector<Real> >	      result;
      set<geoElementSize<Triangle> >	    listaTmp;
      
      // creo la lista 
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	  // metto gli id dei connessi
	  for(UInt j=0; j<3; ++j)  elem.setConnectedId(j, meshPointer->getElement(i).getConnectedId(j));
	  
	  // metto gli id dell'elemento 
	  elem.setId(meshPointer->getElement(i).getId());
	  
	  // setto il suo costo 
	  result = getMinEdgeCost(i, &edge);
	  
	  // controllo che tutto sia ok
	  if((result.first-pNull).norm2()>1e-10)	
	  {
		 // prendo il costo 
		 costo = getEdgeCost(&result.second, result.first);
		 
		 // prendo il costo 
		 elem.setGeoSize(costo);
		 
		 // lo metto nella lista
		 listaTmp.insert(elem);
	  }	  
      }
      
      // metto nella lista
      lista->setElementVector(&listaTmp);
}

void simplification2d<Triangle>::deleteElementList(sortList<geoElementSize<Triangle> > * lista, vector<UInt> * edge, 
						   vector<UInt> * toUpDate)
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
      if(lista->isIn(onEdge[i]))
	lista->remove(onEdge[i]);
    
    // elimino quelli della lista che stanno sull'edge e che verranno quindi collassati 
    sort(vicini.begin(), vicini.end());
    for(UInt i=0; i<onEdge.size(); ++i)		vicini.erase(find(vicini.begin(), vicini.end(), onEdge[i]));
    
    // copio 
    toUpDate->clear();
    toUpDate->resize(vicini.size());
    copy(vicini.begin(), vicini.end(), toUpDate->begin());
}

void simplification2d<Triangle>::upDate(sortList<geoElementSize<Triangle> > * listaBase, vector<UInt> * vicini)
{
    // variabili in gioco 
    Real 			       costo = 0.0;
    vector<UInt>   	                   edgeTmp;
    geoElementSize<Triangle> 		      elem;
    pair<point, vector<Real> >		    result;
        
    // cambio gli eleemtnidi della lista 
    for(UInt i=0; i<vicini->size(); ++i)
    {	  
	  // metto gli id dei connessi
	  for(UInt j=0; j<3; ++j)  elem.setConnectedId(j, meshPointer->getElement(vicini->at(i)).getConnectedId(j));
	  
	  // metto gli id dell'elemento 
	  elem.setId(vicini->at(i));
	  
	  // setto il suo costo 
	  result = getMinEdgeCost(vicini->at(i), &edgeTmp);
	  
	  // controllo che tutto sia ok
	  if((result.first-pNull).norm2()>1e-10)	
	  {
		 // prendo il costo 
		 costo = getEdgeCost(&result.second, result.first);
		 
		 // se è presente lo cambio 
		 if(listaBase->isIn(vicini->at(i)))
		 {
		      // prendo il costo 
		      listaBase->change(vicini->at(i), costo);
		 }
		 else
		 {
		      // metto il costo 
		      elem.setGeoSize(costo);
		 
		      // prendo il costo 
		      listaBase->add(&elem);
		 }
	  }
	  // se ha null ed è presente lo elimino dalla lista 
	  else if(listaBase->isIn(vicini->at(i)))
	  {
		 listaBase->remove(vicini->at(i));
	  }
    }
}

UInt simplification2d<Triangle>::getElementToSimplificate(sortList<geoElementSize<Triangle> > * listaBase)
{
      return(listaBase->findMin());
}

//
// Metodo che controlla 
//
bool simplification2d<Triangle>::control(vector<UInt> * edge, point pNew)
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
	    inv  = ((nPrima*nDopo)>(sqrt(3)/2));
	    
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

//
// Metodo che fa la semplificazione 
//
void simplification2d<Triangle>::simplificate(UInt numNodesMax)
{
      // variabili in uso
      UInt	numNode=meshPointer->getNumNodes();
      UInt numNodeStart=meshPointer->getNumNodes();
      time_t 				 start,end;
      Real          			       dif;
      vector<UInt>			      edge;
      pair<point, vector<Real> >	    result;
      
      // controllo che i punti non siano già sotto 
      if(numNodesMax>=meshPointer->getNumNodes())
      {
	  cout << "I punti della mesh sono " << meshPointer->getNumNodes();
	  cout << " e sono già sotto la soglia " << numNodesMax << endl;
	  return;
      }
      
      // stampe 
      cout << "Processo di semplificazione..." << endl;
      time(&start);
         
      // fino a che i nodi sono più grandi di quanto voglio proseguo con la decimazione 
      while(numNode>numNodesMax)
      {
	  // ciclo sugli elementi 
	  for(UInt i=0; i<meshPointer->getNumElements(); ++i)
	  {
	      // se non è degenere 
	      if(!isTriangleDegenerate(i))
	      {
		  // cerco l'edge più corretto
		  result = getMinEdgeCost(i, &edge);
		  
		  // cotrollo se trovo il punto 
		  if(result.first!=pNull)
		  {
		      // se il punto non è degenere lo metto nella lista 
		      meshPointer->insertNode(result.first);
		  
		      // collasso tenendo buono l'id del secondo 
		      collEdge(&edge);
		  
		      // metto Q
		      Q.push_back(result.second);
		  
		      // diminuisco i punti 
		      --numNode;
		      
		  }
	      }
	      
	      // se è più piccolo mi fermo 
	      if(numNode<numNodesMax)	break;
	  }
	  
	  // se è più piccolo mi fermo 
	  if(numNode<numNodesMax)	break;
      }
      
      // faccio un refresh 
      refresh();
      
      time(&end);
      dif = difftime(end,start);
      cout << "Il processo è partito da " << numNodeStart << " a " << meshPointer->getNumNodes() << " nodi" << endl;
      cout << "Processo di semplificazione completato: " <<  dif << " sec." << endl;
      
      // metto a posto le Q
      setUpQ();
}

void simplification2d<Triangle>::simplificateGreedy(UInt numNodesMax)
{
      // variabili in uso
      UInt 	 	        elemId=0,counter=0;
      UInt	numNode=meshPointer->getNumNodes();
      UInt numNodeStart=meshPointer->getNumNodes();
      time_t 				 start,end;
      Real          			       dif;
      point 					 p;
      vector<UInt>			edge,toAdd;
      pair<point, vector<Real> > 	    result;
      sortList<geoElementSize<Triangle> >    lista;
      
      // controllo che i punti non siano già sotto 
      if(numNodesMax>=meshPointer->getNumNodes())
      {
	  cout << "I punti della mesh sono " << meshPointer->getNumNodes();
	  cout << " e sono già sotto la soglia " << numNodesMax << endl;
	  return;
      }
      
      // stampe 
      cout << "Processo di semplificazione Greedy..." << endl;
      time(&start);
      
      // creo la lista 
      createElementList(&lista);
      
      // fino a che i nodi sono più grandi di quanto voglio proseguo con la decimazione 
      while(numNode>numNodesMax && counter<numNodeStart)
      {		
	  // prendo l'id
	  elemId = getElementToSimplificate(&lista);
	  
	  // prendo l'informazione 
	  result = getMinEdgeCost(elemId, &edge);
	  
	  if(control(&edge, result.first))
	  {
		// metto a posto la lista 
		deleteElementList(&lista, &edge, &toAdd);
		
		// inserisco il nodo 
		meshPointer->insertNode(result.first);
		  
		// collasso tenendo buono l'id del secondo 
		collEdge(&edge);
		  
		// metto Q 
		Q.push_back(result.second);
	  
		// diminuisco i punto 
		--numNode;
	  
		// aggiungo gli elementi modificati 
		upDate(&lista, &toAdd);
	  }
	  else
	  {
		// metto a posto la lista 
		deleteElementList(&lista, &edge, &toAdd);
		
	  }
	  
	  // incremento counter 
	  ++counter;
	    
 	  cout << numNode << " nodi su un limite massimo di " << numNodesMax << "                            \r";
      }
      
      // faccio un refresh 
      refresh();
      
      time(&end);
      dif = difftime(end,start);
      cout << "Il processo è partito da " << numNodeStart << " a " << meshPointer->getNumNodes() << " nodi" << endl;
      cout << "Processo di semplificazione Greedy completato: " <<  dif << " sec." << endl;
}

void simplification2d<Triangle>::simplificateGreedyEasy(UInt numNodesMax)
{
      // variabili in uso
      UInt 	 	        elemId=0,counter=0;
      UInt	numNode=meshPointer->getNumNodes();
      UInt numNodeStart=meshPointer->getNumNodes();
      time_t 				 start,end;
      Real          			       dif;
      point 					 p;
      vector<UInt>			edge,toAdd;
      pair<point, vector<Real> > 	    result;
      sortList<geoElementSize<Triangle> >    lista;
      
      // controllo che i punti non siano già sotto 
      if(numNodesMax>=meshPointer->getNumNodes())
      {
	  cout << "I punti della mesh sono " << meshPointer->getNumNodes();
	  cout << " e sono già sotto la soglia " << numNodesMax << endl;
	  return;
      }
      
      // stampe 
      cout << "Processo di semplificazione Greedy..." << endl;
      time(&start);
      
      // creo la lista 
      createElementList(&lista);
    
      // fino a che i nodi sono più grandi di quanto voglio proseguo con la decimazione 
      while(numNode>numNodesMax && counter<numNodeStart)
      {		
	  // prendo l'id
	  elemId = getElementToSimplificate(&lista);
	  
	  // prendo l'informazione 
	  result = getMinEdgeCost(elemId, &edge);
	  
	  // metto a posto la lista 
	  deleteElementList(&lista, &edge, &toAdd);
		
	  // inserisco il nodo 
	  meshPointer->insertNode(result.first);
		  
	  // collasso tenendo buono l'id del secondo 
	  collEdge(&edge);
		  
	  // metto Q 
	  Q.push_back(result.second);
	  
	  // diminuisco i punto 
	  --numNode;
	  
	  // aggiungo gli elementi modificati 
	  upDate(&lista, &toAdd);
	  
	  // incremento counter 
	  ++counter;
	    
 	  cout << numNode << " nodi su un limite massimo di " << numNodesMax << "                            \r";
      }
      
      // faccio un refresh 
      refresh();
      
      time(&end);
      dif = difftime(end,start);
      cout << "Il processo è partito da " << numNodeStart << " a " << meshPointer->getNumNodes() << " nodi" << endl;
      cout << "Processo di semplificazione Greedy completato: " <<  dif << " sec." << endl;
}

void simplification2d<Triangle>::simplificateStep(UInt numNodesMax, UInt step)
{
      // variabili in uso
      UInt 	 	        elemId=0,counter=0;
      UInt	numNode=meshPointer->getNumNodes();
      UInt numNodeStart=meshPointer->getNumNodes();
      time_t 				 start,end;
      Real          			       dif;
      point 					 p;
      vector<UInt>		       edge,toSimp;
      pair<point, vector<Real> > 	    result;
      sortList<geoElementSize<Triangle> >    lista;
      
      // controllo che i punti non siano già sotto 
      if(numNodesMax>=meshPointer->getNumNodes())
      {
	  cout << "I punti della mesh sono " << meshPointer->getNumNodes();
	  cout << " e sono già sotto la soglia " << numNodesMax << endl;
	  return;
      }
      
      // stampe 
      cout << "Processo di semplificazione con step di " << step << endl;
      time(&start);
      
      // fino a che i nodi sono più grandi di quanto voglio proseguo con la decimazione 
      while(numNode>numNodesMax && counter<numNodeStart)
      {		
	  // creo la lista 
	  createElementList(&lista);
      
	  // faccio un resize di quelli che voglio analizzare 
	  toSimp.resize(step);
	  for(UInt i=0; i<step; ++i)
	  {
	      // prendo il minimo 
	      toSimp[i] = lista.findMin();
	  
	      // lo rimuovo 
	      lista.remove(toSimp[i]);
	  }
	  
	  // elimino quelli messi 
	  for(UInt i=0; i<toSimp.size(); ++i)
	  {
	      // prendo l'identificatore 
	      elemId = toSimp[i];
	      
	      // controllo che non sia degenere 
	      if(!isTriangleDegenerate(elemId))
	      {
		    // prendo l'informazione 
		    result = getMinEdgeCost(elemId, &edge);
	  
		    if(control(&edge, result.first))
		    {
		
			// inserisco il nodo 
			meshPointer->insertNode(result.first);
			
			// metto Q
			Q.push_back(result.second);
		  
			// collasso tenendo buono l'id del secondo 
			collEdge(&edge);
	  
			// diminuisco i punto 
			--numNode;
			
			// incremento counter 
			++counter;
	    
			cout << numNode << " nodi su un limite massimo di " << numNodesMax << "                 \r";
		    }
	       }
		
	       // se li ho collassati tutti mi fermo 
	       if(numNode<=numNodesMax)	break;
	    }
	    
	    cout << numNode << " nodi su un limite massimo di " << numNodesMax << "                 \n";
	    
	    // faccio un refresh 
	    refresh();
	    
	    // metto a posto anche i Q
	    setUpQ();
      }
      
      time(&end);
      dif = difftime(end,start);
      cout << "Il processo è partito da " << numNodeStart << " a " << meshPointer->getNumNodes() << " nodi" << endl;
      cout << "Processo di semplificazione con Step completato: " <<  dif << " sec." << endl;
}


//
// Metodi che stampano 
//
void simplification2d<Triangle>::print(string s)
{
    // variabili  in uso 
    vector<Real>			     val;
    vector<UInt>			    edge;
    pair<point, vector<Real> >		  result; 
    createFile				    file;
    
    // faccio un resize 
    val.resize(meshPointer->getNumElements());
    
    // ciclo sugli elementi 
    for(UInt i=0; i<meshPointer->getNumElements(); ++i)
    {
	  // prendo il punto che devo collassare 
	  result = getMinEdgeCost(i, &edge);
	  
	  // salvo il costo 
	  if(result.first!=pNull)	  val[i] = getEdgeCost(&result.second, result.first);
	  else				  val[i] = -1.0;
    }
    
    // stampi il file 
    file.fileForParaviewElementPropriety(s, meshPointer, &val);
}
