/*!	\file	imp_bmeshInfo.hpp
	\brief	Implementations of members of class bmeshInfo. */
	
#ifndef HH_IMPBMESHINFO_HH
#define	HH_IMPBMESHINFO_HH

/*
	Note: some methods are provided only for some classes of grids,
	e.g. only for triangular grids. To accomplish this, we add some 
	static_assert's evaluating the number of vertices for each element. 
	This is possible since the number of vertices is stored as a static 
	const class attribute. However, in debugging phase we force the
	compiler to generate the code for all methods of template classes.
	Since for each shape there is at least one method not defined, not 
	to get errors at compile time, all static_assert's are enabled 
	only in release mode.
*/

namespace geometry
{
	//
	// Constructors
	//
	
	template<typename SHAPE, MeshType MT>
	bmeshInfo<SHAPE,MT>::bmeshInfo(const shared_ptr<mesh<SHAPE,MT>> & g) :
		connectivity(g)
	{
	}
	
	
	template<typename SHAPE, MeshType MT>
	template<typename... Args>
	bmeshInfo<SHAPE,MT>::bmeshInfo(Args... args) :
		connectivity(args...)
	{
	}
	
	
	//
	// Access attributes
	//
	
	template<typename SHAPE, MeshType MT>
	INLINE connect<SHAPE,MT> bmeshInfo<SHAPE,MT>::getConnectivity() const
	{
		return connectivity;
	}
	
	
	//
	// Get topological info
	//
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bmeshInfo<SHAPE,MT>::getNodesInvolvedInEdgeCollapsing
		(const UInt & id1, const UInt & id2) const
	{
		// Get nodes connected either to id1 or id2
		auto s = set_union(connectivity.node2node[id1], connectivity.node2node[id2]);
		
		// Remove id1 and id2
		s.erase(id1);
		s.erase(id2);
		
		return vector<UInt>(s.begin(), s.end());
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<UInt> bmeshInfo<SHAPE,MT>::getElemsOnEdge
		(const UInt & id1, const UInt & id2) const
	{
		// Get elements connected both to id1 and id2
		auto s = set_intersection(connectivity.node2elem[id1], 
			connectivity.node2elem[id2]);
		return vector<UInt>(s.begin(), s.end()); 
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<UInt> bmeshInfo<SHAPE,MT>::getElemsInvolvedInEdgeCollapsing
		(const UInt & id1, const UInt & id2) const
	{
		// Get elements connected either to id1 or id2
		auto s = set_union(connectivity.node2elem[id1], connectivity.node2elem[id2]);
		return vector<UInt>(s.begin(), s.end()); 
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bmeshInfo<SHAPE,MT>::getElemsModifiedInEdgeCollapsing
		(const UInt & id1, const UInt & id2) const
	{
		// Get elements connected either to id1 or id2, but not both
		auto s = set_symmetric_difference(connectivity.node2elem[id1], 
			connectivity.node2elem[id2]);
		return vector<UInt>(s.begin(), s.end());
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bmeshInfo<SHAPE,MT>::getTriaPatch(const UInt & Id) const
	{
		// This method is provided only for triangular grids
		#ifdef NDEBUG
		static_assert(NV <= 3, 
			"getTriaPatch() is provided only for triangular grids.");
		#endif
			
		// Get element 
		auto elem = connectivity.grid->getElem(Id);
		
		// Get elements connected to at least one of the vertices of the element
		auto s = set_union(connectivity.node2elem[elem[0]],
			connectivity.node2elem[elem[1]], connectivity.node2elem[elem[2]]);
			
		// Remove Id
		s.erase(Id);
		
		return vector<UInt>(s.begin(), s.end());
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bmeshInfo<SHAPE,MT>::getElemPatch(const UInt & Id) const
	{
		// Get element 
		auto elem = connectivity.grid->getElem(Id);
		
		// Get elements connected to at least one of the vertices of the element
		auto s = set_union(connectivity.node2elem[elem[0]], connectivity.node2elem[elem[1]]);
		for (UInt j = 2; j < NV; j++)
			s = set_union(graphItem(s), connectivity.node2elem[elem[j]]);
					
		// Remove Id
		s.erase(Id);
		
		return vector<UInt>(s.begin(), s.end());
	}
	
	
	//
	// Get geometric info
	//
	
	template<typename SHAPE, MeshType MT>
	Real bmeshInfo<SHAPE,MT>::getIntervalLength(const UInt & Id) const
	{
		// This method is provided only for 1D grids
		#ifdef NDEBUG
		static_assert(NV == 2,
			"getIntervalLength() is provided only for 1D grids.");
		#endif
			
		assert(Id < connectivity.grid->getNumElems());
		
		// Get interval length
		auto elem = connectivity.grid->getElem(Id);
		return (connectivity.grid->getNode(elem[0]) - connectivity.grid->getNode(elem[1])).norm2();
	}
	
	
	template<typename SHAPE, MeshType MT>
	Real bmeshInfo<SHAPE,MT>::getTriaArea(const UInt & Id) const
	{
		// This method is provided only for triangular grids
		#ifdef NDEBUG
		static_assert(NV == 3, 
			"getTriaArea() is provided only for triangular grids.");
		#endif
			
		assert(Id < connectivity.grid->getNumElems());
				
		// Get element vertices
		auto elem = connectivity.grid->getElem(Id);
		auto pA = connectivity.grid->getNode(elem[0]);
		auto pB = connectivity.grid->getNode(elem[1]);
		auto pC = connectivity.grid->getNode(elem[2]);
		
		// Get element area
		return 0.5 * ((pB - pA)^(pC - pA)).norm2();
	}
		
	
	template<typename SHAPE, MeshType MT>
	point bmeshInfo<SHAPE,MT>::getNormal(const UInt & Id) const
	{
		// This method is provided only for triangular grids
		#ifdef NDEBUG
		static_assert(NV == 3, 
			"getNormal() is provided only for triangular grids.");
		#endif
			
		assert(Id < connectivity.grid->getNumElems());
				
		// Get (some of) the element vertices
		auto elem = connectivity.grid->getElem(Id);
		auto pA = connectivity.grid->getNode(elem[0]);
		auto pB = connectivity.grid->getNode(elem[1]);
		auto pC = connectivity.grid->getNode(elem[2]);
		
		// Get element normal
		return ((pB - pA)^(pC - pB)).normalize();
	}
	
	
	//
	// Set geometric features
	//
	
	template<typename SHAPE, MeshType MT>
	void bmeshInfo<SHAPE,MT>::setBoundary(const UInt & Id)
	{
		assert(Id < connectivity.grid->getNumElems());
		
		// Extract node-element connections
		auto conn = connectivity.node2elem[Id].getConnected();
		
		// Get the number of different geometric Id's surrounding the node
		set<UInt> geoIds;
		for (auto el : conn)
			geoIds.insert(connectivity.grid->getElem(el).getGeoId());
			
		// Switch the number of different geometric Id's
		auto numGeoIds = geoIds.size();
		if (numGeoIds == 1)	
			connectivity.grid->setBoundary(Id,0);
		else if (numGeoIds == 2)
			connectivity.grid->setBoundary(Id,1);
		else
			connectivity.grid->setBoundary(Id,2);
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE void bmeshInfo<SHAPE,MT>::setBoundary()
	{
		for (UInt id = 0; id < connectivity.grid->getNumNodes(); id++)
			this->setBoundary(id);
	}
}

#endif
