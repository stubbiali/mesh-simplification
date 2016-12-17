/*!	\file	structuredData.cpp
	\brief	Specialization of some members of class structuredData. */
	
#include "structuredData.hpp"

namespace geometry
{
	//
	// Get methods
	//
	
	// Specialization for triangular grids
	template<>
	bbox3d structuredData<Triangle>::getBoundingBox(const UInt & Id) const
	{
		auto elem = grid->getElem(Id);
		return {grid->getNode(elem[0]), grid->getNode(elem[1]),
			grid->getNode(elem[2])};
	}
	
	
	// Specialization for quadrilateral grids
	template<>
	bbox3d structuredData<Quad>::getBoundingBox(const UInt & Id) const
	{
		auto elem = grid->getElem(Id);
		return {grid->getNode(elem[0]), grid->getNode(elem[1]),
			grid->getNode(elem[2]), grid->getNode(elem[3])};
	}
	
	
	//
	// Modify set of bounding boxes
	//
	
	// Specialization for triangular grids
	template<>
	void structuredData<Triangle>::update(const vector<UInt> & ids)
	{
		for (auto id : ids)
		{
			//
			// Remove old bounding box
			//
			
			// Extract element index
			auto idx = grid->getElem(id).getIdx();
			
			// Extract all elements having that index...
			auto range = boxes.equal_range(idx);
			
			// ... Then find and remove the desired one
			auto it_old = find_if(range.first, range.second,
				[id](const bbox3d & bb){ return bb.getId() == id; });
			if (it_old != range.second)
				boxes.erase(it_old);
			
			//
			// Insert new bounding box
			//
			
			auto elem = grid->getElem(id);
			auto it_new = boxes.emplace(id, grid->getNode(elem[0]), 
				grid->getNode(elem[1]), grid->getNode(elem[2]));
				
			// Update grid
			grid->setIdx(id, it_new->getIdx());
		}
	}
	
	
	// Specialization for quadrilateral grids
	template<>
	void structuredData<Quad>::update(const vector<UInt> & ids)
	{
		for (auto id : ids)
		{
			//
			// Remove old bounding box
			//
			
			// Extract element index
			auto idx = grid->getElem(id).getIdx();
			
			// Extract all elements having that index...
			auto range = boxes.equal_range(idx);
			
			// ... Then find and remove the desired one
			auto it_old = find_if(range.first, range.second,
				[id](const bbox3d & bb){ return bb.getId() == id; });
			if (it_old != range.second)
				boxes.erase(it_old);
			
			//
			// Insert new bounding box
			//
			
			auto elem = grid->getElem(id);
			auto it_new = boxes.emplace(id, grid->getNode(elem[0]), 
				grid->getNode(elem[1]), grid->getNode(elem[2]), grid->getNode(elem[3]));
				
			// Update grid
			grid->setIdx(id, it_new->getIdx());
		}
	}
}
