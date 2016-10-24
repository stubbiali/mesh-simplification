#include "triangleMapping.h"

using namespace geometry;

//
// costruttore 
//
triangleMapping::triangleMapping()
{
}
//
// Metodi che creano i triangoli 
//
void triangleMapping::createTriangle(vector<Real> * edgesLen, vector<Real> * angles, vector<point> * nodes)
{
    assert(edgesLen->size()==3);
    assert(angles->size()==3);
    
    // varaibili in uso 
    UInt angIndex,len2Index,len0Index;
    Real 	       ang0,len0,len2;
    
    // sistemo le variabili nel caso in cui il primo angolo è 90°
    if(fabs(angles->at(0)-90.)<1e-08)
    {
	angIndex  = 1;
	len2Index = 0;
	len0Index = 2;
    }
    
    // setto le variabili
    ang0  = (angles->at(angIndex)/180.)*PGRECO;
    len0  = edgesLen->at(len0Index);
    len2  = edgesLen->at(len2Index);
    
    // faccio un resize di nodes
    nodes->resize(3);
    
    // costruisco il primo (è sempre 0.0,0.0,0.0)
    nodes->at(0).setX(0.0);		nodes->at(0).setY(0.0);		      nodes->at(0).setZ(0.0);
    
    // costruisco il secondo (è sempre len0,0.0,0.0)
    nodes->at(1).setX(len0);		nodes->at(1).setY(0.0);		      nodes->at(1).setZ(0.0);
    
    // costruisco il terzo (è len2*cos(angles->at(0)),len2*sin(angles->at(0)),0.0)
    nodes->at(2).setX(len2*cos(ang0));	nodes->at(2).setY(len2*sin(ang0));    nodes->at(2).setZ(0.0);
}


//
// Metodi che legano un triangolo in 3d con uno in 2d
//
void triangleMapping::create2dTo3dMapIso(vector<point> * nodes, vector<Real> * map2dTo3d)
{
    // variabili in uso 
    Real xA,xB,xC,yA,yB,yC,zA,zB,zC,lAB,lAC,cosTheta,sinTheta;
    
    // prendo le coordinate dei punti 
    xA = nodes->at(0).getX();	yA = nodes->at(0).getY();    zA = nodes->at(0).getZ();
    xB = nodes->at(1).getX();	yB = nodes->at(1).getY();    zB = nodes->at(1).getZ();
    xC = nodes->at(2).getX();	yC = nodes->at(2).getY();    zC = nodes->at(2).getZ();
       
    // calcolo la lunghezza lAB e i coseni e seni 
    lAB = sqrt((xA-xB)*(xA-xB)+(yA-yB)*(yA-yB)+(zA-zB)*(zA-zB));
    lAC = sqrt((xA-xC)*(xA-xC)+(yA-yC)*(yA-yC)+(zA-zC)*(zA-zC));
    cosTheta = ((xC-xA)*(xB-xA)+(yC-yA)*(yB-yA)+(zC-zA)*(zB-zA))/(lAB*lAC);
    sinTheta = sqrt(1.-cosTheta*cosTheta);
    
    // aggiorno il vettore map2dTo3d
    map2dTo3d->resize(9,0.0);
    map2dTo3d->at(0) = (xB-xA)/lAB;
    map2dTo3d->at(1) = (yB-yA)/lAB;
    map2dTo3d->at(2) = (zB-zA)/lAB;
    map2dTo3d->at(3) = (xC-xA-map2dTo3d->at(0)*lAC*cosTheta)/(lAC*sinTheta);
    map2dTo3d->at(4) = (yC-yA-map2dTo3d->at(1)*lAC*cosTheta)/(lAC*sinTheta);
    map2dTo3d->at(5) = (zC-zA-map2dTo3d->at(2)*lAC*cosTheta)/(lAC*sinTheta);
    map2dTo3d->at(6) = xA;
    map2dTo3d->at(7) = yA;
    map2dTo3d->at(8) = zA;
}

