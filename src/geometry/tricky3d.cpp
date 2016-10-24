#include "tricky3d.h"

using namespace      std;
using namespace geometry;

//
// Costruttore 
//

tricky3d<Tetra>::tricky3d()
{    
    // definizione dei puntatori e toll
    meshPointer    = NULL;
    toll           = 1e-15;
    pNull.setX(9.9e99);
    pNull.setY(9.9e99);
    pNull.setZ(9.9e99);
}
		      
tricky3d<Tetra>::tricky3d(mesh3d<Tetra> * _meshPointer)
{    
    // varaibili temporanee
    UInt 		   id1,id2;
    mesh1d<Line> 	  reticolo;
    mesh2d<Triangle>          surf;
    map<UInt, UInt>	 surfToVol;
    
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
    conn.buildNodeToElement();
    
    // faccio le connettività di bordo 
    conn.buildBoundaryConnectivity(&surf, &surfToVol, true);
    
    // creo la mesh degli edge di bordo a partire da surf
    surf.createMesh1d(&reticolo);
    
    // inserisco le informazioni dei nodi sul bordo 
    for(UInt i=0; i<reticolo.getNumNodes(); ++i) meshPointer->getNodePointer(surfToVol[i])->setBoundary(1);
    
    // faccio un clear
    bordo.clear();
    
    // faccio un set
    for(UInt i=0; i<reticolo.getNumElements(); ++i)
    {
	// prendo gli id 
	id1 = reticolo.getElement(i).getConnectedId(0);
	id2 = reticolo.getElement(i).getConnectedId(1);
	
	// cambio gli id e li metto nel set 
	reticolo.getElementPointer(i)->setConnectedId(0, surfToVol[id1]);
	reticolo.getElementPointer(i)->setConnectedId(1, surfToVol[id2]);
	
	// lo metto nel set 
	bordo.insert(reticolo.getElement(i));
    }
}

//
// Set delle variabili di ambiente
//

void tricky3d<Tetra>::setMeshPointer(mesh3d<Tetra> * _meshPointer)
{
    // varaibili temporanee
    UInt 		   id1,id2;
    mesh1d<Line> 	  reticolo;
    mesh2d<Triangle>          surf;
    map<UInt, UInt>	 surfToVol;
    
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
    conn.buildNodeToElement();
    
    // faccio le connettività di bordo 
    conn.buildBoundaryConnectivity(&surf, &surfToVol, true);
    
    // creo la mesh degli edge di bordo a partire da surf
    surf.createMesh1d(&reticolo);
    
    // inserisco le informazioni dei nodi sul bordo 
    for(UInt i=0; i<reticolo.getNumNodes(); ++i) meshPointer->getNodePointer(surfToVol[i])->setBoundary(1);
    
    // faccio un clear
    bordo.clear();
    
    // faccio un set
    for(UInt i=0; i<reticolo.getNumElements(); ++i)
    {
	// prendo gli id 
	id1 = reticolo.getElement(i).getConnectedId(0);
	id2 = reticolo.getElement(i).getConnectedId(1);
	
	// cambio gli id e li metto nel set 
	reticolo.getElementPointer(i)->setConnectedId(0, surfToVol[id1]);
	reticolo.getElementPointer(i)->setConnectedId(1, surfToVol[id2]);
	
	// lo metto nel set 
	bordo.insert(reticolo.getElement(i));
    }
}

void tricky3d<Tetra>::setToll(Real _toll)
{
      toll = _toll;
}

Real tricky3d<Tetra>::getToll()
{
      return(toll);
}

