#include "inTriangle.h"

using namespace std;
using namespace geometry;
using namespace predicates;


//
// Costruttori
//
inTriangle::inTriangle()
{
      toll = 1e-15;
      
      // setto i predicati
      exactinit();
      
      // setto le variabili
      A = new REAL[3];
      B = new REAL[3];
      C = new REAL[3];
      D = new REAL[3];
      
      // setto inSegment
      inSeg.setToll(toll);
      
      // setto la variabile che crea le coordinate baricentriche 
      bar.setToll(toll);
}

//
// Metodi per trovare la posizione del punto
//
pair<bool, vector<UInt> > inTriangle::intersec(vector<point> * vertices, point p)
{	
    // controllo che tutto sia ok
    assert(vertices->size()==3);
    
    // varaibli in uso 
    pair<bool, vector<UInt> > result;
    Real  sum,dist,distA,distB,distC;
    bool              inAB,inBC,inCA;
    vector<point>               lato;
    vector<Real>                   t;
    
    
    // --------------------------------------------------------------
    //			DISTANZA CON IL SEGNO
    // --------------------------------------------------------------
    
    // controllo la distanza con segno 
    dist = signedDist(vertices, p);
    
    // se è più grande di toll siamo fuori dal piano definito dal triangolo 
    if(fabs(dist)>toll)
    {
	result.first = false;
	result.second.clear();
	return(result);
    }
    
    // --------------------------------------------------------------
    //		CONTROLLIAMO LA DISTANZA CON I NODI    
    // --------------------------------------------------------------
    
    // calcoliamo le distanze dai nodi 
    distA = (p-vertices->at(0)).norm2();
    distB = (p-vertices->at(1)).norm2();
    distC = (p-vertices->at(2)).norm2();
    
    
    // controllo 
    if(distA<toll)	
    {
	result.first = true;
	result.second.resize(1);
	result.second[0]=vertices->at(0).getId();
	return(result);
    }
    else if(distB<toll)
    {
	result.first = true;
	result.second.resize(1);
	result.second[0]=vertices->at(1).getId();
	return(result);
    }
    else if(distC<toll)
    {
	result.first = true;
	result.second.resize(1);
	result.second[0]=vertices->at(2).getId();
	return(result);
    }
    
    // --------------------------------------------------------------
    //		CONTROLLIAMO LA DISTANZA CON I LATI
    // --------------------------------------------------------------
    
    lato.resize(2);
    lato[0] = vertices->at(0);	lato[1] = vertices->at(1);
    inAB = inSeg.isIn(&lato, p);
    lato[0] = vertices->at(1);	lato[1] = vertices->at(2);
    inBC = inSeg.isIn(&lato, p);
    lato[0] = vertices->at(2);	lato[1] = vertices->at(0);
    inCA = inSeg.isIn(&lato, p);
    
    if(inAB)	
    {
	result.first = true;
	result.second.resize(2);
	result.second[0]=vertices->at(0).getId();
	result.second[1]=vertices->at(1).getId();
	return(result);
    }
    else if(inBC)
    {
	result.first = true;
	result.second.resize(2);
	result.second[0]=vertices->at(1).getId();
	result.second[1]=vertices->at(2).getId();
	return(result);
    }
    else if(inCA)
    {
	result.first = true;
	result.second.resize(2);
	result.second[0]=vertices->at(2).getId();
	result.second[1]=vertices->at(0).getId();
	return(result);
    }
    
    // a questo livello siamo sicuri che il nodo è sul piano definito dal triangolo MA non è sui suoi edge ne nei nodi 
    
    // controllo le sue coordinate baricentriche 
    bar.ofTriangle(vertices, p, &t);
    
    // faccio la somma 
    sum = accumulate(t.begin(), t.end(), 0.0);
    
    // se la somma è 1 il punto è dentro 
    if(fabs(sum-1.0)<toll)
    {
	result.first = true;
	result.second.clear();
	return(result);
    }
    
    // se la somma non è 1 siamo fuori
    result.first = false;
    result.second.clear();
    return(result);
}

