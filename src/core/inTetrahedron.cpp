#include "inTetrahedron.h"

using namespace std;
using namespace geometry;
using namespace predicates;


//
// Costruttori
//
inTetrahedron::inTetrahedron()
{
      toll = 1e-15;

      // setto i predicati
      exactinit();

      // setto le variabili
      A = new REAL[3];
      B = new REAL[3];
      C = new REAL[3];
      D = new REAL[3];
      E = new REAL[3];

      // setto inTria
      inTria.setToll(toll);

      // setto inSegment
      inSeg.setToll(toll);

      // setto la variabile che crea le coordinate baricentriche
      bar.setToll(toll);
}

//
// Metodi per trovare la posizione del punto
//
pair<bool, vector<UInt> > inTetrahedron::intersec(vector<point> * vertices, point p)
{

    // controllo che tutto sia ok
    assert(vertices->size()==4);

    // varaibli in uso
    pair<bool, vector<UInt> > result;
    Real  sum,distA,distB,distC,distD;
    bool           inAB,inBC,inCA,inDA,inDB,inDC;
    bool           inABC,inABD,inACD,inBCD;
    bool                inABCD;
    point normal;
    vector<point>               lato, faccia;
    vector<Real>                   t;


    // --------------------------------------------------------------
    //			DISTANZA CON IL SEGNO
    // --------------------------------------------------------------

//	Real volume = std::abs( (vertices->at(2)-vertices->at(3)) * 
//	((vertices->at(0)-vertices->at(3)) ^ (vertices->at(1)-vertices->at(3)) ) ) / 6;

    normal = (vertices->at(1)-vertices->at(0))^(vertices->at(2)-vertices->at(0));
    inABCD = normal*(p-vertices->at(0)) >= 0 ? true : false;
    
    normal = (vertices->at(3)-vertices->at(1))^(vertices->at(2)-vertices->at(1));
    inABCD = inABCD ^ (normal*(p-vertices->at(1)) >= 0 ? true : false);
    
    normal = (vertices->at(3)-vertices->at(2))^(vertices->at(0)-vertices->at(2));
    inABCD = inABCD ^ (normal*(p-vertices->at(2)) >= 0 ? true : false);
    
    normal = (vertices->at(1)-vertices->at(3))^(vertices->at(0)-vertices->at(3));
    inABCD = inABCD ^ (normal*(p-vertices->at(3)) >= 0 ? true : false);

	// se inABCD è false allora il punto è all'interno o sul bordo del tetra
    if(inABCD)
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
    distD = (p-vertices->at(3)).norm2();


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
    else if(distD<toll)
    {
	result.first = true;
	result.second.resize(1);
	result.second[0]=vertices->at(3).getId();
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
    lato[0] = vertices->at(3);	lato[1] = vertices->at(0);
    inDA = inSeg.isIn(&lato, p);
    lato[0] = vertices->at(3);	lato[1] = vertices->at(1);
    inDB = inSeg.isIn(&lato, p);
    lato[0] = vertices->at(3);	lato[1] = vertices->at(2);
    inDC = inSeg.isIn(&lato, p);

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
    else if(inDA)
    {
	result.first = true;
	result.second.resize(2);
	result.second[0]=vertices->at(3).getId();
	result.second[1]=vertices->at(0).getId();
	return(result);
    }
    else if(inDB)
    {
	result.first = true;
	result.second.resize(2);
	result.second[0]=vertices->at(3).getId();
	result.second[1]=vertices->at(1).getId();
	return(result);
    }
    else if(inDC)
    {
	result.first = true;
	result.second.resize(2);
	result.second[0]=vertices->at(3).getId();
	result.second[1]=vertices->at(2).getId();
	return(result);
    }

    // a questo livello siamo sicuri che il nodo è sulle facce del tetraedro o interno MA non è sui suoi edge ne nei nodi

    // --------------------------------------------------------------
    //		CONTROLLIAMO LA DISTANZA CON LE FACCE
    // --------------------------------------------------------------

    faccia.resize(3);
    faccia[0] = vertices->at(0);
    faccia[1] = vertices->at(1);
    faccia[2] = vertices->at(2);
    inABC = inTria.isIn(&faccia, p);
    faccia[0] = vertices->at(0);
    faccia[1] = vertices->at(1);
    faccia[2] = vertices->at(3);
    inABD = inTria.isIn(&faccia, p);
    faccia[0] = vertices->at(0);
    faccia[1] = vertices->at(2);
    faccia[2] = vertices->at(3);
    inACD = inTria.isIn(&faccia, p);
    faccia[0] = vertices->at(1);
    faccia[1] = vertices->at(2);
    faccia[2] = vertices->at(3);
    inBCD = inTria.isIn(&faccia, p);

    if(inABC)
    {
   	result.first = true;
   	result.second.resize(3);
   	result.second[0]=vertices->at(0).getId();
   	result.second[1]=vertices->at(1).getId();
   	result.second[2]=vertices->at(2).getId();
   	return(result);
    }
    else if(inABD)
    {
   	result.first = true;
   	result.second.resize(3);
   	result.second[0]=vertices->at(0).getId();
   	result.second[1]=vertices->at(1).getId();
   	result.second[2]=vertices->at(3).getId();
   	return(result);
    }
    else if(inACD)
    {
   	result.first = true;
   	result.second.resize(3);
   	result.second[0]=vertices->at(0).getId();
   	result.second[1]=vertices->at(2).getId();
   	result.second[2]=vertices->at(3).getId();
   	return(result);
    }
    else if(inBCD)
    {
   	result.first = true;
   	result.second.resize(3);
   	result.second[0]=vertices->at(1).getId();
   	result.second[1]=vertices->at(2).getId();
   	result.second[2]=vertices->at(3).getId();
   	return(result);
    }

    // a questo livello siamo sicuri che il nodo è all'interno

    // controllo le sue coordinate baricentriche
    bar.ofTetra(vertices, p, &t);

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

bool inTetrahedron::isIn(vector<point> * vertices, point p)
{
    // controllo che tutto sia ok
    assert(vertices->size()==4);

    // varaibli in uso
    Real  sum,distA,distB,distC,distD;
    bool           inAB,inBC,inCA,inDA,inDB,inDC;
    bool           inABC,inABD,inACD,inBCD;
    bool                inABCD;
    point normal;
    vector<point>               lato, faccia;
    vector<Real>                   t;

    // --------------------------------------------------------------
    //			DISTANZA CON IL SEGNO
    // --------------------------------------------------------------

    normal = (vertices->at(1)-vertices->at(0))^(vertices->at(2)-vertices->at(0));
    inABCD = normal*(p-vertices->at(0)) >= (-3*toll) ? true : false;

    normal = (vertices->at(3)-vertices->at(1))^(vertices->at(2)-vertices->at(1));
    inABCD = inABCD ^ (normal*(p-vertices->at(1)) >= (-3*toll) ? true : false);

    normal = (vertices->at(3)-vertices->at(2))^(vertices->at(0)-vertices->at(2));
    inABCD = inABCD ^ (normal*(p-vertices->at(2)) >= (-3*toll) ? true : false);

    normal = (vertices->at(1)-vertices->at(3))^(vertices->at(0)-vertices->at(3));
    inABCD = inABCD ^ (normal*(p-vertices->at(3)) >= (-3*toll) ? true : false);

	// se inABCD è false allora il punto è all'interno o sul bordo del tetra
    if(inABCD)
    	return(false);

    // --------------------------------------------------------------
    //		CONTROLLIAMO LA DISTANZA CON I NODI
    // --------------------------------------------------------------

    // calcoliamo le distanze dai nodi
    distA = (p-vertices->at(0)).norm2();
    distB = (p-vertices->at(1)).norm2();
    distC = (p-vertices->at(2)).norm2();
    distD = (p-vertices->at(3)).norm2();

    // controllo
    if((distA<3*toll) || (distB<3*toll) || (distC<3*toll) || (distD<3*toll)) 	return(true);

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
    lato[0] = vertices->at(3);	lato[1] = vertices->at(0);
    inDA = inSeg.isIn(&lato, p);
    lato[0] = vertices->at(3);	lato[1] = vertices->at(1);
    inDB = inSeg.isIn(&lato, p);
    lato[0] = vertices->at(3);	lato[1] = vertices->at(2);
    inDC = inSeg.isIn(&lato, p);

    if((inAB) || (inBC) || (inCA) || (inDA) || (inDB) || (inDC))   	return(true);

    // a questo livello siamo sicuri che il nodo è sulle facce del tetraedro o interno MA non è sui suoi edge ne nei nodi

    // --------------------------------------------------------------
    //		CONTROLLIAMO LA DISTANZA CON LE FACCE
    // --------------------------------------------------------------

	faccia.resize(3);
	faccia[0] = vertices->at(0);
	faccia[1] = vertices->at(1);
	faccia[2] = vertices->at(2);
	inABC = inTria.isIn(&faccia, p);
	faccia[0] = vertices->at(0);
	faccia[1] = vertices->at(1);
	faccia[2] = vertices->at(3);
	inABD = inTria.isIn(&faccia, p);
	faccia[0] = vertices->at(0);
	faccia[1] = vertices->at(2);
	faccia[2] = vertices->at(3);
	inACD = inTria.isIn(&faccia, p);
	faccia[0] = vertices->at(1);
	faccia[1] = vertices->at(2);
	faccia[2] = vertices->at(3);
	inBCD = inTria.isIn(&faccia, p);

	if((inABC) || (inABD) || (inACD) || (inBCD))   	return(true);

    // a questo livello siamo sicuri che il nodo è all'interno

    // controllo le sue coordinate baricentriche
    bar.ofTetra(vertices, p, &t);

    // faccio la somma
    sum = accumulate(t.begin(), t.end(), 0.0);

    // se la somma è 1 il punto è dentro
    if(fabs(sum-1.0)<3*toll) 	return(true);

    // se la somma non è 1 siamo fuori
    return(false);
}

/*
Real inTetrahedron::signedDist(vector<point> * vertices, point p)
{
    // controllo che tutto sia ok
    assert(vertices->size()==4);

    // setto i nodi del tetraedro
    A[0] = vertices->at(0).getX();
    A[1] = vertices->at(0).getY();
    A[2] = vertices->at(0).getZ();

    B[0] = vertices->at(1).getX();
    B[1] = vertices->at(1).getY();
    B[2] = vertices->at(1).getZ();

    C[0] = vertices->at(2).getX();
    C[1] = vertices->at(2).getY();
    C[2] = vertices->at(2).getZ();
    
    D[0] = vertices->at(3).getX();
    D[1] = vertices->at(3).getY();
    D[2] = vertices->at(3).getZ();

    // setto i nodi
    E[0] = p.getX();
    E[1] = p.getY();
    E[2] = p.getZ();
    
    // faccio il volume
    Real area = std::abs( (vertices->at(2)-vertices->at(3))*((vertices->at(0)-vertices->at(3))^(vertices->at(1)-vertices->at(3))) ) / 6;

    // controllo che il tetraedro non sia degenere
    if(area<(toll*toll*toll))
    {
	cout << "In \"inTetrahedron\" ho trovato un tetraedro degenere!!" << endl;
	return(1.0);
    }

    // I robusti prendicati nel metodo orient3d danno una stima di 6 volte il volume del tetraedro che ha base il triangolo ABC
    // per questo dividiamo per l'area

    // ritorno
    return(orient3d(A,B,C,D)/area);
}	
*/

bool inTetrahedron::triangleControl(point p1, point p2, point p3)
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

bool inTetrahedron::tetrahedronControl(point p1, point p2, point p3, point p4)
{
    // variabile
    bool test1,test2,test3,test4;
    vector<point>  faccia(3);

    // testo le facce
    faccia[0] = p1;
    faccia[1] = p2;
    faccia[2] = p3;
    test1 = inTria.isIn(&faccia, p4);

    // testo le facce
    faccia[0] = p1;
    faccia[1] = p2;
    faccia[2] = p4;
    test2 = inTria.isIn(&faccia, p3);

    // testo le facce
    faccia[0] = p1;
    faccia[1] = p3;
    faccia[2] = p4;
    test3 = inTria.isIn(&faccia, p2);

    // testo le facce
    faccia[0] = p2;
    faccia[1] = p3;
    faccia[2] = p4;
    test4 = inTria.isIn(&faccia, p1);

    // controllo se almeno un punto è allinato e ritorno falso perché non lo voglio aggiungere
    if(test1 || test2 || test3 || test4)	return(false);

    // ritorno vero se non sono allineati
    return(true);
}

//
// Messa a punto di toll
//
void inTetrahedron::setToll(Real _toll)
{
      toll = _toll;

      // setto inTria
      inTria.setToll(toll);

      // setto inSegment
      inSeg.setToll(toll);

      // setto bar
      bar.setToll(toll);
}

Real inTetrahedron::getToll()
{
      return(toll);
}

//
// Metodo che stampa
//
void inTetrahedron::print(pair<bool, vector<UInt> > result)
{
      if(result.first)
      {
	  switch(result.second.size())
	  {
	    case(0):
		      cout << "Il punto è interno al tetraedro" << endl;
		      break;
	    case(1):
		      cout << "Il punto coincide con il nodo " << result.second[0] << endl;
		      break;
	    case(2):
		      cout << "Il punto è all'interno del segmento " << result.second[0] << "  " << result.second[1] << endl;
		      break;
	    case(3):
		      cout << "Il punto è all'interno della faccia " << result.second[0] << "  " << result.second[1] << "  " << result.second[2] << endl;
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
inTetrahedron::~inTetrahedron()
{
      delete [] A;
      delete [] B;
      delete [] C;
      delete [] D;
      delete [] E;
}

