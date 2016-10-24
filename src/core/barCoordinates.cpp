#include "barCoordinates.h"

using namespace geometry;
using namespace std;

//
// Costruttore 
//
barCoordinates::barCoordinates()
{
      // setto la tolleranza 
      toll = 1e-15;
}

//
// Metodi che creano le coordinate baricentriche 
//
void barCoordinates::ofLine(vector<point> * vertices, point p, vector<Real> * bar)
{
      // mi assicuro che i vertici siano giusti
      assert(vertices->size()==2);
      
      // mi assicuro che bar sia vuoto 
      bar->assign(2, 0.0);
      
      // varaibli in uso 
      Real      lung,coor0,coor1;
      
      // calcolo la lunghezza 
      lung = (vertices->at(0)-vertices->at(1)).norm2();
      
      // controllo sulla lunghezza 
      if(lung<toll) cout << "Il segmento è troppo corto per poterci valutare le coordinate barcientriche" << endl;
      
      // calcolo le altre due lunghezze
      coor0 = (p-vertices->at(1)).norm2();
      coor1 = (p-vertices->at(0)).norm2();
      
      // le normalizzo con la lunghezza
      coor0 = coor0 / lung;
      coor1 = coor1 / lung;
      
      // setto i casi estremi in funzione della tolleranza
      if(fabs(coor0)<toll)		coor0 = 0.0;
      else if(fabs(coor0-1.0)<toll)	coor0 = 1.0;
      
      if(fabs(coor1)<toll)		coor1 = 0.0;
      else if(fabs(coor1-1.0)<toll)	coor1 = 1.0;
      
      // le metto in bar
      bar->at(0) = coor0;
      bar->at(1) = coor1;
}

void barCoordinates::ofTriangle(vector<point> * vertices, point p, vector<Real> * bar)
{
      // mi assicuro che i vertici siano giusti
      assert(vertices->size()==3);
      
      // mi assicuro che bar sia vuoto 
      bar->assign(3, 0.0);
      
      // varaibli in uso 
      Real      area,coor0,coor1,coor2;
      
      // calcolo la lunghezza 
      area = ((vertices->at(1)-vertices->at(0))^(vertices->at(2)-vertices->at(0))).norm2()*0.5;
      
      // controllo sulla lunghezza 
      if(area<(toll*toll)) cout << "Il triangolo ha area troppo piccola per la tolleranza fissata" << endl;
      
      // calcolo le altre tre aree
      coor0 = ((p-vertices->at(1))^(p-vertices->at(2))).norm2()*0.5;
      coor1 = ((p-vertices->at(2))^(p-vertices->at(0))).norm2()*0.5;
      coor2 = ((p-vertices->at(0))^(p-vertices->at(1))).norm2()*0.5;
      
      // le normalizzo con l'area del triangolo
      coor0 = coor0 / area;
      coor1 = coor1 / area;
      coor2 = coor2 / area;
      
      // setto i casi estremi in funzione della tolleranza
      if(fabs(coor0)<toll)		coor0 = 0.0;
      else if(fabs(coor0-1.0)<toll)	coor0 = 1.0;
      
      if(fabs(coor1)<toll)		coor1 = 0.0;
      else if(fabs(coor1-1.0)<toll)	coor1 = 1.0;
      
      if(fabs(coor2)<toll)		coor2 = 0.0;
      else if(fabs(coor2-1.0)<toll)	coor2 = 1.0;
      
      // le metto in bar
      bar->at(0) = coor0;
      bar->at(1) = coor1;
      bar->at(2) = coor2;
}

