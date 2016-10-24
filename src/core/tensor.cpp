#include "tensor.h"
 
using namespace geometry;

//-----------------------------------------------------------
//Constructors and destructors
//-----------------------------------------------------------

tensor::tensor(Real Toll)
{
	id = 1;
	
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
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

tensor::tensor(const tensor &v)
{
	id = v.id;
	
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
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

tensor & tensor::operator=(const tensor &v)
{
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
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

Real tensor::operator*(const tensor &A)
{
	Real out = 0.0;
	
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			out += T[i][j] * A.T[i][j];
		}
	}
	
	return(out);
}

tensor tensor::operator+(const tensor &A)
{
	tensor out;
	
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			
			out.sumIJ(i, j, T[i][j] + A.T[i][j] );
			
		}
	}
	
	out.setToll(toll);
	return out;
}

tensor tensor::operator-(const tensor &A)
{
	tensor out;
	
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			
			out.sumIJ(i, j, T[i][j] - A.T[i][j] );
			
		}
	}
	
	out.setToll(toll);
	return out;
}
			
tensor tensor::operator*(const double &a)
{
	tensor out;
	
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			out.setIJ(i, j, T[i][j] * a);
		}
	}
	
	out.setToll(toll);
	return out;
}			
			
tensor tensor::operator/(const double &a)
{
	tensor out;
	
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			out.setIJ(i, j, T[i][j] / a);
		}
	}
	
	out.setToll(toll);
	return out;
}
			
bool tensor::operator<(const tensor &V)
{
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
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

bool tensor::operator!=(const tensor &V)
{
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			if( ( T[i][j] > (V.T[i][j] + toll) ) || ( T[i][j] < (V.T[i][j] - toll) ) )
			{
				return(true);
			}
		}
	}
	
	return(false);
}

Real & tensor::operator()(const UInt & i, const UInt & j)
{
	assert(i < 3);
	assert(j < 3);
	
	return(T[i][j]);
}

//-----------------------------------------------------------
// Operators
//-----------------------------------------------------------

point tensor::leftVectorProduct(point v)
{
	vector<Real> vett(3);
	
	for(UInt j=0; j<3; ++j)
	{
		vett[j] = T[0][j] * v.getX() + T[1][j] * v.getY() + T[2][j] * v.getZ();
	}
	
	return(point(vett));
}

point tensor::rightVectorProduct(point v)
{
	vector<Real> vett(3);
	
	for(UInt i=0; i<3; ++i)
	{
		vett[i] = T[i][0] * v.getX() + T[i][1] * v.getY() + T[i][2] * v.getZ();
	}
	
	return(point(vett));
}

Real tensor::getFrobenius()
{
	Real tot = 0.0;
	
	for(UInt i=0;i<3;++i)
	{
		for(UInt j=0;j<3;++j)
		{
			tot += T[i][j]*T[i][j];
		}
	}

	return(sqrt(tot));
}

void tensor::findAutoval()
{
    // prendo i vettori
    int                            N=3;
    int            LDA=3,LDVL=3,LDVR=3;
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
    AA[0] = static_cast<double>(T[0][0]);	AA[3] = static_cast<double>(T[0][1]);	AA[6] = static_cast<double>(T[0][2]);
    AA[1] = static_cast<double>(T[1][0]);	AA[4] = static_cast<double>(T[1][1]);	AA[7] = static_cast<double>(T[1][2]);
    AA[2] = static_cast<double>(T[2][0]);	AA[5] = static_cast<double>(T[2][1]);	AA[8] = static_cast<double>(T[2][2]);
    
    // calcolo gli autovalori 
    dgeev_("N", "V", &N, AA, &LDA, WR, WI, VL, &LDVL, VR, &LDVR, WORK, &LWORK, &INFO);
    
    // setto gli autovalori 
    A[0] = WR[0];
    A[1] = WR[1];
    A[2] = WR[2];
    
    // setto gli autovettori 
    V1[0]=VR[0];		V1[1]=VR[1];		V1[2]=VR[2];
    V2[0]=VR[3];		V2[1]=VR[4];		V2[2]=VR[5];
    V3[0]=VR[6];		V3[1]=VR[7];		V3[2]=VR[8];  
    
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

void tensor::computeInverse()
{
	Real determinante;
	Real inv[3][3];
			
	determinante =  T[0][0] * T[1][1] * T[2][2] - T[0][0] * T[1][2] * T[2][1] - T[0][1] * T[1][0] * T[2][2] + 
			T[0][1] * T[1][2] * T[2][0] + T[0][2] * T[1][0] * T[2][1] - T[0][2] * T[1][1] * T[2][0];
			
	
	// controllo che il determinante non sia 0
	if(fabs(determinante)<toll)
	{
	    print();
	    assert(fabs(determinante)>toll);
	}
	
	inv[0][0] =  (T[1][1] * T[2][2] - T[1][2] * T[2][1]) / determinante;
	inv[0][1] = -(T[0][1] * T[2][2] - T[0][2] * T[2][1]) / determinante;
	inv[0][2] =  (T[0][1] * T[1][2] - T[0][2] * T[1][1]) / determinante;
	inv[1][0] = -(T[1][0] * T[2][2] - T[1][2] * T[2][0]) / determinante;
	inv[1][1] =  (T[0][0] * T[2][2] - T[0][2] * T[2][0]) / determinante;
	inv[1][2] = -(T[0][0] * T[1][2] - T[0][2] * T[1][0]) / determinante;
	inv[2][0] =  (T[1][0] * T[2][1] - T[1][1] * T[2][0]) / determinante;
	inv[2][1] = -(T[0][0] * T[2][1] - T[0][1] * T[2][0]) / determinante;
	inv[2][2] =  (T[0][0] * T[1][1] - T[0][1] * T[1][0]) / determinante;
	
	
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			T[i][j] = inv[i][j];
		}
	}
}

