/*! \file	imp_bconnect.hpp
	\brief	Definitions of members of class bconnect. */
	
#ifndef HH_IMPBCONNECT_HH
#define HH_IMPBCONNECT_HH

namespace geometry
{
	// 
	// Constructor
	//
	
	template<typename SHAPE, MeshType MT>
	bconnect<SHAPE,MT>::bconnect(smart_ptr<mesh<SHAPE,MT>> g) : grid(g)
	{
		if (g != nullptr)
		{
			// Build all connections and fill set of edges
			buildNode2Node();
			buildNode2Elem();
			buildElem2Elem();
		}
	}
	
	
	//
	// Connections initializers
	//
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::buildNode2Node()
	{
		// TODO
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::buildNode2Elem()
	{
		// TODO
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::buildElem2Elem(const UInt & Id)
	{
		// TODO
	}
}

#endif
