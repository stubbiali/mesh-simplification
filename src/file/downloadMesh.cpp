#include "downloadMesh.h"

using namespace geometry;


//
// Costruttore di default
//

downloadMesh::downloadMesh()
{
}

// ----------------------------
//      FILE DA PARAVIEW
// ----------------------------

void downloadMesh::fileFromParaview(string s, mesh0d<simplePoint> * mesh)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      string                   str;
      UInt     numNode,numElem,tmp;
      UInt                     id1;
      Real                   x,y,z;
      point                      p;
      geoElement<simplePoint>   pt;
      
      // Ottengo le informazioni generali
      file >> numNode;
      file >> numElem;
      file >> tmp;	file >> tmp;	file >> tmp;
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numNode; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    file >> x;
	    file >> y;
	    file >> z;
	    
	    // definisco la variabile p
	    p.setX(x);	p.setY(y);  p.setZ(z);
	    p.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertNode(p);
      }
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numElem; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    file >> tmp;
	    file >> str;
	    file >> id1;
	    
	    // definisco la variabile tria
	    pt.setConnectedId(0, id1-1);
	    pt.setGeoId(tmp);
	    pt.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertElement(pt);
      }
      
      // metto a posto gli id
      mesh->setUpIds();
      
      file.close();
}

void downloadMesh::fileFromParaview(string s, mesh1d<Line> * mesh)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      string                   str;
      UInt     numNode,numElem,tmp;
      UInt                 id1,id2;
      Real                   x,y,z;
      point                      p;
      geoElement<Line>         lin;
      
      // Ottengo le informazioni generali
      file >> numNode;
      file >> numElem;
      file >> tmp;	file >> tmp;	file >> tmp;
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numNode; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    file >> x;
	    file >> y;
	    file >> z;
	    
	    // definisco la variabile p
	    p.setX(x);	p.setY(y);  p.setZ(z);
	    p.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertNode(p);
      }
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numElem; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    file >> tmp;
	    file >> str;
	    file >> id1;
	    file >> id2;
	    
	    // definisco la variabile tria
	    lin.setConnectedId(0, id1-1);
	    lin.setConnectedId(1, id2-1);
	    lin.setGeoId(tmp);
	    lin.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertElement(lin);
      }
      
      // metto a posto gli id
      mesh->setUpIds();
      
      file.close();
}

void downloadMesh::fileFromParaview(string s, mesh2d<Triangle> * mesh)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      string                   str;
      UInt     numNode,numElem,tmp;
      UInt             id1,id2,id3;
      Real                   x,y,z;
      point                      p;
      geoElement<Triangle>    tria;
      
      // Ottengo le informazioni generali
      file >> numNode;
      file >> numElem;
      file >> tmp;	file >> tmp;	file >> tmp;
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numNode; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    file >> x;
	    file >> y;
	    file >> z;
	    
	    // definisco la variabile p
	    p.setX(x);	p.setY(y);  p.setZ(z);
	    p.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertNode(p);
      }
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numElem; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    file >> tmp;
	    file >> str;
	    file >> id1;
	    file >> id2;
	    file >> id3;
	    
	    // definisco la variabile tria
	    tria.setConnectedId(0, id1-1);
	    tria.setConnectedId(1, id2-1);
	    tria.setConnectedId(2, id3-1);
	    tria.setGeoId(tmp);
	    tria.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertElement(tria);
      }
      
      // metto a posto gli id
      mesh->setUpIds();
      
      file.close();
}

void downloadMesh::fileFromParaview(string s, mesh3d<Tetra> * mesh)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      string                   str;
      UInt     numNode,numElem,tmp;
      UInt         id1,id2,id3,id4;
      Real                   x,y,z;
      point                      p;
      geoElement<Tetra>        tet;
      
      // Ottengo le informazioni generali
      file >> numNode;
      file >> numElem;
      file >> tmp;	file >> tmp;	file >> tmp;
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numNode; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    file >> x;
	    file >> y;
	    file >> z;
	    
	    // definisco la variabile p
	    p.setX(x);	p.setY(y);  p.setZ(z);
	    p.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertNode(p);
      }
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numElem; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    file >> tmp;
	    file >> str;
	    file >> id1;
	    file >> id2;
	    file >> id3;
	    file >> id4;
	    
	    // definisco la variabile tria
	    tet.setConnectedId(0, id1-1);
	    tet.setConnectedId(1, id2-1);
	    tet.setConnectedId(2, id3-1);
	    tet.setConnectedId(3, id4-1);
	    tet.setGeoId(tmp);
	    tet.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertElement(tet);
      }
      
      // metto a posto gli id
      mesh->setUpIds();
      
      file.close();
}