void tricky3d<Tetra>::setUp()
{
    // varaibili temporanee
    UInt 		   id1,id2;
    mesh1d<Line> 	  reticolo;
    mesh2d<Triangle>          surf;
    map<UInt, UInt>	 surfToVol;
    
    // setto il punto nullo 
    pNull.setX(9.9e99);
    pNull.setY(9.9e99);
    pNull.setZ(9.9e99);
    
    // faccio un setUp della mesh in input
    meshPointer->setUpIds();
    
    // preparo connect
    conn.setMeshPointer(meshPointer);
    conn.buildNodeToElement();
    
    // faccio le connettività di bordo 
    conn.buildBoundaryConnectivity(&surf, &surfToVol, true);
    
    // creo la mesh degli edge di bordo a partire da surf
    surf.createMesh1d(&reticolo);
    
    // setto a 0 tutti gli id di bordo 
    for(UInt i=0; i<meshPointer->getNumNodes(); ++i)	meshPointer->getNodePointer(i)->setBoundary(0);
    
    // inserisco le informazioni dei nodi sul bordo 
    for(UInt i=0; i<reticolo.getNumNodes(); ++i) meshPointer->getNodePointer(surfToVol[i])->setBoundary(1);
    
    // faccio un clear
    bordo.clear();
    
    // faccio un set
    for(UInt i=0; i<reticolo.getNumElements(); ++i)
    {
	// prendo gli id 
	id1 = reticolo.getElement(i).getConnectedId(0);
	id2 = reticolo.getElement(i).getConnectedId(1);
	
	// cambio gli id e li metto nel set 
	reticolo.getElementPointer(i)->setConnectedId(0, surfToVol[id1]);
	reticolo.getElementPointer(i)->setConnectedId(1, surfToVol[id2]);
	
	// lo metto nel set 
	bordo.insert(reticolo.getElement(i));
    }
}


//
// Metodi per esplorare la mesh
//
bool tricky3d<Tetra>::isNodeDegenerate(UInt nodeId)
{
      assert(nodeId<meshPointer->getNumNodes());
      return(!(meshPointer->getNode(nodeId)!=pNull));
}

void tricky3d<Tetra>::setNodeDegenerate(UInt nodeId)
{
      assert(nodeId<meshPointer->getNumNodes());
      meshPointer->getNodePointer(nodeId)->setX(pNull.getX());
      meshPointer->getNodePointer(nodeId)->setY(pNull.getY());
      meshPointer->getNodePointer(nodeId)->setZ(pNull.getZ());
}

