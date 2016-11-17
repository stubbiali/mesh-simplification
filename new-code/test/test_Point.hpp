/*! \file 	test_Point.hpp
	\brief 	Class storing a N-dimensional point. */

#ifndef HH_TESTPOINT_HH
#define HH_TESTPOINT_HH

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <initializer_list>
#include <cassert>

#include "shapes.hpp"

namespace geometry 
{		    
	/*! Class storing a node of a mesh and performing different operations on it. */
	template<UInt N = 3>
	class Point : public simplePoint 
	{
		protected:
			/*! The node Id. */
			UInt Id;
			
			/*! The coordinates. */
			array<Real,N> coor;
				
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
				\param lst	initializer list
				\param ID	point Id
				\param bond	boundary flag */
	    	Point(initializer_list<Real> lst, const UInt & ID = 0, const UInt & bond = 0);
	    	
	    	/*! Constructor.
				\param lst	initializer list
				\param ID	point Id
				\param bond	boundary flag */
			template<typename... Args>
	    	Point(const UInt & ID, Args... args);

			/*! Constructor. 
				\param c	array with coordinates
				\param ID	point Id
				\param bond	boundary flag */
		    Point(const array<Real,N> & c, const UInt & ID = 0, const UInt & bond = 0);

			/*! Synthetic copy constructor.
				\param p	point */
		    Point(const Point & p) = default;

			/*! Destructor */
		    virtual ~Point() = default;
	};
	
	
	//
	// Implementations
	//
	
	template<UInt N>
	Point<N>::Point(initializer_list<Real> lst, const UInt & ID, const UInt & bond) :
		Id(ID), coor(lst), boundary(bond), active(true)
	{
		assert(coor.size() == N);
	}
	
	
	template<UInt N>
	template<typename... Args>
	Point<N>::Point(const UInt & ID, Args... args) :
		Id(ID), coor(forward<Real>(args...)), boundary(0), active(true)
	{
	}
	
	
	template<UInt N>
	Point<N>::Point(const array<Real,N> & c, const UInt & ID, const UInt & bond) :
		Id(ID), coor(c), boundary(bond), active(true)
	{
	}
}

#endif
