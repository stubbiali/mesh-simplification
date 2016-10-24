#include "tricky2d.h"

using namespace      std;
using namespace geometry;

//
// Costruttore 
//

tricky2d<Triangle>::tricky2d()
{    
    // definizione dei puntatori e toll
    meshPointer    = NULL;
    toll           = 1e-15;
    pNull.setX(9.9e99);
    pNull.setY(9.9e99);
    pNull.setZ(9.9e99);
}
		      
tricky2d<Triangle>::tricky2d(mesh2d<Triangle> * _meshPointer)
{    
    // varaibili temporanee
    UInt 		   id1,id2;
    mesh1d<Line>               bor;
    map<UInt, UInt>	 borToSurf;
    
    // definizione dei puntatori e toll
    meshPointer    = _meshPointer;
    toll           = 1e-15;
    
    // setto il punto nullo 
    pNull.setX(9.9e99);
    pNull.setY(9.9e99);
    pNull.setZ(9.9e99);
    
    // faccio un setUp della mesh in input
    meshPointer->setUpIds();
    
    // preparo connect
    conn.setMeshPointer(meshPointer);
    conn.buildNodeToElement(false);
    
    // faccio le connettività di bordo 
    conn.buildBoundaryConnectivity(&bor, &borToSurf, true, false);
    
    // inserisco le informazioni dei nodi sul bordo 
    for(UInt i=0; i<bor.getNumNodes(); ++i) meshPointer->getNodePointer(borToSurf[i])->setBoundary(1);
    
    // faccio un clear
    bordo.clear();
    
    // faccio un set
    for(UInt i=0; i<bor.getNumElements(); ++i)
    {
	// prendo gli id 
	id1 = bor.getElement(i).getConnectedId(0);
	id2 = bor.getElement(i).getConnectedId(1);
	
	// cambio gli id e li metto nel set 
	bor.getElementPointer(i)->setConnectedId(0, borToSurf[id1]);
	bor.getElementPointer(i)->setConnectedId(1, borToSurf[id2]);
	
	// lo metto nel set 
	bordo.insert(bor.getElement(i));
    }
}

//
// Set delle variabili di ambiente
//

void tricky2d<Triangle>::setMeshPointer(mesh2d<Triangle> * _meshPointer)
{
    // varaibili temporanee
    UInt 		   id1,id2;
    mesh1d<Line>               bor;
    map<UInt, UInt>	 borToSurf;
   
    // definizione dei puntatori e toll
    meshPointer    = _meshPointer;
    
    // faccio un setUp della mesh in input
    meshPointer->setUpIds();
     
    // preparo connect
    conn.setMeshPointer(meshPointer);
    conn.buildNodeToElement(false);
    
    // faccio le connettività di bordo 
    conn.buildBoundaryConnectivity(&bor, &borToSurf, true, false);
    
    // inserisco le informazioni dei nodi sul bordo 
    for(UInt i=0; i<bor.getNumNodes(); ++i) meshPointer->getNodePointer(borToSurf[i])->setBoundary(1);
    
    // faccio un clear
    bordo.clear();
    
    // faccio un set
    for(UInt i=0; i<bor.getNumElements(); ++i)
    {
	// prendo gli id 
	id1 = bor.getElement(i).getConnectedId(0);
	id2 = bor.getElement(i).getConnectedId(1);
	
	// cambio gli id e li metto nel set 
	bor.getElementPointer(i)->setConnectedId(0, borToSurf[id1]);
	bor.getElementPointer(i)->setConnectedId(1, borToSurf[id2]);
	
	// lo metto nel set 
	bordo.insert(bor.getElement(i));
    }
}

void tricky2d<Triangle>::setToll(Real _toll)
{
      toll = _toll;
}

Real tricky2d<Triangle>::getToll()
{
      return(toll);
}

void tricky2d<Triangle>::setUp()
{
    // faccio un setUp della mesh in input
    meshPointer->setUpIds();
    
    // varaibili temporanee
    UInt 		   id1,id2;
    mesh1d<Line>               bor;
    map<UInt, UInt>	 borToSurf;
  
    // preparo connect
    conn.setMeshPointer(meshPointer);
    conn.buildNodeToElement();
    
    // faccio le connettività di bordo 
    conn.buildBoundaryConnectivity(&bor, &borToSurf, true);
    
    // inserisco le informazioni dei nodi sul bordo 
    for(UInt i=0; i<bor.getNumNodes(); ++i) meshPointer->getNodePointer(borToSurf[i])->setBoundary(1);
    
    // faccio un clear
    bordo.clear();
    
    // faccio un set
    for(UInt i=0; i<bor.getNumElements(); ++i)
    {
	// prendo gli id 
	id1 = bor.getElement(i).getConnectedId(0);
	id2 = bor.getElement(i).getConnectedId(1);
	
	// cambio gli id e li metto nel set 
	bor.getElementPointer(i)->setConnectedId(0, borToSurf[id1]);
	bor.getElementPointer(i)->setConnectedId(1, borToSurf[id2]);
	
	// lo metto nel set 
	bordo.insert(bor.getElement(i));
    }
}

void tricky2d<Triangle>::upDateBordo(vector<geoElement<Line> > * _bordo)
{
    // pulisco il bordo 
    bordo.clear();
    
    // metto il bordo 
    for(UInt i=0; i<_bordo->size(); ++i)	bordo.insert(_bordo->at(i));
}

void tricky2d<Triangle>::getBordo(vector<geoElement<Line> > * bordoVect)
{
    // faccio in clear 
    bordoVect->clear();
    bordoVect->reserve(bordo.size());
    
    // salvo 
    for(set<geoElement<Line> >::iterator it=bordo.begin(); it!=bordo.end(); ++it) 	bordoVect->push_back(*it);
}

void tricky2d<Triangle>::clear()
{
      // pulisco 
      conn.clear();
      bordo.clear();
      meshPointer = NULL;
}


//
// Metodi per esplorare la mesh
//
bool tricky2d<Triangle>::isNodeDegenerate(UInt nodeId)
{
      assert(nodeId<meshPointer->getNumNodes());
      return(!(meshPointer->getNode(nodeId)!=pNull));
}

void tricky2d<Triangle>::setNodeDegenerate(UInt nodeId)
{
      assert(nodeId<meshPointer->getNumNodes());
      meshPointer->getNodePointer(nodeId)->setX(pNull.getX());
      meshPointer->getNodePointer(nodeId)->setY(pNull.getY());
      meshPointer->getNodePointer(nodeId)->setZ(pNull.getZ());
      conn.getNodeToElementPointer(nodeId)->clear();
}