void triangleMapping::create3dTo2dMapIso(vector<point> * nodes, vector<Real> * map3dTo2d)
{
    // variabili in uso 
    Real  a,b,c,d,e,f,deter,A1,A2,B1,B2,xA,yA,zA;
    vector<Real>	               map2dTo3d;
    
    // prendo l'altra mappa 
    create2dTo3dMapIso(nodes, &map2dTo3d);
    
    // a partire da quella faccio l'inversa
    a  = map2dTo3d[0];
    b  = map2dTo3d[1];
    c  = map2dTo3d[2];
    d  = map2dTo3d[3];
    e  = map2dTo3d[4];
    f  = map2dTo3d[5];
    xA = map2dTo3d[6];
    yA = map2dTo3d[7];
    zA = map2dTo3d[8];
    
    // calcolo i coefficienti A1, A2, B1, B2 e il determinante 
    A1    = a*a+b*b+c*c;
    B2    = d*d+e*e+f*f;
    A2    = a*d+b*e+c*f;
    B1    = A2;
    deter = A1*B2-B1*A2;
    
    // faccio un resize 
    map3dTo2d->resize(8,0.0);
    map3dTo2d->at(0) = (B2*a-A2*d)/deter;
    map3dTo2d->at(1) = (B2*b-A2*e)/deter;
    map3dTo2d->at(2) = (B2*c-A2*f)/deter;
    map3dTo2d->at(3) = (A1*d-B1*a)/deter;
    map3dTo2d->at(4) = (A1*e-B1*b)/deter;
    map3dTo2d->at(5) = (A1*f-B1*c)/deter;
    map3dTo2d->at(6) = ((B2*a-A2*d)/deter)*xA+((B2*b-A2*e)/deter)*yA+((B2*c-A2*f)/deter)*zA;
    map3dTo2d->at(7) = ((A1*d-B1*a)/deter)*xA+((A1*e-B1*b)/deter)*yA+((A1*f-B1*c)/deter)*zA;
}
    	
void triangleMapping::createBothMapIso(vector<point> * nodes, vector<Real> * map3dTo2d, vector<Real> * map2dTo3d)
{
    // variabili in uso 
    Real  a,b,c,d,e,f,deter,A1,A2,B1,B2,xA,yA,zA;
    
    // prendo l'altra mappa 
    create2dTo3dMapIso(nodes, map2dTo3d);
    
    // a partire da quella faccio l'inversa
    a  = map2dTo3d->at(0);
    b  = map2dTo3d->at(1);
    c  = map2dTo3d->at(2);
    d  = map2dTo3d->at(3);
    e  = map2dTo3d->at(4);
    f  = map2dTo3d->at(5);
    xA = map2dTo3d->at(6);
    yA = map2dTo3d->at(7);
    zA = map2dTo3d->at(8);
    
    // calcolo i coefficienti A1, A2, B1, B2 e il determinante 
    A1    = a*a+b*b+c*c;
    B2    = d*d+e*e+f*f;
    A2    = a*d+b*e+c*f;
    B1    = A2;
    deter = A1*B2-B1*A2;
    
    // faccio un resize 
    map3dTo2d->resize(8,0.0);
    map3dTo2d->at(0) = (B2*a-A2*d)/deter;
    map3dTo2d->at(1) = (B2*b-A2*e)/deter;
    map3dTo2d->at(2) = (B2*c-A2*f)/deter;
    map3dTo2d->at(3) = (A1*d-B1*a)/deter;
    map3dTo2d->at(4) = (A1*e-B1*b)/deter;
    map3dTo2d->at(5) = (A1*f-B1*c)/deter;
    map3dTo2d->at(6) = ((B2*a-A2*d)/deter)*xA+((B2*b-A2*e)/deter)*yA+((B2*c-A2*f)/deter)*zA;
    map3dTo2d->at(7) = ((A1*d-B1*a)/deter)*xA+((A1*e-B1*b)/deter)*yA+((A1*f-B1*c)/deter)*zA;
}

point triangleMapping::applyMap3dTo2d(point p, vector<Real> * map3dTo2d)
{
    // assert 
    assert(map3dTo2d->size()==8);
    
    // variabili in uso 
    Real   x,y,z;
    point result;
    
    // prendo le coordinate
    x = p.getX();
    y = p.getY();
    z = p.getZ();
    
    // faccio il punto 
    result.setX(map3dTo2d->at(0)*x+map3dTo2d->at(1)*y+map3dTo2d->at(2)*z-map3dTo2d->at(6));
    result.setY(map3dTo2d->at(3)*x+map3dTo2d->at(4)*y+map3dTo2d->at(5)*z-map3dTo2d->at(7));
    result.setZ(0.0);
    
    // ritorno il risultato 
    return(result);
}
	      
point triangleMapping::applyMap2dTo3d(point p, vector<Real> * map2dTo3d)
{
    // assert 
    assert(map2dTo3d->size()==9);
    
    // variabili in uso 
    Real     x,y;
    point result;
    
    // prendo le coordinate
    x = p.getX();
    y = p.getY();
    
    // faccio il punto 
    result.setX(map2dTo3d->at(0)*x+map2dTo3d->at(3)*y+map2dTo3d->at(6));
    result.setY(map2dTo3d->at(1)*x+map2dTo3d->at(4)*y+map2dTo3d->at(7));
    result.setZ(map2dTo3d->at(2)*x+map2dTo3d->at(5)*y+map2dTo3d->at(8));
    
    // ritorno il risultato 
    return(result);
}

