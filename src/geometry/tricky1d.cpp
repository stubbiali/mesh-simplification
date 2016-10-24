#include "tricky1d.h"

using namespace      std;
using namespace geometry;

//
// Costruttore 
//

tricky1d<Line>::tricky1d()
{
    // definizione dei puntatori e toll
    meshPointer    = NULL;
    toll           = 1e-15;
    pNull.setX(9.9e99);
    pNull.setY(9.9e99);
    pNull.setZ(9.9e99);
}
		      
tricky1d<Line>::tricky1d(mesh1d<Line> * _meshPointer)
{
    // varaibili temporanee
    UInt 		       id1;
    mesh0d<simplePoint>        bor;
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
	
	// lo metto nel set 
	bordo.insert(borToSurf[id1]);
    }
}

//
// Set delle variabili di ambiente
//

void tricky1d<Line>::setMeshPointer(mesh1d<Line> * _meshPointer)
{
    // varaibili temporanee
    UInt 		       id1;
    mesh0d<simplePoint>        bor;
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
	
	// lo metto nel set 
	bordo.insert(borToSurf[id1]);
    }
}

void tricky1d<Line>::setToll(Real _toll)
{
      toll = _toll;
}

Real tricky1d<Line>::getToll()
{
      return(toll);
}

void tricky1d<Line>::setUp()
{
    // faccio un setUp della mesh in input
    meshPointer->setUpIds();
    
    // varaibili temporanee
    UInt 		       id1;
    mesh0d<simplePoint>        bor;
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
	
	// lo metto nel set 
	bordo.insert(borToSurf[id1]);
    }
}

//
// Metodi per analizzare la mesh 
//
bool tricky1d<Line>::isNodeDegenerate(UInt nodeId)
{
      assert(nodeId<meshPointer->getNumNodes());
      return(meshPointer->getNode(nodeId)==pNull);
}

void tricky1d<Line>::setNodeDegenerate(UInt nodeId)
{
      assert(nodeId<meshPointer->getNumNodes());
      meshPointer->getNodePointer(nodeId)->setX(pNull.getX());
      meshPointer->getNodePointer(nodeId)->setY(pNull.getY());
      meshPointer->getNodePointer(nodeId)->setZ(pNull.getZ());
}

bool tricky1d<Line>::isLineDegenerate(UInt elemId)
{
    // inserisco gli id
    return((meshPointer->getElement(elemId).getConnectedId(0)==0) && (meshPointer->getElement(elemId).getConnectedId(1)==0));
}

void tricky1d<Line>::setLineDegenerate(UInt elemId)
{
    // inserisco gli id
    meshPointer->getElementPointer(elemId)->setConnectedId(0, 0);
    meshPointer->getElementPointer(elemId)->setConnectedId(1, 0);
}

//
// Metodi per andare lungo una direzione specifica
//
void tricky1d<Line>::goOn(UInt start, UInt direzione, vector<UInt> * nodi)
{
    // Variabili in uso
    vector<UInt>	  ids;
    UInt            next,prec;
    
    // mi assicuro che element sia vuoto
    nodi->clear();
    
    // setto la variabile 
    prec  = start;
    next  = direzione;
    
    // faccio un reserve
    nodi->reserve(meshPointer->getNumNodes());
  
    while(next!=start)
    {
	  // inserisco i nodi
	  nodi->push_back(prec);
	  
	  // prendo i nodi della stellata 
	  createStellata(next, &ids);
	  
	  // a un certo punto arriverà a un nodo triplo se siamo su una righa
	  if(ids.size()!=2)
	  {
	      nodi->push_back(next);
	      break;
	  }
	  else
	  {
	      if(ids[0]!=prec)
	      {	
		  // scelgo le nuove variabili 
		  prec = next;
		  next = ids[0];
	      }
	      else
	      {
		  // scelgo le nuove variabili 
		  prec = next;
		  next = ids[1];
	      }
	  }
	  
	  // nel caso in cui ci sono i loop devo aggiungere tutti gli elementi che sono in prec perché ho già effettuato lo 
	  // scambio
	  // 
	  // N.B. metto anche start in modo tale che possa capire controllando il primo e l'ultimo elemento se sto trattando 
	  //      un loop
	  // 
	  if(next==start)
	  {
	       nodi->push_back(prec);
	       nodi->push_back(start);
	  }
     }
     
     // parte usata per il debug
     /*for(UInt i=0; i<element->size(); ++i)
	cout << element->at(i) << "  ";
      cout << endl;*/
}