UInt tricky2d<Triangle>::nextNode(UInt id1, UInt elemId)
{
      assert(id1<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
        
      // serie di if 
      if(meshPointer->getElement(elemId).getConnectedId(0)==id1)	
	 return(meshPointer->getElement(elemId).getConnectedId(1));
      else if(meshPointer->getElement(elemId).getConnectedId(1)==id1)
	return(meshPointer->getElement(elemId).getConnectedId(2));
      else if(meshPointer->getElement(elemId).getConnectedId(2)==id1)
	return(meshPointer->getElement(elemId).getConnectedId(0));
      
      // assert di controllo per stabilire se è uscito con il return del ciclo
      cout << "Non ho trovato il nodo nel metodo nextNode" << endl;
      assert(1==0);
      return(0);
}

UInt tricky2d<Triangle>::precNode(UInt id1, UInt elemId)
{
      assert(id1<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // serie di if 
      if(meshPointer->getElement(elemId).getConnectedId(0)==id1)	
	 return(meshPointer->getElement(elemId).getConnectedId(2));
      else if(meshPointer->getElement(elemId).getConnectedId(1)==id1)
	return(meshPointer->getElement(elemId).getConnectedId(0));
      else if(meshPointer->getElement(elemId).getConnectedId(2)==id1)
	return(meshPointer->getElement(elemId).getConnectedId(1));
      
      // assert di controllo per stabilire se è uscito con il return del ciclo
      cout << "Non ho trovato il nodo nel metodo precNode" << endl;
      assert(1==0);
      return(0);
}

UInt tricky2d<Triangle>::lastNode(UInt id1, UInt id2, UInt elemId)
{
      assert(id1<meshPointer->getNumNodes());
      assert(id2<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // ciclo sui nodi del triangolo
      for(UInt i=0; i<3; ++i)
	    if( (meshPointer->getElement(elemId).getConnectedId(i)!=id1) && 
	        (meshPointer->getElement(elemId).getConnectedId(i)!=id2) )
		  return(meshPointer->getElement(elemId).getConnectedId(i));
      
      // assert di controllo per stabilire se è uscito con il return del ciclo
      cout << "Non ho trovato il nodo nel metodo lastNode" << endl;
      assert(1==0);
      return(0);
}

void tricky2d<Triangle>::lastNode(UInt id1, UInt elemId, vector<UInt> * ids)
{
      assert(id1<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // mi accerto che ids sia vuoto
      ids->clear();
      ids->reserve(2);
      
      // ciclo sui nodi del triangolo
      for(UInt i=0; i<3; ++i)
	    if(meshPointer->getElement(elemId).getConnectedId(i)!=id1)
		  ids->push_back(meshPointer->getElement(elemId).getConnectedId(i));
      
      // assert per accertarmi che tutto sia andato per il meglio
      assert(ids->size()==2);
}

void tricky2d<Triangle>::elementOnEdge(UInt id1, UInt id2, vector<UInt> * ele)
{
      assert(id1<meshPointer->getNumNodes());
      assert(id2<meshPointer->getNumNodes());
      
      // pulisco il vettore in input
      ele->clear();
      ele->reserve(conn.getNodeToElementPointer(id1)->getNumConnected());
      
      // varaibili che userò
      set<UInt>     connessi;
      set<UInt>::iterator it;
      
      // ciclo sugli elementi connessi a id1
      for(UInt i=0; i<conn.getNodeToElementPointer(id1)->getNumConnected(); ++i)
	  connessi.insert(conn.getNodeToElementPointer(id1)->getConnectedId(i));
      
      // ciclo sugli elementi connessi a id2 se lo trovo lo metto nella lista
      for(UInt i=0; i<conn.getNodeToElementPointer(id2)->getNumConnected(); ++i)  
	  if(connessi.find(conn.getNodeToElementPointer(id2)->getConnectedId(i))!=connessi.end())
		  ele->push_back(conn.getNodeToElementPointer(id2)->getConnectedId(i));
}

UInt tricky2d<Triangle>::numElementOnEdge(UInt id1, UInt id2)
{
      // varaibili in uso 
      vector<UInt>	ele;
      
      // prendo gli elementi sull'edge 
      elementOnEdge(id1,id2,&ele);
      
      // ritorno il size 
      return(ele.size());
}

void tricky2d<Triangle>::geoIdOnEdge(UInt id1, UInt id2, vector<UInt> * geo)
{
      assert(id1<meshPointer->getNumNodes());
      assert(id2<meshPointer->getNumNodes());
      
      // varaibili che userò
      set<UInt>		tmpGeo;
      vector<UInt>	   ele;
      
      // prendo gli elementi sull'edge 
      elementOnEdge(id1, id2, &ele);
      
      // metto i geoId
      for(UInt i=0; i<ele.size(); ++i)	tmpGeo.insert(meshPointer->getElement(ele[i]).getGeoId());
      
      // pulisco il vettore in input
      geo->clear();
      geo->resize(tmpGeo.size());
      
      // copio 
      copy(tmpGeo.begin(), tmpGeo.end(), geo->begin());
}

void tricky2d<Triangle>::getElementAround(UInt id1, vector<UInt> * ids)
{
    // inizializzo il vettore 
    ids->assign(conn.getNodeToElementPointer(id1)->getNumConnected(), 0);
    
    // riempio il vettore 
    for(UInt i=0; i<ids->size(); ++i)	ids->at(i) = conn.getNodeToElementPointer(id1)->getConnectedId(i);
}

void tricky2d<Triangle>::getElementAroundBig(UInt id1, vector<UInt> * ids)
{
    // variabili in uso 
    vector<UInt>    tmpId,tmpNode;
    set<UInt>		   result;
    
    // prendo i nodi connessi con 
    createStellata(id1, &tmpNode);
    
    // per ogni elemento prendo i suoi elementi attorno e li metto nel set 
    for(UInt i=0; i<tmpNode.size(); ++i)
    {
	  // prendo i nodi vicini 
	  getElementAround(tmpNode[i], &tmpId);
	  
	  // li metto nel set 
	  for(UInt j=0; j<tmpId.size(); ++j) 	result.insert(tmpId[j]);
    }
    
    // copio in ids
    ids->assign(result.size(), 0);
    copy(result.begin(), result.end(), ids->begin());
}

UInt tricky2d<Triangle>::getNumElementAround(UInt id1)
{
    return(conn.getNodeToElementPointer()->at(id1).getNumConnected());
}

void tricky2d<Triangle>::createStellata(UInt id1, vector<UInt> * ids)
{
      assert(id1<meshPointer->getNumNodes());
      
      // mi accerto che ids sia vuoto
      ids->clear();
      
      // variabile temporanea
      set<UInt>           tmp;
      set<UInt>::iterator  it;
      
      // ciclo sugli elementi connessi per creare la stellata
      for(UInt i=0; i<conn.getNodeToElementPointer(id1)->getNumConnected(); ++i)
	  for(UInt j=0; j<3; ++j)
	      tmp.insert(meshPointer->getElement(conn.getNodeToElementPointer(id1)->getConnectedId(i)).getConnectedId(j));
      
      // trovo il nodo di cui sto calcolando la stellata
      it = tmp.find(id1);
      
      // assert per essere sicuri
      assert(it!=tmp.end());
      
      // elimino il punto 
      tmp.erase(it);
      
      // metto a posto la lista ids
      ids->resize(tmp.size());
      
      // copio la lista
      copy(tmp.begin(),tmp.end(),ids->begin());
}

void tricky2d<Triangle>::createStellataEdge(vector<UInt> * edge, vector<UInt> * ids)
{
      // varaibile in uso 
      set<UInt>			allTmp;
      
      // controllo la dimensione di edge
      assert(edge->size()==2);
      
      // mi assicuro che sia vuoto 
      ids->clear();
      
      //prendo gli elementi 
      for(UInt j=0; j<2; ++j)
	  for(UInt i=0; i<conn.getNodeToElementPointer(edge->at(j))->getNumConnected(); ++i)	
	      allTmp.insert(conn.getNodeToElementPointer(edge->at(j))->getConnectedId(i));
	 
      // copio 
      ids->resize(allTmp.size());
      copy(allTmp.begin(), allTmp.end(), ids->begin());
      
}

void tricky2d<Triangle>::createStellataEdgeWithoutOnEdge(vector<UInt> * edge, vector<UInt> * ids)
{
    // varaibili in uso 
    vector<UInt>::iterator      it;
    vector<UInt>	tmp,onEdge;
    
    // elementi sull'edge
    elementOnEdge(edge->at(0),edge->at(1), &onEdge);
    
    // elementi della stellata 
    createStellataEdge(edge, &tmp);
    
    // faccio un sort 
    sort(tmp.begin(), tmp.end());
    sort(onEdge.begin(), onEdge.end());
    
    // faccio un resize 
    ids->assign(tmp.size(),0);
    
    // faccio la differenza 
    it = set_difference(tmp.begin(), tmp.end(), onEdge.begin(), onEdge.end(), ids->begin());
    ids->resize(it-ids->begin());
}

void tricky2d<Triangle>::createStellataEdgeNode(vector<UInt> * edge, vector<UInt> * ids)
{
      // varaibile in uso 
      UInt 		        elemId;
      set<UInt>	       		allTmp;
      set<UInt>::iterator           it;
      
      // controllo la dimensione di edge
      assert(edge->size()==2);
      
      // mi assicuro che sia vuoto 
      ids->clear();
      
      //prendo gli elementi 
      for(UInt j=0; j<2; ++j)
      {
	  for(UInt i=0; i<conn.getNodeToElementPointer(edge->at(j))->getNumConnected(); ++i)	
	  {
	    // prendo l'id 
	    elemId = conn.getNodeToElementPointer(edge->at(j))->getConnectedId(i);
	    
	    // metto i nodi 
	    for(UInt k=0; k<3; ++k)      allTmp.insert(meshPointer->getElement(elemId).getConnectedId(k));
	  }
      }
      
	  
      // elimino gli elementi dell'edge 
      for(UInt j=0; j<2; ++j)
      {
	  // lo cerco 
	  it = allTmp.find(edge->at(j));
      
	  // elimino ma prima controllo che effettivamente c'è il punto 
	  assert(it!=allTmp.end());
      
	  // lo elimino 
	  allTmp.erase(it);
      }
	 
      // copio 
      ids->resize(allTmp.size());
      copy(allTmp.begin(), allTmp.end(), ids->begin());
      
}

void tricky2d<Triangle>::createBigStellataEdge(vector<UInt> * edge, vector<UInt> * ids)
{
    // variabili in uso 
    vector<UInt>               tmp;
    set<UInt>    elemConn,nodiConn;
    
    // prendo la stellata dell'edge 
    createStellataEdge(edge, &tmp);
    
    // prendo tutti i nodi connessi 
    for(UInt i=0; i<tmp.size(); ++i)
      for(UInt j=0; j<3; ++j)
	nodiConn.insert(meshPointer->getElement(tmp[i]).getConnectedId(j));
    
    // prendo tutti gli elementi connessi ai nodi 
    for(set<UInt>::iterator it=nodiConn.begin(); it!=nodiConn.end(); ++it)
      for(UInt i=0; i<conn.getNodeToElementPointer()->at(*it).getNumConnected(); ++i)
	elemConn.insert(conn.getNodeToElementPointer()->at(*it).getConnectedId(i));
      
    // faccio un resize 
    ids->clear();
    ids->resize(elemConn.size());
    copy(elemConn.begin(), elemConn.end(), ids->begin());
}

void tricky2d<Triangle>::createStellataBor(UInt id1, vector<UInt> * ids)
{
      assert(id1<meshPointer->getNumNodes());
      
      // variabili temporanee
      vector<UInt>	     idsTmp;
      geoElement<Line>		lin;
      
      // creo la stellata 
      createStellata(id1, &idsTmp);
      
      // mi assicuro che ids sia ok
      ids->clear();
      ids->reserve(idsTmp.size());
      
      // setto l'id
      lin.setConnectedId(0, id1);
      
      // controllo che ci sia il connesso 
      for(UInt i=0; i<idsTmp.size(); ++i)
      {
	  // setto la linea 
	  lin.setConnectedId(1, idsTmp[i]);
	  
	  // se non lo trovo 
	  if(bordo.find(lin)!=bordo.end())	ids->push_back(idsTmp[i]);
      }
      
}

UInt tricky2d<Triangle>::getMaxAngle(UInt center, UInt nodeId)
{
      // controllo delle variabili in input
      assert(center<meshPointer->getNumNodes());
      assert(nodeId<meshPointer->getNumNodes());
      
      // variabili temporanee
      bool 	found=false;
      Real cosMaxAngle = 2.0;
      UInt id            = 0;
      point            v1,v2;
      UInt               tmp;
      Real               cos;
      vector<UInt>      star;
      
      // primo vettore
      v1 = (meshPointer->getNode(nodeId)-meshPointer->getNode(center));
      v1 = v1 / v1.norm2();
      
      // creo la stellata
      createStellata(center, &star);
      
      // ciclo sulla stellata
      for(UInt j=0; j<star.size(); ++j)
      {
	      // prendo l'id
	      tmp = star[j];
      
	      // calcolo il vettore e lo normalizzo
	      v2 = (meshPointer->getNode(tmp)-meshPointer->getNode(center));
	      v2 = v2 / v2.norm2();
	      
	      // calcolo il coseno dell'angolo
	      cos = v1*v2;
	      
	      // controllo
	      if((cos<cosMaxAngle) && (tmp!=nodeId))
	      {
		      cosMaxAngle = cos;
		      id          = tmp;    
		      found      = true;
	      }
	      
      }
      
      // devo aver trovato un punto
      assert(found==true);
      
      return(id);
}

bool tricky2d<Triangle>::sameGeoIdAround(UInt id1)
{
      assert(id1<meshPointer->getNumNodes());
      
      // variabili in uso
      set<UInt>             aroundId1;
      
      // prendo tutti i geo id attorno al primo punto
      for(UInt i=0; i<conn.getNodeToElementPointer(id1)->getNumConnected(); ++i)
	    // inserisco il suo geoId
	    aroundId1.insert(meshPointer->getElement(conn.getNodeToElementPointer(id1)->getConnectedId(i)).getGeoId());
      
      // se il size è uno vuol dire che c'è solo un geoId attorno al nodo 
      if(aroundId1.size()==1)	  return(true);
      
      // ritorno falso se il size non è uno 
      return(false);
}

bool tricky2d<Triangle>::sameGeoIdAround(UInt id1, UInt id2)
{
      assert(id1<meshPointer->getNumNodes());
      assert(id2<meshPointer->getNumNodes());
      
      // variabili in uso
      set<UInt>   aroundId1,aroundId2;
      
      // prendo tutti i geo id attorno al primo punto
      for(UInt i=0; i<conn.getNodeToElementPointer(id1)->getNumConnected(); ++i)
	    // inserisco il suo geoId
	    aroundId1.insert(meshPointer->getElement(conn.getNodeToElementPointer(id1)->getConnectedId(i)).getGeoId());
      
      // prendo tutti i geo id attorno al primo punto
      for(UInt i=0; i<conn.getNodeToElementPointer(id2)->getNumConnected(); ++i)
	    // inserisco il suo geoId
	    aroundId2.insert(meshPointer->getElement(conn.getNodeToElementPointer(id2)->getConnectedId(i)).getGeoId());
      
      // se le due dimensioni sono diverse ritorno falso 
      if(aroundId1.size()!=aroundId2.size())	    return(false);
      
      // se le loro dimensioni sono uguali posso usare l'STL
      return(equal(aroundId1.begin(), aroundId1.end(), aroundId2.begin()));
}

bool tricky2d<Triangle>::sameGeoIdAround(UInt id1, UInt id2, UInt id3)
{
      return(sameGeoIdAround(id1, id2) && sameGeoIdAround(id2, id3) && sameGeoIdAround(id3, id1));
}

void tricky2d<Triangle>::geoIdAround(UInt id1, vector<UInt> * geoIds)
{
    // variabili utilizzate 
    set<UInt>     geoTmp;
    
    // ciclo per mettere tutti i geoId
    for(UInt i=0; i<conn.getNodeToElementPointer(id1)->getNumConnected(); ++i)
	geoTmp.insert(meshPointer->getElement(conn.getNodeToElementPointer(id1)->getConnectedId(i)).getGeoId());
    
    // faccio il resize
    geoIds->resize(geoTmp.size());
    
    // la copio
    copy(geoTmp.begin(), geoTmp.end(), geoIds->begin());
}

void tricky2d<Triangle>::geoIdAroundBor(UInt id1, vector<UInt> * geoIds)
{
      // varabili in uso 
      vector<UInt>		      ids;
      set<UInt>			   tmpGeo;
      geoElement<Line>  	      lin;
      set<geoElement<Line> >::iterator it;
      
      // pulisco 
      geoIds->clear();
      
      // prendo la sua stellata 
      createStellataBor(id1, &ids);
      
      if(ids.size()==0)
      {
	  cout << "L'edge che mi hai dato non è di bordo" << endl;
	  return;
      }
      
      // setto il primo connesso 
      lin.setConnectedId(0, id1);
      
      // prendo gli edge 
      for(UInt i=0; i<ids.size(); ++i)
      {
	  // setto l'altro connesso 
	  lin.setConnectedId(1, ids[i]);
	  
	  // lo cerco 
	  it = bordo.find(lin);
	  
	  // lo DEVO trovare 
	  assert(it!=bordo.end());
	  
	  // salvo 
	  tmpGeo.insert(it->getGeoId());
      }
      
      // faccio un resize 
      geoIds->resize(tmpGeo.size());
      copy(tmpGeo.begin(), tmpGeo.end(), geoIds->begin());
}

void tricky2d<Triangle>::getNodeAroundElement(UInt elemId, vector<UInt> * ids)
{
      // variabili in uso
      vector<UInt>		tmp;
      set<UInt>			all;
      
      // per ogni nodo dell'elemento faccio la stellata 
      for(UInt i=0; i<3; ++i)
      {
	  // prendo la stellata 
	  createStellata(meshPointer->getElementPointer(elemId)->getConnectedId(i), &tmp);
	  
	  // la metto in all
	  for(UInt j=0; j<tmp.size(); ++j)	all.insert(tmp[j]);
      }
      
      //copio 
      ids->assign(all.size(), 0);
      copy(all.begin(), all.end(), ids->begin());
}

void tricky2d<Triangle>::getNodeAround(UInt id1, UInt deph, vector<UInt> * ids)
{
      assert(id1<meshPointer->getNumNodes());
      assert(deph>=1);
      
      // variabili in uso
      vector<UInt> tmpRis1,tmpRis2;
      set<UInt>         tmp,fronte;
      set<UInt>::iterator   finder;
      
      //libero il vettore ids
      ids->clear();
      
      // metto id1 nella lista ids
      ids->push_back(id1);
            
      // inizializzo tmp
      fronte.insert(id1);
      
      // faccio la ricerca in base alla profondità
      for(UInt i=0; i<deph; ++i)
      {
	  // pulisco le variabili
	  tmpRis1.clear();
	  tmpRis2.clear();
	
	  // per ogni elemento di tmp
	  for(set<UInt>::iterator it=fronte.begin(); it!=fronte.end(); ++it)
	  {
		// per ogni nodo prendo la sua stellata e inserisco tutti i nodi dei triangoli trovati 
		for(UInt j=0; j<conn.getNodeToElementPointer(*it)->getNumConnected(); ++j)
		{
		    tmp.insert(meshPointer->getElement(conn.getNodeToElementPointer(*it)->getConnectedId(j)).getConnectedId(0));
		    tmp.insert(meshPointer->getElement(conn.getNodeToElementPointer(*it)->getConnectedId(j)).getConnectedId(1));
		    tmp.insert(meshPointer->getElement(conn.getNodeToElementPointer(*it)->getConnectedId(j)).getConnectedId(2));
		}
	  }
	  
	  // pulisco li fronte
	  fronte.clear();
	  
	  // cerco ogni elemento di tmp in fronte e lo elimino in modo da ottenere il fornte che avanza
	  for(vector<UInt>::iterator it=ids->begin(); it!=ids->end(); ++it)
	  {
		// lo cerco
		finder = tmp.find(*it);
		
		// lo elimino dalla lista fronte
		if(finder!=tmp.end())	      tmp.erase(finder);
	  }
	  
	  // tmp diventa il fornte
	  for(set<UInt>::iterator it=tmp.begin(); it!=tmp.end(); ++it) fronte.insert(*it);
	 
	  // salvo il fronte in tmpRis
	  tmpRis1.resize(fronte.size());
	  copy(fronte.begin(), fronte.end(), tmpRis1.begin());
	  
	  // riempio il vettore ids
	  tmpRis2.resize(ids->size());
	  copy(ids->begin(), ids->end(), tmpRis2.begin());

	  // faccio un resize di ids
	  ids->resize(tmpRis1.size()+tmpRis2.size());
	  merge(tmpRis1.begin(), tmpRis1.end(), tmpRis2.begin(), tmpRis2.end(), ids->begin());
      }
}

void tricky2d<Triangle>::getNodeAround(UInt id1, UInt deph, vector<UInt> * ids, UInt limite)
{
      assert(id1<meshPointer->getNumNodes());
      assert(deph>=1);
      
      // variabili in uso
      vector<UInt> tmpRis1,tmpRis2;
      set<UInt>         tmp,fronte;
      set<UInt>::iterator   finder;
      
      //libero il vettore ids
      ids->clear();
      
      // metto id1 nella lista ids
      ids->push_back(id1);
            
      // inizializzo tmp
      fronte.insert(id1);
      
      // faccio la ricerca in base alla profondità
      for(UInt i=0; i<deph; ++i)
      {
	  // pulisco le variabili
	  tmpRis1.clear();
	  tmpRis2.clear();
	  
	  // per ogni elemento di tmp
	  for(set<UInt>::iterator it=fronte.begin(); it!=fronte.end(); ++it)
	  {  
		// per ogni nodo prendo la sua stellata e inserisco tutti i nodi dei triangoli trovati 
		for(UInt j=0; j<conn.getNodeToElementPointer(*it)->getNumConnected(); ++j)
		{
		    tmp.insert(meshPointer->getElement(conn.getNodeToElementPointer(*it)->getConnectedId(j)).getConnectedId(0));
		    tmp.insert(meshPointer->getElement(conn.getNodeToElementPointer(*it)->getConnectedId(j)).getConnectedId(1));
		    tmp.insert(meshPointer->getElement(conn.getNodeToElementPointer(*it)->getConnectedId(j)).getConnectedId(2));
		}
	  }
	  
	  // pulisco li fronte
	  fronte.clear();
	  
	  // cerco ogni elemento di tmp in fronte e lo elimino in modo da ottenere il fornte che avanza
	  for(UInt j=0; j<ids->size(); ++j)
	  {
		// lo cerco
		finder = tmp.find(ids->at(j));
		
		// lo elimino dalla lista fronte
		if(finder!=tmp.end())
		{
		      tmp.erase(finder);
		}
	  }
	  
	  // tmp diventa il fornte
	  for(set<UInt>::iterator it=tmp.begin(); it!=tmp.end(); ++it) fronte.insert(*it);
	 
	  // salvo il fronte in tmpRis
	  tmpRis1.resize(fronte.size());
	  copy(fronte.begin(), fronte.end(), tmpRis1.begin());
	  
	  // riempio il vettore ids
	  tmpRis2.resize(ids->size());
	  copy(ids->begin(), ids->end(), tmpRis2.begin());

	  // faccio un resize di ids
	  ids->resize(tmpRis1.size()+tmpRis2.size());
	  merge(tmpRis1.begin(), tmpRis1.end(), tmpRis2.begin(), tmpRis2.end(), ids->begin());
	    
	  // se ho già trovato id a sufficienza mi fermo
	  if(ids->size()>limite)	return;
      }
}

bool tricky2d<Triangle>::sameEdge(vector<UInt> * edge, UInt id1, UInt id2)
{
    if(edge->size()!=2)										 return(false);
    if(((edge->at(0)==id1) || (edge->at(0)==id2)) && ((edge->at(1)==id1) || (edge->at(1)==id2))) return(true);
    
    return(false);
}

bool tricky2d<Triangle>::isAnEdge(vector<UInt> * edge)
{   
      // variadili in uso
      vector<UInt>	ele;
      
      // se gli id sono troppo alti o uguali fra loro vuol dire che non è un edge 
      if(edge->at(0)>=meshPointer->getNumNodes())	return(false);
      if(edge->at(1)>=meshPointer->getNumNodes())	return(false);
      if(edge->at(1)==edge->at(0))			return(false);
      
      // guardo quanti elementi sono sull'edge
      elementOnEdge(edge->at(0), edge->at(1), &ele);
      
      // se ci sono elementi ritorno vero 
      if(ele.size()>0)	return(true);
      
      // altrimenti ritorno falso 
      return(false);
}

void tricky2d<Triangle>::atroEdge(vector<UInt> * edge, vector<UInt> * altro)
{
      // varaibili in uso 
      vector<UInt>	onEdge;
     
      // prendo i connessi 
      elementOnEdge(edge->at(0), edge->at(1), &onEdge);
    
      // prendo gli altri estremi 
      altro->resize(onEdge.size());
      for(UInt i=0; i<onEdge.size(); ++i)	altro->at(i) = lastNode(edge->at(0), edge->at(1), onEdge[i]);
    
      // deve essere due per concedere il flip
      if(altro->size()!=2)		altro->clear();
}

void tricky2d<Triangle>::getLink(vector<UInt> * edge, vector<vector<UInt> > * link)
{
    //  creo gli altri edge 
    UInt		    id1,id2;
    vector<UInt>        ext(2),elem;
    
    // prendo gli estremi 
    id1 = edge->at(0);
    id2 = edge->at(1);
    
    // prendo gli elementi sull'edge
    elementOnEdge(id1, id2, &elem);
    
    // faccio un reserve 
    link->clear();
    link->reserve(2*elem.size());
    
    // metodo per i link
    for(UInt i=0; i<elem.size(); ++i)
    {
	// metto il primo 
	ext[0] = lastNode(id1, id2, elem[i]);
	ext[1] = id1;
	
	link->push_back(ext);
	
	// metto il secondo
	//ext[0] = lastNode(id1, id2, elem[i]);
	ext[1] = id2;
	
	link->push_back(ext);
    }
}

void tricky2d<Triangle>::getLinkNode(UInt nodeId, vector<vector<UInt> > * link)
{
    // variabili in uso 
    vector<UInt>	    elem,edge;
    
    // prendo tutti gli elementi connessi 
    getElementAround(nodeId, &elem);
    
    // faccio un resize 
    link->clear();
    link->reserve(elem.size()*2);
    
    // per tutti gli elementi prendo gli ultimi due che mi restano 
    for(UInt i=0; i<elem.size(); ++i)
    {
	// prendo l'edge
	lastNode(nodeId, elem[i], &edge);
	
	// se non è di bordo la metto
	link->push_back(edge);
    }
}


//
// Metodi per esplorare la mesh partendo dai triangoli
//
bool tricky2d<Triangle>::areCoherent(UInt elemId1, UInt elemId2)
{
    // varaibili in uso 
    vector<UInt>	  ids;
    
    // trovo i due id comuni
    commonEdge(elemId1, elemId2, &ids);
    
    // controllo 
    if((nextNode(ids[0], elemId1)==precNode(ids[0],elemId2)) ||
       (nextNode(ids[0], elemId2)==precNode(ids[0],elemId1))) return(true);
       
    return(false);
       
}

bool tricky2d<Triangle>::isObtuse(UInt elemId)
{
    assert(elemId<meshPointer->getNumElements());
    
    // variabili in gioco
    set<Real>                    ang;
    
    // ciclo sui nodi del triangolo per calcolare gli angoli
    for(UInt i=0; i<3; ++i)	ang.insert(angolo(meshPointer->getElement(elemId).getConnectedId(i), elemId));
    
    // prendo l'ultimo della lista e controllo la sua lunghezza
    return((*ang.rbegin())>(PGRECO*0.5));
}

bool tricky2d<Triangle>::isBoundary(vector<UInt> * edge)
{
    // varaibili temporanee
    geoElement<Line>  lin;
    
    // setto la linea
    lin.setConnectedId(0, edge->at(0));
    lin.setConnectedId(1, edge->at(1));
    
    // la cerco e vedo il risultato 
    return(bordo.find(lin)!=bordo.end());
}

UInt tricky2d<Triangle>::getBoundaryGeoId(vector<UInt> * edge)
{
    // variabili in uso
    geoElement<Line>	  	    tmpEdge;
    set<geoElement<Line> >::iterator finder;
    
    // setto i connessi 
    tmpEdge.setConnectedId(0, edge->at(0));
    tmpEdge.setConnectedId(1, edge->at(1));
    
    // provo a vedere se è un edge di bordo 
    finder = bordo.find(tmpEdge);
    
    // lo devo trovare 
    assert(finder!=bordo.end());
    
    // ritorno il geoId 
    return(finder->getGeoId());
}

bool tricky2d<Triangle>::isTriangleDegenerate(UInt elemId)
{
    // variabili in uso
    set<UInt> ids;
    
    // inserisco gli id
    ids.insert(meshPointer->getElement(elemId).getConnectedId(0));
    ids.insert(meshPointer->getElement(elemId).getConnectedId(1));
    ids.insert(meshPointer->getElement(elemId).getConnectedId(2));
    
    return(ids.size()!=3);
}

bool tricky2d<Triangle>::isElementDegenerate(UInt elemId)
{
    return(isTriangleDegenerate(elemId));
}

void tricky2d<Triangle>::setTriangleDegenerate(UInt elemId)
{
    // inserisco gli id
    meshPointer->getElementPointer(elemId)->setConnectedId(0, 0);
    meshPointer->getElementPointer(elemId)->setConnectedId(1, 0);
    meshPointer->getElementPointer(elemId)->setConnectedId(2, 0);
}

void tricky2d<Triangle>::stellataTria(UInt elemId, vector<UInt> * ele)
{
    assert(elemId<meshPointer->getNumElements());
    
    // varaili in gioco
    set<UInt>         tutti;
    set<UInt>::iterator  it;
    vector<UInt>        tmp;
    
    // per ogni lato 
    for(UInt i=0; i<3; ++i)
    {
	// prendo tutti gli elementi che hanno in comune un edge
	elementOnEdge(meshPointer->getElement(elemId).getConnectedId((i%3)), 
		      meshPointer->getElement(elemId).getConnectedId((i+1)%3), &tmp);
    
	// li metto in tutti
	for(UInt i=0; i<tmp.size(); ++i)	  tutti.insert(tmp[i]);
    }
    
    // trovo l'elemento
    it = tutti.find(elemId);
    assert(it!=tutti.end());
    tutti.erase(it);
    
    // metto a posto ele	
    ele->resize(tutti.size());
    
    // riempio
    copy(tutti.begin(), tutti.end(), ele->begin());
}

UInt tricky2d<Triangle>::bigStellataTria(UInt elemId, vector<UInt> * ele)
{
    assert(elemId<meshPointer->getNumElements());
    
    // varaili in gioco
    set<UInt>         tutti;
    set<UInt>::iterator  it;
    vector<UInt>        tmp;
    
    // per ogni lato 
    for(UInt i=0; i<3; ++i)
    {
	// prendo tutti gli elementi che hanno in comune un edge
	getElementAround(meshPointer->getElement(elemId).getConnectedId((i)), &tmp);
    
	// li metto in tutti
	for(UInt i=0; i<tmp.size(); ++i)	  tutti.insert(tmp[i]);
    }
    
    // trovo l'elemento
    it = tutti.find(elemId);
    assert(it!=tutti.end());
    tutti.erase(it);
    
    // metto a posto ele	
    ele->resize(tutti.size());
    
    // riempio
    copy(tutti.begin(), tutti.end(), ele->begin());
    
    return(ele->size());
}

void tricky2d<Triangle>::oppositeTria(UInt id1, UInt id2, UInt elemId, vector<UInt> * ids)
{	
      assert(id1<meshPointer->getNumNodes());
      assert(id2<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // variabile in gioco 
      vector<UInt>       tmp;
      
      // cerco gli elementi che insistono sull'edge
      elementOnEdge(id1, id2, &tmp);
      
      // riempio il vettore ids
      for(UInt i=0; i<tmp.size(); ++i)
	  // se è diverso 
	  if(tmp[i]!=elemId)	
	      ids->push_back(tmp[i]);
}

bool tricky2d<Triangle>::sameGeoId(UInt elemId1, UInt elemId2)
{
      assert(elemId1<meshPointer->getNumElements());
      assert(elemId2<meshPointer->getNumElements());
      
      return (meshPointer->getElement(elemId1).getGeoId()==meshPointer->getElement(elemId2).getGeoId());
}

bool tricky2d<Triangle>::normalVarr(UInt elemId1, UInt elemId2, Real angolo)
{
      assert(elemId1<meshPointer->getNumElements());
      assert(elemId2<meshPointer->getNumElements());
      
      // variabili in gioco
      Real coseno = cos(angolo);
      
      return((getTriangleNormal(elemId1)*getTriangleNormal(elemId2))>coseno);
}

void tricky2d<Triangle>::commonEdge(UInt elemId1, UInt elemId2, vector<UInt> * ids)
{
      assert(elemId1<meshPointer->getNumElements());
      assert(elemId2<meshPointer->getNumElements());
      
      // varaibili in uso
      vector<UInt>       ids1(3);
      vector<UInt>       ids2(3);
      vector<UInt>     common(6);
      vector<UInt>::iterator  it;
      
      // pulisco ids
      ids->clear();
      
      // metto tutto nelle variabili
      ids1[0] = meshPointer->getElement(elemId1).getConnectedId(0);
      ids1[1] = meshPointer->getElement(elemId1).getConnectedId(1);
      ids1[2] = meshPointer->getElement(elemId1).getConnectedId(2);
      
      ids2[0] = meshPointer->getElement(elemId2).getConnectedId(0);
      ids2[1] = meshPointer->getElement(elemId2).getConnectedId(1);
      ids2[2] = meshPointer->getElement(elemId2).getConnectedId(2);
      
      // li ordino
      sort(ids1.begin(), ids1.end());
      sort(ids2.begin(), ids2.end());
      
      // trovo i comuni
      it = set_intersection(ids1.begin(), ids1.end(), ids2.begin(), ids2.end(), common.begin());
      
      // faccio un resize di ids
      ids->resize(it-common.begin());
      
      // copio il contenuto di common
      copy(common.begin(), it, ids->begin());
}

void tricky2d<Triangle>::orient(UInt elemId, point normal)
{
      assert(elemId<meshPointer->getNumElements());
  
      // variabili temporanee
      vector<UInt>   connessi;
      UInt                 id;

      // trovo la direzione di due generici lati dell'elemento
      point p1 = (meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(1))-
		  meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(0)));
      point p2 = (meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(2))-
		  meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(0)));
      
      // controllo che la normale non sia zero se è zero ritorno
      if((p1^p2).norm2()<1e-15)	return;
	
      // trovo la normale
      point actualNormal = (p1^p2) / ((p1^p2).norm2());
	
      // se sono diretti in senso opposto li devo rinumerare
      if((actualNormal*normal)<0.0)
      {
	    id = meshPointer->getElement(elemId).getConnectedId(0);
	    meshPointer->getElementPointer(elemId)->setConnectedId(0,meshPointer->getElementPointer(elemId)->getConnectedId(1));
	    meshPointer->getElementPointer(elemId)->setConnectedId(1, id);
      }
}

//
// Metodi per fare alcuni calcoli sui punti della mesh
//

Real tricky2d<Triangle>::angolo(UInt nodeId, UInt elemId)
{
      assert(nodeId<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // varaibili
      vector<UInt>      ids;
      
      // trovo gli altri due nodi 
      lastNode(nodeId, elemId, &ids);
      
      // calcolo i vettori
      point v1 = (meshPointer->getNode(nodeId)-meshPointer->getNode(ids[0]));
      point v2 = (meshPointer->getNode(nodeId)-meshPointer->getNode(ids[1]));
      
      // calcolo l'angolo in radianti 
      Real rad = (v1*v2)/((v1.norm2())*(v2.norm2()));
      
      // controllo il valore ottenuto in modo da essere sicuri che sia [-1, 1]
      if(fabs(rad+1.)<toll)		return(PGRECO*(-0.5));
      if(fabs(rad-1.)<toll)		return(PGRECO*(0.5));
      
      return(acos(rad));
}

point tricky2d<Triangle>::getPointNormal(UInt nodeId)
{
      assert(nodeId<meshPointer->getNumNodes());
  
      // varaibili 
      point    							             normalFromPoint;
      UInt                    							       idTmp;
      vector<point>  normalFromTria(conn.getNodeToElementPointer(nodeId)->getNumConnected());
      vector<Real>             pesi(conn.getNodeToElementPointer(nodeId)->getNumConnected());
      vector<UInt>            						  	         ids;
      
      //
      // Il modo in cui viene calcolata la normale a un nodo della mesh è preso dall'articolo "Metodi per l'interpretazione
      // delle curvature di geometrie definite con mesh triangolari" di Di Angelo e Di Stefano
      //
      
      for(UInt i=0; i<conn.getNodeToElementPointer(nodeId)->getNumConnected(); ++i)
      {
	  // prendo l'id dell'elemento
	  idTmp = conn.getNodeToElementPointer(nodeId)->getConnectedId(i);
	  
	  // metto la sua normale nel vettore
	  normalFromTria[i] = getTriangleNormal(idTmp);
	  
	  // calcolo i suoi pesi
	  ids.clear();
	  lastNode(nodeId, idTmp, &ids);
	  
	  // metto i pesi
	  pesi[i] = (sin(angolo(nodeId, idTmp)) / 
		     ( ((meshPointer->getNode(nodeId)-meshPointer->getNode(ids[0])).norm2())*
		       ((meshPointer->getNode(nodeId)-meshPointer->getNode(ids[1])).norm2()) ));
      }
      
      // calcolo la normale
      for(UInt i=0; i<pesi.size(); ++i)	  normalFromPoint.add(normalFromTria[i]*pesi[i]);
      
      return(normalFromPoint / normalFromPoint.norm2());
}

Real tricky2d<Triangle>::getVoronoiAreaAround(UInt nodeId)
{
    // varaibili in gioco 
    Real area = 0.0;
    
    // questa formula per ritrovare l'area di voronoi in un triangolo è presa dall'articolo
    // "Discrete Differential-Geometry Operators for Triangulated 2-Manifolds" di Mark Meyer, Mathieu Desbrun, Peter Schroder
    // and Alan H. Barr
    
    for(UInt i=0; i<conn.getNodeToElementPointer(nodeId)->getNumConnected(); ++i)
	area += getVoronoiArea(nodeId, conn.getNodeToElementPointer(nodeId)->getConnectedId(i));
    
    return(area);
}

Real tricky2d<Triangle>::getNodeCurvature(UInt nodeId)
{
     // Variabili in uso
     vector<UInt>         ids,elem;
     UInt  nodeAlpha,nodeBeta,molt;
     UInt         eleAlpha,eleBeta;
     Real           	     coeff;
     point       tmp,vettCurvature;
     
     // vedo se il nodo è di bordo
     molt = meshPointer->getNode(nodeId).getBoundary();
     
     // questa formula per ritrovare la curvatura di un punto è presa dall'articolo
     // "Discrete Differential-Geometry Operators for Triangulated 2-Manifolds" di Mark Meyer, Mathieu Desbrun, Peter Schroder,
     // and Alan H. Barr
     
     switch(molt)
     {
         // nel caso in cui è un nodo interno
	 case(0):
		  // prendo la stellata del nodo 
		  createStellata(nodeId, &ids);
		  
		  // ciclo sui punti della stellata e calcolo della curvatura
		  for(UInt i=0; i<ids.size(); ++i)
		  {
		      tmp = (meshPointer->getNode(nodeId)-meshPointer->getNode(ids[i]));
		      
		      // trovo gli elementi che insistono su questo edge
		      elementOnEdge(nodeId, ids[i], &elem);
		      
		      // posso farlo solamente per i nodi interni 
		      assert(elem.size()==2);
		      
		      // setto la variabili nodeAlpha, eleAlpha
		      eleAlpha  = elem[0];
		      nodeAlpha = lastNode(nodeId, ids[i], eleAlpha);

		      // setto la variabili nodeBeta, eleBeta
		      eleBeta   = elem[1];
		      nodeBeta  = lastNode(nodeId, ids[i], eleBeta);
		      
		      // creo il coefficiente
		      coeff     = 1.0/tan(angolo(nodeAlpha, eleAlpha))+1.0/tan(angolo(nodeBeta, eleBeta));
		      
		      // lo sommo al vettore che conterrà la curvatura
		      vettCurvature.add(tmp, coeff);
		      
		  }
		  
		  // calcolo il vettore 
		  vettCurvature = vettCurvature * (1./(2.0*getVoronoiAreaAround(nodeId)));
		  
		  // ritorno la curvatura
		  return(vettCurvature.norm2()*0.5);
		  
		  break;
	 // é un nodo lungo il bordo
	 case(1):
		  // per il momento ritorno 0
		  return(-1.0);
		  break;
     }
     
     cout << "Non so come calcolare la curvatura del nodo" << endl;
     return(0.0);
}

//
// Metodi per fare alcuni calcoli sui triangloli della mesh
//

point tricky2d<Triangle>::getTriangleNormal(UInt elemId)
{
      assert(elemId<meshPointer->getNumElements());
      
      // varaibile
      point normale(0.0,0.0,0.0);
      
      point v1 = ( meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(1))-
		   meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(0)) );
      point v2 = ( meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(2))-
		   meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(0)) );

      // controllo che il prodotto vettore non sia degenere 
      if((v1^v2).norm2()<meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(1)).getToll())   return(normale);
      
      // calcolo la normale e la normalizzo
      normale = v1^v2;
      normale = normale / normale.norm2();
      return(normale);
}

