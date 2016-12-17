/*!	\file	imp_simplification.hpp
	\brief	Implementations of members of class simplification. */
	
#ifndef HH_IMPSIMPLIFICATION_HH
#define HH_IMPSIMPLIFICATION_HH

#include <unordered_set>
#ifdef NDEBUG
#include <chrono>
#endif

namespace geometry
{
	//
	// Constructors
	//
	
	template<MeshType MT, typename CostClass>
	template<typename... Args>
	simplification<Triangle, MT, CostClass>::simplification
		(const string & file, Args... args) :
		gridOperation(file), costObj(&gridOperation, args...), 
		structData(gridOperation), intrs(gridOperation.getPointerToMesh()), dontTouch(true)
	{
		#ifdef NDEBUG
			using namespace std::chrono;
			high_resolution_clock::time_point start, stop;
		#endif
		
		// Important control on coherence between the inputs:
		// error if the CostClass supports distributed data but the
		// mesh is purely geometrical
		static_assert(std::is_base_of<bcost<Triangle, MT, CostClass>, CostClass>::value,
			"CostClass must be coherent with the mesh type.");

		// Create the set of collapsingEdge's ordered by cost
		#ifndef NDEBUG
			setupCollapsingSet();
		#else
			start = high_resolution_clock::now();
			setupCollapsingSet();
			stop = high_resolution_clock::now();
			auto dif_collapsingSet = duration_cast<milliseconds>(stop-start).count();
			cout << "Elapsed time for initializing collapsingSet: " << 
				dif_collapsingSet << " ms" << endl;
		#endif

		// Define the fixed element
		#ifndef NDEBUG
			findDontTouchId();
		#else
			start = high_resolution_clock::now();
			findDontTouchId();
			stop = high_resolution_clock::now();
			auto dif_dontTouch = duration_cast<milliseconds>(stop-start).count();
			cout << "Elapsed time for finding fixed element: " << 
				dif_dontTouch << " ms" << endl;
		#endif
	}
	
	
	//
	// Initialization and refreshing methods
	//

	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::setupCollapsingSet()
	{
		// Extract edges
		auto edges = gridOperation.getCPointerToConnectivity()->getEdges();
	
		// Loop on the edges list 
		for (auto edge : edges) 
			// Take the cost information and add it to the lists
			getCost(edge[0], edge[1]);  
	}
	
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::refresh()
	{
		// Remove inactive nodes and elements from grid
		gridOperation.getPointerToMesh()->refresh();
		
		// Re-build connectivity
		gridOperation.getPointerToConnectivity()->refresh();
		
		// Possibly update CostClass object and re-build
		// set of collapsingEdge's
		costObj.setMeshOperation(&gridOperation);
		setupCollapsingSet();
		
		// Update structured data
		structData.setMesh(gridOperation);
		
		// Possibly update fixed element
		if (dontTouch)
			findDontTouchId();
	}
	
	
	//
	// Set methods
	//
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::setGrid(const mesh<Triangle,MT> & grid)
	{
		// Set mesh and build connections
		gridOperation.setMesh(grid);
		
		// Possibly update CostClass object and re-build
		// set of collapsingEdge's
		costObj.setMeshOperation(&gridOperation);
		setupCollapsingSet();
		
		// Update structured data
		structData.setMesh(gridOperation);
		
		// Set mesh for class intersection
		intrs.setMesh(gridOperation.getCPointerToMesh());
		
		// Possibly find fixed element
		if (dontTouch)
			findDontTouchId();
	}


