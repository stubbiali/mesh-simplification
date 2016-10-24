#include "triangleLineIntersection.h"

using namespace std;
using namespace geometry;

//
// Costruttori
//
triangleLineIntersection::triangleLineIntersection()
{
      // setto le tolleranza  
      toll = 1e-15;
      lineInter.setToll(toll);
      inTria.setToll(toll);
}

//
// Metodi per trovare la posizione del punto
//
pair<int, vector<point> > triangleLineIntersection::intersec(vector<point> * abc, vector<point> * de)
{
      // assert per essere sicuri che l'input sia giusto
      assert(abc->size()==3);
      assert(de->size()==2);
      
      // variabili in gioco 
      Real       distD,distE,normalNorm,noto,val;
      bool   inP,inD,inE,interE1,interE2,interE3;
      point                      normale,dir,tmp;
      pair<int, vector<point> >           result;
      vector<point>                         lato;
      
      // direzione del segmento 
      dir = de->at(1)-de->at(0);
      
      // controllo la lunghezza 
      if(dir.norm2()<toll)	
      {
	  cout << "I punti del segmento preso in analisi sono troppo corti" << endl;
	  return(result);
      }
      
      // calcolo la normale
      normale = (abc->at(1)-abc->at(0))^(abc->at(2)-abc->at(0));
      normalNorm = normale.norm2();
      
      // controllo la lunghezza 
      if(normalNorm<(toll*toll))	
      {
	  cout << "Il triangolo perso in esame per l'intersezione linea traingolo è degenere" << endl;
	  return(result);
      }
      else
      {
	  normale = normale / normalNorm;
      }
      
      // controllo il termine noto del piano
      noto = abc->at(0)*normale*(-1.0);
      
      // calcolo le distanze con il segno e anche se gli estremi sono nel triangolo
      distD = inTria.signedDist(abc, de->at(0));
      distE = inTria.signedDist(abc, de->at(1));
      inD   = inTria.isIn(abc, de->at(0));
      inE   = inTria.isIn(abc, de->at(1));
      
      // controllo che i due punti siano sul piano del triangolo 
      if((fabs(distD)<toll) && (fabs(distE)<toll))
      {
	    // controllo che non sia un segmento totalmente contenuti 
	    if(inD && inE)
	    {
		  result.first = CONTENT;
		  result.second.resize(2);	    
		  result.second[0] = de->at(0);
		  result.second[1] = de->at(1);	    
		  return(result);
	    }
	    else if(inD)
	    {
		  result.first = CONTENT;
		  result.second.resize(1);	    
		  result.second[0] = de->at(0);
		  return(result);
	    }
	    else if(inE)
	    {
		  result.first = CONTENT;
		  result.second.resize(1);	    
		  result.second[0] = de->at(1);
		  return(result);
	    }
	    
	    // controllo le intersezioni con i lati 
	    lato.resize(2);
	    lato[0] = abc->at(0);
	    lato[1] = abc->at(1);
	    
	    interE1 = lineInter.doIntersec(&lato, de);
	    
	    lato.clear();
	    lato.resize(2);
	    lato[0] = abc->at(1);
	    lato[1] = abc->at(2);
	    
	    interE2 = lineInter.doIntersec(&lato, de);
	    
	    lato.clear();
	    lato.resize(2);
	    lato[0] = abc->at(2);
	    lato[1] = abc->at(0);
	    
	    interE3 = lineInter.doIntersec(&lato, de);
	        
	    // se ho intesecato almeno con un lato 
	    // 
	    // QUESTA ANALISI È MOLTO COMPLESSA PER IL MOMENTO CI LIMITIAMO A DIRE SE SONO CONTENUTI SENZA SPECIFICARE 
	    // I PUNTI DI INTERSEZIONE 
	    // 
	    if(interE1 || interE2 || interE3)
	    {
		  result.first = CONTENT;
		  result.second.clear();	    
		  return(result);
	    }
	    
	    // se non è vero nulla di prima è esterno 
	    result.first = NOTINTERSEC;
	    result.second.clear();	    
	    return(result);
      }
      
      // un punto è dentro l'altro è sul triangolo 
      if(inD)
      {
	    result.first = INTERSEC;
	    result.second.resize(1);	    
	    result.second[0] = de->at(0);
	    return(result);
      }
      else if(inE)
      {
	    result.first = INTERSEC;
	    result.second.resize(1);	    
	    result.second[0] = de->at(1);
	    return(result);
      }
	
      // a questo livello sappiamo che i punti non giaciono sul piano definito dal triangolo e le quantità non sono 
      // DEGENERI!!!
      
      // se il prodotto è positivo vuol dire che il segmento sta TOTALMENTE sopra o sotto il triangolo
      if((distD*distE)>0.0)
      {
	    result.first = NOTINTERSEC;
	    result.second.clear();
	    return(result);
      }
      
      // calcolo il valore 
      val = ((-1.0)*(de->at(0)*normale)-noto)/(dir*normale);
      
      // calcolo il punto 
      tmp = de->at(0)+dir*val;
      
      // controllo che il punto sia dentro il triangolo 
      inP = inTria.isIn(abc, tmp);
      
      // ritorno il valore perché sono sicuro che sè all'interno del triangolo
      if(inP)
      {
	    result.first = INTERSEC;
	    result.second.resize(1);
	    result.second[0]=tmp;
	    return(result);
      }
      
      // sono sicuro che non intersecano 
      result.first = NOTINTERSEC;
      result.second.clear();
      
      return(result);
      
}