point tricky2d<Triangle>::getEdgeNormal(UInt elemId, UInt id1, UInt id2)
{
      assert(elemId<meshPointer->getNumElements());
      assert(id1<meshPointer->getNumNodes());
      assert(id2<meshPointer->getNumNodes());
      
      // variabili 
      UInt                        id3;
      point normalEdge,normalTria,dir;
      
      // prendo l'ultimo id 
      id3 = lastNode(id1, id2, elemId);
      
      // prendo la normale al triangolo 
      normalTria = getTriangleNormal(elemId);
      
      // prendo il lato 
      dir = meshPointer->getNode(id2)-meshPointer->getNode(id1);
      dir.normalize();
      
      // prendo la normale all'edge 
      normalEdge = dir^normalTria;
      
      // controllo che sia giusta 
      if(normalEdge*(meshPointer->getNode(id3)-meshPointer->getNode(id1))>0)	normalEdge = normalEdge*(-1.);
      
      // ritorno la normale 
      return(normalEdge);
}

Real tricky2d<Triangle>::getTriangleArea(UInt elemId)
{
      	// variabili temporanee
	point    p1,p2,p3;
	Real  a,b,c,p,val;
	
	// ricavo i punti
	p1 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(0));
	p2 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(1));
	p3 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(2));
	

	// ricavo le lunghezze
	a = (p2-p1).norm2();	
	b = (p3-p2).norm2();
	c = (p1-p3).norm2();
	p = (a+b+c)*0.5;
	
	// controllo val
	val = p*(p-a)*(p-b)*(p-c);
	
	if(val<0.0)			return(0.0);
	if(sqrt(fabs(val))<toll) 	return(0.0);
	
	// ritorno l'area 
	return(sqrt(fabs(val)));
}

