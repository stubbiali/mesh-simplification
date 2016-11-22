/*!	\file	inline_intersect.hpp
	\brief	Definitions of inlined members of class intersect<Triangle>. */
	
#ifndef HH_INLINEINTERSECT_HH
#define HH_INLINEINTERSECT_HH

namespace geometry
{
	//
	// Get methods
	//
	
	INLINE shared_ptr<bmesh<Triangle>> intersect<Triangle>::getMesh()
	{
		return grid;
	}
	
	
	//
	// Auxiliary methods
	//
	
	INLINE Real intersect<Triangle>::getTriArea2d(const point2d & a, 
		const point2d & b, const point2d & c)
	{
		return 0.5 * ((b - a)^(c - b))[2];
	}
}

#endif
