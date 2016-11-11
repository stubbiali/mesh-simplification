/*! \file	hash.hpp
	\brief	Specialize hash functor for user-defined classes. */
	
#ifndef HH_HASH_HH
#define HH_HASH_HH
		
#include "shapes.hpp"
#include "geoElement.hpp"

namespace std
{
	using namespace geometry; 
	
	/*! Specialization for geoElement<Line>. */
	template<>
	struct hash<geoElement<Line>>
	{
		/*! Call operator.
			\param g	a geoElement<Line>
			\return		the associated hash value */
		size_t operator()(const geoElement<Line> & g) const;
	};
}

#endif