//
// Metodi per fare le mappe fra due triangoli in 2d
//

void triangleMapping::createTria1ToTria2Map2d(vector<point> * nodes1, vector<point> * nodes2, vector<Real> * tria1ToTria2)
{
    // variabili in uso 
    point          abe,cdf;
    point     	       tmp;
    point      noto1,noto2;
    tensor		 M;
    
    // prendo le coordinate e le metto nella colonna 
    tmp.setX(nodes1->at(0).getX());    tmp.setY(nodes1->at(1).getX());    tmp.setZ(nodes1->at(2).getX());	M.setCol(0,tmp);
    tmp.setX(nodes1->at(0).getY());    tmp.setY(nodes1->at(1).getY());    tmp.setZ(nodes1->at(2).getY());	M.setCol(1,tmp);
    tmp.setX(1.); 		       tmp.setY(1.);  	 		  tmp.setZ(1.);		  	        M.setCol(2,tmp);
    
    // inverto la matrice 
    M.computeInverse();
    
    // setto i noti 
    noto1.setX(nodes2->at(0).getX());  noto1.setY(nodes2->at(1).getX());  noto1.setZ(nodes2->at(2).getX());
    noto2.setX(nodes2->at(0).getY());  noto2.setY(nodes2->at(1).getY());  noto2.setZ(nodes2->at(2).getY());
        
    // risolvo i due sitemi e prendo i valori 
    abe = M.rightVectorProduct(noto1);
    cdf = M.rightVectorProduct(noto2);
    
    // faccio un resize 
    tria1ToTria2->resize(6);
    tria1ToTria2->at(0) = abe.getX();
    tria1ToTria2->at(1) = abe.getY();
    tria1ToTria2->at(2) = cdf.getX();
    tria1ToTria2->at(3) = cdf.getY();
    tria1ToTria2->at(4) = abe.getZ();
    tria1ToTria2->at(5) = cdf.getZ();
}
	      
point triangleMapping::applyMap2dTo2d(point pt, vector<Real> * tria1ToTria2)
{
    // assert 
    assert(tria1ToTria2->size()==6);
    
    // variabili in uso 
    Real     x,y;
    point result;
    
    // prendo le coordinate
    x = pt.getX();
    y = pt.getY();
    
    // faccio il punto 
    result.setX(tria1ToTria2->at(0)*x+tria1ToTria2->at(1)*y+tria1ToTria2->at(4));
    result.setY(tria1ToTria2->at(2)*x+tria1ToTria2->at(3)*y+tria1ToTria2->at(5));
    result.setZ(0.0);
    
    // ritorno il risultato 
    return(result);      
  
}

//
// Metodo per calcolare quantità utili per triangoli in 2d
//
point triangleMapping::getCircumcenter(vector<point> * nodes)
{
    // variabili in uso 
    Real xA,xB,xC,yA,yB,yC,deter,AA,BB;
    point 		        result;
    
    // prendo le coordinate 
    xA = nodes->at(0).getX();    xB = nodes->at(1).getX();    xC = nodes->at(2).getX();
    yA = nodes->at(0).getY();    yB = nodes->at(1).getY();    yC = nodes->at(2).getY();
    
    // calcolo il determinante 
    deter = (xB-xA)*(yC-yA)-(yB-yA)*(xC-xA);
    AA    = 0.5*((xB*xB-xA*xA)+(yB*yB-yA*yA));
    BB    = 0.5*((xC*xC-xA*xA)+(yC*yC-yA*yA));
    
    // setto result 
    result.setX((AA*(yC-yA)+BB*(yA-yB))/deter);
    result.setY((AA*(xA-xC)+BB*(xB-xA))/deter);
    result.setZ(0.0);
    
    return(result);
}

point triangleMapping::getCircumcenterOnTriaPlane(vector<point> * nodes)
{
    // variabili in uso 
    point 			     circ2d;
    vector<Real>	map3dTo2d,map2dTo3d;
    vector<point>	          tria2d(3);
    
    // creo le mappe
    createBothMapIso(nodes, &map3dTo2d, &map2dTo3d);
    
    // creo il triangolo 
    tria2d[0] = applyMap3dTo2d(nodes->at(0), &map3dTo2d);
    tria2d[1] = applyMap3dTo2d(nodes->at(1), &map3dTo2d);
    tria2d[2] = applyMap3dTo2d(nodes->at(2), &map3dTo2d);
    
    // creo il circocentro 
    circ2d = getCircumcenter(&tria2d);
    
    // lo mappo indietro
    return(applyMap2dTo3d(circ2d, &map2dTo3d));
}