void tensor::transpose()
{
	Real transp[3][3];
	
	//Copia
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			transp[i][j] = T[i][j];
		}
	}
	
	//Scrittura trasposta
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			T[i][j] = transp[j][i];
		}
	}
}

void tensor::clear()
{
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
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

void tensor::unity()
{
	// pulisco 
	clear();
	
	// setto gli autovalori 
	A[0]=1.0;
	A[1]=1.0;
	A[2]=1.0;
	
	// setto gli autovettori 
	V1[0] = 1.0;
	V2[1] = 1.0;
	V3[2] = 1.0;
	
	// setto la diagonale 
	T[0][0] = 1.0;
	T[1][1] = 1.0;
	T[2][2] = 1.0;
}

Real tensor::norm2()
{
    // creo gli autovalori 
    findAutoval();
    
    // ritorno il raggio spettrale 
    return(max(max(fabs(A[0]),fabs(A[1])),fabs(A[2])));
}

void tensor::directProduct(point a, point  b)
{
	T[0][0] = a.getX() * b.getX();
	T[0][1] = a.getX() * b.getY();
	T[0][2] = a.getX() * b.getZ();
	
	T[1][0] = a.getY() * b.getX();
	T[1][1] = a.getY() * b.getY();
	T[1][2] = a.getY() * b.getZ();
	
	T[2][0] = a.getZ() * b.getX();
	T[2][1] = a.getZ() * b.getY();
	T[2][2] = a.getZ() * b.getZ();
}

tensor tensor::matrixProduct(const tensor &A)
{
	tensor out;
	
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			for(UInt k=0; k<3; ++k)
			{
				out.sumIJ(i, j, T[i][k] * A.T[k][j] );
			}
		}
	}
	
	out.setToll(toll);
	return out;
}

void tensor::subtractComponetAlong(point n)
{
    // faccio il prodotto 
    tensor            Tmp,N,ris,identity,P;   
    
    // prendo N
    N.directProduct(n,n);
    
    // setto l'identità
    identity.unity();
    
    // salvo la vecchia matrice 
    for(UInt i=0; i<3; ++i)
      for(UInt j=0; j<3; ++j)
	Tmp.setIJ(i,j,T[i][j]);
    
    // operatore che proietta
    P = (identity-N);
    
    // stampo 
    ris = P.matrixProduct(Tmp);
    
    // salvo nella nuova matrice 
    for(UInt i=0; i<3; ++i)
      for(UInt j=0; j<3; ++j)
	T[i][j] = ris.getIJ(i,j);
          
}

//-----------------------------------------------------------
// GET-SET functions
//-----------------------------------------------------------

void tensor::sumIJ(UInt i,UInt j, Real value)
{
	assert((i<3) && (j<3));
	
	T[i][j] += value;
}

void tensor::setRow(const UInt & i, const point & value)
{
	assert(i < 3);
	
	for(UInt j=0; j<3; ++j)
	{
		T[i][j] = value.getI(j);
	}
}

void tensor::setCol(const UInt & j, const point & value)
{
	assert(j < 3);
	
	for(UInt i=0; i<3; ++i)
	{
		T[i][j] = value.getI(i);
	}
}

point tensor::getRow(UInt i)
{
	assert(i < 3);
	
	point V;
	
	for(UInt j=0; j<3; ++j)
	{
		V.setI(j, T[i][j]);
	}
	
	return(V);
}

point tensor::getCol(UInt j)
{
	assert(j < 3);
	
	point V;
	
	for(UInt i=0; i<3; ++i)
	{
		V.setI(i, T[i][j]);
	}
	
	return(V);
}

Real tensor::getAutoVal(UInt i)
{
    assert(i<3);
    return(A[i]);
}

point tensor::getAutoVect(UInt i)
{
    assert(i<3);
    point p;
    switch(i)
    {
      case(0):
	      p.setX(V1[0]);
	      p.setY(V1[1]);
	      p.setZ(V1[2]);
	      break;
      case(1):
	      p.setX(V2[0]);
	      p.setY(V2[1]);
	      p.setZ(V2[2]);
	      break;
      case(2):
	      p.setX(V3[0]);
	      p.setY(V3[1]);
	      p.setZ(V3[2]);
	      break;
    }
    
    return(p);
}


//-----------------------------------------------------------
// print
//-----------------------------------------------------------

void tensor::print()
{
	std::cout << "Tensor components" << std::endl;
	
	for(UInt i=0; i<3; ++i)
	{
		for(UInt j=0; j<3; ++j)
		{
			std::cout << T[i][j] << " ";
		}
		
		std::cout << std::endl;
	}
	
	std::cout << "toll " << toll << endl << endl;
}
