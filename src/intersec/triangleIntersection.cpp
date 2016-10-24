#include "triangleIntersection.h"


#include "../geometry/mesh2d.hpp" 
#include "../file/createFile.h" 

using namespace std;
using namespace geometry;

//
// Costruttori
//
triangleIntersection::triangleIntersection()
{
      // setto la tolleranza  
      toll = 1e-15;
      inSeg.setToll(toll);
      inTria.setToll(toll);
      lineInter.setToll(toll);
}

//
// Metodi per trovare la posizione del punto
//
pair<int, vector<point> > triangleIntersection::intersec(vector<point> * abc, vector<point> * def)
{
      // assert per essere sicuri che l'input sia giusto
      assert(abc->size()==3);
      assert(def->size()==3);
      
      // variabili in uso 
      bool       aIn2,bIn2,cIn2,dIn1,eIn1,fIn1;
      bool                          equalPt[3];
      Real                       normaA,normaB;
      point                normalA,normalB,dir;
      pair<int, vector<point> >         result;
      pair<int, vector<point> >      resultTmp;
      pair<int, vector<point> >     resultLato;
      vector<point>              segABC,segDEF;
      vector<point>		       latoExt;
      
      // calcolo le normali 
      normalA = (abc->at(1)-abc->at(0))^(abc->at(2)-abc->at(0));
      normaA  = normalA.norm2();
      
      // controllo la norma
      if(normaA<(toll*toll))
      {
	  cout << "Il triangolo che sto intersecando è degenere" << endl;
	  result.first = NOTINTERSEC;
	  result.second.clear();
	  return(result);
      }
      else
      {
	  normalA = normalA / normaA;
      }
      
      // calcolo le normali 
      normalB = (def->at(1)-def->at(0))^(def->at(2)-def->at(0));
      normaB  = normalB.norm2();
      
      // controllo la norma
      if(normaB<(toll*toll))
      {
	  cout << "Il triangolo che sto intersecando è degenere" << endl;
	  result.first = NOTINTERSEC;
	  result.second.clear();
	  return(result);
      }
      else
      {
	  normalB = normalB / normaB;
      }
            
      // ----------------------------------------------------
      //            controllo che non siano coincidenti
      // ----------------------------------------------------
      
      // setto la variabile 
      equalPt[0] = false;
      equalPt[1] = false;
      equalPt[2] = false;
      
      // ciclo su tutti i nodi per controllare che siano coincidenti 
      for(UInt i=0; i<3; ++i)
      {
	  for(UInt j=0; j<3; ++j)
	  {
	      if((abc->at(i)-def->at(j)).norm2()<toll)	equalPt[i]=true;
	  }
      }
      
      // se sono tutti uguali ritorno 
      if(equalPt[0] && equalPt[1] && equalPt[2])
      {
	  result.first = COINCIDE;
	  result.second.clear();
	  return(result);
      }
      
      // ----------------------------------------------------
      //            controllo che siano complanari 
      // ----------------------------------------------------
      
      // controllo le normali
      if(fabs(normalA*normalB-1.0)<toll)
      {
	  // controllo che siano complanari 
	  aIn2 = inTria.isIn(def, abc->at(0));
	  bIn2 = inTria.isIn(def, abc->at(1));
	  cIn2 = inTria.isIn(def, abc->at(2));
	  dIn1 = inTria.isIn(abc, def->at(0));
	  eIn1 = inTria.isIn(abc, def->at(1));
	  fIn1 = inTria.isIn(abc, def->at(2));	 
	  
	  // in questo caso sappiamo che def contiene abc
	  if(aIn2 && bIn2 && cIn2)
	  {
		result.first = CONTENT;
		result.second.resize(3);
		result.second[0] = abc->at(0);
		result.second[1] = abc->at(1);
		result.second[2] = abc->at(2);
		return(result);
	  }
	  else if(dIn1 && eIn1 && fIn1)
	  {
		result.first = CONTENT;
		result.second.resize(3);
		result.second[0] = def->at(0);
		result.second[1] = def->at(1);
		result.second[2] = def->at(2);
		return(result);
	  }
	  // caso in cui un lato o un punto sono dentro al triangolo  
	  else if((!aIn2 && bIn2  && cIn2) || (aIn2 && !bIn2 && cIn2)  || (aIn2 && bIn2 && !cIn2) || 
		  (!aIn2 && !bIn2 && cIn2) || (aIn2 && !bIn2 && !cIn2) || (!aIn2 && bIn2 && !cIn2))
	  {
		result.first =CONTENT;
		return(result);
		
	  }
	  else if((!dIn1 && eIn1  && fIn1) || (dIn1 && !eIn1 && fIn1)  || (dIn1 && eIn1 && !fIn1) ||
		  (!dIn1 && !eIn1 && fIn1) || (dIn1 && !eIn1 && !fIn1) || (!dIn1 && eIn1 && !fIn1))
	  {
		result.first =CONTENT;
		return(result);
		
	  }
	  
	  //
	  // Analisi più dettagliata dei lati 
	  //
	  
	  // faccio un reserve
	  result.second.reserve(6);
	  
	  for(UInt s=0; s<3; ++s)
	  {
	    // considero il triangolo def fissato e faccio variare i lati  di abc 
	    latoExt.clear();
	    latoExt.resize(2);
	    latoExt[0] = abc->at(s);	latoExt[1] = abc->at((s+1)%3);
	  
	    // interseco 
	    resultLato = triaLineInter.intersec(def, &latoExt);
	  
	    // slavo nel vettore 
	    if(resultLato.first!=0)	
	      for(UInt k=0; k<resultLato.second.size(); ++k)
		result.second.push_back(resultLato.second[k]);
	  }
	  
	  if(result.second.size()!=0)
	  {
	      result.first = CONTENT;
	      return(result);
	  }
	  else
	  {
	      result.first = NOTINTERSEC;
	      return(result);
	  }	  
      }
      
      // ----------------------------------------------------
      //     controllo il caso in cui non sono complanari
      // ----------------------------------------------------
      
      // calcolo i segmenti 
      findPoint(def, abc, &segABC);
      findPoint(abc, def, &segDEF);
      
      // agisco in modo differente a seconda del size dei vettori
      // 
      // l'out di findPoint infatti è un vettore che ha size o 1 o 2 o 0
      // 
      if((segABC.size()==1) && (segDEF.size()==1))
      {
	   if((segABC[0]-segDEF[0]).norm2()<toll)
	   {
	      result.first = INTERSEC;
	      result.second.resize(1);
	      result.second[0] = segABC[0];
	      return(result);
	   }
	   else
	   {
	      result.first = NOTINTERSEC;
	      result.second.clear();
	      return(result);
	   }
      }
      else if((segABC.size()==1) && (segDEF.size()==2))
      {
	   if(inSeg.isIn(&segDEF, segABC[0]))
	   {
	      result.first = INTERSEC;
	      result.second.resize(1);
	      result.second[0] = segABC[0];
	      return(result);
	   }
	   else
	   {
	      result.first = NOTINTERSEC;
	      result.second.clear();
	      return(result);
	   }
      }
      else if((segDEF.size()==1) && (segABC.size()==2))
      {
	   if(inSeg.isIn(&segABC, segDEF[0]))
	   {
	      result.first = INTERSEC;
	      result.second.resize(1);
	      result.second[0] = segDEF[0];
	      return(result);
	   }
	   else
	   {
	      result.first = NOTINTERSEC;
	      result.second.clear();
	      return(result);
	   }
      }
      else if((segABC.size()==0) || (segDEF.size()==0))
      {
	    result.first = NOTINTERSEC;
	    result.second.clear();
	    return(result);
      }
      
      // restituisco il riusltato
      resultTmp = lineInter.intersec(&segABC, &segDEF);
      
      // controllo il risultato di resultTmp e lo interpreto nel caso dei triangoli
      if(resultTmp.first==NOTINTERSEC)
      {
	  result.first = NOTINTERSEC;
	  result.second.clear();
	  return(result);
      }
      else
      {
	  result.first = INTERSEC;
	  result.second = resultTmp.second; 
	  
	  return(result);
      }
      
      // ritorno falso perché non ho trovato nulla 
      result.first = NOTINTERSEC;
      result.second.clear();
      return(result);
}