void downloadMesh::fileFromParaviewNodePropriety(string s, mesh2d<Triangle> * mesh, vector<Real> * ris)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      string                   str;
      UInt     numNode,numElem,tmp;
      UInt             id1,id2,id3;
      Real                   x,y,z;
      point                      p;
      geoElement<Triangle>    tria;
      
      // Ottengo le informazioni generali
      file >> numNode;
      file >> numElem;
      file >> tmp;	file >> tmp;	file >> tmp;
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numNode; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    file >> x;
	    file >> y;
	    file >> z;
	    
	    // definisco la variabile p
	    p.setX(x);	p.setY(y);  p.setZ(z);
	    p.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertNode(p);
      }
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numElem; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    file >> tmp;
	    file >> str;
	    file >> id1;
	    file >> id2;
	    file >> id3;
	    
	    // definisco la variabile tria
	    tria.setConnectedId(0, id1-1);
	    tria.setConnectedId(1, id2-1);
	    tria.setConnectedId(2, id3-1);
	    tria.setGeoId(tmp);
	    tria.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertElement(tria);
      }
      
      // metto a posto gli id
      mesh->setUpIds();
      
      // salto le righe
      file >> tmp;
      file >> tmp;
      file >> str;
      file >> str;
      
      // faccio un resize di ris
      ris->resize(numNode);
      
      // riempio ris 
      for(UInt i=0; i<numNode; ++i)
      {
	  file >> tmp;
	  file >> ris->at(i);
      }
      
      file.close();
}

// ----------------
//     FILE .OFF
// ----------------
void downloadMesh::fileFromOffFormat(string s, mesh2d<Triangle> * mesh)
{	
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      string                   str;
      UInt     numNode,numElem,tmp;
      UInt             id1,id2,id3;
      Real                   x,y,z;
      point                      p;
      geoElement<Triangle>    tria;
      
      // Ottengo le informazioni generali
      file >> str;
      file >> numNode;
      file >> numElem;
      file >> tmp;
      
      // faccio dei reserve 
      mesh->getNodePointer()->reserve(numNode);
      mesh->getElementPointer()->reserve(numElem);
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numNode; ++i)
      {
	    // prendo le informazioni
	    file >> x;
	    file >> y;
	    file >> z;
	    
	    // definisco la variabile p
	    p.setX(x);	p.setY(y);  p.setZ(z);
	    p.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertNode(p);
      }
      
      // Ricavo le informazioni dei nodi
      for(UInt i=0; i<numElem; ++i)
      {
	    // prendo le informazioni
	    file >> tmp;
	    
	    // assert per essere sicuro che sto trattando dei triangoli
	    if(tmp!=3)
	    {
		cout << "Il lettore deve prendere in ingresso solo triangoli" << endl;
		assert(1==0);
	    }
	    
	    // salvo gli identificatoi 
	    file >> id1;
	    file >> id2;
	    file >> id3;
	    
	    // definisco la variabile tria
	    tria.setConnectedId(0, id1);
	    tria.setConnectedId(1, id2);
	    tria.setConnectedId(2, id3);
	    tria.setGeoId(0);
	    tria.setId(i);
	    
	    // la metto nella mesh
	    mesh->insertElement(tria);
      }
}

// ----------------------------
//      FILE DA NETGEN
// ----------------------------