UInt tricky3d<Tetra>::nextNode(UInt id1, UInt elemId)
{
      assert(id1<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // ciclo sui nodi del triangolo
      for(UInt i=0; i<4; ++i)
	    if(meshPointer->getElement(elemId).getConnectedId(i)==id1)
		return(meshPointer->getElement(elemId).getConnectedId(((i+1)%4)));
      
      // assert di controllo per stabilire se è uscito con il return del ciclo
      cout << "Non ho trovato il nodo nel metodo nextNode" << endl;
      assert(1==0);
      return(0);
}

UInt tricky3d<Tetra>::precNode(UInt id1, UInt elemId)
{
      assert(id1<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // ciclo sui nodi del triangolo
      for(UInt i=0; i<4; ++i)
	    if(meshPointer->getElement(elemId).getConnectedId(i)==id1)
		return(meshPointer->getElement(elemId).getConnectedId(((i+3)%4)));
	    
      
      // assert di controllo per stabilire se è uscito con il return del ciclo
      cout << "Non ho trovato il nodo nel metodo precNode" << endl;
      assert(1==0);
      return(0);
}

UInt tricky3d<Tetra>::lastNode(UInt id1, UInt id2, UInt id3, UInt elemId)
{
      assert(id1<meshPointer->getNumNodes());
      assert(id2<meshPointer->getNumNodes());
      assert(id3<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // ciclo sui nodi del triangolo
      for(UInt i=0; i<4; ++i)
	    if( (meshPointer->getElement(elemId).getConnectedId(i)!=id1) && 
	        (meshPointer->getElement(elemId).getConnectedId(i)!=id2) && 
	        (meshPointer->getElement(elemId).getConnectedId(i)!=id3) )
		  return(meshPointer->getElement(elemId).getConnectedId(i));
      
      // assert di controllo per stabilire se è uscito con il return del ciclo
      cout << "Non ho trovato il nodo nel metodo lastNode" << endl;
      assert(1==0);
      return(0);
}

void tricky3d<Tetra>::lastNode(UInt id1, UInt elemId, vector<UInt> * ids)
{
      assert(id1<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // mi accerto che ids sia vuoto
      ids->clear();
      ids->reserve(3);
      
      // ciclo sui nodi del triangolo
      for(UInt i=0; i<4; ++i)
	    if(meshPointer->getElement(elemId).getConnectedId(i)!=id1)
		  ids->push_back(meshPointer->getElement(elemId).getConnectedId(i));
      
      // assert per accertarmi che tutto sia andato per il meglio
      assert(ids->size()==3);
}

void tricky3d<Tetra>::elementOnEdge(UInt id1, UInt id2, vector<UInt> * ele)
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

void tricky3d<Tetra>::elementOnFace(UInt id1, UInt id2, UInt id3, vector<UInt> * ele)
{
      assert(id1<meshPointer->getNumNodes());
      assert(id2<meshPointer->getNumNodes());
      assert(id3<meshPointer->getNumNodes());
      
      // pulisco il vettore in input
      ele->clear();
      ele->reserve(conn.getNodeToElementPointer(id1)->getNumConnected());
      
      // varaibili che userò
      set<UInt>     connessi1,connessi2;
      set<UInt>::iterator            it;
      
      // ciclo sugli elementi connessi a id1
      for(UInt i=0; i<conn.getNodeToElementPointer(id1)->getNumConnected(); ++i)
	  connessi1.insert(conn.getNodeToElementPointer(id1)->getConnectedId(i));
      
      for(UInt i=0; i<conn.getNodeToElementPointer(id2)->getNumConnected(); ++i)
	  connessi2.insert(conn.getNodeToElementPointer(id2)->getConnectedId(i));
      
      // ciclo sugli elementi connessi a id2 se lo trovo lo metto nella lista
      for(UInt i=0; i<conn.getNodeToElementPointer(id3)->getNumConnected(); ++i)  
	  if((connessi1.find(conn.getNodeToElementPointer(id3)->getConnectedId(i))!=connessi1.end()) &&
	     (connessi2.find(conn.getNodeToElementPointer(id3)->getConnectedId(i))!=connessi2.end()) )
		  ele->push_back(conn.getNodeToElementPointer(id3)->getConnectedId(i));
}

void tricky3d<Tetra>::getElementAround(UInt id1, vector<UInt> * ids)
{
    // inizializzo il vettore 
    ids->assign(conn.getNodeToElementPointer(id1)->getNumConnected(), 0);
    
    // riempio il vettore 
    for(UInt i=0; i<ids->size(); ++i)	ids->at(i) = conn.getNodeToElementPointer(id1)->getConnectedId(i);
}


UInt tricky3d<Tetra>::getNumElementAround(UInt id1)
{
    return(conn.getNodeToElementPointer()->at(id1).getNumConnected());
}


void tricky3d<Tetra>::createStellata(UInt id1, vector<UInt> * ids)
{
      assert(id1<meshPointer->getNumNodes());
      
      // mi accerto che ids sia vuoto
      ids->clear();
      
      // variabile temporanea
      set<UInt>           tmp;
      set<UInt>::iterator  it;
      
      // ciclo sugli elementi connessi per creare la stellata
      for(UInt i=0; i<conn.getNodeToElementPointer(id1)->getNumConnected(); ++i)
	  for(UInt j=0; j<4; ++j)
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

void tricky3d<Tetra>::createStellataEdge(vector<UInt> * edge, vector<UInt> * ids)
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

void tricky3d<Tetra>::createStellataEdgeNode(vector<UInt> * edge, vector<UInt> * ids)
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
	    for(UInt k=0; k<4; ++k)      allTmp.insert(meshPointer->getElement(elemId).getConnectedId(k));
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


void tricky3d<Tetra>::createBigStellataEdge(vector<UInt> * edge, vector<UInt> * ids)
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


void tricky3d<Tetra>::createStellataBor(UInt id1, vector<UInt> * ids)
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

bool tricky3d<Tetra>::sameGeoIdAround(UInt id1)
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

bool tricky3d<Tetra>::sameGeoIdAround(UInt id1, UInt id2)
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

bool tricky3d<Tetra>::sameGeoIdAround(UInt id1, UInt id2, UInt id3)
{
      return(sameGeoIdAround(id1, id2) && sameGeoIdAround(id2, id3));
}

bool tricky3d<Tetra>::sameGeoIdAround(UInt id1, UInt id2, UInt id3, UInt id4)
{
      return(sameGeoIdAround(id1, id2) && sameGeoIdAround(id2, id3) && sameGeoIdAround(id3, id4));
}

void tricky3d<Tetra>::geoIdAround(UInt id1, vector<UInt> * geoIds)
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

//
// Metodi per esplorare la mesh partendo dai triangoli
//
bool tricky3d<Tetra>::isBoundary(vector<UInt> * edge)
{
    // varaibili temporanee
    geoElement<Line>  lin;
    
    // setto la linea
    lin.setConnectedId(0, edge->at(0));
    lin.setConnectedId(1, edge->at(1));
    
    // la cerco e vedo il risultato 
    return(bordo.find(lin)!=bordo.end());
}

bool tricky3d<Tetra>::isElementDegenerate(UInt elemId)
{
    // variabili in uso
    set<UInt> ids;
    
    // inserisco gli id
    ids.insert(meshPointer->getElement(elemId).getConnectedId(0));
    ids.insert(meshPointer->getElement(elemId).getConnectedId(1));
    ids.insert(meshPointer->getElement(elemId).getConnectedId(2));
    ids.insert(meshPointer->getElement(elemId).getConnectedId(3));
    
    return(ids.size()!=4);
}

void tricky3d<Tetra>::setElementDegenerate(UInt elemId)
{
    // inserisco gli id
    meshPointer->getElementPointer(elemId)->setConnectedId(0, 0);
    meshPointer->getElementPointer(elemId)->setConnectedId(1, 0);
    meshPointer->getElementPointer(elemId)->setConnectedId(2, 0);
    meshPointer->getElementPointer(elemId)->setConnectedId(3, 0);
}

Real tricky3d<Tetra>::orientation(UInt elemId)
{
    // prendo gli id 
    point p1 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(0));
    point p2 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(1));
    point p3 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(2));
    point p4 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(3));
    point n  = (p2-p1)^(p3-p1);
    
    return(n*(p4-p1));
}

void tricky3d<Tetra>::stellataTetra(UInt elemId, vector<UInt> * ele)
{
    assert(elemId<meshPointer->getNumElements());
    
    // varaili in gioco
    set<UInt>         tutti;
    set<UInt>::iterator  it;
    vector<UInt>        tmp;
    
    // per ogni lato 
    for(UInt i=0; i<4; ++i)
    {
	// prendo tutti gli elementi che hanno in comune un edge
	elementOnFace(meshPointer->getElement(elemId).getConnectedId((i%4)), 
		      meshPointer->getElement(elemId).getConnectedId((i+1)%4),
		      meshPointer->getElement(elemId).getConnectedId((i+2)%4), &tmp);
    
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

UInt tricky3d<Tetra>::oppositeTetra(UInt id1, UInt id2, UInt id3, UInt elemId)
{	
      assert(id1<meshPointer->getNumNodes());
      assert(id2<meshPointer->getNumNodes());
      assert(id3<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // variabile in gioco 
      vector<UInt>       tmp;
      
      // cerco gli elementi che insistono sull'edge
      elementOnFace(id1, id2, id3, &tmp);
      
      // riempio il vettore ids
      for(UInt i=0; i<tmp.size(); ++i)
	  // se è diverso 
	  if(tmp[i]!=elemId)	
	      return(tmp[i]);
	  
      // stampa dell'errore
      cout << "ATTENZIONE: il metodo oppositeTetra non ha funzionato bene" << endl;
      
      // ritorno un valore fittizio
      return(0);
}

bool tricky3d<Tetra>::sameGeoId(UInt elemId1, UInt elemId2)
{
      assert(elemId1<meshPointer->getNumElements());
      assert(elemId2<meshPointer->getNumElements());
      
      return (meshPointer->getElement(elemId1).getGeoId()==meshPointer->getElement(elemId2).getGeoId());
}

void tricky3d<Tetra>::commonFace(UInt elemId1, UInt elemId2, vector<UInt> * ids)
{
      assert(elemId1<meshPointer->getNumElements());
      assert(elemId2<meshPointer->getNumElements());
      
      // varaibili in uso
      vector<UInt>       ids1(4);
      vector<UInt>       ids2(4);
      vector<UInt>     common(6);
      vector<UInt>::iterator  it;
      
      // pulisco ids
      ids->clear();
      
      // metto tutto nelle variabili
      ids1[0] = meshPointer->getElement(elemId1).getConnectedId(0);
      ids1[1] = meshPointer->getElement(elemId1).getConnectedId(1);
      ids1[2] = meshPointer->getElement(elemId1).getConnectedId(2);
      ids1[3] = meshPointer->getElement(elemId1).getConnectedId(3);
      
      ids2[0] = meshPointer->getElement(elemId2).getConnectedId(0);
      ids2[1] = meshPointer->getElement(elemId2).getConnectedId(1);
      ids2[2] = meshPointer->getElement(elemId2).getConnectedId(2);
      ids2[3] = meshPointer->getElement(elemId2).getConnectedId(3);
      
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

bool tricky3d<Tetra>::jumpGeoIdTetra(UInt elemId)
{
      // controllo 
      assert(elemId<meshPointer->getNumElements());
      
      // varaibili in uso 
      vector<UInt>	ele;
      set<UInt>	     geoIds;
      
      // prendo la stellata 
      stellataTetra(elemId, &ele);
      
      // controllo i geoId 
      for(UInt i=0; i<ele.size(); ++i)	geoIds.insert(meshPointer->getElement(ele[i]).getGeoId());
      geoIds.insert(meshPointer->getElement(elemId).getGeoId());
      
      // controllo il size se è diverso da 1 vuol dire che c'è un salto 
      return(geoIds.size()!=1);
}

//
// Metodi per fare alcuni calcoli sui punti della mesh
//

Real tricky3d<Tetra>::angolo(UInt nodeId, UInt elemId)
{
      assert(nodeId<meshPointer->getNumNodes());
      assert(elemId<meshPointer->getNumElements());
      
      // varaibili
      Real 			     num,den;
      tensor 		                   T;
      vector<UInt>                  ids(3,0);
      vector<Real>    ang(3,0.0),dist(3,0.0);
      
      // trovo gli altri due nodi 
      lastNode(nodeId, elemId, &ids);
      
      // calcolo i vettori
      point v1 = (meshPointer->getNode(nodeId)-meshPointer->getNode(ids[0]));
      point v2 = (meshPointer->getNode(nodeId)-meshPointer->getNode(ids[1]));
      point v3 = (meshPointer->getNode(nodeId)-meshPointer->getNode(ids[2]));
      
      // creo una matrice con i vettori per riga
      T.setRow(0, v1);
      T.setRow(1, v2);
      T.setRow(2, v3);
      
      // ----------------------------------------------------------------------
      // calcolo dell'angolo diedrale tramite la formula di Oosterom e Strackee 
      // ----------------------------------------------------------------------
      
      // calcolo delle distanze 
      dist[0] = v1.norm2();
      dist[1] = v2.norm2();
      dist[2] = v3.norm2();
      
      // calcolo degli angoli 
      ang[0] = v2*v3;
      ang[1] = v3*v1;
      ang[2] = v1*v2;
      
      // calcolo il numeratore 
      num  = T.getThirdInvariant();
      
      // calcolo del valore dell'arcotangente 
      den = accumulate(dist.begin(),dist.end(),1.0,multiplies<Real>())+inner_product(dist.begin(),dist.end(),ang.begin(),0.0);
      
      // ritorno il valore
      return(2.*atan(num/den));
}

//
// Metodi per fare alcuni calcoli sui triangloli della mesh
//

Real tricky3d<Tetra>::getTetraVolume(UInt elemId)
{
      	// variabili temporanee
	point       p1,p2,p3,p4;
	
	// ricavo i punti
	p1 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(0));
	p2 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(1));
	p3 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(2));
	p4 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(3));
	
	// calcolo il volume 
	return(fabs((((p2-p4)^(p3-p4))*(p1-p4))/6.0));
}

Real tricky3d<Tetra>::getTetraVolumeOriented(UInt elemId)
{
      	// variabili temporanee
	point       p1,p2,p3,p4;
	
	// ricavo i punti
	p1 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(0));
	p2 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(1));
	p3 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(2));
	p4 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(3));
	
	// calcolo il volume 
	return((((p2-p1)^(p3-p1))*(p4-p1))/6.0);
}