bool triangleIntersection::doIntersec(vector<point> * abc, vector<point> * def)
{
      // assert per essere sicuri che l'input sia giusto
      assert(abc->size()==3);
      assert(def->size()==3);
      
      // variabili in uso 
      bool       aIn2,bIn2,cIn2,dIn1,eIn1,fIn1,resultTmp;
      bool                                    equalPt[3];
      Real                                 normaA,normaB;
      point                          normalA,normalB,dir;
      vector<point>                        segABC,segDEF;
      
      // calcolo le normali 
      normalA = (abc->at(1)-abc->at(0))^(abc->at(2)-abc->at(0));
      normaA  = normalA.norm2();
      
      // controllo la norma
      if(normaA<(toll*toll))
      {
	  cout << "Il triangolo che sto intersecando è degenere (abc)" << endl;
	  return(false);
      }
      else
      {
	  normalA = normalA / normaA;
      }
      
      // calcolo le normali 
      normalB = (def->at(1)-def->at(0))^(def->at(2)-def->at(0));
      normaB  = normalB.norm2();
      
      // controllo la norma
      if(normaB<(toll*toll))
      {
	  cout << "Il triangolo che sto intersecando è degenere (def)" << endl;
	  return(false);
      }
      else
      {
	  normalB = normalB / normaB;
      }
      
      // ----------------------------------------------------
      //            controllo che non siano coincidenti
      // ----------------------------------------------------
      
      // setto la variabile 
      equalPt[0] = false;
      equalPt[1] = false;
      equalPt[2] = false;
      
      // ciclo su tutti i nodi per controllare che siano coincidenti 
      for(UInt i=0; i<3; ++i)
      {
	  for(UInt j=0; j<3; ++j)
	  {
	      if((abc->at(i)-def->at(j)).norm2()<toll)	equalPt[i]=true;
	  }
      }
      
      // se sono tutti uguali ritorno 
      if(equalPt[0] && equalPt[1] && equalPt[2])	return(true);
	
      // ----------------------------------------------------
      //            controllo che siano complanari 
      // ----------------------------------------------------
      
      // controllo le normali
      if(fabs(normalA*normalB-1.0)<toll)
      {
	  // controllo che siano complanari 
	  aIn2 = inTria.isIn(def, abc->at(0));
	  bIn2 = inTria.isIn(def, abc->at(1));
	  cIn2 = inTria.isIn(def, abc->at(2));
	  dIn1 = inTria.isIn(abc, def->at(0));
	  eIn1 = inTria.isIn(abc, def->at(1));
	  fIn1 = inTria.isIn(abc, def->at(2));
	  
	  if(dIn1 || eIn1 || fIn1 || aIn2 || bIn2 || cIn2)	return(true);
	  else							return(false);
      }
      
      // ----------------------------------------------------
      //     controllo il caso in cui non sono complanari
      // ----------------------------------------------------
     
      // calcolo i segmenti 
      findPoint(def, abc, &segABC);
      findPoint(abc, def, &segDEF);
      
      // agisco in modo differente a seconda del size dei vettori
      if((segABC.size()==1) && (segDEF.size()==1))
      {
	   if((segABC[0]-segDEF[0]).norm2()<toll) return(true);
	   else					  return(false);
      }
      else if((segABC.size()==1) && (segDEF.size()==2))
      {
	   if(inSeg.isIn(&segDEF, segABC[0]))     return(true);
	   else					  return(false);
      }
      else if((segDEF.size()==1) && (segABC.size()==2))
      {
	   if(inSeg.isIn(&segABC, segDEF[0]))     return(true);
	   else					  return(false);
      }
      else if((segABC.size()==0) || (segDEF.size()==0))
      {
	    return(false);
      }
      
      // restituisco il riusltato
      resultTmp = lineInter.doIntersec(&segABC, &segDEF);
	
      // ritorno falso perché non ho trovato nulla 
      return(resultTmp);
}

