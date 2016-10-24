#include "insidePolygon.h"


using namespace geometry;
//
// Costruttori
//
insidePolygon::insidePolygon()
{
    meshPointer= NULL;
    toll       =1e-14;
}

//
// Metodo che serve per settare le variabili 
//
void insidePolygon::setMeshPointer(mesh1d<Line>  * _meshPointer)
{
    meshPointer=_meshPointer;
    
    // setto li puntatore alla mesh e creo la struttura di ricerca 
    inter.setMeshPointer1(meshPointer);
    search.setMeshPointer(meshPointer);
}

//
// Metodi per trovare la posizione del punto
//  
int insidePolygon::isInside(point p_to_test) 		  
{
    // varaibili in uso 
    UInt	    		   	       N_inter;
    Real					     h;
    pair<bool,vector<UInt> >	     		result;
    point 	   	 	  	     pMax,pMin;
    point		  	  	     p_ext_add;
    mesh0d<simplePoint> 	   	         cloud;
    mesh1d<Line> 	   	   	       segment;

    // ------------------------------------------
    //Controllo se il punto P_TO_TEST è del bordo
    // ------------------------------------------
    
    // controllo che non sia contenuto 
    result = search.isIn(p_to_test);
    
    // se lo trovo 
    if(result.first)	return(ONBOUNDARY);
  
    // prendo un punto esterno 
    meshPointer->createBBox(pMax,pMin);
    
    // prendo la massimah
    h = meshPointer->maxH();
      
    // prendo il punto esterno 
    p_ext_add.setX(pMax.getX()+h);
    p_ext_add.setY(pMax.getY()+h);
    p_ext_add.setZ(0.0);
   
    // resetto le intersezioni 
    N_inter=0;  

    // Metto i punti 
    segment.insertNode(p_to_test);
    segment.insertNode(p_ext_add);

    // faccio un resize 
    segment.getElementPointer()->resize(1);
    segment.getElementPointer(0)->setConnectedId(0,0);
    segment.getElementPointer(0)->setConnectedId(1,1);
	    
    // sistemo gli id 
    segment.setUpIds();
	    
    // setto le variabili di inter
    if(inter.getMeshPointer1()==NULL)	inter.setMeshPointer1(meshPointer);
    inter.setMeshPointer2(&segment);
	    
    // faccio l'intersezione 
    inter.createIntersection(&cloud);
	    
    // conto il numero di intersezioni 
    N_inter=cloud.getNumNodes();
    
    // se sono pari 
    if(N_inter%2!=0) 	return(INSIDE);

    // se sono dispati 
    return(OUTSIDE);
}

point insidePolygon::findInternal()
{
    //p_ext all'inizio so che e' sicuramnete esterno al poligono 
    //Ricerca di un possibile punto interno

    // varaibli in uso 
    point internal,p1,p2;
    connect1d<Line> conn(meshPointer);

    // creo la connessione nodo nodo
    conn.buildNodeToNode();

    // ciclo sui nodi per cercare un possibile candidato 
    for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
    {
	if(conn.getNodeToNodePointer(i)->getNumConnected()==2)
	{
	  // prendo i due nodi 
	  p1=meshPointer->getNode(conn.getNodeToNodePointer(i)->getConnectedId(0));
	  p2=meshPointer->getNode(conn.getNodeToNodePointer(i)->getConnectedId(1));
	
	  // creo il punto medio 
	  internal.replace(p1,p2,0.5);

	  if((internal-meshPointer->getNode(i)).norm2()>toll)
	  {
	      internal.setZ(0.0);
	      if(isInside(internal))	return internal;
	  }
	}	
    }	
 
    cout << "NON ho trovato il punto interno" << endl;
    return(internal);
}

//
// Messa a punto di toll
//
void insidePolygon::setToll(Real _toll)
{
      toll = _toll;
      search.setToll(toll);
      inter.setToll(toll);
}

Real insidePolygon::getToll()
{
      return(toll);
}