bool inTriangle::isIn(vector<point> * vertices, point p)
{
    // controllo che tutto sia ok
    assert(vertices->size()==3);
    
    // varaibli in uso 
    Real  sum,dist,distA,distB,distC;
    bool              inAB,inBC,inCA;
    vector<point>               lato;
    vector<Real>                   t;
   
    // --------------------------------------------------------------
    //			DISTANZA CON IL SEGNO
    // --------------------------------------------------------------
    
    // controllo la distanza con segno 
    dist = signedDist(vertices, p);
    
    // se è più grande di toll siamo fuori dal piano definito dal triangolo 
    if(fabs(dist)>toll) return(false);
    
    // --------------------------------------------------------------
    //		CONTROLLIAMO LA DISTANZA CON I NODI    
    // --------------------------------------------------------------
    
    // calcoliamo le distanze dai nodi 
    distA = (p-vertices->at(0)).norm2();
    distB = (p-vertices->at(1)).norm2();
    distC = (p-vertices->at(2)).norm2();
    
    // controllo 
    if((distA<toll) || (distB<toll) || (distC<toll)) return(true);
    
    // --------------------------------------------------------------
    //		CONTROLLIAMO LA DISTANZA CON I LATI
    // --------------------------------------------------------------
    
    lato.resize(2);
    lato[0] = vertices->at(0);	lato[1] = vertices->at(1);
    inAB = inSeg.isIn(&lato, p);
    lato[0] = vertices->at(1);	lato[1] = vertices->at(2);
    inBC = inSeg.isIn(&lato, p);
    lato[0] = vertices->at(2);	lato[1] = vertices->at(0);
    inCA = inSeg.isIn(&lato, p);
    
    if((inAB) || (inBC) || (inCA))	return(true);
    
    
    // a questo livello siamo sicuri che il nodo è sul piano definito dal triangolo MA non è sui suoi edge ne nei nodi 
    
    // controllo le sue coordinate baricentriche 
    bar.ofTriangle(vertices, p, &t);
    
    // faccio la somma 
    sum = accumulate(t.begin(), t.end(), 0.0);
    
    // se la somma è 1 il punto è dentro 
    if(fabs(sum-1.0)<toll)	return(true);
    
    // se la somma non è 1 siamo fuori
    return(false);
}

Real inTriangle::signedDist(vector<point> * vertices, point p)
{
    // controllo che tutto sia ok
    assert(vertices->size()==3);
    
    // setto i nodi del triangolo 
    A[0] = vertices->at(0).getX();
    A[1] = vertices->at(0).getY();
    A[2] = vertices->at(0).getZ();
    
    B[0] = vertices->at(1).getX();
    B[1] = vertices->at(1).getY();
    B[2] = vertices->at(1).getZ();
    
    C[0] = vertices->at(2).getX();
    C[1] = vertices->at(2).getY();
    C[2] = vertices->at(2).getZ();
    
    // setto i nodi 
    D[0] = p.getX();
    D[1] = p.getY();
    D[2] = p.getZ();

    // faccio l'area
    Real area = ((vertices->at(1)-vertices->at(0))^(vertices->at(2)-vertices->at(0))).norm2()*0.5;
    
    // controllo che il triangolo non sia degenere 
    if(area<(toll*toll))	
    {
	cout << "In \"inTriangle\" ho trovato un triangolo degenere!!" << endl;
	return(1.0);
    }
    
    // I robusti prendicati nel metodo orient3d danno una stima di 6 volte il volume del tetraedro che ha base il triangolo ABC 
    // per questo dividiamo per l'area
    
    // ritorno 
    return(orient3d(A,B,C,D)/area);
}

bool inTriangle::triangleControl(point p1, point p2, point p3)
{
      // variabile
      bool test1,test2,test3;
      vector<point>  lato(2);
      
      // testo i lati
      lato[0] = p1;
      lato[1] = p2;
      test1 = inSeg.isIn(&lato, p3);
      
      // testo i lati
      lato[0] = p2;
      lato[1] = p3;
      test2 = inSeg.isIn(&lato, p1);
      
      // testo i lati
      lato[0] = p3;
      lato[1] = p1;
      test3 = inSeg.isIn(&lato, p2);
      
      // controllo se almeno un punto è allinato e ritorno falso perché non lo voglio aggiungere 
      if(test1 || test2 || test3)	return(false);
      
      // ritorno vero se non sono allineati 
      return(true);
}

//
// Messa a punto di toll
//
void inTriangle::setToll(Real _toll)
{
      toll = _toll;
      
      // setto inSegment
      inSeg.setToll(toll);
      
      // setto bar
      bar.setToll(toll);
}

Real inTriangle::getToll()
{
      return(toll);
}

//
// Metodo che stampa
//
void inTriangle::print(pair<bool, vector<UInt> > result)
{
      if(result.first)
      {
	  switch(result.second.size())
	  {
	    case(0):
		      cout << "Il punto è interno al triangolo" << endl;
		      break;
	    case(1):
		      cout << "Il punto coincide con il nodo " << result.second[0] << endl;
		      break;
	    case(2):
		      cout << "Il punto è all'interno del segmento " << result.second[0] << "  " << result.second[1] << endl;
		      break;
	  } 
      }
      else
      {
	  cout << "Il punto NON è contenuto" << endl;
	  assert(result.second.size()==0);
      }
  
}

//
// Distruttore
//
inTriangle::~inTriangle()
{
      delete [] A;
      delete [] B;
      delete [] C;
      delete [] D;	
}

