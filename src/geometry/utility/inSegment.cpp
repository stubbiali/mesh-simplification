#include "inSegment.h"

using namespace std;
using namespace geometry;
using namespace predicates;

//
// Costruttori
//
inSegment::inSegment()
{
      // setto la tolleranza  
      toll = 1e-15;
      
      // setto i predicati 
      exactinit();
      
      // setto le variabili
      A = new REAL[2];
      B = new REAL[2];
      C = new REAL[2];
}

//
// Metodi per trovare la posizione del punto
//
pair<bool, vector<UInt> > inSegment::intersec(vector<point> * vertices, point p)
{   
      // controllo la dimensione 
      assert(vertices->size()==2);
      
      // setto le variabili
      Real    distTmp,dist0,dist1,lung,lungTmp;
      point                        	   dir;
      vector<Real>	                  dist;
      set<Real>			      distProj;
      pair<bool, vector<UInt> >         result;
      
      // prendo la direzione della retta 
      dir  = vertices->at(1)-vertices->at(0);
      lung = dir.norm2();
      
      // faccio un reserve
      result.second.reserve(1);
      
      // faccio un resize 
      dist.reserve(3);
      
      // controllo la lunghezza del segmento 
      if(lung<toll)
      {
	  cout << "Il segmento è più corto di " << toll << endl;
	  result.first  = false;
	  return(result);
      }
      
      // normalizzo 
      dir = dir / lung;
      
      // calcolo le distanze dai punti 
      dist0 = (vertices->at(0)-p).norm2();
      dist1 = (vertices->at(1)-p).norm2();
	
      // contrllo che il punto non sia in uno dei due estremi
      if(dist0<toll)
      {
	  result.first  = true;
	  result.second.push_back(vertices->at(0).getId());
	  return(result);
      }
      else if(dist1<toll)
      {
	  result.first  = true;
	  result.second.push_back(vertices->at(1).getId());
	  return(result);
      }
      
      // calcolo le distanze dal segmento proiettate sulle 3 coordinate 
      for(UInt i=0; i<3; ++i)
      {
	  // salvo
	  A[0] = vertices->at(0).getI(i%3);
	  B[0] = vertices->at(1).getI(i%3);
	  C[0] = p.getI(i%3);
	  
	  // salvo
	  A[1] = vertices->at(0).getI((i+1)%3);
	  B[1] = vertices->at(1).getI((i+1)%3);
	  C[1] = p.getI((i+1)%3);
	  
	  // metto in dist
	  distTmp = orient2d(A, B, C);
	  
	  // calcolo la lunghezza
	  lungTmp = sqrt((A[0]-B[0])*(A[0]-B[0])+(A[1]-B[1])*(A[1]-B[1]));
	  
	  // controllo che la lunghezza non sia nulla 
	  if(lungTmp>toll)		  distProj.insert((distTmp*distTmp)/(lungTmp*lungTmp));
      }
      
      //
      // facendo la norma di dist otteniamo approssimativamente due volte l'area del triangolo che ha per estremi i tre nodi
      // se dividiamo per la lunghezza del segmento abbiamo un'approssimazione dell'altezza del triangolo.
      //
      
      // le metto tutti in distProj
      dist.reserve(3);
      for(set<Real>::iterator it=distProj.begin(); it!=distProj.end(); ++it)	dist.push_back(*it);
      
      // faccio un accumulate
      if(dist.size()==2)	distTmp = sqrt(dist[0]+dist[1]);
      else if(dist.size()==3)	distTmp = sqrt(dist[1]+dist[2]);
      else if(dist.size()==1)	distTmp = sqrt(dist[0]);
      
      // se la distanza è minore di toll allora siamo sulla retta altrimenti siamo al di fuori
      if(distTmp>toll)
      {
	  result.first  = false;
	  return(result);
      }
      else if((dist1<lung) && (dist0<lung))
      {
	  result.first  = true;
	  return(result);
      }
      
      // se non ha fatto nessuno dei due di prima 
      result.first  = false;
      return(result);
}

