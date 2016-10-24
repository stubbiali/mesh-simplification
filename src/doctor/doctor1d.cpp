#include "doctor1d.h"

using namespace std;
using namespace geometry;

//
// Costruttori
//
doctor1d<Line>::doctor1d() : tricky1d<Line>()
{
}

doctor1d<Line>::doctor1d(mesh1d<Line> * _meshPointer) : tricky1d<Line>(_meshPointer)
{
}

//
// Metodi di supporto 
//
void doctor1d<Line>::refresh()
{
      // varaibili in uso 
      vector<point>                    tmpPt;
      vector<geoElement<Line> >        tmpLn;
      point                                p;
      geoElement<Line>	                 lin;
      vector<bool>    		      active;
      vector<UInt>       	       newId;
      UInt                  tmp,cont,id1,id2;
      Real                             x,y,z;
      vector<geoElement<Line> >  lineListTmp;
      
      // faccio un reserve
      active.reserve(meshPointer->getNumNodes());
      newId.reserve(meshPointer->getNumNodes());
      tmpPt.reserve(meshPointer->getNumNodes());
      tmpLn.reserve(meshPointer->getNumElements());
      
      // setto il vettore active che mi dice quali nodi sono attivi
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
      {
	    active.push_back(false);
	    newId.push_back(-1);
      }
      
      // vedo quali sono gli elementi attivi
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	    if(!isLineDegenerate(i))
	    {
		  id1 = meshPointer->getElement(i).getConnectedId(0);
		  id2 = meshPointer->getElement(i).getConnectedId(1);
		  
		  active[id1] = true;
		  active[id2] = true;
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
	    if(!isLineDegenerate(i))
	    {
		  id1 = newId[meshPointer->getElement(i).getConnectedId(0)];
		  id2 = newId[meshPointer->getElement(i).getConnectedId(1)];
		  tmp = meshPointer->getElement(i).getGeoId();
		  
		  // creo la mia nuova variabile triangolo
		  lin.setConnectedId(0,id1);
		  lin.setConnectedId(1,id2);
		  lin.setGeoId(tmp);
		  lin.setId(cont);
		  
		  // metto nella lista dei triangoli
		  tmpLn.push_back(lin);
	    }
      }
      
      // libero le liste
      meshPointer->clear();
      
      // riempio i nodi
      meshPointer->insertNode(&tmpPt);
      
      // riempio gli elementi
      meshPointer->insertElement(&tmpLn);
        
      // metto a posto gli id
      meshPointer->setUpIds();
      
      // faccio un setUp
      setUp();
}

//
// Raffinamento in base alla lunghezza
//
void doctor1d<Line>::refineUniform(Real lung)
{
      // Variabili in gioco
      Real 			  dif,toto;
      UInt                         numNodi;
      time_t  			 start,end;
      vector<point>        tmpNodi,newNodi;
      vector<geoElement<Line> >    newElem;
      
      // output
      cout << "Processo di raffinamento uniforme della mesh..." << endl;
      time(&start);
 
      // cerco di capire quanti punti dovrò aggiungere
      toto = getMeshLenght();
      
      // stimo quanti nodi devo aggiungere
      numNodi = static_cast<UInt>(floor(toto/lung))+1;
      
      // faccio un reserve per i punti
      newNodi.reserve(numNodi);
      
      // faccio un reserve gli elementi
      newElem.reserve(numNodi-1);
      
      // inserisco i nodi
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
      {
	  newNodi.push_back(meshPointer->getNode(i));
      }
      
      // ciclo sugli elementi
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	      // calcolo la lunghezza del segmento
	      toto = ( meshPointer->getNode(meshPointer->getElement(i).getConnectedId(0))-
	               meshPointer->getNode(meshPointer->getElement(i).getConnectedId(1))).norm2();
		
	      // se la lunghezza dell'elemento è troppo grande
	      if((toto>lung*(1.5)) && (fabs(toto-lung)>lung))
	      {     
		  // divido il segmento
		  divideLine(i, lung, toto, &newNodi, &newElem);
	      }
	      else
	      {
		 // inserisco l'elemento
		 newElem.push_back(meshPointer->getElement(i));
	      }
      }
      
      // rimetto a posto la mesh1d
      meshPointer->insertNode(&newNodi);
      meshPointer->insertElement(&newElem);
      
      // metto a posto gli id
      setUp();
      
      time(&end);
      dif = difftime(end,start);
      cout << "Processo di raffinamento uniforme della mesh completato: " <<  dif << " sec." << endl;
}

void doctor1d<Line>::divideLine(UInt id, Real lung, Real toto, vector<point> * newNodi, vector<geoElement<Line> > * newElem)
{
      // variabili in gioco
      point    dir,start,pTmp;
      Real       pezzo,tmpLun;
      geoElement<Line>    lin;
      
      // calcolo la lunghezza del segmento
      tmpLun = floor(toto/lung);
      pezzo  = lung + (toto-(tmpLun*lung))/tmpLun;
      
      // calcolo la direzione del segmento
      dir = (meshPointer->getNode(meshPointer->getElement(id).getConnectedId(1))-
             meshPointer->getNode(meshPointer->getElement(id).getConnectedId(0)));
      
      // normalizzo il vettore
      dir = dir / dir.norm2();
      
      // calcolo lo start
      start = meshPointer->getNode(meshPointer->getElement(id).getConnectedId(0));
      
      // inserisco il primo nodo
      newNodi->push_back(start + dir*pezzo);
      
      // faccio il primo segmento
      lin.setConnectedId(0, meshPointer->getElement(id).getConnectedId(0));
      lin.setConnectedId(1, (newNodi->size()-1));
      lin.setGeoId(meshPointer->getElement(id).getGeoId());
      
      // inserisco nella lista degli elementi
      newElem->push_back(lin);
      
      // inserisco i nodi nella lista dei nodi
      for(UInt i=2; i<static_cast<UInt>(tmpLun); ++i)
      {	
	    // calcolo il punto
	    pTmp = start + dir*i*pezzo;
	    
	    // lo metto nella lista il nuovo punto
	    newNodi->push_back(pTmp);
	    
	    // faccio il primo segmento
	    lin.setConnectedId(0, (newNodi->size()-2));
	    lin.setConnectedId(1, (newNodi->size()-1));
      
	    // inserisco nella lista degli elementi
	    newElem->push_back(lin);
      }
      
      // faccio l'ultimo segmento
      lin.setConnectedId(0, (newNodi->size()-1));
      lin.setConnectedId(1, meshPointer->getElement(id).getConnectedId(1));
      
      // inserisco nella lista degli elementi
      newElem->push_back(lin);
}