	//
	// Compute cost and apply collapse
	//
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::
		getCost(const UInt & id1, const UInt & id2)
	{
		// First make sure the fixed element is not involved
		if (dontTouch)
			if ((id1 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[0]) ||
				(id1 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[1]) ||
				(id1 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[2]) ||
				(id2 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[0]) ||
				(id2 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[1]) ||
				(id2 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[2]))
				return;
				
		//
		// Get potentially valid points
		//
		
		auto pointsList = costObj.getPointsList(id1, id2);
		if (pointsList.empty())
			return;
				
		//
		// Extract elements and data involved in the collapse
		//
		
		auto invElems = gridOperation.getElemsInvolvedInEdgeCollapsing(id1,id2);
		auto toRemove = gridOperation.getElemsOnEdge(id1,id2);
		auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1,id2);
		auto toMove = gridOperation.getDataModifiedInEdgeCollapsing(invElems);
		
		// Furthermore, check that the number of elements insisting 
		// on the edge is exactly two
		if (toRemove.size() != 2)
			return;
		
		// Get normals to the elements involved in the collapse
		vector<point3d> oldNormals;
		oldNormals.reserve(toKeep.size());
		for (auto elem : toKeep)
			oldNormals.emplace_back(gridOperation.getNormal(elem));
			
		//
		// Update connections
		//
		
		// Store old id1 and make id2 inactive
		auto P(gridOperation.getCPointerToMesh()->getNode(id1));
		gridOperation.getPointerToMesh()->setNodeInactive(id2);
		
		// Update node-node, node-element and element-node connections
		auto oldConnections = gridOperation.getPointerToConnectivity()
			->applyEdgeCollapse(id2, id1, toRemove, toKeep);	
									
		//
		// Get the cheapest edge
		//
				
		// Auxiliary variables
		bool valid;
		vector<UInt>::const_iterator it1;
		vector<point3d>::const_iterator oldNormal; 
		Real opt_cost(numeric_limits<Real>::max());
		UInt opt_cPoint(pointsList.size());
		
		for (UInt i = 0; i < pointsList.size(); ++i)
		{
			//
			// Set collapsing point
			//
		
			// Change coordinates and boundary flag of id1
			gridOperation.getPointerToMesh()->setNode(id1, pointsList[i]);
			
			// Update structured data
			structData.update(toRemove, toKeep); 
			
			// Project data points and update data-element 
			// and element-data connections
			auto oldData = gridOperation.project(toMove, toKeep);
			gridOperation.getPointerToConnectivity()->eraseElemInData2Elem(toRemove);
			
			//
			// Check collapse validity
			//
			
			valid = true;
						
			for (it1 = toKeep.cbegin(), oldNormal = oldNormals.cbegin(); 
				it1 != toKeep.cend() && oldNormal != oldNormals.cend() && valid; 
				++it1, ++oldNormal)
			{
				// No degenerate triangles
				valid = (gridOperation.getTriArea(*it1) > TOLL);
				
				// No triangle inversions
				valid = valid && ((*oldNormal) * gridOperation.getNormal(*it1) > TOLL);
				
				// No empty triangles
				valid = valid && !(gridOperation.isEmpty(*it1));
				
				// No mesh self-intersections
				auto elems = structData.getNeighbouringElements(*it1);
				for (auto it2 = elems.cbegin(); it2 != elems.cend() && valid; ++it2)
					valid = valid && !(intrs.intersect(*it1, *it2));
			}
			
			//
			// Get cost associated with edge collapse
			//
			
			if (valid)
			{
				auto cost = costObj.getCost(id1, id2, pointsList[i], toKeep, toMove);
								
				// Check if it is the smallest so far
				if (cost < opt_cost - TOLL)
				{
					opt_cost = cost;
					opt_cPoint = i;
				}
			}
			
			//
			// Restore data-element and element-data connections
			//
			
			gridOperation.undo(toMove, oldData);
			gridOperation.getPointerToConnectivity()->insertElemInData2Elem(toRemove);
		}
			
		//
		// Restoration
		//
		
		// Restore connections
		gridOperation.getPointerToConnectivity()
			->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
		// Restore list of nodes
		gridOperation.getPointerToMesh()->setNode(id1, P);
		gridOperation.getPointerToMesh()->setNodeActive(id2);
		
		// Restore structured data
		structData.update(invElems);
					