void downloadMesh::fileFromNetgen(string s, mesh2d<Triangle> * mesh)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      vector<point>                   tmpNode;
      vector<geoElement<Triangle> >    tmpEle;
      set<UInt>                         oldId;
      map<UInt,UInt>                 oldToNew;
      string                              str;
      UInt                numNode,numElem,tmp;
      UInt                        id1,id2,id3;
      Real                              x,y,z;
      point                                 p;
      geoElement<Triangle>               tria;
      
      // Vedo il numero dei nodi
      file >> numNode;
      
      for(UInt i=0; i<numNode; ++i)
      {
	   // ricavo le informazioni
	   file >> x;
	   file >> y;
	   file >> z;
	   
	   // creo la variabile p
	   p.setX(x);	  p.setY(y);	p.setZ(z);
	   
	   // la metto in tmpNode
	   tmpNode.push_back(p);
      }
      
      // Vedo il numero degli elementi tetraedrici
      file >> numElem;
      
      for(UInt i=0; i<numElem; ++i)
      {
	  // Salto le informazioni
	  for(UInt j=0; j<5; ++j)
	  {
	      file >> tmp;
	  }
      }
      
      // Passo agli elementi triangolari
      file >> numElem;
      
      for(UInt i=0; i<numElem; ++i)
      {
	  // Ricavo le informazioni
	  file >> tmp;
	  file >> id1;
	  file >> id2;
	  file >> id3;
	  
	  tria.setConnectedId(0,id1-1);	
	  tria.setConnectedId(1,id2-1);	
	  tria.setConnectedId(2,id3-1);
	  tria.setGeoId(tmp);
	  
	  // la metto in una lista temporanea
	  tmpEle.push_back(tria);
	  
	  // inserisco nella lista oldId i vecchi id
	  oldId.insert(id1-1);
	  oldId.insert(id2-1);	
	  oldId.insert(id3-1);
      }
      
      // creo una mappa per la rinumerazione e inserisco i nodi nell'elenco puntato da mesh
      tmp = 0;
      for(set<UInt>::iterator it=oldId.begin(); it!=oldId.end(); ++it)
      {
	    // inserisco il nodo
	    mesh->insertNode(tmpNode[*it]);
	    
	    // creo l'associazione
	    oldToNew[*it] = tmp;
	    ++tmp;
      }
      
      // sistemo gli id dei vertici e li inserisco nella mesh
      for(UInt i=0; i<numElem; ++i)
      {
	    // considero l'elemento i-esimo
	    tria = tmpEle[i];
	    
	    // metto a posto i suoi id
	    id1 = oldToNew[tria.getConnectedId(0)];
	    id2 = oldToNew[tria.getConnectedId(1)];
	    id3 = oldToNew[tria.getConnectedId(2)];
	    
	    // lo inserisco nella mesh
	    tria.setConnectedId(0,id1);
	    tria.setConnectedId(1,id2);
	    tria.setConnectedId(2,id3);
	    
	    // lo metto nella lista mesh
	    mesh->insertElement(tria);
      }
      
      // metto a posto gli id
      mesh->setUpIds();
      
      file.close();
}

