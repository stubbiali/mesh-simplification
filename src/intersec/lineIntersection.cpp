#include "lineIntersection.h"

using namespace std;
using namespace geometry;

//
// Costruttori
//
lineIntersection::lineIntersection()
{
      // setto la tolleranza  
      toll = 1e-15;
      inSeg.setToll(toll);
}

//
// Metodi per trovare la posizione del punto
//
pair<int, vector<point> > lineIntersection::intersec(vector<point> * ab, vector<point> * cd)
{
      // assert per essere sicuri che l'input sia giusto
      assert(ab->size()==2);
      assert(cd->size()==2);
      
      //
      // Questo medo di fare le intersezioni è ispirato a quello preso alla pagina web Wolfram MathWord line-line Intersection
      //
      
      // variabili in uso
      Real        samePlane,distAC,distAD,distBC,distBD;
      bool          AinCD,BinCD,CinAB,DinAB,PinAB,PinCD;
      pair<int, vector<point> >			 result;
      
      // calcolo se sono sullo stesso piano 
      samePlane = (cd->at(1)-ab->at(0))*((ab->at(1)-ab->at(0))^(cd->at(1)-cd->at(0)));
      
      // se non sono complanari mi fermo 
      if(fabs(samePlane)>toll)
      {
	    result.first = NOTINTERSEC;
	    return(result);
      }
      
      // calcolo le varie distanze per controllare che non siamo in altri casi particolari
      distAC = (ab->at(0)-cd->at(0)).norm2();
      distAD = (ab->at(0)-cd->at(1)).norm2();
      distBC = (ab->at(1)-cd->at(0)).norm2();
      distBD = (ab->at(1)-cd->at(1)).norm2();
      
      // ------------------------------------------------------------
      //		CONTROLLO LA COINCIDENZA 
      // ------------------------------------------------------------
      // coincidono
      if((distAC<toll) && (distBD<toll))
      {
	    result.first = COINCIDE;
	    result.second.resize(4);
	    result.second[0] = ab->at(0);
	    result.second[1] = cd->at(0);
	    result.second[2] = ab->at(1);
	    result.second[3] = cd->at(1);
	    return(result);
      }
      else if((distAD<toll) && (distBC<toll))
      {
	    result.first = COINCIDE;
	    result.second.resize(4);
	    result.second[0] = ab->at(0);
	    result.second[1] = cd->at(1);
	    result.second[2] = ab->at(1);
	    result.second[3] = cd->at(0);
	    return(result);
      }
      
      // ------------------------------------------------------------
      //		CONTROLLO SE SONO CONTENUTI
      // ------------------------------------------------------------
      
      // controllo che non siano coincidenti
      AinCD = inSeg.isIn(cd, ab->at(0));
      BinCD = inSeg.isIn(cd, ab->at(1));
      CinAB = inSeg.isIn(ab, cd->at(0));
      DinAB = inSeg.isIn(ab, cd->at(1));
      
      // controllo che AB sia contenuto in CD
      if(AinCD && BinCD)
      {
	    result.first = CONTENT;
	    result.second.resize(2);
	    result.second[0] = ab->at(0);
	    result.second[1] = ab->at(1);
	    return(result);
      }
      // controllo che CD sia contenuto in AB
      else if(CinAB && DinAB)
      {
	    result.first = CONTENT;
	    result.second.resize(2);
	    result.second[0] = cd->at(0);
	    result.second[1] = cd->at(1);
	    return(result);
      }
      
      // ------------------------------------------------------------
      //		CONTROLLO SE SONO CONSECUTIVI
      // ------------------------------------------------------------
      // sono consecutivi per l'estremo AC
      if(distAC<toll)
      {
	    result.first = CONSECUTIVE;
	    result.second.resize(2);
	    result.second[0] = ab->at(0);
	    result.second[1] = cd->at(0);
	    return(result);
      }
      // sono consecutivi per l'estremo AD
      else if(distAD<toll)
      {
	    result.first = CONSECUTIVE;
	    result.second.resize(2);
	    result.second[0] = ab->at(0);
	    result.second[1] = cd->at(1);
	    return(result);
      }
      // sono consecutivi per l'estremo BC
      else if(distBC<toll)
      {
	    result.first = CONSECUTIVE;
	    result.second.resize(2);
	    result.second[0] = ab->at(1);
	    result.second[1] = cd->at(0);
	    return(result);
      }
      // sono consecutivi per l'estremo BD
      else if(distBD<toll)
      {
	    result.first = CONSECUTIVE;
	    result.second.resize(2);
	    result.second[0] = ab->at(1);
	    result.second[1] = cd->at(1);
	    return(result);
      }
      
      // Qui c'è ancora il rischio che i due segmenti giaciano sullo stesso piano e siano uno contenuto nell'altro potrei avere
      // un caso come in fiugra sotto 
      // 
      //       a<---------c<-------->b------->d
      // 
      point a    = ab->at(1)-ab->at(0);
      point b    = cd->at(1)-cd->at(0);
      point c    = cd->at(0)-ab->at(0);
      
      // se sono paralleli 
      if((a^b).norm2()<toll)
      {
	      // a questo livello al massimo sono contenuti in parte
	      result.second.reserve(4);	      
	      if(AinCD)		result.second.push_back(ab->at(0));
	      if(BinCD)		result.second.push_back(ab->at(1));
	      if(CinAB)		result.second.push_back(cd->at(0));
	      if(DinAB)		result.second.push_back(cd->at(1));
	      
	      // controllo se non ho almeno aggiunto due punti 
	      if(result.second.size()>=1)	 result.first=INTERSEC;
	      else				 result.first=NOTINTERSEC;
	      
	      return(result);
      }
      
      // ------------------------------------------------------------
      //		CALCOLO L'INTERSEZIONE
      // ------------------------------------------------------------
      
      // calcolo l'intersezione:
      // a questo livello siamo sicuri che i due segmenti INTERSECANO e NON ci sono casi degeneri!!

      Real abCrossProductNorm = (a^b).norm2();
      Real  coef = ((c^b)*(a^b))/(abCrossProductNorm);
      point p    = ab->at(0)+a*coef;
      
      // controllo che sia dentro all'intervallo perché con i conti di prima si calcola l'intersezione fra le due rette 
      PinAB = inSeg.isIn(ab, p);
      PinCD = inSeg.isIn(cd, p);
      
      if(PinAB && PinCD)
      {
	  result.first = INTERSEC;
	  result.second.resize(1);
	  result.second[0] = p;
	  return(result);	
      }
      
      result.first = NOTINTERSEC;
      return(result);
}

