#include "miniMatrix.h"
 
using namespace geometry;

//-----------------------------------------------------------
//Constructors and destructors
//-----------------------------------------------------------

miniMatrix::miniMatrix(Real Toll)
{
	id = 1;
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			T[i][j] = 0.0;
		}
		
		// setto gli autovettori
		V1[i]=0.0;
		V2[i]=0.0;
		V3[i]=0.0;
		A[i] =0.0;
	}
	
	//setto le variabili di classe 
	toll = Toll;
	
}

miniMatrix::miniMatrix(const miniMatrix &v)
{
	id = v.id;
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			T[i][j] = v.T[i][j];
		}
		
		// setto gli autovettori
		V1[i]=v.V1[i];
		V2[i]=v.V2[i];
		V3[i]=v.V3[i];
		A[i] =v.A[i];
	}
	
	toll = v.toll;
}


//-----------------------------------------------------------
// Standard operators
//-----------------------------------------------------------

miniMatrix & miniMatrix::operator=(const miniMatrix &v)
{
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			T[i][j] = v.T[i][j];
		}
		// setto gli autovettori
		V1[i]=v.V1[i];
		V2[i]=v.V2[i];
		V3[i]=v.V3[i];
		A[i] =v.A[i];
	}
	
	//setto le variabili di classe 
	toll = v.toll;
	
	return *this;
}

Real miniMatrix::operator*(const miniMatrix &A)
{
	Real out = 0.0;
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			out += T[i][j] * A.T[i][j];
		}
	}
	
	return(out);
}

miniMatrix miniMatrix::operator+(const miniMatrix &A)
{
	miniMatrix out;
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			
			out.sumIJ(i, j, T[i][j] + A.T[i][j] );
			
		}
	}
	
	out.setToll(toll);
	return out;
}

miniMatrix miniMatrix::operator-(const miniMatrix &A)
{
	miniMatrix out;
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			
			out.sumIJ(i, j, T[i][j] - A.T[i][j] );
			
		}
	}
	
	out.setToll(toll);
	return out;
}
			
miniMatrix miniMatrix::operator*(const double &a)
{
	miniMatrix out;
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			out.setIJ(i, j, T[i][j] * a);
		}
	}
	
	out.setToll(toll);
	return out;
}			
			
miniMatrix miniMatrix::operator/(const double &a)
{
	miniMatrix out;
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			out.setIJ(i, j, T[i][j] / a);
		}
	}
	
	out.setToll(toll);
	return out;
}
			
bool miniMatrix::operator<(const miniMatrix &V)
{
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			if( T[i][j] < (V.T[i][j] - toll) )
			{
				return(true);
			}
			
			if( T[i][j] > (V.T[i][j] + toll) )
			{
				return(false);
			}
		}
	}
	
	return(false);
}

bool miniMatrix::operator!=(const miniMatrix &V)
{
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			if( ( T[i][j] > (V.T[i][j] + toll) ) || ( T[i][j] < (V.T[i][j] - toll) ) )
			{
				return(true);
			}
		}
	}
	
	return(false);
}

Real & miniMatrix::operator()(const UInt & i, const UInt & j)
{
	assert(i < 2);
	assert(j < 2);
	
	return(T[i][j]);
}

//-----------------------------------------------------------
// Operators
//-----------------------------------------------------------

point miniMatrix::leftVectorProduct(point v)
{
	vector<Real> vett(3,0.0);
	
	for(UInt j=0; j<2; ++j)
	{
		vett[j] = T[0][j] * v.getX() + T[1][j] * v.getY();
	}
	
	return(point(vett));
}

point miniMatrix::rightVectorProduct(point v)
{
	vector<Real> vett(3,0.0);
	
	for(UInt i=0; i<2; ++i)
	{
		vett[i] = T[i][0] * v.getX() + T[i][1] * v.getY();
	}
	
	return(point(vett));
}

miniMatrix miniMatrix::matrixProduct(const miniMatrix &A)
{
	miniMatrix out;
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			for(UInt k=0; k<2; ++k)
			{
				out.sumIJ(i, j, T[i][k] * A.T[k][j] );
			}
		}
	}
	
	out.setToll(toll);
	return out;
}

Real miniMatrix::getFrobenius()
{
	Real tot = 0.0;
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			tot += T[i][j]*T[i][j];
		}
	}

	return(sqrt(tot));
}

