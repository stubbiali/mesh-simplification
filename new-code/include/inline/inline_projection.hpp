/*!	\file	inline_projection.hpp
	\brief	Implementations of inlined members of class projection<Triangle>. */
	
#ifndef HH_INLINEPROJECTION_HH
#define HH_INLINEPROJECTION_HH

namespace geometry
{
	//
	// Get methods
	//
	
	INLINE projection<Triangle> * projection<Triangle>::getPointerToProjection()
	{
		return this;
	}
}

#endif