Real tricky3d<Tetra>::getTetraPerimeter(UInt elemId)
{
      // variabili temporanee
      point         p1,p2,p3,p4;
      vector<Real>  lung(6,0.0);
	
      // ricavo i punti
      p1 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(0));
      p2 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(1));
      p3 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(2));
      p4 = meshPointer->getNode(meshPointer->getElement(elemId).getConnectedId(3));
	
      // ricavo le lunghezze
      lung[0] = (p1-p2).norm2();
      lung[1] = (p1-p3).norm2();
      lung[2] = (p1-p4).norm2();
      lung[3] = (p2-p3).norm2();
      lung[4] = (p2-p4).norm2();
      lung[5] = (p3-p4).norm2();
      
      // sommo tutto e ritorno 
      return(accumulate(lung.begin(), lung.end(), 1.0, multiplies<Real>()));
}

void tricky3d<Tetra>::getEdges(UInt elemId, vector<vector<UInt>  > * edges)
{
    assert(elemId<meshPointer->getNumElements());
    
    // varaibili in uso 
    UInt id1,id2,id3,id4;
    
    // faccio un clear e resize
    edges->clear();
    edges->resize(6);
    for(UInt i=0; i<6; ++i)	edges->at(i).resize(2);
    
    // prendo gli id 
    id1 = meshPointer->getElement(elemId).getConnectedId(0);
    id2 = meshPointer->getElement(elemId).getConnectedId(1);
    id3 = meshPointer->getElement(elemId).getConnectedId(2);
    id4 = meshPointer->getElement(elemId).getConnectedId(3);
    
    // metto gli edge 
    edges->at(0)[0] = id1;    edges->at(0)[1] = id2;
    edges->at(1)[0] = id2;    edges->at(1)[1] = id3;
    edges->at(2)[0] = id3;    edges->at(2)[1] = id1;
    edges->at(3)[0] = id1;    edges->at(3)[1] = id4;
    edges->at(4)[0] = id2;    edges->at(4)[1] = id4;
    edges->at(5)[0] = id3;    edges->at(5)[1] = id4;
}

