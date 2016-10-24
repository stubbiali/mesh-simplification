#include "point.h"

namespace geometry
{

//-----------------------------------------------------------
//Constructors and destructors
//-----------------------------------------------------------

point::point(Real xx, Real yy, Real zz, Real Toll, UInt bound)
{
	id   = 0;
	X[0] = xx;
	X[1] = yy;
	X[2] = zz;
	toll = Toll;
	boundary = bound;
    Idx_i = 0;
    Idx_j = 0;
    Idx_k = 0;
        
}

point::point(vector<Real> Y, Real Toll, UInt bound)
{
  
	id   = 0;
	for(UInt i=0; i < 3; ++i)
	{
		X[i] = Y[i];
	}
	
	toll = Toll;
	boundary = bound;
}

point::point(const point &V)
{
	X[0] = V.X[0];
	X[1] = V.X[1];
	X[2] = V.X[2];
	toll = V.toll;
	id   = V.id;
	boundary = V.boundary;
    Idx_i = V.Idx_i;
    Idx_j = V.Idx_j;
    Idx_k = V.Idx_k;
}

point::~point()
{
}


//-----------------------------------------------------------
// Standard operators
//-----------------------------------------------------------

point & point::operator=(const point &V)
{
	for(UInt i=0; i<3; ++i)
	{
		X[i] = V.X[i];
	}
	
	toll = V.toll;
	id   = V.id;
	boundary = V.boundary;
    Idx_i = V.Idx_i;
    Idx_j = V.Idx_j;
    Idx_k = V.Idx_k;  
	
	return *this;
}

point point::operator+(const point &V) const
{
	return point(X[0]+V.X[0], X[1]+V.X[1], X[2]+V.X[2], toll);
}

point point::operator-(const point &V) const
{
	return point(X[0]-V.X[0], X[1]-V.X[1], X[2]-V.X[2], toll);
}

point point::operator^(const point &V) const
{
	return  point(  X[1]*V.X[2] - V.X[1]*X[2],
			X[2]*V.X[0] - V.X[2]*X[0],
			X[0]*V.X[1] - V.X[0]*X[1] , toll);
}

point point::operator/(const Real &a) const
{
	return point(X[0]/a, X[1]/a, X[2]/a, toll);
}

point point::operator*(const Real &a) const
{
	return point(X[0]*a, X[1]*a, X[2]*a, toll);
}

Real point::operator*(const point &V) const
{
	return(X[0]*V.X[0] + X[1]*V.X[1] + X[2]*V.X[2]);
}

bool point::operator<(const point &V) const
{
	for(UInt i=0; i<3; ++i)
	{
		if(X[i] < (V.X[i] - toll))
		{
			return(true);
		}

		if(X[i] > (V.X[i] + toll))
		{
			return(false);
		}
	}

	return(false);
}

bool point::operator!=(const point &V) const
{
	bool equal[3];

	equal[0] = (X[0] >= (V.X[0] - toll)) && (X[0] <= (V.X[0] + toll));
	equal[1] = (X[1] >= (V.X[1] - toll)) && (X[1] <= (V.X[1] + toll));
	equal[2] = (X[2] >= (V.X[2] - toll)) && (X[2] <= (V.X[2] + toll));

	return(!(equal[0] && equal[1] && equal[2] ) );
}

bool point::operator==(const point &V) const
{
	bool equal[3];

	equal[0] = (X[0] >= (V.X[0] - toll)) && (X[0] <= (V.X[0] + toll));
	equal[1] = (X[1] >= (V.X[1] - toll)) && (X[1] <= (V.X[1] + toll));
	equal[2] = (X[2] >= (V.X[2] - toll)) && (X[2] <= (V.X[2] + toll));

	return((equal[0] && equal[1] && equal[2]));
}

// operatore binario che permette di fare il confronto fra due punti 
bool samePoint(const point & p1, const point & p2)
{
    if((p1-p2).norm2()<p1.norm2()) return(true);
    return(false);
}

//-----------------------------------------------------------
// Other functions
//-----------------------------------------------------------

void point::directProduct(point P, const Real & a)
{
	X[0] = P.X[0] * a;
	X[1] = P.X[1] * a;
	X[2] = P.X[2] * a;
}

Real point::norm2() const
{
	return(sqrt(X[0]*X[0] + X[1]*X[1] + X[2]*X[2]) );
}

void point::normalize()
{
    // prendo la lunghezza 
    Real lung = norm2();
    
    // faccio un controllo sulle lunghezze
    if(lung<toll)	cout << "ATTENZIONE il nodo ha lunghezza nulla!!" << endl;
    
    // cambio le coordinate 
    X[0] = X[0]/lung;
    X[1] = X[1]/lung;
    X[2] = X[2]/lung;
}

void point::print()
{
	std::cout << id << ": " << X[0] << " " << X[1] << " " << X[2] << " " << boundary << "  " << toll << '\n';
}

void point::setCoor(vector<Real> * val)
{
    assert(val->size()==3);
    X[0]=val->at(0);
    X[1]=val->at(1);
    X[2]=val->at(2);
}

//-----------------------------------------------------------
// Rapid mesh1dar combination funcions
//-----------------------------------------------------------

void point::replace(const point &V1, const point &V2, const Real &d)
{
	X[0] = (V1.X[0] + V2.X[0])*d;
	X[1] = (V1.X[1] + V2.X[1])*d;
	X[2] = (V1.X[2] + V2.X[2])*d;
}

void point::replace(const point &V1, const point &V2, const point &V3, const Real &d)
{
	X[0] = (V1.X[0] + V2.X[0] + V3.X[0])*d;
	X[1] = (V1.X[1] + V2.X[1] + V3.X[1])*d;
	X[2] = (V1.X[2] + V2.X[2] + V3.X[2])*d;
}

void point::replace(const point &V1, const point &V2, const point &V3, const point &V4, const Real &d)
{
	X[0] = (V1.X[0] + V2.X[0] + V3.X[0] + V4.X[0])*d;
	X[1] = (V1.X[1] + V2.X[1] + V3.X[1] + V4.X[1])*d;
	X[2] = (V1.X[2] + V2.X[2] + V3.X[2] + V4.X[2])*d;
}

void point::add(const point &V1)
{
	X[0] += V1.X[0];
	X[1] += V1.X[1];
	X[2] += V1.X[2];	
}

void point::add(const point &V1, const Real &d)
{
	X[0] += V1.X[0] * d;
	X[1] += V1.X[1] * d;
	X[2] += V1.X[2] * d;
}

//
// ATTENZIONE: nella conversione alle coordinate sferiche c'è una tollleranza di 10-07 per capire se un punto è nullo!!
//
point point::sfericalCoor() const
{
	// creo il punto che verrà restituito 
	point 		    sferical;
	
	// prendo le variabili raggio 
	Real rho    = this->norm2();
	
	
	// se sto valutando le coordinate barcientriche dell'origine ritorno 
	if(rho<1e-07)		       return(sferical);
	
	// angolo phi 
	Real phi    = 0.0;
	
	// sistema le divisioni per 
	if(fabs(X[0])<1e-07)
	{	
		if(X[1]<0.0)		phi = PGRECO*1.5;
		else			phi = PGRECO*0.5;
	}
	else if(fabs(X[1])<1e-07)
	{	
		if(X[0]<0.0)		phi = PGRECO;
		else			phi = 0.0;
	}
	else if((X[0]>0.0) && (X[1]>0.0)) 
	{
		phi = atan(X[1]/X[0]);
	}
	else if((X[0]>0.0) && (X[1]<0.0))
	{
		phi = 2*PGRECO+atan(X[1]/X[0]);
	}
	else if((X[0]<0.0) && (X[1]>0.0))
	{
		phi = PGRECO+atan(X[1]/X[0]);
	}
	else //if((p.getX()<0.0) && (p.getY()<0.0))
	{
		phi = PGRECO+atan(X[1]/X[0]);
	}
	

	// angolo theta
	Real theta  = acos(X[2]/rho);
	
	// creo il punto 
	sferical.setX(rho);
	sferical.setY(theta);
	sferical.setZ(phi);
	
	return(sferical);
}

vector<Real> point::torusCoor(Real R, Real r)
{
	// variabili in uso 
	vector<Real>	coor(2,0.0);
	
	// controllo il raggio 
	if(R<(r-1e-07))
	{
	    cout << "Hai sbagliato a dare i raggi" << endl;
	    return(coor);
	}
	
	// -----------------
	// calcolo di theta
	// -----------------
	// controllo il caso in cui è sulla cresta in alto o in basso del toro 
	if(abs(abs(X[2]/r)-1.)<1e-07)	
	{
	    if(X[2]>0.0)	coor[0] = PGRECO/2.;
	    else		coor[0] = -PGRECO/2.;
	}
	else
	{
	    // calcolo l'arco seno 
	    coor[0] = asin(X[2]/r);

	    // controllo per mettere l'angolo fra -PGRECO e + PGRECO
	    if(sqrt(X[0]*X[0]+X[1]*X[1])<R)
	    {
		if(X[2]>0.0) coor[0] = PGRECO-coor[0];
		else	     coor[0] = -PGRECO-coor[0];
	    }
	}
	
	// --------------
	// calcolo di phi
	// --------------
	// controllo il caso in cui è sulla cresta interna ed esterna
	if(abs(abs(X[1]/(R+r*cos(coor[0])))-1.)<1e-07)
	{
	    if(X[1]>0.0)	coor[1] = PGRECO/2.;
	    else       		coor[1] = -PGRECO/2.;
	}
	else
	{
	    // calcolo phi
	    coor[1] = asin(X[1]/(R+r*cos(coor[0])));
	
	    // controllo per mettere l'angolo fra -PGRECO e + PGRECO
	    if(X[0]<0.0)
	    {
		if(X[1]>=0.0) 	coor[1] =  PGRECO - coor[1];
		else		coor[1] =  -PGRECO - coor[1];
	    }
	}
	
	// ritorno le coordinate 
	return(coor);
}


}