void miniMatrix::findAutoval()
{
    // prendo i vettori
    int                            N=2;
    int            LDA=2,LDVL=2,LDVR=2;
    int                       LWORK=81;
    int                           INFO;
    double  * AA   = new double[N*LDA];
    double  * WR   = new double[N];
    double  * WI   = new double[N];
    double  * VL   = new double[LDVL*N];
    double  * VR   = new double[LDVR*N];
    double  * WORK = new double[LWORK];
    
    //
    // ATTENZIONE per trovare autovalori siano reali però si possono salvare anche quelli non reali  e autovettori supponiamo che
    // siano reali 
    //
   
    // setto la matrice 
    AA[0] = static_cast<double>(T[0][0]);	AA[2] = static_cast<double>(T[0][1]);	
    AA[1] = static_cast<double>(T[1][0]);	AA[3] = static_cast<double>(T[1][1]);	
    
    // calcolo gli autovalori 
    dgeev_("N", "V", &N, AA, &LDA, WR, WI, VL, &LDVL, VR, &LDVR, WORK, &LWORK, &INFO);
    
    // setto gli autovalori 
    A[0] = WR[0];
    A[1] = WR[1];
    
    // setto gli autovettori 
    V1[0]=VR[0];		V1[1]=VR[1];
    V2[0]=VR[2];		V2[1]=VR[3];
    
    // pulisco 
    delete [] AA;
    delete [] WR;
    delete [] WI;
    delete [] VL;
    delete [] VR;
    delete [] WORK;

}

//-----------------------------------------------------------
// Internal operators
//-----------------------------------------------------------

void miniMatrix::computeInverse()
{
	Real determinante;
	Real inv[2][2];
			
	determinante =  T[0][0]*T[1][1]-T[0][1]*T[1][0];
			
	// controllo che il determinante non sia 0
//  	assert(fabs(determinante)>toll);
	
	inv[0][0] =   T[1][1] / determinante;
	inv[0][1] = - T[0][1] / determinante; 
	inv[1][0] = - T[1][0] / determinante;
	inv[1][1] =   T[0][0] / determinante;
	
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			T[i][j] = inv[i][j];
		}
	}
}

void miniMatrix::transpose()
{
	Real transp[2][2];
	
	//Copia
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			transp[i][j] = T[i][j];
		}
	}
	
	//Scrittura trasposta
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			T[i][j] = transp[j][i];
		}
	}
}

void miniMatrix::clear()
{
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			
			T[i][j] = 0.0;
			
		}
		// setto gli autovettori
		V1[i]=0.0;
		V2[i]=0.0;
		V3[i]=0.0;
		A[i]=0.0;
	}
}

void miniMatrix::unity()
{
	// pulisco 
	clear();
	
	// setto gli autovalori 
	A[0]=1.0;
	A[1]=1.0;
	
	// setto gli autovettori 
	V1[0] = 1.0;
	V2[1] = 1.0;
	
	// setto la diagonale 
	T[0][0] = 1.0;
	T[1][1] = 1.0;
}

Real miniMatrix::norm2()
{
    // creo gli autovalori 
    findAutoval();
    
    // ritorno il raggio spettrale 
    return(max(fabs(A[0]),fabs(A[1])));
}

//-----------------------------------------------------------
// GET-SET functions
//-----------------------------------------------------------

void miniMatrix::sumIJ(UInt i,UInt j, Real value)
{
	assert((i<2) && (j<2));
	
	T[i][j] += value;
}

void miniMatrix::setRow(const UInt & i, const point & value)
{
	assert(i < 2);
	
	for(UInt j=0; j<2; ++j)
	{
		T[i][j] = value.getI(j);
	}
}

void miniMatrix::setCol(const UInt & j, const point & value)
{
	assert(j < 2);
	
	for(UInt i=0; i<2; ++i)
	{
		T[i][j] = value.getI(i);
	}
}

point miniMatrix::getRow(UInt i)
{
	assert(i < 2);
	
	point V;
	
	for(UInt j=0; j<2; ++j)
	{
		V.setI(j, T[i][j]);
	}
	V.setZ(0.0);
	
	return(V);
}

point miniMatrix::getCol(UInt j)
{
	assert(j < 2);
	
	point V;
	
	for(UInt i=0; i<2; ++i)
	{
		V.setI(i, T[i][j]);
	}
	V.setZ(0.0);
	
	return(V);
}

Real miniMatrix::getAutoVal(UInt i)
{
    assert(i<2);
    return(A[i]);
}

point miniMatrix::getAutoVect(UInt i)
{
    assert(i<2);
    point p;
    switch(i)
    {
      case(0):
	      p.setX(V1[0]);
	      p.setY(V1[1]);
	      p.setZ(0.0);
	      break;
      case(1):
	      p.setX(V2[0]);
	      p.setY(V2[1]);
	      p.setZ(0.0);
	      break;
    }
    
    return(p);
}


//-----------------------------------------------------------
// print
//-----------------------------------------------------------

void miniMatrix::print()
{
	std::cout << "MiniMatrix components" << std::endl;
	
	for(UInt i=0; i<2; ++i)
	{
		for(UInt j=0; j<2; ++j)
		{
			std::cout << T[i][j] << " ";
		}
		
		std::cout << std::endl;
	}
	
	std::cout << "toll " << toll << endl << endl;
}