void downloadMesh::fileFromNetgen(string s, mesh2d<Triangle> * mesh, UInt geoId)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      vector<point>                   tmpNode;
      vector<geoElement<Triangle> >    tmpEle;
      set<UInt>                         oldId;
      map<UInt,UInt>                 oldToNew;
      string                              str;
      UInt                numNode,numElem,tmp;
      UInt                        id1,id2,id3;
      Real                              x,y,z;
      point                                 p;
      geoElement<Triangle>               tria;
      
      // Vedo il numero dei nodi
      file >> numNode;
      
      for(UInt i=0; i<numNode; ++i)
      {
	   // ricavo le informazioni
	   file >> x;
	   file >> y;
	   file >> z;
	   
	   // creo la variabile p
	   p.setX(x);	  p.setY(y);	p.setZ(z);
	   
	   // la metto in tmpNode
	   tmpNode.push_back(p);
      }
      
      // Vedo il numero degli elementi tetraedrici
      file >> numElem;
      
      for(UInt i=0; i<numElem; ++i)
      {
	  // Salto le informazioni
	  for(UInt j=0; j<5; ++j)
	  {
	      file >> tmp;
	  }
      }
      
      // Passo agli elementi triangolari
      file >> numElem;
      
      for(UInt i=0; i<numElem; ++i)
      {
	  // Ricavo le informazioni
	  file >> tmp;
	  file >> id1;
	  file >> id2;
	  file >> id3;
	  
	  if(tmp==geoId)
	  {	  
	      tria.setConnectedId(0,id1-1);	
	      tria.setConnectedId(1,id2-1);	
	      tria.setConnectedId(2,id3-1);
	      tria.setGeoId(tmp);
	   
	      // la metto in una lista temporanea
	      tmpEle.push_back(tria);
	  
	      // inserisco nella lista oldId i vecchi id
	      oldId.insert(id1-1);
	      oldId.insert(id2-1);	
	      oldId.insert(id3-1);
	  }
      }
      
      // creo una mappa per la rinumerazione e inserisco i nodi nell'elenco puntato da mesh
      tmp = 0;
      for(set<UInt>::iterator it=oldId.begin(); it!=oldId.end(); ++it)
      {
	    // inserisco il nodo
	    mesh->insertNode(tmpNode[*it]);
	    
	    // creo l'associazione
	    oldToNew[*it] = tmp;
	    ++tmp;
      }
      
      // sistemo gli id dei vertici e li inserisco nella mesh
      for(UInt i=0; i<tmpEle.size(); ++i)
      {
	    // considero l'elemento i-esimo
	    tria = tmpEle[i];
	    
	    // metto a posto i suoi id
	    id1 = oldToNew[tria.getConnectedId(0)];
	    id2 = oldToNew[tria.getConnectedId(1)];
	    id3 = oldToNew[tria.getConnectedId(2)];
	    
	    // lo inserisco nella mesh
	    tria.setConnectedId(0,id1);
	    tria.setConnectedId(1,id2);
	    tria.setConnectedId(2,id3);
	    
	    // lo metto nella lista mesh
	    mesh->insertElement(tria);
      }
      
      // metto a posto gli id
      mesh->setUpIds();
      
      file.close();
}

void downloadMesh::fileFromNetgen(string s, mesh3d<Tetra> * mesh)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      string                              str;
      UInt                numNode,numElem,tmp;
      UInt                    id1,id2,id3,id4;
      Real                              x,y,z;
      point                                 p;
      geoElement<Triangle>               tria;
      geoElement<Tetra>                   tet;
      
      // Vedo il numero dei nodi
      file >> numNode;
      
      for(UInt i=0; i<numNode; ++i)
      {
	   // ricavo le informazioni
	   file >> x;
	   file >> y;
	   file >> z;
	   
	   // creo la variabile p
	   p.setX(x);	  p.setY(y);	p.setZ(z);
	   
	   // la metto in tmpNode
	   mesh->insertNode(p);
      }
      
      // Vedo il numero degli elementi tetraedrici
      file >> numElem;
      
      for(UInt i=0; i<numElem; ++i)
      {
	  // ricavo le informazioni
	  file >> tmp;
	  file >> id1;
	  file >> id2;
	  file >> id3;
	  file >> id4;
	  
	  // Creo l'elemento temporaneo
	  tet.setGeoId(tmp);
	  tet.setId(i);
	  tet.setConnectedId(0,id1-1);
	  tet.setConnectedId(1,id2-1);
	  tet.setConnectedId(2,id3-1);
	  tet.setConnectedId(3,id4-1);
	  
	  // lo inserisco nella mesh
	  mesh->insertElement(tet);
      }
      
      // metto a posto gli id
      mesh->setUpIds();
      
      file.close();
}

