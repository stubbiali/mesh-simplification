#include "intervalIntersection.h"

using namespace geometry;
using namespace std;

//
// Costruttore
//
intervalIntersection::intervalIntersection()
{
      toll = 1e-16;
}

//
// Intersezione degli intervalli
//
pair<bool, vector<Real> > intervalIntersection::intersec(Real A1, Real A2, Real B1, Real B2)
{
      // Variabili temporanee
      Real                    lungA,lungB; 
      vector<Real>         	tA(2,0.0);
      vector<Real>         	tB(2,0.0);
      vector<Real>             tmp(2,0.0);
      pair<bool, vector<Real> >    result;
      
      
      // metto in tA in quello più piccolo 
      tA[0] = A1;	tA[1] = A2;
      tB[0] = B1;	tB[1] = B2;
      
      // li oridino
      std::sort(tA.begin(), tA.end());
      std::sort(tB.begin(), tB.end());
      
      // controllo chi è più in la (ossia verso meno infinito) sia tA
      if(tB[0]<(tA[0]-toll))
      { 
	  std::copy(tA.begin(),  tA.end(),  tmp.begin());
	  std::copy(tB.begin(),  tB.end(),  tA.begin());
	  std::copy(tmp.begin(), tmp.end(), tB.begin());
      }
      
      // calcolo le lunghezze
      lungA = fabs(tA[0]-tA[1]);
      lungB = fabs(tB[0]-tB[1]);
      
      // ----------------
      // CASI DEGENERI!!!
      // ----------------
      // controllo le lunghezze e guardo i casi degeneri
      if(lungA<toll)
      {
	  if(fabs(tA[0]-tB[0])<toll)
	  {
	      result.first = true;
	      result.second.assign(1, 0.0);
	      result.second[0]=tA[1];
	      
	      // ritorno il risultato 
	      return(result);
	  }
	  else
	  {
	      result.first = false;
	      result.second.clear();
	  
	      // ritorno il risultato 
	      return(result); 
	  }
      }
      // ----------------
      // CASI DEGENERI!!!
      // ----------------
      // controllo le lunghezze e guardo i casi degeneri
      if(lungB<toll)
      {
	  if(tB[0]<tA[1]+toll)
	  {
	      result.first = true;
	      result.second.assign(1, 0.0);
	      result.second[0]=tB[0];
	      
	      // ritorno il risultato 
	      return(result);
	  }
	  else
	  {
	      result.first = false;
	      result.second.clear();
	  
	      // ritorno il risultato 
	      return(result); 
	  }
      }
      
      // in questo modo siamo sicuri che non intersecano 
      if(tA[1]<(tB[0]-toll))
      {
	  result.first = false;
	  result.second.clear();
	  
	  // ritorno il risultato 
	  return(result);
      }
      // in questo caso intersecano solo per un punto 
      else if(fabs(tA[1]-tB[0])<toll)
      {
	  result.first = true;
	  result.second.assign(1, 0.0);
	  result.second[0]=tA[1];
	  
	  // ritorno il risultato 
	  return(result);
      }
      // siamo sicuri che intersecano 
      else
      {
	  result.first = true;
      }
      
      // faccio un merge 
      tmp.assign(4,0.0);
      std::merge(tA.begin(), tA.end(), tB.begin(), tB.end(), tmp.begin());
      
      // controllo che non ci siano doppi
      if( (fabs(tmp[0]-tmp[1])<toll) && (fabs(tmp[2]-tmp[3])<toll))
      {
	  result.second.assign(2, 0.0);
	  result.second[0]=tmp[0];
	  result.second[1]=tmp[2];
      }
      else if(fabs(tmp[0]-tmp[1])<toll) 
      {
	  result.second.assign(2, 0.0);
	  result.second[0]=tmp[0];
	  result.second[1]=tmp[2];
      }
      else if(fabs(tmp[1]-tmp[2])<toll) 
      {
	  result.second.assign(2, 0.0);
	  result.second[0]=tmp[1];
	  result.second[1]=tmp[3];
      }
      else if(fabs(tmp[2]-tmp[3])<toll) 
      {
	  result.second.assign(2, 0.0);
	  result.second[0]=tmp[1];
	  result.second[1]=tmp[3];
      }
      else
      {
	  result.second.assign(2, 0.0);
	  result.second[0]=tmp[1];
	  result.second[1]=tmp[2];
      }
      
      return(result);      
}

bool intervalIntersection::doIntersec(Real A1, Real A2, Real B1, Real B2)
{
      // Variabili temporanee
      Real                    lungA,lungB; 
      vector<Real>         	tA(2,0.0);
      vector<Real>         	tB(2,0.0);
      vector<Real>             tmp(2,0.0);
      
      // metto in tA in quello più piccolo 
      tA[0] = A1;	tA[1] = A2;
      tB[0] = B1;	tB[1] = B2;
      
      // li oridino
      std::sort(tA.begin(), tA.end());
      std::sort(tB.begin(), tB.end());
      
      // controllo chi è più in la sia tA
      if(tB[0]<(tA[0]-toll))
      { 
	  std::copy(tA.begin(),  tA.end(),  tmp.begin());
	  std::copy(tB.begin(),  tB.end(),  tA.begin());
	  std::copy(tmp.begin(), tmp.end(), tB.begin());
      }
      
      // calcolo le lunghezze
      lungA = fabs(tA[0]-tA[1]);
      lungB = fabs(tB[0]-tB[1]);
      
      // ----------------
      // CASI DEGENERI!!!
      // ----------------
      // controllo le lunghezze e guardo i casi degeneri
      if(lungA<toll)
      {
	  if(fabs(tA[0]-tB[0])<toll)
	  {
	      // ritorno il risultato 
	      return(true);
	  }
	  else
	  {
	      // ritorno il risultato 
	      return(false); 
	  }
      }
      // ----------------
      // CASI DEGENERI!!!
      // ----------------
      // controllo le lunghezze e guardo i casi degeneri
      if(lungB<toll)
      {
	  if(tB[0]<(tA[1]-toll))
	  {
	      // ritorno il risultato 
	      return(true);
	  }
	  else
	  {
	      // ritorno il risultato 
	      return(false); 
	  }
      }
      
      // in questo modo siamo sicuri che non intersecano 
      if(tA[1]<(tB[0]-toll))
      {
	  // ritorno il risultato 
	  return(false);
      }
      // in questo caso intersecano solo per un punto 
      else if(fabs(tA[1]-tB[0])<toll)
      {	  
	  // ritorno il risultato 
	  return(true);
      }
      
      // siamo sicuri che intersecano 
      return(true);
}


//
// Messa a punto di toll
//
void intervalIntersection::setToll(Real _toll)
{
      toll = _toll;
}

Real intervalIntersection::getToll()
{
      return(toll);
}

//
// Metodo che stampa
//
void intervalIntersection::print(pair<bool, vector<Real> > result)
{
      if(result.first)
      {
	  cout << "Intersecano" << endl;
	  for(UInt i=0; i<result.second.size(); ++i)
	    cout << result.second[i] << "  ";
	  cout << endl;
      }
      else
      {
	  cout << "NON Intersecano" << endl;
	  cout << "il size del secondo membro è " << result.second.size() << endl;
      }
  
}
