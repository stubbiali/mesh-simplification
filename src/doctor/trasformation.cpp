#include "trasformation.h"

using namespace geometry;

//
// Costruttore 
//
trasformation::trasformation()
{
}

//
// Genereica affinità 
//
void trasformation::aff(vector<point> * nodi, tensor T, point v)
{
      // Variabili temporanee
      Real X,Y,Z; 

      // ciclo sui nodi della mesh e calcolo delle nuove coordinate
      for(UInt i=0; i<nodi->size(); ++i)
      {
	    // Prendo le coordinate del nodo i-esimo
	    X = nodi->at(i).getX();
	    Y = nodi->at(i).getY();
	    Z = nodi->at(i).getZ();
	    
	    // calcolo le nuove
	    nodi->at(i).setX((X*T.getIJ(0,0))+(Y*T.getIJ(0,1))+(Z*T.getIJ(0,2))+v.getX());
	    nodi->at(i).setY((X*T.getIJ(1,0))+(Y*T.getIJ(1,1))+(Z*T.getIJ(1,2))+v.getY());
	    nodi->at(i).setZ((X*T.getIJ(2,0))+(Y*T.getIJ(2,1))+(Z*T.getIJ(2,2))+v.getZ());
      }
}

//
// Trasformazioni rigide
//
void trasformation::trasla(vector<point> * nodi, point v)
{
	// ciclo sui nodi della mesh e calcolo delle nuove coordinate
	for(UInt i=0; i<nodi->size(); ++i)     nodi->at(i).add(v);
}

void trasformation::rotAroundAxes(vector<point> * nodi, point v, Real theta)
{
      // conversione dell'angolo in radianti
      point                              tmp;
      Real rad  = (theta / 360.0)*2.0*PGRECO;
      tensor                               T;
      
      // normalizzo il vettore
      point p   = v / v.norm2();
      
      // metto a posto la matrice
      T.setIJ(0,0,(p.getX()*p.getX()+(1.0-p.getX()*p.getX())*cos(rad)));
      T.setIJ(0,1,((1.0-cos(rad))*p.getX()*p.getY()-sin(rad)*p.getZ()));
      T.setIJ(0,2,((1.0-cos(rad))*p.getX()*p.getZ()+sin(rad)*p.getY()));
      
      T.setIJ(1,0,((1.0-cos(rad))*p.getY()*p.getX()+sin(rad)*p.getZ()));
      T.setIJ(1,1,(p.getY()*p.getY()+(1.0-p.getY()*p.getY())*cos(rad)));
      T.setIJ(1,2,((1.0-cos(rad))*p.getY()*p.getZ()-sin(rad)*p.getX()));
      
      T.setIJ(2,0,((1.0-cos(rad))*p.getZ()*p.getX()-sin(rad)*p.getY()));
      T.setIJ(2,1,((1.0-cos(rad))*p.getZ()*p.getY()+sin(rad)*p.getX()));
      T.setIJ(2,2,(p.getZ()*p.getZ()+(1.0-p.getZ()*p.getZ())*cos(rad)));
      
      // setto v a 0 perché non voglio una traslazione
      tmp.setX(0.0);	tmp.setY(0.0);	tmp.setZ(0.0);
      
      // faccio l'affinità
      aff(nodi,T,tmp);
}

//
// Dilatazione
//
void trasformation::dilata(vector<point> * nodi, point v)
{
	// creo le variabli che mi serviranno per chiamare il metodo aff
	tensor               T;
	point  vt(0.0,0.0,0.0);
	
	// trovo il centro di massa della superficie e effettuo due traslazioni prima e dopo la dilatazione così da 
	// evitare che la superficie "scappi"
	for(UInt i=0; i<nodi->size(); ++i)	vt.add(nodi->at(i));
	vt = (vt / static_cast<Real>(nodi->size()))*(-1.0);
	
	// la traslo
	trasla(nodi,vt);
	
	// Variabili temporanee
	Real X,Y,Z; 
      
	// ciclo sui nodi della mesh e calcolo delle nuove coordinate
	for(UInt i=0; i<nodi->size(); ++i)
	{
	      // Prendo le coordinate del nodo i-esimo
	      X = nodi->at(i).getX();
	      Y = nodi->at(i).getY();
	      Z = nodi->at(i).getZ();
	    
	      // calcolo le nuove
	      nodi->at(i).setX(X*v.getX());
	      nodi->at(i).setY(Y*v.getY());
	      nodi->at(i).setZ(Z*v.getZ());
	}
	
	// la riporto indietro
	vt = vt * (-1.0);
	trasla(nodi,vt);
}