Real tricky2d<Triangle>::getTriangleArea(point p1, point p2, point p3)
{
      	// variabili temporanee
	Real  a,b,c,p,val;
	
	// ricavo le lunghezze
	a = (p2-p1).norm2();
	b = (p3-p2).norm2();
	c = (p1-p3).norm2();
	p = (a+b+c)*0.5;
	
	// controllo val
	val = p*(p-a)*(p-b)*(p-c);
	if(val<0.0)			return(0.0);
	if(sqrt(fabs(val))<toll) 	return(0.0);
	
	// ritorno l'area 
	return(sqrt(fabs(val)));
}

Real tricky2d<Triangle>::getTrianglePerimeter(UInt elemId)
{
      // variabili temporanee
      point p1,p2,p3;
      Real  a,b,c;
	
      // ricavo i punti
      p1 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(0));
      p2 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(1));
      p3 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(2));
	
      // ricavo le lunghezze
      a = (p2-p1).norm2();
      b = (p3-p2).norm2();
      c = (p1-p3).norm2();
      
      return(a+b+c);
}

Real tricky2d<Triangle>::getVoronoiArea(UInt nodeId, UInt elemId)
{
      assert(elemId<meshPointer->getNumElements());
      assert(nodeId<meshPointer->getNumNodes());
  
      // Variabili in uso 
      vector<UInt>      ids;
      
      // questa formula per ritrovare l'area di voronoi in un triangolo è presa dall'articolo
      // "Discrete Differential-Geometry Operators for Triangulated 2-Manifolds" di Mark Meyer, Mathieu Desbrun, Peter Schroder,
      // and Alan H. Barr
      	
      if(isObtuse(elemId))
      {
	 if(angolo(nodeId, elemId)>(PGRECO*0.5))
	 {
	    return(getTriangleArea(elemId)*0.5);
	 }
	 else
	 {
	    return(getTriangleArea(elemId)*0.25);
	 }
      }
      
      // trovo i nodi rimanenti
      lastNode(nodeId, elemId, &ids);
	 
      // cacolo la lunghezza dei lati
      Real a   = (meshPointer->getNode(nodeId)-meshPointer->getNode(ids[0])).norm2();
      Real b   = (meshPointer->getNode(nodeId)-meshPointer->getNode(ids[1])).norm2();
	 
      // calcolo la curvatura
      Real cur = 0.125*(a*a/tan(angolo(ids[1], elemId))+
			b*b/tan(angolo(ids[0], elemId)));
	  
      return(cur);
}

