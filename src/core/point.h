#ifndef POINT_H_
#define POINT_H_

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>

#include "shapes.hpp"

namespace geometry
{
  
// TEST la conversione alle coordinate sferiche è fatta con una tolleranza fissata a 10-07!!
  
using namespace std;

/*! Classe che implementa le principali operazioni che si possono fare con un punto geometrico. */

class point : public simplePoint 
{

	public:
		    /// The coordinates
		    Real X[3];

		    /// The node id
		    UInt id;
	
		    /// The geometric tolerance
		    Real toll;
		    
		    /// Intero che indica
		    /// <ol>
		    /// <li>  0 nodo interno
		    /// <li>  1 nodo di bordo
		    /// <li>  2 nodo triplo
		    /// </ol>
		    UInt boundary;
                    
            //Indici i,j,k del punto
            UInt Idx_i;
            UInt Idx_j;
            UInt Idx_k;


	/// Constructors
	public:
		    /*! Constructor */
/*ok*/		    point(Real xx=0.0, Real yy=0.0, Real zz=0.0, Real Toll=1e-15, UInt bond=0);

		    /*! Constructor */
/*ok*/		    point(vector<Real> Y, Real Toll=1e-15, UInt bond=0);

		    /*! Copy constructor */
/*ok*/		    point(const point &V);

		    /*! Destructor */
/*ok*/		    virtual ~point();


	/// Operators
	public:
		    /*! The equality operator */
/*ok*/		    point & operator=(const point &V);

		    /*! Sum operator */
/*ok*/		    point operator+(const point &V) const;

		    /*! Difference operator */
/*ok*/		    point operator-(const point &V) const;

		    /*! Cross product */
/*ok*/		    point operator^(const point &V) const;

		    /*! Division by a scalar */
/*ok*/		    point operator/(const Real &a) const;

		    /*! Product by a scalar */
/*ok*/		    point operator*(const Real &a) const;

		    /*! Operatore prodotto scalare */
/*ok*/		    Real operator*(const point &V) const;

		    /*! Less operator: a vector is "less" than another if its first coordinate is less than the other vctor one. If the first component value is equal the second one is considered and so on */
/*ok*/	    	    bool operator<(const point &V) const;

		    /*! Inequality operator: two vectors are equal if their components are equal to the geometric tolerance */
/*ok*/		    bool operator!=(const point &V) const;

		    /*! Inequality operator: two vectors are equal if their components are equal to the geometric tolerance */
/*ok*/		    bool operator==(const point &V) const;


	///Rapid mesh1dar combination functions
	public:
		    /*! Fast mesh1dar combination function this = (V1 + V2) *d */
/*ok*/		    void replace(const point &V1, const point &V2, const Real &d);

		    /*! Fast mesh1dar combination function this = (V1 + V2 + V3) *d */
/*ok*/		    void replace(const point &V1, const point &V2, const point &V3, const Real &d);

		    /*! Fast mesh1dar combination function this = (V1 + V2 + V3 + V4) *d */
/*ok*/		    void replace(const point &V1, const point &V2, const point &V3, const point &V4, const Real &d);
		    
		    /*! Add V1 to this* point*/
/*ok*/		    void add(const point &V1);
		    
		    /*! Add V1*d to this* point*/
/*ok*/	            void add(const point &V1, const Real &d);


	/// Other functions
	public:
		    void directProduct(point P, const Real & a);
	
		     /*! The norm of the R3 vector */
/*ok*/		     Real norm2() const;

		     /*! Metodo che normalizza il vettore */
		     void normalize();

		    /*! Print to screen of the point data */
/*ok*/		    void print(); 

	///Get Functions
	public:
		    /*! Get the X component */
/*ok*/		    inline Real getX() const;

		    /*! Get the Y component */
/*ok*/		    inline Real getY() const;

		    /*! Get the Z component */
/*ok*/	  	    inline Real getZ() const;

		    /*! Get the i-th component */
/*ok*/		    inline Real getI(const UInt & i) const;
	
		    /*! Get the geometric tolerance */
/*ok*/		    inline Real getToll() const;
	
		    /*! Get the Id */
/*ok*/		    inline UInt getId() const;

		    /*! Get boundary information */
/*ok*/		    inline UInt getBoundary() const;
                          
            /*! Get I index */
            inline UInt getI() const {return Idx_i;};
            
            /*! Get J index */
            inline UInt getJ() const {return Idx_j;};
            
            /*! Get K index */
            inline UInt getK() const {return Idx_k;};

                    
                    

	///Set Functions
	public:
		    /*! Set the X component */
/*ok*/	            inline void setX(const Real & xx);

		    /*! Set the Y component */
/*ok*/		    inline void setY(const Real & yy);

		    /*! Set the Z component */
/*ok*/		    inline void setZ(const Real & zz);

		    /*! Metodo che setta il punto in base a un vettore */
		    void setCoor(vector<Real> * val);

		    /*! Set the i-th component */
/*ok*/		    inline void setI(const UInt & i, const Real & val);
	
		    /*! Set the geometric tolerance */
/*ok*/	            inline void setToll(const Real & Toll);
	
		    /*! Set the Id*/
/*ok*/	            inline void setId(const UInt & Id);

		    /*! Set boundary information*/
/*ok*/	            inline void setBoundary(UInt bond);
                          
                    /*! Set I index */
                    inline void setI(UInt idx_i) { Idx_i=idx_i;};
                    
                    /*! Set J index */
                    inline void setJ(UInt idx_j) { Idx_j=idx_j;};
                    
                    /*! Set K index */
                    inline void setK(UInt idx_k) { Idx_k = idx_k;};

	//
	// Metodo che permette di convertire le coordinate 
	//
	public:
		    /*! Metodo che ritorna un punto con scritto in coordinate sferiche */
		    point sfericalCoor() const;
		    
		    /*! Metodo che ritorna le coordinate toroidali solamente gli angoli tetha, cerchio che ruota [0], 
		        e phi, "diametro" del toro [1] 
		        \param R raggio del cerchio più grande
		        \param r raggio del cerchio più piccolo */
		    vector<Real> torusCoor(Real R, Real r);

};

// operatore binario che permette di fare il confronto fra due punti 
bool samePoint(const point & p1, const point & p2);
      
//-----------------------------------------------------------------------------
// INLINE FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------
// Get Functions
//-----------------------------------------------------------

inline Real point::getI(const UInt & i) const
{
	assert(i<3);
	return(X[i]);
}

inline Real point::getToll() const
{
	return(toll);
}

inline UInt point::getId() const
{
	return(id);
}

inline UInt point::getBoundary() const
{
	return(boundary);
}

inline Real point::getX() const
{
	return(X[0]);
}

inline Real point::getY() const
{
	return(X[1]);
}

inline Real point::getZ() const
{
	return(X[2]);
}

//-----------------------------------------------------------
// Set Functions
//-----------------------------------------------------------

inline void point::setX(const Real  & xx)
{
	X[0] = xx;
}

inline void point::setY(const Real & yy)
{
	X[1] = yy;
}

inline void point::setZ(const Real & zz)
{
	X[2] = zz;
}

inline void point::setI(const UInt & i, const Real & val)
{
	assert(i<3);
	X[i] = val;
}

inline void point::setToll(const Real & Toll)
{
	toll = Toll;
}

inline void point::setId(const UInt & Id)
{
	id = Id;
}

inline void point::setBoundary(UInt bound)
{
	boundary = bound;
}


}

#endif /* POINT_H_ */