void triangleIntersection::findPoint(vector<point> * plane, vector<point> * tria, vector<point> * pt)
{
      // assert per essere sicuri che l'input sia giusto
      assert(plane->size()==3);
      assert(tria->size()==3);
      
      // mi assicuro che il vettore sia vuoto 
      pt->clear();
      pt->reserve(2);
      
      // variabili in uso 
      point                          normal;
      Real    dist0,dist1,dist2,noto,modulo;
      vector<point>                    lato;
      
      // calcolo il piano 
      normal = (plane->at(1)-plane->at(0))^(plane->at(2)-plane->at(0));
      modulo = normal.norm2();
      
      // controllo che il piano sia ok
      if(modulo<(toll*toll))
      {
	  cout << "Il piano preso in esame è degenere" << endl;
	  return;
      }
      else
      {
	    // normalizzo 
	   normal = normal / modulo;
      }
      
      // creo il termine noto 
      noto = plane->at(0)*normal*(-1.0);
 
      // calcolo la distanza con segno 
      dist0 = inTria.signedDist(plane, tria->at(0));
      dist1 = inTria.signedDist(plane, tria->at(1));
      dist2 = inTria.signedDist(plane, tria->at(2));
      
      // --------------------------------------------------
      // controllo che tutti i lati non giaciano sul piano 
      // --------------------------------------------------
      
      if((fabs(dist0)<toll) && (fabs(dist1)<toll))
      {
	    pt->push_back(tria->at(0));
	    pt->push_back(tria->at(1));
	    return;
      }
      else if((fabs(dist1)<toll) && (fabs(dist2)<toll))
      {
	    pt->push_back(tria->at(1));
	    pt->push_back(tria->at(2));
	    return;
      }
      else if((fabs(dist2)<toll) && (fabs(dist0)<toll))
      {
	    pt->push_back(tria->at(2));
	    pt->push_back(tria->at(0));
	    return;
      }
      
      // ---------------------------------------------------------
      // controllo che non ci sia un solo punto che sia sul piano 
      // ---------------------------------------------------------
      
      if(fabs(dist0)<toll)
      {
	  // metto il punto che giace sul piano
	  pt->push_back(tria->at(0));
	  
	  // provo a vedere se l'altro interseca 
	  if(dist1*dist2<0.0)	pt->push_back(findPoint(tria->at(1), tria->at(2), normal, noto));
	  
	  // ritorno
	  return;
      }
      else if(fabs(dist1)<toll)
      {	
	  // metto il punto che giace sul piano
	  pt->push_back(tria->at(1));
	  
	  // provo a vedere se l'altro interseca 
	  if(dist2*dist0<0.0)	pt->push_back(findPoint(tria->at(2), tria->at(0), normal, noto));
	  
	  // ritorno
	  return;
      }
      else if(fabs(dist2)<toll) 	
      {
	  // metto il punto che giace sul piano
	  pt->push_back(tria->at(2));
	  
	  // provo a vedere se l'altro interseca 
	  if(dist0*dist1<0.0)	pt->push_back(findPoint(tria->at(0), tria->at(1), normal, noto));
	  
	  // ritorno
	  return;
      }
      
      // ---------------------------------------------------------
      //      controllo che non ci siano elementi a cavallo 
      // ---------------------------------------------------------
      
      if(((dist0*dist1)<0.0) && ((dist0*dist2)<0.0))
      {
	  pt->push_back(findPoint(tria->at(0), tria->at(1), normal, noto));
	  pt->push_back(findPoint(tria->at(0), tria->at(2), normal, noto));
      }
      else if(((dist1*dist2)<0.0) && ((dist1*dist0)<0.0))
      {
	  pt->push_back(findPoint(tria->at(1), tria->at(2), normal, noto));
	  pt->push_back(findPoint(tria->at(1), tria->at(0), normal, noto));
      }
      else if(((dist2*dist0)<0.0) && ((dist2*dist1)<0.0))
      {
	  pt->push_back(findPoint(tria->at(2), tria->at(0), normal, noto));
	  pt->push_back(findPoint(tria->at(2), tria->at(1), normal, noto));
      }
      
}

point triangleIntersection::findPoint(point A, point B, point normal, Real noto)
{
      // variabili in uso 
      Real              t;
      point             p;
      
      // faccio i conti senza ulteriori controlli
      
      // trovo il coefficiente t
      t = (-noto-A*normal)/((B-A)*normal);
      
      // creo il punto 
      p = A+(B-A)*t;
      
      // ritorno
      return(p);
}

//
// Messa a punto di toll
//
void triangleIntersection::setToll(Real _toll)
{
      toll = _toll;
      inSeg.setToll(toll);
      inTria.setToll(toll);
      lineInter.setToll(toll);
      triaLineInter.setToll(toll);
}

Real triangleIntersection::getToll()
{
      return(toll);
}

//
// Metodi di stampa 
//
void triangleIntersection::print(pair<int, vector<point> > result)
{
      switch(result.first)
      {
	case(0):
		cout << "NON Intersecano" << endl;
		assert(result.second.size()==0);
		break;
	case(1):
		cout << "Intersecano" << endl;
		for(UInt i=0; i<result.second.size(); ++i)	result.second[i].print();
		break;
	case(3):
		cout << "Contenuti" << endl;
		break;
	case(4):
		cout << "Coincidono" << endl;
		break;
      }
}