Real tricky2d<Triangle>::getMinEdge(UInt elemId, vector<UInt> * edge)
{
      assert(elemId<meshPointer->getNumElements());
      
      // variadili in uso
      set<geoElementSize<Line> >  list;
      geoElementSize<Line>         lin;
      
      // pulisco edge
      edge->clear();
      
      // trovo la lunghezza di tutti i lati
      for(UInt i=0; i<3; ++i)
      {
	    // setto la variabile lin
	    lin.setConnectedId(0, meshPointer->getElement(elemId).getConnectedId(i));
	    lin.setConnectedId(1, meshPointer->getElement(elemId).getConnectedId(((i+1) % 3)));
	    lin.setGeoSize((meshPointer->getNode(lin.getConnectedId(0))-meshPointer->getNode(lin.getConnectedId(1))).norm2());
	    
	    // la metto nella lista
	    list.insert(lin);
      }
      
      // riempio il vettore da restituire 
      edge->push_back(list.begin()->getConnectedId(0));
      edge->push_back(list.begin()->getConnectedId(1));
      
      return((meshPointer->getNode(list.begin()->getConnectedId(0))-
	      meshPointer->getNode(list.begin()->getConnectedId(1))).norm2());
}

Real tricky2d<Triangle>::getMaxEdge(UInt elemId, vector<UInt> * edge)
{	
      assert(elemId<meshPointer->getNumElements());
      
      // variadili in uso
      set<geoElementSize<Line> >  list;
      geoElementSize<Line>         lin;
      
      // pulisco edge
      edge->clear();
    
      // trovo la lunghezza di tutti i lati
      for(UInt i=0; i<3; ++i)
      {
	    // setto la variabile lin
	    lin.setConnectedId(0, meshPointer->getElement(elemId).getConnectedId(i));
	    lin.setConnectedId(1, meshPointer->getElement(elemId).getConnectedId(((i+1) % 3)));
	    lin.setGeoSize((meshPointer->getNode(lin.getConnectedId(0))-meshPointer->getNode(lin.getConnectedId(1))).norm2());
	    
	    // la metto nella lista
	    list.insert(lin);
      }
      
      // riempio il vettore da restituire 
      edge->push_back(list.rbegin()->getConnectedId(0));
      edge->push_back(list.rbegin()->getConnectedId(1));
      
      return((meshPointer->getNode(list.rbegin()->getConnectedId(0))-
	      meshPointer->getNode(list.rbegin()->getConnectedId(1))).norm2());
}