pair<vector<Real>, vector<vector<UInt> > > tricky3d<Tetra>::getMinEdge(UInt elemId)
{
      assert(elemId<meshPointer->getNumElements());
      
      // variadili in uso
      UInt 				       cont = 0;
      set<geoElementSize<Line> >  		  lista;
      geoElementSize<Line>         		    lin;
      pair<vector<Real>, vector<vector<UInt> > > result; 
      
      // faccio un resize 
      result.first.resize(6);
      result.second.resize(6);
      
      // vedo quanti lati ha il mio elemento
      UInt num     = Tetra::numEdges;
      UInt numConn = Tetra::numVerticesForEdges;
      
      // ciclo per creare tutti gli edge 
      for(UInt j=0; j<num*numConn; j=j+numConn)
      {	
	  // riempio la linea
	  for(UInt k=0; k<numConn; ++k)  
	    lin.setConnectedId(k, meshPointer->getElement(elemId).getConnectedId(Tetra::edgeConn[j+k]));
		
	  // setto il size
	  lin.setGeoSize((meshPointer->getNode(lin.getConnectedId(0))-meshPointer->getNode(lin.getConnectedId(1))).norm2());
	  
	  // la metto nella lista 
	  lista.insert(lin);
	  lin.setId(0);
      }
      
      // riempio l'output
      for(set<geoElementSize<Line> >::iterator it=lista.begin(); it!=lista.end(); ++it)
      {
	  // salvo il size
	  result.first[cont] = it->getGeoSize();
	  
	  // salvo gli estremi 
	  result.second[cont].resize(2,0);
	  result.second[cont][0] = it->getConnectedId(0);
	  result.second[cont][1] = it->getConnectedId(1);
	  
	  // incremento cont
	  ++cont;
      }
 
      // ritorno 
      return(result);
}

