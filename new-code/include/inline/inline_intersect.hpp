/*!	\file	inline_intersect.hpp
	\brief	Definitions of inlined members of class intersect<Triangle>. */
	
#ifndef HH_INLINEINTERSECT_HH
#define HH_INLINEINTERSECT_HH

namespace geometry
{
	//
	// Access mesh
	//
	
	INLINE void intersect<Triangle>::setMesh(const bmesh<Triangle> * pg)
	{
		grid = pg;
	}
}

#endif