// 
// Metodo che aggiunge un punto in un segmento
// 
void doctor1d<Line>::splitEdge(UInt id, point p)
{
    // variabili in uso
    geoElement<Line>   lin;
    vector<UInt>  toChange;
    UInt               tmp;
    graphItem        lista;
    
    // inserisco il nodo nella lista
    p.setId(meshPointer->getNumNodes());
    meshPointer->insertNode(p);
    
    // salvo l'id del secondo estremo
    tmp = meshPointer->getElementPointer()->at(id).getConnectedId(1);
    
    // creo i due nuovi segmenti 
   
    // Modifico quello alla posizione id
    meshPointer->getElementPointer()->at(id).setConnectedId(1, (meshPointer->getNumNodes()-1));
    
    // Aggiungo l'altro in coda
    lin.setConnectedId(0, (meshPointer->getNumNodes()-1));
    lin.setConnectedId(1, tmp);
    lin.setGeoId(meshPointer->getElement(id).getGeoId());
    lin.setId(meshPointer->getNumElements());
    meshPointer->insertElement(lin);
    
    // -----------------------------------
    // sistemo le connessioni NodoElemento
    // -----------------------------------
    
    // sistemo la connessione del punto inserito
    lista.connectedResize(0);
    lista.connectedPushBack(id);
    lista.connectedPushBack(meshPointer->getNumElements()-1);
    conn.getNodeToElementPointer()->push_back(lista);
    
    // sistemo quella del punto "meshPointer->getElementPointer()->at(id).getConnectedId(1)"
    // 
    // perchè per il fatto che ho cambiato le connessioni dell'elemento id le connessioni del nodo 
    // "meshPointer->getElementPointer()->at(id).getConnectedId(0)" sono già ok
    for(UInt i=0; i<conn.getNodeToElementPointer(tmp)->getNumConnected(); ++i)
      // se trovo il punto tmp
      if(conn.getNodeToElementPointer(tmp)->getConnectedId(i)==id)
	    conn.getNodeToElementPointer(tmp)->setConnectedId(i, meshPointer->getNumElements()-1);
}

void doctor1d<Line>::collEdge(UInt id, point p)
{
      // variabili in uso
      UInt              id1,id2;
      vector<UInt>	   elem;
      vector<UInt>::iterator it;
      graphItem           lista;
      
      // prendo i nodi 
      id1 = meshPointer->getElement(id).getConnectedId(0);
      id2 = meshPointer->getElement(id).getConnectedId(1);
      
      // prendo i due nodi 
//       if((conn.getNodeToElementPointer(id1)->getNumConnected()>2) || (conn.getNodeToElementPointer(id2)->getNumConnected()>2))
//       {
// 	   cout << "Il nodo è triplo non può essere collassato!" << endl;
// 	   return;
//       }
      
      // faccio un reserve 
      elem.reserve(conn.getNodeToElementPointer(id1)->getNumConnected()+conn.getNodeToElementPointer(id2)->getNumConnected());
      
      // metto tutti i connessi 
      for(UInt i=0; i<conn.getNodeToElementPointer(id1)->getNumConnected(); ++i)
	elem.push_back(conn.getNodeToElementPointer(id1)->getConnectedId(i));
      
      for(UInt i=0; i<conn.getNodeToElementPointer(id2)->getNumConnected(); ++i)
	elem.push_back(conn.getNodeToElementPointer(id2)->getConnectedId(i));
      
      // elimino l'identificatore dell'elemento che devo collassare 
      sort(elem.begin(), elem.end());
      it = unique(elem.begin(), elem.end());
      elem.resize(distance(elem.begin(),it));
      
      // cerco quello in mezzo e lo elimino 
      it = find(elem.begin(), elem.end(), id);
      assert(it!=elem.end());
      elem.erase(it);
      
      // ------------------------
      //  sistemo la connessione
      // ------------------------
      lista.connectedResize(elem.size());
      for(UInt i=0; i<elem.size(); ++i)	lista.setConnectedId(i,elem[i]);
      conn.getNodeToElementPointer()->push_back(lista);
      
      // ---------------------------
      //  setto degenere l'elemento 
      // ---------------------------
      setLineDegenerate(id);
	
      //  per ogni elemento connesso a elem[i] setto che è connesso ai nuovi punti 
      for(UInt i=0; i<elem.size(); ++i)
      {
	  meshPointer->getElementPointer(elem[i])->change(id1, meshPointer->getNumNodes());
	  meshPointer->getElementPointer(elem[i])->change(id2, meshPointer->getNumNodes());
      }
      
      // -----------------------------
      //  aggiungo il punto alla fine 
      // -----------------------------
      p.setId(meshPointer->getNumNodes());
      meshPointer->insertNode(p);
}