void barCoordinates::ofTetra(vector<point> * vertices, point p, vector<Real> * bar)
{
      // mi assicuro che i vertici siano giusti
      assert(vertices->size()==4);
      
      // mi assicuro che bar sia vuoto 
      bar->assign(4, 0.0);
      
      // varaibli in uso 
      Real      vol,coor0,coor1,coor2,coor3;
      

      // calcolo la lunghezza 
      vol = fabs(((vertices->at(1)-vertices->at(0))^(vertices->at(2)-vertices->at(0)))*(vertices->at(3)-vertices->at(0)))*(1./6.);
      
      // controllo sulla lunghezza 
      if(vol<(toll*toll*toll)) cout << "Il triangolo ha area troppo piccola per la tolleranza fissata" << endl;
      
      // calcolo le altre tre aree
      coor0 = fabs(((vertices->at(1)-p)^(vertices->at(2)-p))*(vertices->at(3)-p))*(1./6.);
      coor1 = fabs(((vertices->at(2)-p)^(vertices->at(3)-p))*(vertices->at(0)-p))*(1./6.);
      coor2 = fabs(((vertices->at(3)-p)^(vertices->at(0)-p))*(vertices->at(1)-p))*(1./6.);
      coor3 = fabs(((vertices->at(0)-p)^(vertices->at(1)-p))*(vertices->at(2)-p))*(1./6.);
      
      // le normalizzo con l'area del triangolo
      coor0 = coor0 / vol;
      coor1 = coor1 / vol;
      coor2 = coor2 / vol;
      coor3 = coor3 / vol;
      
      // setto i casi estremi in funzione della tolleranza
      if(fabs(coor0)<toll)		coor0 = 0.0;
      else if(fabs(coor0-1.0)<toll)	coor0 = 1.0;
      
      if(fabs(coor1)<toll)		coor1 = 0.0;
      else if(fabs(coor1-1.0)<toll)	coor1 = 1.0;
      
      if(fabs(coor2)<toll)		coor2 = 0.0;
      else if(fabs(coor2-1.0)<toll)	coor2 = 1.0;
      
      if(fabs(coor3)<toll)		coor3 = 0.0;
      else if(fabs(coor3-1.0)<toll)	coor3 = 1.0;
      
      // le metto in bar
      bar->at(0) = coor0;
      bar->at(1) = coor1;
      bar->at(2) = coor2;
      bar->at(3) = coor3;  
}

point barCoordinates::pointOfLine(vector<point> * vertices, vector<Real> * bar)
{
    // mi assicuro che i vertici siano giusti
    assert(vertices->size()==2);
    assert(bar->size()==2);
    
    return(vertices->at(0)*bar->at(0)+vertices->at(1)*bar->at(1));
}

point barCoordinates::pointOfTriangle(vector<point> * vertices, vector<Real> * bar)
{
    // mi assicuro che i vertici siano giusti
    assert(vertices->size()==3);
    assert(bar->size()==3);
    
    return(vertices->at(0)*bar->at(0)+vertices->at(1)*bar->at(1)+vertices->at(2)*bar->at(2));
}

point barCoordinates::pointOfTetra(vector<point> * vertices, vector<Real> * bar)
{
    // mi assicuro che i vertici siano giusti
    assert(vertices->size()==4);
    assert(bar->size()==4);
    
    return(vertices->at(0)*bar->at(0)+vertices->at(1)*bar->at(1)+vertices->at(2)*bar->at(2)+vertices->at(3)*bar->at(3));
}

ReferenceGeometry barCoordinates::whereIsNode(vector<Real> * bar)
{
    // variabili in uso 
    UInt 	contEdge=0,contFace=0,numCoor=bar->size();
    vector<Real>::iterator 			       it;
    
    // controllo che effettivamente sono coordinate barcentriche 
    if(fabs(accumulate(bar->begin(), bar->end(), 0.0)-1.)>toll)
    {
	cout << "ATTENZIONE: non mi hai passato coordinate baricentriche nel metodo \" whereIsNode \" " << endl;
	return(VERTEX);
    }
    
    // controllo se è su un vertice
    for(it=bar->begin(); it!=bar->end(); ++it)
    {
	if(fabs(*it-1.)<toll)		return(VERTEX);
    }
      
    // controllo se è su un lato 
    for(it=bar->begin(); it!=bar->end(); ++it)
    {
	if(fabs(*it)<toll)	++contEdge;
    }
    
    // distinguo come mi comporto a seconda dei casi 
    switch(numCoor)
    {
      case 1:
	      // caso di un edge 
	      return(EDGE);
	      break;
      case 3:
	      // caso di un triangolo 
	      if(contEdge==1)	return(EDGE);
	      break;
      case 4:
	      // caso di un tetraedro
	      if(contEdge==2)	return(EDGE);
	      break;
    }
    
    // controllo se è su una faccia 
    for(it=bar->begin(); it!=bar->end(); ++it)
    {
	if(fabs(*it)<toll)	++contFace;
    }
    
    // distinguo come mi comporto a seconda dei casi 
    switch(numCoor)
    {
      case 3:
	      // caso di un triangolo 
	      return(FACE);
	      break;
      case 4:
	      // caso di un tetraedro
	      if(contFace==1)	return(FACE);
	      break;
    }
    
    // a questo livello deve essere per forza dentro all'elemento di riferimento 
    return(VOLUME);
}

//
// Messa a punto di toll
//
void barCoordinates::setToll(Real _toll)
{
      toll = _toll;
}

Real barCoordinates::getToll()
{
      return(toll);
}



