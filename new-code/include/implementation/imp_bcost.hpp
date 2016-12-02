/*!	\file	imp_bcost.hpp
	\brief	Implementations of members of class bcost. */
	
#ifndef HH_IMPBCOST_HH
#define HH_IMPBCOST_HH

#include <string>
#include <stdexcept>

namespace geometry
{
	//
	// Constructor
	//
	
	template<typename SHAPE, MeshType MT, typename D>
	bcost<SHAPE,MT,D>::bcost(bmeshOperation<SHAPE,MT> * bmo) :
		worker(bmo)
	{
	}
	
	
	// 
	// Set methods
	//
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE void bcost<SHAPE,MT,D>::setMeshOperation(bmeshOperation<SHAPE,MT> * bmo)
	{
		static_cast<D *>(this)->imp_setMeshOperation(bmo);
	}
	
	
	//
	// Get methods
	//
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE vector<point> bcost<SHAPE,MT,D>::getPointList(const UInt & id1, 
		const UInt & id2) const
	{
		return static_cast<D *>(this)->imp_getPointList(id1, id2);
	}
	
	
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE Real bcost<SHAPE,MT,D>::getCost(const UInt & id1, const UInt & id2, 
		const point3d & p) const
	{
		return static_cast<D *>(this)->imp_getCost(id1, id2, p);
	}
	
	
	//
	// Update list
	//
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE void bcost<SHAPE,MT,D>::addCollapseInfo(const UInt & id1, const UInt & id2, 
		const point3d & p, const Real & val)
	{
		cInfoList.emplace(id1, id2, p, val);
	}
	
	
	template<typename SHAPE, MeshType MT, typename D>
	Real bcost<SHAPE,MT,D>::eraseCollapseInfo(const UInt & id1, const UInt & id2)
	{
		// Find the edge and before erasing it extract the related cost
		// If the edge cannot be found, throw an exception
		auto it = cInfoList.find({id1, id2});
		if (it != cInfoList.end())
		{
			auto ans = it->getCost();
			cInfoList.erase(it);
			return ans;
		}
		else
		{
			string msg("Edge (" + to_string(id1) + ", " + to_string(id2) + ") not found.");
			throw runtime_error(msg);
		}
	}
	
	
	template<typename SHAPE, MeshType MT, typename D>
	void bcost<SHAPE,MT,D>::update(const UInt & id)
	{
		static_cast<D *>(this)->imp_update(id);
	}
}

#endif