void downloadMesh::fileFromNetgen(string s, mesh3d<Tetra> * mesh, UInt geoId)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      vector<point>                   tmpNode;
      vector<geoElement<Tetra> >       tmpEle;
      set<UInt>                         oldId;
      map<UInt,UInt>                 oldToNew;
      string                              str;
      UInt                numNode,numElem,tmp;
      UInt                    id1,id2,id3,id4;
      Real                              x,y,z;
      point                                 p;
      geoElement<Tetra>                   tet;
      
      // Vedo il numero dei nodi
      file >> numNode;
      
      for(UInt i=0; i<numNode; ++i)
      {
	   // ricavo le informazioni
	   file >> x;
	   file >> y;
	   file >> z;
	   
	   // creo la variabile p
	   p.setX(x);	  p.setY(y);	p.setZ(z);
	   
	   // la metto in tmpNode
	   tmpNode.push_back(p);
      }
      
      // Vedo il numero degli elementi tetraedrici
      file >> numElem;
      
      for(UInt i=0; i<numElem; ++i)
      {
	  // Ricavo le informazioni
	  file >> tmp;
	  file >> id1;
	  file >> id2;
	  file >> id3;
	  file >> id4;
	  
	  if(tmp==geoId)
	  {	  
	      tet.setConnectedId(0,id1-1);	
	      tet.setConnectedId(1,id2-1);	
	      tet.setConnectedId(2,id3-1);
	      tet.setConnectedId(3,id4-1);
	      tet.setGeoId(tmp);
	   
	      // la metto in una lista temporanea
	      tmpEle.push_back(tet);
	  
	      // inserisco nella lista oldId i vecchi id
	      oldId.insert(id1-1);
	      oldId.insert(id2-1);	
	      oldId.insert(id3-1);
	      oldId.insert(id4-1);
	  }
      }
      
      // creo una mappa per la rinumerazione e inserisco i nodi nell'elenco puntato da mesh
      tmp = 0;
      for(set<UInt>::iterator it=oldId.begin(); it!=oldId.end(); ++it)
      {
	    // inserisco il nodo
	    mesh->insertNode(tmpNode[*it]);
	    mesh->getNodePointer()->at(tmp).setId(tmp);
	    
	    // creo l'associazione
	    oldToNew[*it] = tmp;
	    ++tmp;
      }
      
      // sistemo gli id dei vertici e li inserisco nella mesh
      for(UInt i=0; i<tmpEle.size(); ++i)
      {
	    // considero l'elemento i-esimo
	    tet = tmpEle[i];
	    
	    // metto a posto i suoi id
	    id1 = oldToNew[tet.getConnectedId(0)];
	    id2 = oldToNew[tet.getConnectedId(1)];
	    id3 = oldToNew[tet.getConnectedId(2)];
	    id4 = oldToNew[tet.getConnectedId(3)];
	    
	    // lo inserisco nella mesh
	    tet.setConnectedId(0,id1);
	    tet.setConnectedId(1,id2);
	    tet.setConnectedId(2,id3);
	    tet.setConnectedId(3,id4);
	    tet.setId(i);
	    
	    // lo metto nella lista mesh
	    mesh->insertElement(tet);
      }
      
      // metto a posto gli id
      mesh->setUpIds();
      
      file.close();
}

// ----------------------------
//      FILE DA GOCAD
// ----------------------------
void downloadMesh::fileFromGocad(string s, mesh2d<Triangle> * mesh)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // Variabili in uso
      string               tmp;
      UInt          numNode(0);
      UInt           numEle(0);
      point                  p;
      UInt      id,id1,id2,id3;
      geoElement<Triangle> tri;
      Real               x,y,z;
      
      // salto l'header
      while(tmp!="VRTX")
      {
	  file >> tmp;
      }
      
      // conto i nodi
      while(tmp=="VRTX")
      {  
	   // salto tutto per sapere quanti sono i vertici
	   file >> tmp;
	   file >> tmp;
	   file >> tmp;
	   file >> tmp;
	   file >> tmp;
	   
	   // conto
	   ++numNode;
      }
      
      // conto i vertici 
      while(tmp=="TRGL")
      {
	   // salto tutto per sapere quanti sono i triangoli
	   file >> tmp;
	   file >> tmp;
	   file >> tmp;
	   file >> tmp;
	   
	   // conto
	   ++numEle;
      }
      
      // faccio un reserve dato che ora so quanti sono sia i veritici che gli elementi
      mesh->getNodePointer()->reserve(numNode);
      mesh->getElementPointer()->reserve(numEle);
      
      // ritorno all'inizio
      file.seekp(0);
      
      // salto l'header
      while(tmp!="VRTX")
      {
	  file >> tmp;
      }
      
      // setto l'id
      id=0;
      
      // inserisco i nodi
      while(tmp=="VRTX")
      {  
	   // prendo le informazioni
	   file >> tmp;
	   file >> x;
	   file >> y;
	   file >> z;
	   file >> tmp;
	
	   // setto l'id del punto e le sue coordinate
	   p.setId(id);
	   p.setX(x*0.0001);
	   p.setY(y*0.0001);
	   p.setZ(z*0.0001);
	   
	   // lo metto nella mesh
	   mesh->insertNode(p);
	  
	   // incremento id
	   ++id;
	   
      }
      
      // setto l'id
      id=0;
      
      // inserisco i triangoli
      while(tmp=="TRGL")
      {
	   // prendo le informazioni
	   file >> id1;
	   file >> id2;
	   file >> id3;
	   file >> tmp;
	   
	   // setto l'id e il geoId
	   tri.setId(id);
	   tri.setGeoId(0);
	   tri.setConnectedId(0, (id1-1));
	   tri.setConnectedId(1, (id2-1));
	   tri.setConnectedId(2, (id3-1));
	   
	   // lo metto nella mesh
	   mesh->insertElement(tri);
	   
	   // incremento id
	   ++id;
      }
      
      // setto gli id 
      mesh->setUpIds();
      
      // chiudo il file 
      file.close();
      
}