void tricky2d<Triangle>::getEdges(UInt elemId, vector<vector<UInt>  > * edges)
{
    // variabili in uso 
    vector<UInt>	edge(2);
    
    // faccio un reserve
    edges->clear();
    edges->reserve(3);
    
    // per ogni lato 
    for(UInt i=0; i<3; ++i)
    {
	// prendo l'edge 
	edge[0] = meshPointer->getElement(elemId).getConnectedId(i%3);
	edge[1] = meshPointer->getElement(elemId).getConnectedId((i+1)%3);
	
	// lo metto nella lista 
	edges->push_back(edge);
    }
}

bool tricky2d<Triangle>::getBoundEdges(UInt elemId, vector<vector<UInt>  > * edges)
{
    // variabili in uso 
    vector<UInt>	edge(2);
    
    // faccio un reserve
    edges->clear();
    edges->reserve(3);
    
    // per ogni lato 
    for(UInt i=0; i<3; ++i)
    {
	// prendo l'edge 
	edge[0] = meshPointer->getElement(elemId).getConnectedId(i%3);
	edge[1] = meshPointer->getElement(elemId).getConnectedId((i+1)%3);
	
	// controllo se è di bordo 
	if(isBoundary(&edge))	edges->push_back(edge);
    }
    
    // ritorno vero se ho trovato un edge di bordo
    return(edges->size()!=0);
}