bool lineIntersection::doIntersec(vector<point> * ab, vector<point> * cd)
{
      // assert per essere sicuri che l'input sia giusto
      assert(ab->size()==2);
      assert(cd->size()==2);
      
      //
      // Questo medo di fare le intersezioni è ispirato a quello preso alla pagina web Wolfram MathWord line-line Intersection
      //
      
      // variabili in uso
      Real        samePlane,distAC,distAD,distBC,distBD;
      bool          AinCD,BinCD,CinAB,DinAB,PinAB,PinCD;
      
      // calcolo se sono sullo stesso piano 
      samePlane = (cd->at(1)-ab->at(0))*((ab->at(1)-ab->at(0))^(cd->at(1)-cd->at(0)));
      
      // se non sono complanari mi fermo 
      if(fabs(samePlane)>toll)	return(false);
      
      // calcolo le varie distanze per controllare che non siamo in altri casi particolari
      distAC = (ab->at(0)-cd->at(0)).norm2();
      distAD = (ab->at(0)-cd->at(1)).norm2();
      distBC = (ab->at(1)-cd->at(0)).norm2();
      distBD = (ab->at(1)-cd->at(1)).norm2();
      
      // ------------------------------------------------------------
      //		CONTROLLO LA COINCIDENZA 
      // ------------------------------------------------------------
      // coincidono
      if(((distAC<toll) && (distBD<toll)) || ((distAD<toll) && (distBC<toll)))    return(true);
      
      // ------------------------------------------------------------
      //		CONTROLLO SE SONO CONTENUTI
      // ------------------------------------------------------------
      
      // controllo che non siano coincidenti
      AinCD = inSeg.isIn(cd, ab->at(0));
      BinCD = inSeg.isIn(cd, ab->at(1));
      CinAB = inSeg.isIn(ab, cd->at(0));
      DinAB = inSeg.isIn(ab, cd->at(1));
      
      // controllo che AB sia contenuto in CD
      if((AinCD && BinCD) || (CinAB && DinAB))   return(true);
      
      // ------------------------------------------------------------
      //		CONTROLLO SE SONO CONSECUTIVI
      // ------------------------------------------------------------
      // sono consecutivi per l'estremo AC
      if((distAC<toll) || (distAD<toll) || (distBC<toll) || (distBD<toll))	   return(true);
   
      // Qui c'è ancora il rischio che i due segmenti giaciano sullo stesso piano e siano uno contenuto nell'altro potrei avere
      // un caso come in fiugra sotto 
      // 
      //       a<---------c<-------->b------->d
      // 
      point a    = ab->at(1)-ab->at(0);
      point b    = cd->at(1)-cd->at(0);
      point c    = cd->at(0)-ab->at(0);
      
      // se sono paralleli 
      if((a^b).norm2()<toll) 
      {
	      // a questo livello al massimo sono contenuti in parte
	      if((AinCD) || (BinCD) || (CinAB) || (DinAB))	return(true);
	      else						return(false);
      }
      
      // ------------------------------------------------------------
      //		CALCOLO L'INTERSEZIONE
      // ------------------------------------------------------------
      
      // calcolo l'intersezione:
      // a questo livello siamo sicuri che i due segmenti INTERSECANO e NON ci sono casi degeneri!!

      Real abCrossProductNorm = (a^b).norm2();
      Real  coef = ((c^b)*(a^b))/(abCrossProductNorm);
      point p    = ab->at(0)+a*coef;
      
      // controllo che sia dentro all'intervallo perché con i conti di prima si calcola l'intersezione fra le due rette 
      PinAB = inSeg.isIn(ab, p);
      PinCD = inSeg.isIn(cd, p);
      
      // siamo sicuri che coincidono
      if(PinAB && PinCD)	 return(true);	
      
      // se non siamo usciti prima non coincidono 
      return(false);
}



//
// Messa a punto di toll
//
void lineIntersection::setToll(Real _toll)
{
      toll = _toll;
      inSeg.setToll(toll);
}

Real lineIntersection::getToll()
{
      return(toll);
}

//
// Metodi di stampa 
//
void lineIntersection::print(pair<int, vector<point> > result)
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
	case(2):
		cout << "Consecutivi" << endl;
		assert(result.second.size()==2);
		result.second[0].print();
		result.second[1].print();
		break;
	case(3):
		cout << "Contenuti" << endl;
		for(UInt i=0; i<result.second.size(); ++i)	result.second[i].print();
		break;
	case(4):
		cout << "Coincidono" << endl;
		assert(result.second.size()>=2);
		for(UInt i=0; i<result.second.size(); ++i)	result.second[i].print();
		break;
      }
}