// ----------------------------
//      FILE DA MEDIT
// ----------------------------
void downloadMesh::fileFromMedit(string s, mesh2d<Triangle> * mesh)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // Variabili in uso
      string               tmp;
      UInt          numNode(0);
      UInt           numEle(0);
      point                  p;
      UInt  geo,id,id1,id2,id3;
      geoElement<Triangle> tri;
      Real               x,y,z;
      
      // salto l'header
      while(tmp!="Vertices")
      {
	  file >> tmp;
      }
      
      // setto i nodi
      file >> numNode;
      mesh->getNodePointer()->reserve(numNode);
      
      // setto l'id
      id=0;
      
      // inserisco i nodi
      for(UInt i=0; i<numNode; ++i)
      {  
	   file >> x;
	   file >> y;
	   file >> z;
	   file >> tmp;
	
	   // setto l'id del punto e le sue coordinate
	   p.setId(id);
	   p.setX(x);
	   p.setY(y);
	   p.setZ(z);
	   
	   // lo metto nella mesh
	   mesh->insertNode(p);
	  
	   // incremento id
	   ++id;
      }
      
      // setto l'id
      file >> tmp;
      
      // cerco la parte che ha i tirnagoli
      while(tmp!="Triangles")
      {
	  file >> tmp;
      }
      
      // setto gli elementi
      file >> numEle;
      mesh->getNodePointer()->reserve(numEle);
      
      // setto l'id
      id=0;
      
      // inserisco i nodi
      for(UInt i=0; i<numEle; ++i)
      {  
	   // prendo le informazioni
	   file >> id1;
	   file >> id2;
	   file >> id3;
	   file >> geo;
	   
	   // setto l'id e il geoId
	   tri.setId(id);
	   tri.setGeoId(geo);
	   tri.setConnectedId(0, (id1-1));
	   tri.setConnectedId(1, (id2-1));
	   tri.setConnectedId(2, (id3-1));
	   
	   // lo metto nella mesh
	   mesh->insertElement(tri);
	   
	   // incremento id
	   ++id;
      }
      
      // faccio un setup
      mesh->setUpIds();
      
      // chiudo il file 
      file.close();
}

