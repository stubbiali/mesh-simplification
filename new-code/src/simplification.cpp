/*!	\file	simplification.cpp
	\brief	Specialization of some members of class simplification. */
			
#include "simplification.hpp"

// Include inlined class members
#ifndef INLINED
#include "inline/inline_simplification.hpp"
#endif

namespace geometry
{
	//
	// Compute cost and apply collapse
	//
		
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		getCost_f(const UInt & id1, const UInt & id2)
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
		// Extract elements involved in the collapse
		//
		
		#ifndef ENABLE_SELF_INTERSECTIONS
			auto invElems = gridOperation.getElemsInvolvedInEdgeCollapsing(id1,id2); 
		#endif
		auto toRemove = gridOperation.getElemsOnEdge(id1,id2);
		auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1,id2);
		
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
		
		// Store old id1
		auto P(gridOperation.getCPointerToMesh()->getNode(id1));
		
		// Update node-node, node-element and element-node connections
		auto oldConnections = gridOperation.getPointerToConnectivity()
			->applyEdgeCollapse(id2, id1, toRemove, toKeep);
			
		#ifndef ENABLE_SELF_INTERSECTIONS
			// For each involved element, get its patch
			// This will come useful when checking for mesh self-intersections
			vector<vector<UInt>> patches;
			patches.reserve(toKeep.size());
			for (auto elem : toKeep)
				patches.push_back(gridOperation.getTriPatch(elem));
		#endif
									
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
			#ifndef ENABLE_SELF_INTERSECTIONS
				structData.update(toRemove, toKeep); 
			#endif
			
			//
			// Check collapse validity
			//
			
			valid = true;
						
			for (it1 = toKeep.cbegin(), oldNormal = oldNormals.cbegin(); 
				it1 != toKeep.cend() && oldNormal != oldNormals.cend() && valid; 
				++it1, ++oldNormal)
			{
				// No degenerated triangles
				valid = valid && (gridOperation.getTriArea(*it1) > TOLL);
				
				// No triangle inversions
				valid = valid && ((*oldNormal) * gridOperation.getNormal(*it1) > TOLL);
				
				// No mesh self-intersections
				#ifndef ENABLE_SELF_INTERSECTIONS
					if (valid)
					{
						// Make the elements surrounding *it1 inactive
						// In this way, they will be disregarded in the checks
						for (auto elem : patches[it1-toKeep.cbegin()])
							gridOperation.getPointerToMesh()->setElemInactive(elem);
						
						// Extract elements whose bounding box intersect the one of *it1
						// and perform triangle-triangle intersection tests
						auto elems = structData.getNeighbouringElements(*it1);
						for (auto it2 = elems.cbegin(); it2 != elems.cend() && valid; ++it2)
							valid = valid && !(intrs.intersect(*it1, *it2));
						
						// Restore elements surrounding *it1
						for (auto elem : patches[it1-toKeep.cbegin()])
							gridOperation.getPointerToMesh()->setElemActive(elem);
					}
				#endif
			}
						
			//
			// Get cost associated with edge collapse
			//
			
			if (valid)
			{
				auto cost = costObj.getCost_f(id1, id2, pointsList[i]);
								
				// Check if it is the smallest so far
				if (cost < opt_cost - TOLL)
				{
					opt_cost = cost;
					opt_cPoint = i;
				}
			}
		}
			
		//
		// Restoration
		//
		
		// Restore connections
		gridOperation.getPointerToConnectivity()
			->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
		// Restore list of nodes
		gridOperation.getPointerToMesh()->setNode(id1, P);
		
		// Restore structured data
		#ifndef ENABLE_SELF_INTERSECTIONS
			structData.update(invElems);
		#endif
			
		//
		// Update collapseInfo's and collapsingEdge's lists
		//
		// First make sure that an optimal point has actually been found
		
		if (opt_cPoint < pointsList.size())
		{
			costObj.addCollapseInfo_f(id1, id2, opt_cost, pointsList[opt_cPoint]);
			collapsingSet.emplace(id1, id2, opt_cost, pointsList[opt_cPoint]);
		}
	}
	
	
	template<>
	void simplification<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>>::
		getCost_f(const UInt & id1, const UInt & id2)
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
		
		// Store old id1 
		auto P(gridOperation.getCPointerToMesh()->getNode(id1));
		
		// Update node-node, node-element and element-node connections
		auto oldConnections = gridOperation.getPointerToConnectivity()
			->applyEdgeCollapse(id2, id1, toRemove, toKeep);	
		
		//	
		// No edges sharing more than two nodes
		//
		
		auto nodes = gridOperation.getCPointerToConnectivity()->getNode2Node(id1).getConnected();
		for (auto node : nodes)
		{
			auto shared = gridOperation.getNodesOnEdge(id1, node);
			if (shared.size() != 2)
			{
				// Restore connections
				gridOperation.getPointerToConnectivity()
					->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
				// Restore list of nodes
				gridOperation.getPointerToMesh()->setNode(id1, P);
				
				return;
			}
		}	
			
		#ifndef ENABLE_SELF_INTERSECTIONS
			// For each involved element, get its patch
			// This will come useful when checking for mesh self-intersections
			vector<vector<UInt>> patches;
			patches.reserve(toKeep.size());
			for (auto elem : toKeep)
				patches.push_back(gridOperation.getTriPatch(elem));
		#endif
															
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
			
			#ifndef ENABLE_SELF_INTERSECTIONS			
				// Update structured data
				structData.update(toKeep); 
			#endif
						
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
				
				#ifndef ENABLE_SELF_INTERSECTIONS
					// No mesh self-intersections
					if (valid)
					{
						// Make the elements surrounding *it1 inactive
						// In this way, they will be disregarded in the checks
						for (auto elem : patches[it1-toKeep.cbegin()])
							gridOperation.getPointerToMesh()->setElemInactive(elem);
						
						// Extract elements whose bounding box intersect the one of *it1
						// and perform triangle-triangle intersection tests
						auto elems = structData.getNeighbouringElements(*it1);
						for (auto it2 = elems.cbegin(); it2 != elems.cend() && valid; ++it2)
							valid = valid && !(intrs.intersect(*it1, *it2));
						
						// Restore elements surrounding *it1
						for (auto elem : patches[it1-toKeep.cbegin()])
							gridOperation.getPointerToMesh()->setElemActive(elem);
					}
				#endif
			}
			
			//
			// Get cost associated with edge collapse
			//
			
			if (valid)
			{
				auto cost = costObj.getCost_f(id1, id2, pointsList[i]);
					
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
		
		#ifndef ENABLE_SELF_INTERSECTIONS
			// Restore structured data
			structData.update(toKeep);
		#endif
					
		//
		// Update collapseInfo's and collapsingEdge's lists
		//
		// First make sure that an optimal point has actually been found
		
		if (opt_cPoint < pointsList.size())
		{ 
			costObj.addCollapseInfo_f(id1, id2, opt_cost, pointsList[opt_cPoint]);
			collapsingSet.emplace(id1, id2, opt_cost, pointsList[opt_cPoint]);
		}
	}
		
	
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		update(const UInt & id1, const UInt & id2, const point3d & cPoint)
	{
		//
		// Extract elements involved in the collapse
		//
		
		auto toRemove = gridOperation.getElemsOnEdge(id1,id2);
		auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1,id2);
		
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
						
		// 
		// Update CostClass object and structured data
		//
		
		costObj.update(id1, id2, toRemove);
		#ifndef ENABLE_SELF_INTERSECTIONS
			structData.update(toRemove, toKeep);
		#endif
				
		// 
		// Remove from collapseInfo's and collapsingEdge's lists
		// the edges previously connected to id2
		//
		
		auto id2Conn = gridOperation.getCPointerToConnectivity()->getNode2Node(id2).getConnected();
		for (auto conn : id2Conn)
		{
			auto costPair = costObj.eraseCollapseInfo(id2, conn);
			if (costPair.first)
				id2 < conn ? collapsingSet.erase({id2, conn, costPair.second}) :
					collapsingSet.erase({conn, id2, costPair.second});
		}
						
		//
		// Extract edges whose cost must be re-computed
		//
		// These edges are the ones which shared at least a vertex 
		// with the edges sharing the collapsing point
		
		unordered_set<pair<UInt,UInt>> invEdges;
		
		// Get node-node connections for collapsing node...
		auto id1Conn = gridOperation.getCPointerToConnectivity()
			->getNode2Node(id1).getConnected();
		// ... and for each node get node-node connections
		for (auto node_i : id1Conn)
		{
			auto iConn = gridOperation.getCPointerToConnectivity()
				->getNode2Node(node_i).getConnected();
				
			// Save the egde
			for (auto node_j : iConn)
				(node_j > node_i) ? invEdges.emplace(node_i, node_j) :
					invEdges.emplace(node_j, node_i);
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
			getCost_f(edge.first, edge.second);
		}
	}
}
