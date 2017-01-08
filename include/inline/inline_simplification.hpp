/*!	\file	inline_simplification.hpp
	\brief	Specialization of some members of class simplification. */
			
#ifndef HH_INLINESIMPLIFICATION_HH
#define HH_INLINESIMPLIFICATION_HH

namespace geometry
{
	template<>
	INLINE void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		getCost(const UInt & id1, const UInt & id2)
	{
		getCost_f(id1, id2);
	}
	
	
	template<>
	INLINE void simplification<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>>::
		getCost(const UInt & id1, const UInt & id2)
	{
		getCost_f(id1, id2);
	}
}

#endif