void downloadMesh::fileFromMedit(string s, vector<UInt> * flagPt, mesh2d<Triangle> * surf, mesh3d<Tetra> * vol)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // Variabili in uso
      string                   tmp;
      UInt              numNode(0);
      UInt  numTria(0),numTetra(0);
      point                  	 p;
      UInt  geo,id,id1,id2,id3,id4;
      geoElement<Triangle>     tri;
      geoElement<Tetra>        tet;
      Real                   x,y,z;
      
      // salto l'header
      while(tmp!="Vertices")  file >> tmp;
      
      // setto i nodi
      file >> numNode;
      surf->getNodePointer()->reserve(numNode);
      vol->getNodePointer()->reserve(numNode);
      flagPt->reserve(numNode);
      
      // setto l'id
      id=0;
      
      // inserisco i nodi
      for(UInt i=0; i<numNode; ++i)
      {  
	   file >> x;
	   file >> y;
	   file >> z;
	   file >> geo;
	
	   // setto l'id del punto e le sue coordinate
	   p.setId(id);
	   p.setX(x);
	   p.setY(y);
	   p.setZ(z);
	   
	   // lo metto nelle mesh
	   surf->insertNode(p);
	   vol->insertNode(p);
	   
	   // metto la flag del nodo 
	   flagPt->push_back(geo);
	   
	   // incremento id
	   ++id;
      }
      
      // setto l'id
      file >> tmp;
      
      // cerco la parte che ha i tirnagoli
      while(tmp!="Tetrahedra")	  file >> tmp;
      
      // setto gli elementi di superficie
      file >> numTetra;
      vol->getNodePointer()->reserve(numTetra);
      
      // setto l'id
      id=0;
      
      // inserisco i nodi
      for(UInt i=0; i<numTetra; ++i)
      {  
	   // prendo le informazioni
	   file >> id1;
	   file >> id2;
	   file >> id3;
	   file >> id4;
	   file >> geo;
	   
	   // setto l'id e il geoId
	   tet.setId(id);
	   tet.setGeoId(geo);
	   tet.setConnectedId(0, (id1-1));
	   tet.setConnectedId(1, (id2-1));
	   tet.setConnectedId(2, (id3-1));
	   tet.setConnectedId(3, (id4-1));
	   
	   // lo metto nella mesh
	   vol->insertElement(tet);
	   
	   // incremento id
	   ++id;
      }
      
      // faccio un setup
      vol->setUpIds();
      
      // setto l'id
      file >> tmp;
      
      // cerco la parte che ha i tirnagoli
      while(tmp!="Triangles")	  file >> tmp;
      
      // setto gli elementi di superficie
      file >> numTria;
      surf->getNodePointer()->reserve(numTria);
      
      // setto l'id
      id=0;
      
      // inserisco i nodi
      for(UInt i=0; i<numTria; ++i)
      {  
	   // prendo le informazioni
	   file >> id1;
	   file >> id2;
	   file >> id3;
	   file >> geo;
	   
	   // setto l'id e il geoId
	   tri.setId(id);
	   tri.setGeoId(geo);
	   tri.setConnectedId(0, (id1-1));
	   tri.setConnectedId(1, (id2-1));
	   tri.setConnectedId(2, (id3-1));
	   
	   // lo metto nella mesh
	   surf->insertElement(tri);
	   
	   // incremento id
	   ++id;
      }
      
      // faccio un setup
      surf->setUpIds();
      
      // chiudo il file 
      file.close();
}

// ----------------------------
//      FILE DA MEDIT
// ----------------------------
void downloadMesh::fileFromVTK(string s, mesh2d<Triangle> * mesh)
{
      file.open(s.c_str());
      mesh->clear();
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // Variabili in uso
      string               tmp;
      UInt          numNode(0);
      UInt           numEle(0);
      point                  p;
      UInt  geo,id,id1,id2,id3;
      geoElement<Triangle> tri;
      Real               x,y,z;
      
      // salto l'header
      while(tmp!="POINTS")
      {
	  file >> tmp;
      }
      
      // prendo il numero di nodi 
      file >> numNode;
      // salto l'informazione sul tipo di numeri che contiene il file di VTK "vtk DataFile Version 3.2"
      file >> tmp;
      
      mesh->getNodePointer()->reserve(numNode);
      
      // setto l'id
      id=0;
      
      // inserisco i nodi
      for(UInt i=0; i<numNode; ++i)
      {  
	   file >> x;
	   file >> y;
	   file >> z;
	   
	   // setto l'id del punto e le sue coordinate
	   p.setId(id);
	   p.setX(x);
	   p.setY(y);
	   p.setZ(z);
	   
	   // lo metto nella mesh
	   mesh->insertNode(p);
	  
	   // incremento id
	   ++id;
      }
      
      // chiudo il file e lo riapro 
      file.close();
      file.open(s.c_str());
      tmp.clear();
      
      // salto l'header
      while(tmp!="POLYGONS")
      {
	  file >> tmp;
      }
      
      // prendo il numero di elementi 
      file >> numEle;
      // salto l'informazione sul tipo di numeri che contiene il file di VTK "vtk DataFile Version 3.2"
      file >> tmp;
      
      
      mesh->getNodePointer()->reserve(numEle);
      
      // setto l'id
      id=0;
      
      // inserisco i nodi
      for(UInt i=0; i<numEle; ++i)
      {  
	   // prendo le informazioni
	   file >> geo;
	   file >> id1;
	   file >> id2;
	   file >> id3;
	   
	   
	   // setto l'id e il geoId
	   tri.setId(id);
	   tri.setGeoId(geo);
	   tri.setConnectedId(0, id1);
	   tri.setConnectedId(1, id2);
	   tri.setConnectedId(2, id3);
	   
	   // lo metto nella mesh
	   mesh->insertElement(tri);
	   
	   // incremento id
	   ++id;
      }
      
      // setto gli id 
      mesh->setUpIds();
      
      // chiudo il file
      file.close();
}

