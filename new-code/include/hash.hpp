/*! \file	hash.hpp
	\brief	Specialize hash functor for user-defined classes. */
	
#ifndef HH_HASH_HH
#define HH_HASH_HH
		
#include "shapes.hpp"
#include "geoElement.hpp"

namespace geometry
{
	/*!	Forward declaration of boundingBox. */
	template<UInt N>
	class boundingBox;
}

namespace std
{
	using namespace geometry; 
	
	/*! Specialization for geoElement<Line>. */
	template<>
	struct hash<geoElement<Line>>
	{
		/*! Call operator.
			\param g	a geoElement<Line> object
			\return		the associated hash value */
		size_t operator()(const geoElement<Line> & g) const;
	};
			
	/*!	Specialization for boundingBox. */
	template<>
	template<UInt N>
	struct hash<boundingBox<N>>
	{
		/*! Call operator.
			\param bb	a N-dimensional bounding box object
			\return		the associated hash value */
		size_t operator()(const boundingBox<N> & bb) const;
	};
}

/*!	Include implementations of template specializations. */
#include "implementation/imp_hash.hpp"

#endif