bool inSegment::isIn(vector<point> * vertices, point p)
{
      // controllo la dimensione 
      assert(vertices->size()==2);
      
      Real    distTmp,dist0,dist1,lung,lungTmp;
      point                        	   dir;
      vector<Real>	                  dist;
      set<Real>			      distProj;
      pair<bool, vector<UInt> >         result;
      
      // prendo la direzione della retta 
      dir  = vertices->at(1)-vertices->at(0);
      lung = dir.norm2();
      
      // faccio un reserve
      result.second.reserve(1);
      
      // faccio un resize 
      dist.reserve(3);
      
      // controllo la lunghezza del segmento 
      if(lung<toll)
      {
	  cout << "Il segmento è più corto di " << toll << endl;
	  return(false);
      }
      
      // normalizzo 
      dir = dir / lung;
      
      // calcolo le distanze dai punti 
      dist0 = (vertices->at(0)-p).norm2();
      dist1 = (vertices->at(1)-p).norm2();
	
      // contrllo che il punto non sia in uno dei due estremi
      if(dist0<toll)			     return(true);
      else if(dist1<toll)		     return(true);
      
      // calcolo le distanze dal segmento proiettate sulle 3 coordinate 
      for(UInt i=0; i<3; ++i)
      {
	  // salvo
	  A[0] = vertices->at(0).getI(i%3);
	  B[0] = vertices->at(1).getI(i%3);
	  C[0] = p.getI(i%3);
	  
	  // salvo
	  A[1] = vertices->at(0).getI((i+1)%3);
	  B[1] = vertices->at(1).getI((i+1)%3);
	  C[1] = p.getI((i+1)%3);
	  
	  // metto in dist
	  distTmp = orient2d(A, B, C);
	  
	  // calcolo la lunghezza
	  lungTmp = sqrt((A[0]-B[0])*(A[0]-B[0])+(A[1]-B[1])*(A[1]-B[1]));
	  
	  // controllo che la lunghezza non sia nulla 
	  if(lungTmp>toll)		  distProj.insert((distTmp*distTmp)/(lungTmp*lungTmp));
      }
      
      //
      // facendo la norma di dist otteniamo approssimativamente due volte l'area del triangolo che ha per estremi i tre nodi
      // se dividiamo per la lunghezza del segmento abbiamo un'approssimazione dell'altezza del triangolo.
      //
      
      // le metto tutti in distProj
      dist.reserve(3);
      for(set<Real>::iterator it=distProj.begin(); it!=distProj.end(); ++it)	dist.push_back(*it);
      
      // faccio un accumulate
      if(dist.size()==2)	distTmp = sqrt(dist[0]+dist[1]);
      else if(dist.size()==3)	distTmp = sqrt(dist[1]+dist[2]);
      else if(dist.size()==1)	distTmp = sqrt(dist[0]);
      
      // se la distanza è minore di toll allora siamo sulla retta altrimenti siamo al di fuori
      if(distTmp>toll)						return(false);
      else if((dist1<lung) && (dist0<lung))  			return(true);
      
      // se non ha fatto nessuno dei due di prima 
      return(false);
}

//
// Messa a punto di toll
//
void inSegment::setToll(Real _toll)
{
      toll = _toll;
}

Real inSegment::getToll()
{
      return(toll);
}

//
// Metodo che stampa
//
void inSegment::print(pair<bool, vector<UInt> > result)
{
      if(result.first)
      {
	  cout << "Il punto è contenuto ";
	  if(result.second.size()==0)
	  {
	    cout << "all'interno" << endl;
	  }
	  else
	  {
	    assert(result.second.size()==1);
	    cout << " e COINCIDE con il punto " << result.second[0] << endl;  
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
inSegment::~inSegment()
{
      delete [] A;
      delete [] B;
      delete [] C;
}