// -------------------
//   download da TXT
// -------------------
void downloadMesh::fileNodes(string s, mesh0d<simplePoint> * mesh)
{
      file.open(s.c_str());
      mesh->clear();
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // varaibili in uso 
      UInt  cont=0,numNodi;
      Real tmp=0.0;
      
      // calcolo la lunghezza 
      while(!file.eof())
      {
	  file >> tmp;
	  ++cont;
      }
      
      // chiudo il file 
      file.close();
      
      // calcolo quanti nodi ci sono 
      numNodi = cont/3;    
      
      // faccio un resize 
      mesh->getNodePointer()->resize(numNodi);
      
      // riapro il file e metto le informazioni 
      file.open(s.c_str());
      
      // calcolo la lunghezza 
      cont = 0;
      while(cont!=numNodi)
      {
	    // metto le coordinate
	    file >> tmp;
	    mesh->getNodePointer()->at(cont).setX(tmp);
	    file >> tmp;
	    mesh->getNodePointer()->at(cont).setY(tmp);
	    file >> tmp;
	    mesh->getNodePointer()->at(cont).setZ(tmp);
	    
	    // incremento il punto 
	    ++cont;
      }
      
      // chiudo il file 
      file.close();
      
      // faccio gli elementi 
      mesh->getElementPointer()->resize(mesh->getNumNodes());
      
      // setto le coordinate
      for(UInt i=0; i<mesh->getNumNodes(); ++i)	mesh->getElementPointer(i)->setConnectedId(0, i);
      
      // faccio un setUp
      mesh->setUpIds();
}

// -------------------
//  download freeFem
// -------------------
void downloadMesh::fileFromPlaneMSH(string s, mesh2d<Triangle> * mesh)
{
      file.open(s.c_str());
      
      if(!file.is_open())
      {
	      cout << "ERRORE: file " << s << " non trovato" << endl;
      }
      
      // pulisco la struttura mesh data in input
      mesh->clear();
      
      // Variabili in gioco 
      vector<point>                   tmpNode;
      vector<geoElement<Triangle> >    tmpEle;
      UInt                numNode,numElem,tmp;
      UInt                        id1,id2,id3;
      Real                                x,y;
      point                                 p;
      geoElement<Triangle>               tria;
      
      // Vedo il numero dei nodi
      file >> numNode;
      file >> numElem;
      file >> tmp;
      
      for(UInt i=0; i<numNode; ++i)
      {
	   // ricavo le informazioni
	   file >> x;
	   file >> y;
	   file >> tmp;
	   
	   // creo la variabile p
	   p.setX(x);	  p.setY(y);	p.setZ(0.0);
	   p.setBoundary(tmp);
	   
	   // la metto in tmpNode
	   tmpNode.push_back(p);
      }
      
      for(UInt i=0; i<numElem; ++i)
      {
	  // Ricavo le informazioni
	  file >> id1;
	  file >> id2;
	  file >> id3;
	  file >> tmp;
	  
	  tria.setConnectedId(0,id1-1);	
	  tria.setConnectedId(1,id2-1);	
	  tria.setConnectedId(2,id3-1);
	  tria.setGeoId(tmp);
	  
	  // la metto in una lista temporanea
	  tmpEle.push_back(tria);
      }
      
      // metto tutto nella mesh 
      mesh->insertNode(&tmpNode);
      mesh->insertElement(&tmpEle);
      
      // metto a posto gli id
      mesh->setUpIds();
      
      file.close();
}

