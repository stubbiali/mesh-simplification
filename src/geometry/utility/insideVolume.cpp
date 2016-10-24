#include "insideVolume.h"


using namespace geometry;
//
// Costruttori
//
insideVolume::insideVolume()
{
    meshPointer= NULL;
    toll       =1e-14;
}

//
// Metodo che serve per settare le variabili 
//
void insideVolume::setMeshPointer(mesh2d<Triangle>  * _meshPointer)
{
    meshPointer=_meshPointer;
    
    // setto li puntatore alla mesh e creo la struttura di ricerca 
    inter.setMeshPointer1(meshPointer);
    search.setMeshPointer(meshPointer);
}

//
// Metodi per trovare la posizione del punto
//  
int insideVolume::isInside(point p_to_test) 		  
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
    p_ext_add.setZ(pMax.getZ()+h);
  
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

point insideVolume::findInternal()
{
    // variabili in uso 
    point toTest,bar1,bar2;
    point   n1,n2,p1,p2,p3;
    
    // setto inter
    inter.setMeshPointer1(meshPointer);
    
    // ciclo su tutti gli elementi della mesh
    for(UInt i=0; i<meshPointer->getNumElements(); ++i)
    {
	// prendo i punti
	p1   = meshPointer->getNode(meshPointer->getElement(i).getConnectedId(0));
	p2   = meshPointer->getNode(meshPointer->getElement(i).getConnectedId(1));
	p3   = meshPointer->getNode(meshPointer->getElement(i).getConnectedId(2));
	
	n1   = ((p2-p3)^(p1-p3));
	n1   = n1 / n1.norm2();
	
	// calcolo il baricentro 
	bar1.replace(p1, p2, p3, 1./3.);
      
	for(UInt j=0; j<meshPointer->getNumElements(); ++j)
	{
	      // prendo i punti
	      p1   = meshPointer->getNode(meshPointer->getElement(j).getConnectedId(0));
	      p2   = meshPointer->getNode(meshPointer->getElement(j).getConnectedId(1));
	      p3   = meshPointer->getNode(meshPointer->getElement(j).getConnectedId(2));
	      
	      n2   = ((p2-p3)^(p1-p3));
	      n2   = n2 / n2.norm2();
	
	      // calcolo il baricentro 
	      bar2.replace(p1, p2, p3, 1./3.);
	      
	      // creo il punto 
	      toTest.replace(bar1, bar2, 0.5);
	      
	      if((fabs(n1*n2)<0.5) && (isInside(toTest)==INSIDE)) return(toTest);
	}
    }
    
    cout << "Non ho trovato il punto interno al volume" << endl;
    assert(1==0);
    return toTest;
}

//
// Messa a punto di toll
//
void insideVolume::setToll(Real _toll)
{
      toll = _toll;
      search.setToll(toll);
      inter.setToll(toll);
}

Real insideVolume::getToll()
{
      return(toll);
}