void tricky1d<Line>::goOn(UInt start, UInt direzione, UInt end, vector<UInt> * nodi)
{
    // Variabili in uso
    vector<UInt>	  ids;
    UInt            next,prec;
    
    // mi assicuro che element sia vuoto
    nodi->clear();
    
    // setto la variabile 
    prec  = start;
    next  = direzione;
    
    // faccio un reserve
    nodi->reserve(meshPointer->getNumNodes());
  
    while(next!=end)
    {
	  // inserisco i nodi
	  nodi->push_back(prec);
	  
	  // prendo i nodi della stellata 
	  createStellata(next, &ids);
	  
	  // a un certo punto arriverà a un nodo triplo se siamo su una righa
	  if(ids.size()!=2)
	  {
	      nodi->push_back(next);
	      break;
	  }
	  else
	  {
	      if(ids[0]!=prec)
	      {	
		  // scelgo le nuove variabili 
		  prec = next;
		  next = ids[0];
	      }
	      else
	      {
		  // scelgo le nuove variabili 
		  prec = next;
		  next = ids[1];
	      }
	  }
     }

     // metto gli ultimi nodi 
     nodi->push_back(prec);
     nodi->push_back(end);
     
     // parte usata per il debug
     /*for(UInt i=0; i<element->size(); ++i)
	cout << element->at(i) << "  ";
      cout << endl;*/
}

void tricky1d<Line>::goOn(UInt start, vector<UInt> * nodi)
{
      // Variabili in uso
      vector<UInt>  ids,tmp1,tmp2;
      vector<UInt>::iterator   it;
      
      // mi assicuro che il vettore in input sia vuoto
      nodi->clear();
      
      // prendo i nodi della stellata 
      createStellata(start, &ids);
      
      // controllo che il punto dato non sia triplo o di bordo
      switch(ids.size())
      {
	case(2):
		// vado in una e nell'altra direzione
		goOn(start, ids[0], &tmp1);
		goOn(start, ids[1], &tmp2);
	    
		// se è vera questa cosa siamo in un loop
		if(((*tmp1.begin())==(*tmp1.rbegin())) && ((*tmp2.begin())==(*tmp2.rbegin())))
		{		
		    // resize
		    nodi->resize(static_cast<UInt>(tmp1.size()));
		  
		    // copio
		    copy(tmp1.begin(), tmp1.end(), nodi->begin());
		  
		    // ritorno
		    return;
		}
	    
		// faccio un resize
		nodi->resize(tmp2.size()-1);
	    
		// lo riempio al contrario
		reverse(tmp1.begin(), tmp1.end());
	    
		// faccio un resize di nodi
		nodi->resize(static_cast<UInt>(tmp1.size()+tmp2.size()-1));
	    
		// li aggiungo a tmp
		copy(tmp1.begin(),   tmp1.end(), nodi->begin());
		copy(tmp2.begin()+1, tmp2.end(), nodi->begin()+tmp1.size());
		
		// mi fermo 
		break;
	case(1):
		// se è connesso solo a un elemento vado in una sola direzione
		// proseguo in una direzione
		goOn(start, ids[0], nodi);
		break;
	case(0):
		cout << "ATTENZIONE: il nodo non è connesso a nulla" << endl;
		break;
	default:
		cout << "ATTENZIONE: il nodo è connesso a più di un elemento" << endl;
      }
}

void tricky1d<Line>::setUpList(vector<UInt> * nodi)
{  
      if((*nodi->begin())==(*nodi->rbegin()))
      {
	  // variabili in gioco
	  vector<UInt>::iterator prec,next,start;
		
	  // lo riempio al contrario
	  start = min_element(nodi->begin(), nodi->end());
		
	  // se start è all'inizio
	  if(start==nodi->begin())
	  {
		next = start+1;
		prec = start+nodi->size()-2;
	  }
	  else
	  {
		next = start+1;
		prec = start-1;
	  }
	  
	  // se prec è più piccolo di next creo il nuovo ciclo per identificare il loop
	  (*prec < *next) ? goOn(*start, *prec, nodi) : goOn(*start, *next, nodi);
      }
      else if((*nodi->begin())>(*nodi->rbegin()))
      {
	  // lo riempio al contrario
	  reverse(nodi->begin(), nodi->end());
      }
}