		//
		// Update collapseInfo's and collapsingEdge's lists
		//
		// First make sure that an optimal point has actually been found
		
		if (opt_cPoint < pointsList.size())
		{ 
			costObj.addCollapseInfo(id1, id2, opt_cost, pointsList[opt_cPoint]);
			collapsingSet.emplace(id1, id2, opt_cost, pointsList[opt_cPoint]);
		}
	}
	

	// Specialization for grids without distributed data
	// This method requires a specialization for each purely geometric cost class 
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		getCost(const UInt & id1, const UInt & id2);
			
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::
		update(const UInt & id1, const UInt & id2, const point3d & cPoint)
	{
		//
		// Extract elements and data involved in the collapse
		//
		
		auto invElems = gridOperation.getElemsInvolvedInEdgeCollapsing(id1,id2);
		auto toRemove = gridOperation.getElemsOnEdge(id1,id2);
		auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1,id2);
		auto toMove = gridOperation.getDataModifiedInEdgeCollapsing(invElems);
		
		//
		// Update mesh and connections
		//
		
		// Set node id1 as collapsing point
		gridOperation.getPointerToMesh()->setNode(id1, cPoint);
		
		// Set node id2 inactive
		gridOperation.getPointerToMesh()->setNodeInactive(id2);
					
		// Update element-node, node-node and node-element connections
		gridOperation.getPointerToConnectivity()
			->applyEdgeCollapse(id2, id1, toRemove, toKeep);
			
		// Project involved data points and update data-element
		// and element-data connections
		gridOperation.project(toMove, toKeep);
		gridOperation.getPointerToConnectivity()->eraseElemInData2Elem(toRemove);
		
		// 
		// Update CostClass object and structured data
		//
		
		costObj.update(id1, id2, toRemove);
		structData.update(toRemove, toKeep);
		
		// 
		// Remove from collapseInfo's and collapsingEdge's lists
		// the edges previously connected to id2
		//
		
		auto id2Conn = gridOperation.getCPointerToConnectivity()->getNode2Node(id2).getConnected();
		for (auto conn : id2Conn)
		{
			auto costPair = costObj.eraseCollapseInfo(id2, conn);
			if (costPair.first)
				collapsingSet.erase({id2, conn, costPair.second});
		}
			
		//
		// Extract edges whose cost must be re-computed
		//
		// These edges are the ones which shared at least a vertex 
		// with the triangles in the extended patch of the collapsing node
		
		unordered_set<pair<UInt,UInt>> invEdges;
		
		// Get node-node connections for collapsing node, ...
		auto id1Conn = gridOperation.getCPointerToConnectivity()
			->getNode2Node(id1).getConnected();
		// ... for each node get the node-node connections...
		for (auto node_i : id1Conn)
		{
			auto iConn = gridOperation.getCPointerToConnectivity()
				->getNode2Node(node_i).getConnected();
			// ... and also for these nodes get node-node connections
			for (auto node_j : iConn)
			{
				auto jConn = gridOperation.getCPointerToConnectivity()
					->getNode2Node(node_j).getConnected();
					
				// Finally, save the edge
				for (auto node_k : jConn)
					(node_k > node_j) ? invEdges.emplace(node_j, node_k) :
						invEdges.emplace(node_k, node_j);
			}
		}
			
		//
		// Re-compute cost for involved edges
		//
		// First erase old costs from lists of collapseInfo's and
		// collapsingEdge's, then insert updated values
		
		for (auto edge : invEdges)
		{
			// Remove edge from collapseInfo's list and get cost
			auto costPair = costObj.eraseCollapseInfo(edge.first, edge.second);
			
			// Remove edge from collapsingEdge's list
			if (costPair.first)
				collapsingSet.erase({edge.first, edge.second, costPair.second});
			
			// Compute new cost and possibly add it to collapseInfo's
			// and collapsingEdge's lists
			getCost(edge.first, edge.second);
		}
	}
	
	
	// Specialization for grids without distributed data
	// This method requires a specialization for each purely geometric cost class
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		update(const UInt & id1, const UInt & id2, const point3d & cPoint);
	

	//
	// Methods for handling the fixed element
	//
	
	template<MeshType MT, typename CostClass>
	INLINE void simplification<Triangle, MT, CostClass>::enableDontTouch()
	{
		dontTouch = true;
	}
	
	
	template<MeshType MT, typename CostClass>
	INLINE void simplification<Triangle, MT, CostClass>::disableDontTouch()
	{
		dontTouch = false;
	}
	
	
	template<MeshType MT, typename CostClass>
	INLINE bool simplification<Triangle, MT, CostClass>::getDontTouch() const
	{
		return dontTouch;
	}
	
	
	template<MeshType MT, typename CostClass>
	INLINE void simplification<Triangle, MT, CostClass>::setDontTouchId(const UInt & Id)
	{
		dontTouchId = Id;
	}
	
	
	template<MeshType MT, typename CostClass>
	INLINE UInt simplification<Triangle, MT, CostClass>::getDontTouchId() const
	{
		return dontTouchId;
	}
	
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::findDontTouchId()
	{		
		dontTouch = true;
		
		// Compute global barycenter
		auto global_bar = gridOperation.getMeshBarycenter();
		
		// Get the element whose barycenter is closest to the global one
		Real opt_dist(numeric_limits<Real>::max());
		for (UInt i = 0; i < gridOperation.getCPointerToMesh()->getElemsListSize(); ++i)
		{
			if (gridOperation.getCPointerToMesh()->getElem(i).isActive())
			{
				// Compute local barycenter
				auto local_bar = gridOperation.getElemBarycenter(i);
				
				// Check if it is the closest element
				auto dist = (local_bar - global_bar).norm2();
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					dontTouchId = i;
				}
			}
		}
	}


	//
	// Methods which make the simplification
	//

	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::simplificate(const UInt & numNodesMax,
		const string & file)
	{
		// Check if the current number of nodes is below the threshold
		auto numNodesStart(gridOperation.getCPointerToMesh()->getNumNodes());
		if (numNodesMax >= numNodesStart)
		{
			cout << "The number of mesh points is " << gridOperation.getCPointerToMesh()->getNumNodes()
				<< ", already below the given threshold " << numNodesMax << endl;
			return;
		}

		//
		// Run simplification
		//

		cout << "Simplification process..." << endl;
		#ifdef NDEBUG
		using namespace std::chrono;
		high_resolution_clock::time_point start = high_resolution_clock::now();
		#endif

		// Iterative collapse until numNodeMax is reached
		while (gridOperation.getCPointerToMesh()->getNumNodes() > numNodesMax)
		{
			// Take the first valid collapsing edge with the minimum cost
			auto minCostEdge = collapsingSet.cbegin();
			auto id1 = minCostEdge->getId1();
			auto id2 = minCostEdge->getId2();
			auto cPoint = minCostEdge->getCollapsingPoint();
			
			// Update the mesh, the connectivities, the structured data, CostClass object
			// Re-compute cost for involved edges
			update(id1, id2, cPoint);
		}
		
		#ifdef NDEBUG
		high_resolution_clock::time_point stop = high_resolution_clock::now();
		auto dif = duration_cast<milliseconds>(stop-start).count();
		#endif
		
		//
		// Refresh the mesh
		//
		
		gridOperation.getPointerToMesh()->refresh();

		//
		// Print ...
		//
		
		// ... to screen
		cout << "The mesh size passed from " << numNodesStart << " to " << 
			gridOperation.getCPointerToMesh()->getNumNodes() << " nodes" << endl;
		#ifdef NDEBUG
		cout << "Simplification process completated in " <<  dif << " ms" << endl;
		#endif
		
		// ... to file
		if (!(file.empty()))
			gridOperation.getPointerToMesh()->print(file);
	}
}

#endif
