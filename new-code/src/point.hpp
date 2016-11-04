/*! \file point.hpp
	\brief Class storing a point. */

#ifndef HH_POINT_HH
#define HH_POINT_HH

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>

#include "shapes.hpp"

namespace geometry {

	using namespace std;

	/*! Tolerance. */
	#define TOLL 1e-10
    
	/*! Class storing a node of a mesh and performing different operations on it. */
	class point : public simplePoint {

		private:
		
			/*! The node id. */
			UInt id;
			
			/*! The coordinates. */
			array<Real,3> coor;
				
			/*! Flag for boundary type:
			 	<ol>
			 	<li>  0 internal node
			 	<li>  1 boundary node
			 	<li>  2 triple (i.e. fixed) node
			 	</ol> */
			UInt boundary;
	                
		public:
		
			//
			// Constructors and destructor
			//
		
			/*! Constructor.
				\param x	first coordinate
				\param y	second coordinate
				\param z	third coordinate
				\param ID	point id
				\param bond	boundary flag */
	    	point(const Real & x = 0.0, const Real & y = 0.0, const Real & z = 0.0, const UInt & ID = 0, const UInt & bond = 0);

			/*! Constructor. 
				\param c	array with coordinates
				\param ID	point id
				\param bond	boundary flag */
		    point(const array<Real,3> & c, const UInt & ID = 0, const UInt & bond = 0);

			/*! Synthetic copy constructor.
				\param V	point */
		    point(const point & V) = default;

			/*! Destructor */
		    virtual ~point() = default;
			
			//
			// Operators
			//
			
			/*! The equality operator. 
				\param V	point */
			point & operator=(const point & V);

			/*! Sum operator. 
				\param pA	first point
				\param pB	second point 
				\return 	result */
			friend point operator+(const point & pA, const point & pB);

			/*! Difference operator. 
				\param pA	first point
				\param pB	second point 
				\return 	result */
			friend point operator-(const point & pA, const point & pB);

			/*! Cross product. 
				\param pA	first point
				\param pB	second point 
				\return 	result */
			friend point operator^(const point & pA, const point & pB);

			/*! Division by a scalar. 
				\param p	point
				\param a	scalar 
				\return 	result */
			friend point operator/(const point & p, const Real & a);

			/*! Product by a scalar. 
				\param p	point
				\param a	scalar 
				\return 	result */
			friend point operator*(const point & p, const Real & a);

			/*! Scalar product. 
				\param pA	first point
				\param pB	second point 
				\return 	result */
			friend Real operator*(const point & pA, const point & pB);

			/*! Less than operator: a point is "less" than another if its first coordinate is less than the other point one. If the first component value is equal the second one is considered and so on. 
				\param pA	LHS
				\param pB	RHS 
				\return 	bool reporting the result */
			friend bool operator<(const point & pA, const point & pB);

			/*! Inequality operator: two points are equal if their components are equal up to the geometric tolerance. 
				\param pA	LHS
				\param pB	RHS 
				\return 	bool reporting the result */ 
			friend bool operator!=(const point & pA, const point & pB);

			/*! Inequality operator: two points are equal if their components are equal up to the geometric tolerance. 
				\param pA	LHS
				\param pB	RHS 
				\return 	bool reporting the result */
			friend bool operator==(const point & pA, const point & pB);
			
			/*! Access operator (non const version).
				\param i	index of the coordinate
				\return		coordinate */
			inline Real & operator[](const UInt & i) {return coor[i];};
			
			/*! Access operator (const version).
				\param i	index of the coordinate
				\return		coordinate */
			inline Real operator[](const UInt & i) const {return coor[i];};
			
			/*! Output stream operator.
				\param out	output stream
				\param p	point to print
				\return update output stream */
			friend ostream & operator<<(ostream & out, const point & p);

			//
			// Replace methods
			//
			
			/*! Combination function: \$ d * (V1 + V2) \$.
				\param V1	first point
				\param V2	second point 
				\param d	scalar */
			void replace(const point & V1, const point & V2, const Real & d);

			/*! Combination function: \$ d * (V1 + V2 + V3) \$. 
				\param V1	first point
				\param V2	second point 
				\param V3	third point
				\param d	scalar */
			void replace(const point & V1, const point & V2, const point & V3, const Real & d);

			/*! Combination function: \$ d * (V1 + V2 + V3 + V4) \$.
				\param V1	first point
				\param V2	second point 
				\param V3	third point
				\param V4	fourth point
				\param d	scalar */
			void replace(const point & V1, const point & V2, const point & V3, const point & V4, const Real & d);
			
			//
			// Norm methods
			//
				
			/*! The Euclidean norm of the vector.
				\return	the norm */
			inline Real norm2() const {return std::sqrt(coor[0]*coor[0] + coor[1]*coor[1] + coor[2]*coor[2]);};

			/*! Normalize the coordinates vector. */
			void normalize();

			//
			// Get methods
			//
		
			/*! Get the id.
				\return the id */
			inline UInt getId() const {return id;};

			/*! Get boundary flag.
				\return the boundary flag */
			inline UInt getBoundary() const {return boundary;};
						                
			//
			// Set methods
			//

			/*! Set the array of coordinates.
				\param newCoor	the new coordinates */
			void setCoor(const array<Real,3> & newCoor);

			/*! Set the id.
				\param newId	the new id */
			inline void setId(const UInt & newId) {id = newId;};

			/*! Set boundary information. 
				\param newBond	the new boundary flag */
			inline void setBoundary(const UInt & newBond) {boundary = newBond;};
		     
		private:                     
			/*! Print to output the point data.
				\param out	the output string */
			virtual void print(ostream & out) const;

	};

}

#endif


