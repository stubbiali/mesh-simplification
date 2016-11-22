/*!	\file	imp_intersect.hpp
	\brief	Implementations of template members of class intersect. */

#ifndef HH_IMPINTERSECT_HH
#define HH_IMPINTERSECT_HH

namespace geometry
{
	//
	// Constructor
	//
	
	template<typename... Args>
	intersect<Triangle>::intersect(Args... args) :
		grid(make_shared<bmesh<Triangle>>(args...))
	{
	}
}

#endif