pair<bool,Real> tricky2d<Triangle>::getBoundaryAngle(UInt nodeId)
{
    // variabili in uso
    point            p0,p1,p2;
    vector<UInt>  	  ids; 
    pair<bool,Real>    result; 
    
    // prendo i nodi connessi che sono di bordo 
    createStellataBor(nodeId, &ids);
    
    // controllo il size di ids se è maggiore di 2 l'angolo non è ben definito 
    if(ids.size()>2 || ids.size()<=1)		result.first=false;
    else					result.first=true;
    
    // se posso calcolo l'angolo 
    if(result.first)
    {
	// prendo i tre punti 
	p0 = meshPointer->getNode(nodeId);
	p1 = meshPointer->getNode(ids[0]);
	p2 = meshPointer->getNode(ids[1]);
	
	// calcolo l'angolo 
	result.second = ((p1-p0)*(p2-p0))/((p1-p0).norm2()*(p2-p0).norm2());
    }
    
    // rititorno il risultato 
    return(result);
}

Real tricky2d<Triangle>::getAngleAlongEdge(vector<UInt> * edge)
{
    // variabili in uso 
    Real		    cosAngolo=0;
    pair<bool, Real>	result1,result2;
    
    // chiamo due volte getBoundary edge
    result1 = getBoundaryAngle(edge->at(0));
    result2 = getBoundaryAngle(edge->at(1));
    
    // controllo il bool se uno dei due è vero ritorno -1
    if(!result1.first || !result2.first)	cosAngolo = -1.;
    else					cosAngolo = max(result1.second, result2.second);
    
    // altrimenti ritorno il minimo
    return(cosAngolo);
}


