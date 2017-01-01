#ifndef MINIMATRIX_H_
#define MINIMATRIX_H_

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

/*! Classe che implementa una matrice 2x2 il primo indice è la riga e il secondo la colonna e entrambi gli indici 
    partono da 0.*/
class miniMatrix
{
		private:
			/// Components
			Real T[2][2];
			
			/// autovettori
			Real V1[2];
			Real V2[2];
			Real V3[2];
			
			/// autovalori
			Real A[2];
			
			/// The geometric tolerance
			Real toll;
			
			/// The id
			UInt id;
		
		//Constructors______________________________________________________	
		public:
			/*! Constructor */
/*ok*/			miniMatrix(Real Toll = 1e-10);
		
			/*! Copy constructor */
/*ok*/			miniMatrix(const miniMatrix &v);
		
		//Standard Operators____________________________________________________
		public:
			/*! Equality */
/*ok*/			miniMatrix & operator=(const miniMatrix &v);
			
			/*! Scalar product */
/*ok*/			Real operator*(const miniMatrix &A);

			/*! Plus */
/*ok*/			miniMatrix operator+(const miniMatrix &A);
			
			/*! Minus */
/*ok*/			miniMatrix operator-(const miniMatrix &A);
			
			/*! Product for a scalar */
/*ok*/			miniMatrix operator*(const double &a);
			
			/*! Divide for a scalar */
/*ok*/			miniMatrix operator/(const double &a);
			
			/*! Is minor */
/*ok*/			bool   operator<(const miniMatrix &V);

			/*! Is not equal */
/*ok*/			bool   operator!=(const miniMatrix &V);
			
			/*! Operatore accesso diretto alle componenti */
			Real & operator()(const UInt & i, const UInt & j);

		//Operators_____________________________________________________________
		public:
			/*! Determinante */
/*ok*/			inline Real getDet();
			
			/*! Frobenius norm*/
/*ok*/			Real getFrobenius();
			
			/*! Prodotto con il vettore a SINISTRA della matrice */
/*ok*/			point leftVectorProduct(point v);
			
			/*! Prodotto con il vettore a DESTRA della matrice */
/*ok*/			point rightVectorProduct(point v);
			
			/*! Direct product */
/*ok*/			void directProduct(point a, point b);
			
			/*! Prodotto fra la matrice di classe e quella passata alla funzine (i.e. T*A) */
/*ok*/			miniMatrix matrixProduct(const miniMatrix &A);

			/*! Metodo che crea la lista di autovalori e autovettori*/
			void findAutoval();
			
			/*! Metodo per avere la norma L2 del miniMatrixe */
			Real norm2();
			
			/*! Set the miniMatrix to identity */
/*ok*/			void unity();

			/*! Set the miniMatrix to null */
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

inline Real miniMatrix::getIJ(const UInt & i, const UInt & j)
{
	assert((i<2) && (j<2));
	return(T[i][j]);
}

inline void miniMatrix::setIJ(UInt i, UInt j, Real value)
{
	assert((i<2) && (j<2));
	T[i][j] = value;
}

inline void miniMatrix::setToll(Real Toll)
{
	toll = Toll;
}

inline Real miniMatrix::getToll()
{
	return(toll);
}

inline void miniMatrix::setId(UInt Id)
{
	id = Id;
}

inline UInt miniMatrix::getId()
{
	return(id);
}

//-----------------------------------------------------------
// operazioni
//-----------------------------------------------------------
inline Real miniMatrix::getDet()
{
	return(T[0][0]*T[1][1]-T[0][1]*T[1][0]);
}




}
#endif