bool triangleLineIntersection::doIntersec(vector<point> * abc, vector<point> * de)
{
      // assert per essere sicuri che l'input sia giusto
      assert(abc->size()==3);
      assert(de->size()==2);
      
      // variabili in gioco 
      Real       distD,distE,normalNorm,noto,val;
      bool   inP,inD,inE,interE1,interE2,interE3;
      point                      normale,dir,tmp;
      vector<point>                         lato;
      
      // direzione del segmento 
      dir = de->at(1)-de->at(0);
      
      // controllo la lunghezza 
      if(dir.norm2()<toll)	
      {
	  cout << "I punti del segmento preso in analisi sono troppo corti" << endl;
	  return(false);
      }
      
      // calcolo la normale
      normale = (abc->at(1)-abc->at(0))^(abc->at(2)-abc->at(0));
      normalNorm = normale.norm2();
      
      // controllo la lunghezza 
      if(normalNorm<toll)	
      {
	  cout << "Il triangolo perso in esame per l'intersezione linea traingolo è degenere" << endl;
	  return(false);
      }
      else
      {
	  normale = normale / normalNorm;
      }
      
      // controllo il termine noto del piano
      noto = abc->at(0)*normale*(-1.0);
      
      // calcolo le distanze con il segno e anche se gli estremi sono nel triangolo
      distD = inTria.signedDist(abc, de->at(0));
      distE = inTria.signedDist(abc, de->at(1));
      inD   = inTria.isIn(abc, de->at(0));
      inE   = inTria.isIn(abc, de->at(1));
      
      // controllo che i due punti siano sul piano del triangolo 
      if((fabs(distD)<toll) && (fabs(distE)<toll))
      {
	    // controllo che non sia un segmento totalmente contenuti 
	    if(inD || inE)	return(true);
	    
	    // controllo le intersezioni con i lati 
	    lato.resize(2);
	    lato[0] = abc->at(0);
	    lato[1] = abc->at(1);
	    
	    interE1 = lineInter.doIntersec(&lato, de);
	    
	    lato.clear();
	    lato.resize(2);
	    lato[0] = abc->at(1);
	    lato[1] = abc->at(2);
	    
	    interE2 = lineInter.doIntersec(&lato, de);
	    
	    lato.clear();
	    lato.resize(2);
	    lato[0] = abc->at(2);
	    lato[1] = abc->at(0);
	    
	    interE3 = lineInter.doIntersec(&lato, de);
	    
	    // se ho intesecato almeno con un lato 
	    // 
	    // QUESTA ANALISI È MOLTO COMPLESSA PER IL MOMENTO CI LIMITIAMO A DIRE SE SONO CONTENUTI SENZA SPECIFICARE 
	    // I PUNTI DI INTERSEZIONE 
	    // 
	    if(interE1 || interE2 || interE3)	return(true);
      }
      
      // un punto è dentro l'altro è sul triangolo 
      if(inD || inE)	return(true);
	
      // a questo livello sappiamo che i punti non giaciono sul piano definito dal triangolo e le quantità non sono 
      // DEGENERI!!!
      
      // se il prodotto è positivo vuol dire che il segmento sta TOTALMENTE sopra o sotto il triangolo
      if((distD*distE)>0.0)	return(false);
      
      // calcolo il valore 
      val = ((-1.0)*(de->at(0)*normale)-noto)/(dir*normale);
      
      // calcolo il punto 
      tmp = de->at(0)+dir*val;
      
      // controllo che il punto sia dentro il triangolo 
      inP = inTria.isIn(abc, tmp);
      
      // ritorno il valore perché sono sicuro che sè all'interno del triangolo
      if(inP)	return(true);
      
      
      return(false);
}



//
// Messa a punto di toll
//
void triangleLineIntersection::setToll(Real _toll)
{
      toll = _toll;
      lineInter.setToll(toll);
      inTria.setToll(toll);
}

Real triangleLineIntersection::getToll()
{
      return(toll);
}

//
// Metodi di stampa 
//
void triangleLineIntersection::print(pair<int, vector<point> > result)
{
      switch(result.first)
      {
	case(0):
		cout << "NON Intersecano" << endl;
		assert(result.second.size()==0);
		break;
	case(1):
		cout << "Intersecano" << endl;
		assert(result.second.size()==1);
		result.second[0].print();
		break;
	case(3):
		cout << "Contenuti" << endl;
		if(result.second.size()==0)
		{	
		    cout << "Intersezione complessa" << endl;
		}
		else
		{
		    for(UInt i=0; i<result.second.size(); ++i)	result.second[i].print();
		}
		break;
      }
}



