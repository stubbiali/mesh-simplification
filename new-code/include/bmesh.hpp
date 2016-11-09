/*! \file	bmesh.hpp
	\brief	A base class for storing a mesh. */
	
#ifndef HH_BMESH_HH
#define HH_BMESH_HH

#include <vector>

#include "point.hpp"
#include "geoElement.hpp"

namespace geometry
{
	/*! This is a base class for storing a mesh. A mesh is stored as a vector of nodes and
		a vector of elements.
		Although this class can be instantiated, the user should preferer the class mesh, 
		which inherits from bmesh and also allows for data distributed over the mesh. */
		
	template<typename SHAPE>
	class bmesh
	{
		protected:
			/*! Vector of nodes. */
			vector<point> 				nodes;
			
			/*! Vector of elements. */
			vector<geoElement<SHAPE>> 	elems;
	}
}

#endif