pair<vector<Real>, vector<vector<UInt> > > tricky3d<Tetra>::getMaxEdge(UInt elemId)
{	
      assert(elemId<meshPointer->getNumElements());
      
      // variadili in uso
      UInt 				       cont = 0;
      set<geoElementSize<Line> >  		  lista;
      geoElementSize<Line>         		    lin;
      pair<vector<Real>, vector<vector<UInt> > > result; 
      
      // faccio un resize 
      result.first.resize(6);
      result.second.resize(6);
      
      // vedo quanti lati ha il mio elemento
      UInt num     = Tetra::numEdges;
      UInt numConn = Tetra::numVerticesForEdges;
      
      // ciclo per creare tutti gli edge 
      for(UInt j=0; j<num*numConn; j=j+numConn)
      {	
	  // riempio la linea
	  for(UInt k=0; k<numConn; ++k)  
	    lin.setConnectedId(k, meshPointer->getElement(elemId).getConnectedId(Tetra::edgeConn[j+k]));
		
	  // setto il size
	  lin.setGeoSize((meshPointer->getNode(lin.getConnectedId(0))-meshPointer->getNode(lin.getConnectedId(1))).norm2());
	  
	  // la metto nella lista 
	  lista.insert(lin);
	  lin.setId(0);
      }
      
      // riempio l'output
      for(set<geoElementSize<Line> >::reverse_iterator it=lista.rbegin(); it!=lista.rend(); ++it)
      {
	  // salvo il size
	  result.first[cont] = it->getGeoSize();
	  
	  // salvo gli estremi 
	  result.second[cont].resize(2,0);
	  result.second[cont][0] = it->getConnectedId(0);
	  result.second[cont][1] = it->getConnectedId(1);
	  
	  // incremento cont
	  ++cont;
      }
 
      // ritorno 
      return(result);
}