Real tricky2d<Triangle>::getMinEdge(UInt elemId)
{
      assert(elemId<meshPointer->getNumElements());
      
      // variadili in uso
      set<geoElementSize<Line> >  list;
      geoElementSize<Line>         lin;
      
      // trovo la lunghezza di tutti i lati
      for(UInt i=0; i<3; ++i)
      {
	    // setto la variabile lin
	    lin.setConnectedId(0, meshPointer->getElement(elemId).getConnectedId(i));
	    lin.setConnectedId(1, meshPointer->getElement(elemId).getConnectedId(((i+1) % 3)));
	    lin.setGeoSize((meshPointer->getNode(lin.getConnectedId(0))-meshPointer->getNode(lin.getConnectedId(1))).norm2());
	    
	    // la metto nella lista
	    list.insert(lin);
      }
      
      return((meshPointer->getNode(list.begin()->getConnectedId(0))-
	      meshPointer->getNode(list.begin()->getConnectedId(1))).norm2());
}

Real tricky2d<Triangle>::getMaxEdge(UInt elemId)
{	
      assert(elemId<meshPointer->getNumElements());
      
      // variadili in uso
      set<geoElementSize<Line> >  list;
      geoElementSize<Line>         lin;
      
      // trovo la lunghezza di tutti i lati
      for(UInt i=0; i<3; ++i)
      {
	    // setto la variabile lin
	    lin.setConnectedId(0, meshPointer->getElement(elemId).getConnectedId(i));
	    lin.setConnectedId(1, meshPointer->getElement(elemId).getConnectedId(((i+1) % 3)));
	    lin.setGeoSize((meshPointer->getNode(lin.getConnectedId(0))-meshPointer->getNode(lin.getConnectedId(1))).norm2());
	    
	    // la metto nella lista
	    list.insert(lin);
      }
      
      return((meshPointer->getNode(list.rbegin()->getConnectedId(0))-
	      meshPointer->getNode(list.rbegin()->getConnectedId(1))).norm2());
}

Real tricky2d<Triangle>::getMinAngle(UInt elemId)
{
    return(min(min(angolo(meshPointer->getElement(elemId).getConnectedId(0), elemId),
		   angolo(meshPointer->getElement(elemId).getConnectedId(1), elemId)),
		   angolo(meshPointer->getElement(elemId).getConnectedId(2), elemId)));
}

Real tricky2d<Triangle>::getMaxAngle(UInt elemId)
{
    return(max(max(angolo(meshPointer->getElement(elemId).getConnectedId(0), elemId),
		   angolo(meshPointer->getElement(elemId).getConnectedId(1), elemId)),
		   angolo(meshPointer->getElement(elemId).getConnectedId(2), elemId)));
}

//
// Metodo di stampa  
//
void tricky2d<Triangle>::printBoundary(string s)
{
    // varaibili in uso 
    mesh1d<Line>	linee;
    createFile		 file;
    
    // metto i nodi 
    linee.insertNode(meshPointer->getNodePointer());
    
    // faccio un reserve 
    linee.getElementPointer()->reserve(bordo.size());
    
    // metto gli elementi 
    for(set<geoElement<Line> >::iterator it=bordo.begin(); it!=bordo.end(); ++it)	linee.insertElement(*it);
    
    // salvo il file 
    file.fileForParaview(s, &linee);
    
}