void tricky1d<Line>::sameDirection(vector<UInt> * correct, vector<UInt> * toTest)
{
    // variabili in uso 
    vector<UInt>              tmp,risultato;
    UInt                    start,dir1,dir2;
    UInt     startPos=0,dir1Pos=0,dir2Pos=0;
    bool        findStart,findDir1,findDir2;
    bool                             isLoop;
    
    // inizializzo start e dir
    start     = correct->at(0);
    dir1      = correct->at(1);
    dir2      = correct->at(2);
    findStart = false;
    findDir1  = false;
    findDir2  = false;
    
    // vedo se stiamo testando un loop
    if(toTest->at(0)==toTest->at(toTest->size()-1)) isLoop = true;
    else					    isLoop = false;
    
    // se devo testare un loop faccio in modo che non ci siano elementi doppi eliminando l'ultimo 
    if(isLoop)
    {
	// faccio un resize
	tmp.resize(toTest->size()-1);
	
	// e copio
	copy(toTest->begin(), toTest->begin()+toTest->size()-1, tmp.begin());
    }
    else
    {
        // faccio un resize
        tmp.resize(toTest->size());
	
	// e copio
	copy(toTest->begin(), toTest->end(), tmp.begin());
    }
    
    // ciclo per sapere se il vettore toTest è diretto nel verso giusto
    for(UInt i=0; i<tmp.size(); ++i)
    {
	// vedo se è start
	if(tmp[i]==start)
	{
	    findStart = true;
	    startPos  = i;
	}
	// vedo se è end
	if(tmp[i]==dir1)
	{
	    findDir1   = true;
	    dir1Pos    = i;
	}
	// vedo se è end
	if(tmp[i]==dir2)
	{
	    findDir2   = true;
	    dir2Pos    = i;
	}
	
	// se li ho trovati mi fermo 
	if(findDir2 && findDir1 && findStart)
	{
	    break;
	}
    }
    
    // oriento il vettore toTest
    if(dir1Pos<startPos || dir1Pos>dir2Pos)
    {
	reverse(tmp.begin(), tmp.end());
	
	// cambio il valore di startPos
	startPos = (tmp.size()-1)-startPos;
    }
    
    // ruoto il vettore 
    rotate(tmp.begin(), tmp.begin()+static_cast<int>(startPos), tmp.end());
    
    // sistemo il vettore di output
    if(isLoop)
    {
	// faccio un reserve
	toTest->clear();
	toTest->reserve(tmp.size()+1);
	
	// metto in toTest
	for(UInt i=0; i<tmp.size(); ++i) toTest->push_back(tmp[i]);
	
	// dato che è un loop mettiamo il primo indice uguale all'ultimo
	toTest->push_back(tmp[0]);
	
    }
    else
    {
	// faccio un resize
	toTest->resize(tmp.size());
	
	// copio 
	copy(tmp.begin(), tmp.end(), toTest->begin());
    }
    
}

void tricky1d<Line>::setUpMeshOrientation(vector<UInt> * nodi)
{
    // varaibili in uso 
    geoElement<Line>	elem;
    
    for(UInt i=0; i<(nodi->size()-1); ++i)
    {
	// prendo tutti i connessi 
	for(UInt j=0; j<conn.getNodeToElementPointer(nodi->at(i))->getNumConnected(); ++j)
	{
	    // prendo l'elemento 
	    elem = meshPointer->getElement(conn.getNodeToElementPointer(nodi->at(i))->getConnectedId(j));
	    
	    // se uno dei due nodi è quello successivo mi fermo 
	    if(elem.getConnectedId(0)==nodi->at(i+1) || elem.getConnectedId(1)==nodi->at(i+1))	break;
	}
	
	// cabio le connessioni 
	meshPointer->getElementPointer(elem.getId())->setConnectedId(0, nodi->at(i));
	meshPointer->getElementPointer(elem.getId())->setConnectedId(1, nodi->at(i+1));
    }
}

