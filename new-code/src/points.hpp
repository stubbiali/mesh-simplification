/*! \file point.hpp
	\brief Class storing a point. */

#ifndef HH_POINTS_HH
#define HH_POINTS_HH

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>

#include "shapes.hpp"

namespace geometry {

	using namespace std;

	// Forward declaration of searchPoint.
	class searchPoint;

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
			
			//
			// Conversion methods
			//
			
			/*! Convert to a searchPoint object.
				\return		the related searchPoint object */
			operator searchPoint() const;
		     
		private:                     
			/*! Print to output the point data.
				\param out	the output string */
			virtual void print(ostream & out) const;

	};
	
	
	/*! Class inheriting simplePoint and storing the indices for structured data search. 
		All methods are modified so to keep the indices updated after any operation. */
	class searchPoint final : public simplePoint
	{
		private:
			/*! ID. */
			UInt					id;
			
			/*! Indices. */
			std::array<UInt,3> 		idx;
			
			/*! Static variables storing
				<ol>
				<li> the North-East point of the grid
				<li> the South-West point of the grid 
				<li> the cells sizes
				<li> the number of cells along each direction
				</ol>
				Note: this is done because the code is supposed to deal with one mesh at time. */
			static point			pNE;
			static point 			pSW;
			static array<Real,3> 	dl;
			static array<UInt,3> 	numCells;
			
		public:
			//
			// Constructors
			//
			
			/*! Constructor.
				\param idx	array with indices
				\param ID	point id */
			searchPoint(const array<UInt,3> & idx, const UInt & ID = 0);
			
			/*! Constructor.
				\param x	first coordinate
				\param y	second coordinate
				\param z	third coordinate
				\param ID	point id
				\param bond	boundary flag */
			searchPoint(const Real & x = 0.0, const Real & y = 0.0, const Real & z = 0.0, const UInt & ID = 0);
			
			/*! Constructor.
				\param c	array with coordinates
				\param ID	point id
				\param bond	boundary flag */		
			searchPoint(const array<Real,3> & c, const UInt & ID = 0);
			
			/*! Constructor.
				\param p	a point */
			searchPoint(const point & p);
						
			/*! Copy constructor.
				\param p	a point */
			searchPoint(const searchPoint & p) = default;
					
			//
			// Operators
			//
			
			/*! The equality operator. 
				\param V	point */
			searchPoint & operator=(const searchPoint & V);

			/*! Less than operator: a searchPoint is "less" than another if its index is smaller than the other one. 
				\param pA	LHS
				\param pB	RHS 
				\return 	bool reporting the result */
			friend bool operator<(const searchPoint & pA, const searchPoint & pB);
			
			/*! Inequality operator: two points are equal if they fall within the same cell. 
				\param pA	LHS
				\param pB	RHS 
				\return 	bool reporting the result */ 
			friend inline bool operator!=(const searchPoint & pA, const searchPoint & pB) {return (pA.idx != pB.idx);};

			/*! Inequality operator: two points are equal if they fall within the same cell. 
				\param pA	LHS
				\param pB	RHS 
				\return 	bool reporting the result */
			friend inline bool operator==(const searchPoint & pA, const searchPoint & pB) {return (pA.idx == pB.idx);};
			
			/*! Access operator (non const version).
				\param i	component
				\return		index */
			inline UInt & operator[](const UInt & i) {return idx[i];};
			
			/*! Access operator (const version).
				\param i	component
				\return		index */
			inline UInt operator[](const UInt & i) const {return idx[i];};
			
			/*! Output stream operator.
				\param out	output stream
				\param p	point to print
				\return update output stream */
			friend ostream & operator<<(ostream & out, const searchPoint & p);
					
			//
			// Get methods 
			//
			
			/*! Get the id.
				\return		the point id */
			inline UInt getId() const {return id;};
			
			/*! Get North-East point.
				\return 	the North-East point of the grid. */
			inline static point getPNE() {return searchPoint::pNE;};
						
			/*! Get South-West point.
				\return 	the South-West point of the grid. */
			inline static point getPSW() {return searchPoint::pSW;};
			
			/*! Get one size of the cells.
				\param i	component
				\return 	the size */
			inline static Real getdl(const UInt & i) {return searchPoint::dl[i];};
			
			/*! Get cells sizes.
				\return		array with cells sizes */
			inline static array<Real,3> getdl() {return searchPoint::dl;};
			
			/*! Get number of cells along one direction.
				\param i	direction
				\return		number of cells */
			inline static UInt getNumCells(const UInt & i) {return searchPoint::numCells[i];};
			
			/*! Get number of cells along each direction.
				\return		array with number of cells along each direction */
			inline static array<UInt,3> getNumCells() {return searchPoint::numCells;};
			
			//
			// Set methods 
			//
			
			/*! Set the id.
				\param idNew	the new id */
			inline void setId(const UInt & idNew) {id = idNew;};
			
			/*! Set the North-East point.
				\param p	the new North-East point */
			static void setPNE(const point & p);
			
			/*! Set the South-West point.
				\param p	the new South-West point */
			static void setPSW(const point & p);
			
			/*! Set one cells size.
				\param i	component
				\param val	value */
			static void setdl(const UInt & i, const Real & val);
			
			/*! Set all cells sizes.
				\param val	array with new cells sizes */
			static void setdl(const array<Real,3> & val);
			
			/*! Set number of cells along one direction.
				\param i	component
				\param val	value */
			static void setNumCells(const UInt & i, const UInt & val);
			
			/*! Set number of cells along each direction.
				\param val	array with new number of cells */
			static void setNumCells(const array<UInt,3> & val);
			
			/*! Initialize extrema of the grid and cells sizes.
				This method is supposed to be called ONLY ONCE and BEFORE any use of the class.
				\param pne	North-East point
				\param psw	South-West point
				\param dx	cells size along x
				\param dy	cells size along y
				\param dz	cells size along z */
			static void initialize(const point & pne = point(1.,1.,1.), const point & psw = point(0.,0.,0.), const Real & dx = 1., const Real & dy = 1., const Real & dz = 1.);
			
			//
			// Methods to keep static variables coherent
			//     
			
			/*! Update number of cells along one direction. 
				\param i	direction */
			static void updateNumCells(const UInt & i);            
			
			/*! Update number of cells along each direction. */
			static void updateNumCells();
			
			/*! Update cells size along one direction.
				\param i	direction */
			static void updateCellSize(const UInt & i);
			
			/*! Update cells sizes along each direction. */
			static void updateCellSizes();
			
						
		private:      
			//
			// Print method
			//
			
			/*! Print to output the point data.
				\param out	the output string */
			void print(ostream & out) const;
	};

}

#endif


