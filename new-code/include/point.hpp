/*! \file 	point.hpp
	\brief 	Class storing a point. */

#ifndef HH_POINT_HH
#define HH_POINT_HH

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>

#include "shapes.hpp"

namespace geometry 
{		    
	/*! Class storing a node of a mesh and performing different operations on it. */
	class point : public simplePoint 
	{
		protected:
			/*! The node Id. */
			UInt Id;
			
			/*! The coordinates. */
			array<Real,3> coor;
				
			/*! Flag for boundary type:
			 	<ol>
			 	<li>  0 internal node
			 	<li>  1 boundary node
			 	<li>  2 triple (i.e. fixed) node
			 	</ol> */
			UInt boundary;
			
			/*! Active flag. */
			bool active;
	                
		public:
			//
			// Constructors and destructor
			//
		
			/*! Constructor.
				\param x	first coordinate
				\param y	second coordinate
				\param z	third coordinate
				\param ID	point Id
				\param bond	boundary flag */
	    	point(const Real & x = 0.0, const Real & y = 0.0, const Real & z = 0.0, const UInt & ID = 0, const UInt & bond = 0);

			/*! Constructor. 
				\param c	array with coordinates
				\param ID	point Id
				\param bond	boundary flag */
		    point(const array<Real,3> & c, const UInt & ID = 0, const UInt & bond = 0);

			/*! Synthetic copy constructor.
				\param p	point */
		    point(const point & p) = default;

			/*! Destructor */
		    virtual ~point() = default;
			
			//
			// Operators
			//
			
			/*! The equality operator. 
				\param p	point */
			virtual point & operator=(const point & p);

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
			
			/*! Product by a scalar. 
				\param a	scalar 
				\param p	point
				\return 	result */
			friend point operator*(const Real & a, const point & p);

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
			Real & operator[](const UInt & i);
			
			/*! Access operator (const version).
				\param i	index of the coordinate
				\return		coordinate */
			Real operator[](const UInt & i) const;
			
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
			Real norm2() const;

			/*! Normalize the coordinates vector. */
			void normalize();

			//
			// Get methods
			//
		
			/*! Get the Id.
				\return the Id */
			UInt getId() const;

			/*! Get boundary flag.
				\return the boundary flag */
			UInt getBoundary() const;
			
			/*! Check if the point is active.
				\return active flag */
			bool isActive() const;
						                
			//
			// Set methods
			//

			/*! Set the array of coordinates.
				\param newCoor	the new coordinates */
			void setCoor(const array<Real,3> & newCoor);

			/*! Set the Id.
				\param newId	the new Id */
			void setId(const UInt & newId);

			/*! Set boundary information. 
				\param newBond	the new boundary flag */
			void setBoundary(const UInt & newBond);
			
			/*! Set active flag. */
			void setActive(const bool & flag = true);
								     
		private:              
			/*! Print to output the point data.
				\param out	the output string */
			virtual void print(ostream & out) const;
	};
}

/*! Include definitions of inlined members and friend functions. */
#include "inline/inline_point.hpp"

#endif


