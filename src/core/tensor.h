#ifndef TENSOR_H_
#define TENSOR_H_

#include "shapes.hpp"
#include "point.h"

/*
extern "C"{

extern void dgeev_(const char *, const char *, int *, double *, int *, double *, double *, 
		   double *, int *, double *, int *, double *, int *, int *);

}
*/

namespace geometry
{
   
using namespace std;

/*! Classe che implementa il tensore 3x3 il primo indice è la riga e il secondo la colonna e entrambi gli indici 
    partono da 0.*/
class tensor
{
		private:
			/// Components
			Real T[3][3];
			
			/// autovettori
			Real V1[3];
			Real V2[3];
			Real V3[3];
			
			/// autovalori
			Real A[3];
			
			/// The geometric tolerance
			Real toll;
			
			/// The id
			UInt id;
		
		//Constructors______________________________________________________	
		public:
			/*! Constructor */
/*ok*/			tensor(Real Toll = 1e-10);
		
			/*! Copy constructor */
/*ok*/			tensor(const tensor &v);
		
		//Standard Operators____________________________________________________
		public:
			/*! Equality */
/*ok*/			tensor & operator=(const tensor &v);
			
			/*! Scalar product */
/*ok*/			Real operator*(const tensor &A);

			/*! Plus */
/*ok*/			tensor operator+(const tensor &A);
			
			/*! Minus */
/*ok*/			tensor operator-(const tensor &A);
			
			/*! Product for a scalar */
/*ok*/			tensor operator*(const double &a);
			
			/*! Divide for a scalar */
/*ok*/			tensor operator/(const double &a);
			
			/*! Is minor */
/*ok*/			bool   operator<(const tensor &V);

			/*! Is not equal */
/*ok*/			bool   operator!=(const tensor &V);
			
			/*! Operatore accesso diretto alle componenti */
			Real & operator()(const UInt & i, const UInt & j);

		//Operators_____________________________________________________________
		public:
			/*! First invariant*/
/*ok*/			inline Real getFirstInvariant();
			
			/*! Second invariant*/
/*ok*/			inline Real getSecondInvariant();
			
			/*! Third invariant*/
/*ok*/			inline Real getThirdInvariant();
			
			/*! Frobenius norm*/
/*ok*/			Real getFrobenius();
			
			/*! Prodotto con il vettore a SINISTRA della matrice */
/*ok*/			point leftVectorProduct(point v);
			
			/*! Prodotto con il vettore a DESTRA della matrice */
/*ok*/			point rightVectorProduct(point v);
			
			/*! Direct product */
/*ok*/			void directProduct(point a, point b);
			
			/*! Prodotto fra la matrice di classe e quella passata alla funzine (i.e. T*A) */
/*ok*/			tensor matrixProduct(const tensor &A);

			/*! Metodo che crea la lista di autovalori e autovettori*/
			void findAutoval();
			
			/*! Metodo che effettua l'operazione (I-n*n)*T 
			   \param n direzione lungo cui sottrarre */
			void subtractComponetAlong(point n);
			
			/*! Metodo per avere la norma L2 del tensore */
			Real norm2();
			
			/*! Set the tensor to identity */
/*ok*/			void unity();

			/*! Set the tensor to null */
/*ok*/			void clear();
			
		//Internal Operators______________________________________________________
		public:
			/*! Compute the matrix inverse */
/*ok*/			void computeInverse();

			/*! Compute the matrix traspose */
/*ok*/			void transpose();
			
		//GET-SET Functions_______________________________________________________
		public:
			/*! Get the i-j component*/
/*ok*/			inline Real getIJ(const UInt & i, const UInt & j);
			
			/*! Set the i-j component*/
/*ok*/			inline void setIJ(UInt i, UInt j, Real value);
			
			/*! Sum into the i-j component*/
/*ok*/			void sumIJ(UInt i, UInt j, Real value);
			
			/*! Set the geometric tolerance */
/*ok*/			inline void setToll(Real Toll);
			
			/*! Get the geometric tolerance */
/*ok*/			inline Real getToll();
			
			/*! Set dell'i-esima riga */
/*ok*/			void setRow(const UInt & i, const point & value);
			
			/*! Set dell'i-esima colonna */
/*ok*/			void setCol(const UInt & j , const point & value);
			
			/*! L'i-esima riga */
/*ok*/			point getRow(UInt i);
			
			/*! L'i-esima colonna */
/*ok*/			point getCol(UInt j);
			
			/*! Set dell'Id */
/*ok*/			inline void setId(UInt Id);
			
			/*! Restituzione dell'Id */
/*ok*/			inline UInt getId();
  
			/*! Restituisce l'autovalore i-esimo*/
			Real getAutoVal(UInt i);
			
			/*! Restituisce l'autovettore i-esimo*/
			point getAutoVect(UInt i);
				
		//Print to screen_______________________________________________________
		public:
			/*! Print to screen */
/*ok*/			void print();
			
};

//-----------------------------------------------------------------------------
// INLINE FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------
// Set/Get Functions
//-----------------------------------------------------------

inline Real tensor::getIJ(const UInt & i, const UInt & j)
{
	assert((i<3) && (j<3));
	return(T[i][j]);
}

inline void tensor::setIJ(UInt i, UInt j, Real value)
{
	assert((i<3) && (j<3));
	T[i][j] = value;
}

inline void tensor::setToll(Real Toll)
{
	toll = Toll;
}

inline Real tensor::getToll()
{
	return(toll);
}

inline void tensor::setId(UInt Id)
{
	id = Id;
}

inline UInt tensor::getId()
{
	return(id);
}

//-----------------------------------------------------------
// operazioni
//-----------------------------------------------------------
inline Real tensor::getFirstInvariant()
{
	return(T[0][0] + T[1][1] + T[2][2]);
}

inline Real tensor::getSecondInvariant()
{
	return( T[0][0]*(T[1][1] + T[2][2]) + T[1][1]*T[2][2] - 
		(T[0][1]*T[1][0] + T[0][2]*T[2][0] + T[1][2]*T[2][1]) );
}

inline Real tensor::getThirdInvariant()
{
	return(	  T[0][0] * T[1][1] * T[2][2] - T[0][0] * T[1][2] * T[2][1] 
		- T[1][0] * T[0][1] * T[2][2] + T[1][0] * T[0][2] * T[2][1] 
		+ T[2][0] * T[0][1] * T[1][2] - T[2][0] * T[0][2] * T[1][1] );
}

}





#endif
