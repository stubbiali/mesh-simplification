/*! \file	imp_bconnect.hpp
	\brief	Definitions of members of class bconnect. */
	
#ifndef HH_IMPBCONNECT_HH
#define HH_IMPBCONNECT_HH

#include <array>
#include <stdexcept>

namespace geometry
{
	// 
	// Constructor
	//
	
	template<typename SHAPE, MeshType MT>
	bconnect<SHAPE,MT>::bconnect(const shared_ptr<bmesh<SHAPE>> & bg) : 
		grid(make_shared<mesh<SHAPE,MT>>(*bg))
	{
		// Build all connections and fill set of edges
		buildNode2Node();
		buildNode2Elem();
		buildElem2Elem();
	}
	
	
	template<typename SHAPE, MeshType MT>
	bconnect<SHAPE,MT>::bconnect(const shared_ptr<mesh<SHAPE,MT>> & g) : 
		grid(g)
	{
		// Build all connections and fill set of edges
		buildNode2Node();
		buildNode2Elem();
		buildElem2Elem();
	}
		
	
	template<typename SHAPE, MeshType MT>
	template<typename... Args>
	bconnect<SHAPE,MT>::bconnect(Args... args) :
		grid(make_shared<mesh<SHAPE,MT>>(args...))
	{
		// Build all connections and fill set of edges
		buildNode2Node();
		buildNode2Elem();
		buildElem2Elem();
	}
	
	
	//
	// Initialize and clear connections
	//
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::buildNode2Node()
	{
		if (grid != nullptr)
		{
			// Reserve memory
			node2node.clear();
			node2node.reserve(grid->getNumNodes());
						
			// Set nodes Id's
			for (UInt id = 0; id < grid->getNumNodes(); id++)
				node2node.emplace_back(id);
							
			// Loop over all elements
			geoElement<SHAPE> elem;
			UInt id1, id2;
			for (UInt id = 0; id < grid->getNumElems(); id++)
			{
				// Extract element
				elem = grid->getElem(id);
								
				// Loop over edges
				for (UInt j = 0; j < N; j+=2)
				{
					// Extract extrema of the edge
					id1 = elem[SHAPE::edgeConn[j]];
					id2 = elem[SHAPE::edgeConn[j+1]];
										
					// Add node-node connections
					node2node[id1].insert(id2);
					node2node[id2].insert(id1);
										
					// Update set of edges
					edges.emplace(array<UInt,2>({id1,id2}));
				}
			}
		}
		else	throw runtime_error("Mesh pointer not set.");
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::buildNode2Elem()
	{
		if (grid != nullptr)
		{
			// Reserve memory
			node2elem.clear();
			node2elem.reserve(grid->getNumNodes());
			
			// Set nodes Id's
			for (UInt id = 0; id < grid->getNumNodes(); id++)
				node2elem.emplace_back(id);
				
			// Loop over elements
			geoElement<SHAPE> elem;
			for (UInt id = 0; id < grid->getNumElems(); id++)
			{
				// Extract element
				elem = grid->getElem(id);
				
				// Loop over its vertices and add node-element connections
				for (UInt j = 0; j < NV; j++)
					node2elem[elem[j]].insert(id);
			}
		}
		else	throw runtime_error("Mesh pointer not set.");
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::buildElem2Elem(const UInt & Id)
	{
		// Some checks (only debug mode)
		assert(grid != nullptr);
		assert(!(node2elem.empty()));
		assert(Id < grid->getNumElems());

		// First, clear the old connections
		elem2elem[Id].clear();	
		
		// Extract element
		auto elem = grid->getElem(Id);
		
		// Find the elements which share an edge with Id
		for (UInt j = 0; j < N; j++)
		{
			auto s = set_intersection(node2elem[elem[SHAPE::edgeConn[j]]], 
				node2elem[elem[SHAPE::edgeConn[j+1]]]);
			elem2elem[Id].insert(s);
		}
		
		// Remove the element itself from the patch
		elem2elem[Id].erase(Id);
	}
		
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::buildElem2Elem()
	{
		if (grid != nullptr)
		{
			// First, build the node-element connections
			if (node2elem.empty())
				buildNode2Elem();
				
			// Reserve memory
			elem2elem.clear();
			elem2elem.reserve(grid->getNumElems());
				
			// Loop over all elements		
			for (UInt id = 0; id < grid->getNumElems(); id++)
			{
				// Set element Id
				elem2elem.emplace_back(id);
				
				// Create element-element connections
				buildElem2Elem(id);
			}
		}
		else	throw runtime_error("Mesh pointer not set.");
	}
		
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::refresh()
	{
		// Clear all connections and the set of edges
		this->clear();
		
		// Re-build all connections and the set of edges
		buildNode2Node();
		buildNode2Elem();
		buildElem2Elem();
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::clear()
	{
		// Clear connections
		node2node.clear();
		node2elem.clear();
		elem2elem.clear();
		
		// Clear set of edges
		edges.clear();
	}
	
	
	//
	// Modify connections
	//
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::replaceNodeInNode2Node(const UInt & oldId, const UInt & newId)
	{
		// First, remove the possible connection between oldId and newId
		node2node[oldId].erase(newId);
		node2node[newId].erase(oldId);
		
		// Make oldId inactive and extract its connected
		node2node[oldId].setInactive();
		auto oldIdConn = node2node[oldId].getConnected();
		
		// The nodes previously connected to oldId are now connected to newId
		node2node[newId].insert(oldIdConn);
		
		// For all connected, replace oldId with newId
		for (auto id : oldIdConn)
			node2node[id].replace(oldId,newId);
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::replaceNodeInNode2Node(const UInt & oldId, const UInt & newId, const vector<UInt> & involved)
	{
		// First, remove the possible connection between oldId and newId
		node2node[newId].erase(oldId);
		
		// Make oldId inactive
		node2node[oldId].setInactive();
		
		// The nodes previously connected to oldId are now connected to newId
		node2node[newId].setConnected(involved);
		
		// For all connected, replace oldId with newId
		for (auto id : involved)
			node2node[id].replace(oldId,newId);
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::replaceNodeInNode2Elem(const UInt & oldId, const UInt & newId)
	{
		// Move the connected from oldId to newId
		node2elem[newId].insert(node2elem[oldId].getConnected());
		
		// Make oldId inactive
		node2elem[oldId].setInactive();
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::eraseElemsInNode2Elem(const vector<UInt> & toRemove)
	{
		for (auto id : toRemove)
		{
			auto elem = grid->getElem(id);
			for (UInt j = 0; j < NV; j++)
				node2elem[elem[j]].erase(id);
		}
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::eraseElemsInNode2Elem(const vector<UInt> & toRemove, 
		const UInt & newId, const vector<UInt> & involved)
	{
		// Remove elements from newId
		node2elem[newId].erase(toRemove[0]);
		node2elem[newId].erase(toRemove[1]);
		
		// Remove elements from all other involved nodes
		for (auto node : involved)
		{
			node2elem[node].erase(toRemove[0]);
			node2elem[node].erase(toRemove[1]);
		}
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::eraseElemsInElem2Elem(const vector<UInt> & toRemove)
	{
		for (auto id : toRemove)
		{
			// Make element inactive
			elem2elem[id].setInactive();
			
			// Extract connected elements
			auto conn = elem2elem[id].getConnected();
			
			// Re-build connections for the connected elements
			for (auto connId : conn)
				buildElem2Elem(connId);
		}
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::applyEdgeCollapsing(const UInt & oldId, 
		const UInt & newId, const vector<UInt> & toRemove)
	{
		// Update node-node connections
		replaceNodeInNode2Node(oldId, newId);
		
		// Update node-element connections
		replaceNodeInNode2Elem(oldId, newId);
		eraseElemsInNode2Elem(toRemove);
		
		// Update element-element connections
		eraseElemsInElem2Elem(toRemove);
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::applyEdgeCollapsing(const UInt & oldId, 
		const UInt & newId, const vector<UInt> & toRemove, const vector<UInt> & involved)
	{
		// Update node-node connections
		replaceNodeInNode2Node(oldId, newId, involved);
		
		// Update node-element connections
		replaceNodeInNode2Elem(oldId, newId);
		eraseElemsInNode2Elem(toRemove, newId, involved);
		
		// Update element-element connections
		eraseElemsInElem2Elem(toRemove);
	}
	
	
	//
	// Get methods
	//
	
	template<typename SHAPE, MeshType MT>
	INLINE shared_ptr<mesh<SHAPE,MT>> bconnect<SHAPE,MT>::getMesh()
	{
		return grid;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<geoElement<Line>> bconnect<SHAPE,MT>::getEdges() const
	{
		return vector<geoElement<Line>>(edges.cbegin(), edges.cend());
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE graphItem bconnect<SHAPE,MT>::getNode2Node(const UInt & Id) const
	{
		assert(Id < grid->getNumNodes());
		return node2node[Id];
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<graphItem> bconnect<SHAPE,MT>::getNode2Node() const
	{
		return node2node;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE graphItem bconnect<SHAPE,MT>::getNode2Elem(const UInt & Id) const
	{
		assert(Id < grid->getNumNodes());
		return node2elem[Id];
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<graphItem> bconnect<SHAPE,MT>::getNode2Elem() const
	{
		return node2elem;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE graphItem bconnect<SHAPE,MT>::getElem2Elem(const UInt & Id) const
	{
		assert(Id < grid->getNumElems());
		return elem2elem[Id];
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<graphItem> bconnect<SHAPE,MT>::getElem2Elem() const
	{
		return elem2elem;
	}
	
	
	//
	// Set methods
	//
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::setMesh(const shared_ptr<mesh<SHAPE,MT>> & newGrid)
	{
		// Set mesh pointer
		grid = newGrid;
		
		// (Re-)build connections
		refresh();
	}
}

#endif
