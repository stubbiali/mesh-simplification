/*!	\file 	structuredData.hpp
	\brief	Class building and handling the hexaedral Cartesian grid
			surrounding a three-dimensional mesh and employed in the
			structured data search. */
			
#ifndef HH_STRUCTUREDDATA_HH
#define HH_STRUCTUREDDATA_HH

#include <memory>
#include <unordered_set>

#include "boundingBox.hpp"
#include "bmesh.hpp"

namespace geometry
{
	/*!	The bounding boxes for each element of a three-dimensional
		mesh are stored in an unordered multi set. This should enable 
		a quick access to the boxes based on their indices. */
	template<typename SHAPE>
	class structuredData
	{
		private:
			shared_ptr<bmesh<SHAPE>>	grid;
			
			unordered_multiset<bbox3d> 	boxes;
			
		public:
			//
			// Constructors
			//
			
			structuredData(const shared_ptr<bmesh<SHAPE>> & g = nullptr);
			
			template<typename... Args>
			structuredData(Args... args);
	};
}

#endif