//
// Metodi che permettono di avere delle informazioni topologiche sui nodi 
//
UInt tricky1d<Line>::getElementBetween(UInt id1, UInt id2)
{
    // variabili in uso 
    vector<UInt>      comm;
    
    // trovo quale è il comune
    conn.getNodeToElementPointer(id1)->common(*conn.getNodeToElementPointer(id2), &comm);
    
    // if per controllare le uscite
    if(comm.size()==1)		return((*comm.begin()));
    else if(comm.size()>1)	cout << "Attenzione ho trovato " << comm.size() <<" elementi fra " << id1 << "  " << id2 << endl;
    else			cout << "Non ho trovato gli elementi comuni a " << id1 << "  " << id2 << endl;
    
    // ritorno il numero degli elementi
    return(meshPointer->getNumElements());
}

UInt tricky1d<Line>::getNumElementAround(UInt id1)
{
    return(conn.getNodeToElementPointer(id1)->getNumConnected());
}

void tricky1d<Line>::createStellata(UInt id1, vector<UInt> * ids)
{
      assert(id1<meshPointer->getNumNodes());
      
      // mi accerto che ids sia vuoto
      ids->clear();
      
      // variabile temporanea
      set<UInt>           tmp;
      set<UInt>::iterator  it;
      
      // ciclo sugli elementi connessi per creare la stellata
      for(UInt i=0; i<conn.getNodeToElementPointer(id1)->getNumConnected(); ++i)
	  for(UInt j=0; j<2; ++j)
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

void tricky1d<Line>::createStellataBor(UInt id1, vector<UInt> * ids)
{
      assert(id1<meshPointer->getNumNodes());
      
      // variabili temporanee
      vector<UInt>	     idsTmp;
      
      // creo la stellata 
      createStellata(id1, &idsTmp);
      
      // mi assicuro che ids sia ok
      ids->clear();
      ids->reserve(idsTmp.size());
      
      // controllo che ci sia il connesso 
      for(UInt i=0; i<idsTmp.size(); ++i)
	  if(bordo.find(idsTmp[i])!=bordo.end())	ids->push_back(idsTmp[i]);
      
}

//
// Metodi che permettono di valutare determinate quantità
//
Real tricky1d<Line>::getElementLenght(UInt id1, UInt id2)
{
    // prendo l'elemento che condividono
    UInt elem = getElementBetween(id1, id2);
    
    // se esiste l'elemento che unisce i punti ritono la lunghezza
    if(elem<meshPointer->getNumElements())
	 return(meshPointer->getNode(id1)-meshPointer->getNode(id2)).norm2();
    
    // altrimenti stampo un errore
    cout << "I nodi " << id1 << " e " << id2 << " non sono connessi" << endl;
    return(0.0);
}

Real tricky1d<Line>::getMeshLenght()
{
      // setto la variabile lun
      Real lung = 0.0;
      
      // aggiungo la lunghezza di ogni pezzo della mesh 
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	   lung = lung + (meshPointer->getNode(meshPointer->getElement(i).getConnectedId(0))-
			  meshPointer->getNode(meshPointer->getElement(i).getConnectedId(1))).norm2();
      }
      
      // ritorno il valore calcolato
      return(lung);
}

Real tricky1d<Line>::getMeshLenght(vector<UInt> * ids)
{
    // variabili in uso 
    Real     lung = 0.0;
    
    // calcolo la lunghezza 
    for(UInt i=0; i<(ids->size()-1); ++i) lung = lung + getElementLenght(ids->at(i),ids->at(i+1));
    
    // ritorno il valore
    return lung;
      
}

Real tricky1d<Line>::getArea(vector<UInt> * ids)
{
    // variabili in uso 
    vector<Real>            x,y;
    Real    area1=0.0,area2=0.0;
    
    // riempio i due vettori
    x.reserve(ids->size());
    y.reserve(ids->size());
    
    // riempio le coordinate
    for(UInt i=0; i<ids->size(); ++i)
    {
	x.push_back(meshPointer->getNode(ids->at(i)).getX());
	y.push_back(meshPointer->getNode(ids->at(i)).getY());
    }
    
    // faccio le somme usando i mitici algoritmi dell'stl
    area1 = inner_product(x.begin(), (x.begin()+x.size()-1), y.begin()+1, 0.0);
    area2 = inner_product(y.begin(), (y.begin()+y.size()-1), x.begin()+1, 0.0);
     
    return(fabs(area1-area2)*0.5);
}



