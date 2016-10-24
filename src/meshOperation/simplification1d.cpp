#include "simplification1d.h"

using namespace std;
using namespace geometry;

//
// Costruttore 
//
simplification1d<Line>::simplification1d() : doctor1d<Line>()
{
      // setto la tolleranza dell'handler
      hand.setToll(toll);
      vir.setToll(toll);
}

simplification1d<Line>::simplification1d(mesh1d<Line> * _meshPointer) : doctor1d<Line>(_meshPointer)
{  
      // setto la tolleranza dell'handler
      hand.setToll(toll);
      vir.setToll(toll);
}

void simplification1d<Line>::setMeshPointer(mesh1d<Line> * _meshPointer)
{
    // chiamo il setMeshPointer del dottore
    doctor1d<Line>::setMeshPointer(_meshPointer);
    
    // setto la tolleranza dell'handler
    hand.setToll(toll);
    vir.setToll(toll);
}

//
// Metodo per la divisione 
//
void simplification1d<Line>::findBarrier(mesh0d<simplePoint> * cloud)
{
    // variabili in uso 
    vector<UInt>	ids;
    
    // pulisco la variabile in input 
    cloud->clear();
    cloud->getNodePointer()->reserve(meshPointer->getNumNodes());
    
    // per ogni nodo controllo le connessioni 
    for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
    {
	  // vedo quanti nodi sono connessi 
	  createStellata(i, &ids);
	  
	  // se è maggiore di 2 lo metto in cluod  
	  if(ids.size()>2)	cloud->insertNode(meshPointer->getNode(i));
    }
    
    // faccio un setUp se ho messo nodi 
    if(cloud->getNumNodes()!=0)		cloud->setUpElement();
}

void simplification1d<Line>::findPieces(vector<vector<UInt> > * pieces)
{
    // varaibili in uso 
    UInt		      num;
    map<UInt,UInt>	 extToInt;
    mesh1d<Line>      tmpMesh,tmp;
    tricky1d<Line>	    trick;
    mesh0d<simplePoint>   barrier;
    
    // copio in tmpMesh
    tmpMesh = *meshPointer;
    
    // trovo i punti da non valicare 
    findBarrier(&barrier);
    
    // setto il virus e diffondo la malattia 
    if(barrier.getNumNodes()!=0)
    {
	vir.setMeshPointer(&tmpMesh);
	num = vir.virusDiffusion(&barrier);
    }
    else
    {
	tmpMesh.setUpGeoId(1);
	num = 1;
    }
    
    // faccio un resize 
    pieces->resize(num);    
    
    // setto l'handler 
    hand.setMeshPointer(&tmpMesh);
    
    // per ogni id estraggo la sequenza 
    for(UInt i=0; i<num; ++i)
    {
	
	// estraggo la mesh 
	hand.extractMesh(&tmp, i+1);
	
	// prendo la mesh faccio le connessioni 
	trick.setMeshPointer(&tmp);
	
	// creo la lista 
	trick.goOn(0, &pieces->at(i));
	
	// la sistemo 
	trick.setUpList(&pieces->at(i));
	
	// faccio la mappa 
	hand.createMap(tmp.getNodePointer(), &extToInt);
	
	// sistemo i pezzi 
	for(UInt j=0; j<pieces->at(i).size(); ++j)	pieces->at(i)[j] = extToInt[pieces->at(i)[j]];
    }
      
}

//
// Metodi per il controllo dell'apparteneza al cilindro 
//
Real simplification1d<Line>::distace(UInt id1, UInt id2, UInt toTest)
{
      // varaibili in uso 
      point 	      p1,p2,pExt,dir;
      
      // prendo i punti 
      p1   = meshPointer->getNode(id1);
      p2   = meshPointer->getNode(id2);
      pExt = meshPointer->getNode(toTest)-p1;
      
      // prendo la direzione 
      dir  = (p2-p1);
      dir.normalize();
      
      
      return((dir*(dir*pExt)-pExt).norm2());
      
}

inline bool simplification1d<Line>::isInCylinder(UInt id1, UInt id2, UInt toTest, Real limite)
{
      return(distace(id1, id2, toTest)<(limite+toll));
}

//
// Metodo che fa la semplificazione 
//
void simplification1d<Line>::simplificate(Real limite)
{
      // varaibili in uso 
      bool	  	    firstTime;
      UInt  	   id1,id2,idNew,edge;
      UInt  	 idStart,idStartNew=0;
      point 		  	    p;
      vector<UInt>                tmp;
      vector<vector<UInt> >    pieces;
      
      // prendo i pezzi 
      findPieces(&pieces);
      
      // ciclo sui pezzi 
      for(UInt i=0; i<pieces.size(); ++i)
      {
	    // salvo il vettore 
	    tmp = pieces[i];
	    
	    // deve avere almeno dimensione 3 se no è già semplificato al massimo 
	    if(tmp.size()>2)
	    {
		// inizializzo 
		id1       = tmp[0];
		id2       = tmp[1];
		idStart   = tmp[0];
		
		// questa variabile serve per vedere quale è la prima volta in cui il test incylinder fallisce 
		// in questo modo so in quale id è andato a finire idStart 
		firstTime = false;
		
		// ciclo sugli elementi 
		for(UInt j=2; j<tmp.size(); ++j)
		{
		    // prendo il nodo da contro llare se è nel cilindro 
		    idNew = tmp[j];
		    
		    // se è dentro contraggo l'edge e aggiorno le variabili 
		    if(isInCylinder(id1, id2, idNew, limite))
		    {
			  // prendo p
			  p = meshPointer->getNode(id1);
			  
			  // trovo l'edge in comune 
			  edge = getElementBetween(id1,id2);
			  
			  // contraggo 
			  collEdge(edge, p);
			  
			  // aggiorno id2 e id1
			  id2 = idNew;
			  
			  // aggiorno id1 perché ho sistemato le connessioni con l'ultimo della lista!!!
			  id1 = meshPointer->getNumNodes()-1;  
			  
		    }
		    else// non si contrae nulla ma si aggiorna il segmento 
		    {
			  // aggiorno idStartNew
			  if(!firstTime)
			  {
			      idStartNew = id1;
			      firstTime  = true;
			  }
			  
			  // prendo il nuovo segmento 
			  id1 = id2;
			  id2 = idNew; 			  
		    }	
		}
		
		
		// se ho contratto tutti non entro nell'else di prima quindi devo aggiornare la variabile idStartNew
		// a mano
		if(!firstTime) idStartNew = id1;
		
		// sistemo gli altri elementi di pieces 
		for(UInt j=i; j<pieces.size(); ++j)
		{
		    if(pieces[j][0]==idStart)			pieces[j][0] 		        = idStartNew;
		    if(pieces[j][pieces[j].size()-1]==idStart)	pieces[j][pieces[j].size()-1]   = idStartNew;
		}
	    }
      }
      
      // faccio un refresh 
      refresh();
      
}



