/*!	\file	DataGeo.cpp
	\brief	Implementations of members of class DataGeo. */
	
#include <limits>

#include "DataGeo.hpp"
#include "array_operators.hpp"
#include "Eigen/Dense"

// Include implementations of inlined class members
#ifndef INLINED
#include "inline/inline_DataGeo.hpp"
#endif

namespace geometry
{
	// Simplify notation for Eigen matrices and vectors
	using namespace Eigen;
	using Vector3r = Matrix<Real,3,1>;
	using Matrix3r = Matrix<Real,3,3>;
	
	
	//
	// Constructors
	//
	
	DataGeo::DataGeo(const Real & a, const Real & b, const Real & c,
		bmeshOperation<Triangle, MeshType::DATA> * bmo) :
		bcost<Triangle, MeshType::DATA, DataGeo>(bmo), weight({a,b,c})
	{
		if (this->oprtr != nullptr)
		{
			// Create list of Q matrices
			buildQs();
			
			// Extract original locations of data points
			getOriginalDataPointsLocations();
			
			// Compute quantity of information for all elements
			buildQuantityOfInformation();
			
			// Get maximum for each cost function
			getMaximumCosts();
		}
	}
	
	
	//
	// Initialization and update
	//
	
	array<Real,10> DataGeo::getKMatrix(const UInt & id) const
	{
		assert(id < this->oprtr->getPointerToMesh()->getNumElems());
		
		// Extract the first vertex of the triangle
		auto elem = this->oprtr->getPointerToMesh()->getElem(id);
		auto p = this->oprtr->getPointerToMesh()->getNode(elem[0]);
		
		// Compute unit normal and (signed) distance from the origin
		// for the plane identified by the triangle
		auto N = this->oprtr->getNormal(id);
		auto d = -(N*p);
		
		// Construct matrix K
		return array<Real,10>({N[0]*N[0], N[0]*N[1], N[0]*N[2], N[0]*d,
			N[1]*N[1], N[1]*N[2], N[1]*d, N[2]*N[2], N[2]*d, d*d});
	}
	
	
	void DataGeo::buildQs()
	{
		assert(this->oprtr != nullptr);
		
		// Extract number of nodes and elements
		auto numNodes = this->oprtr->getPointerToMesh()->getNumNodes();
		auto numElems = this->oprtr->getPointerToMesh()->getNumElems();
		
		// Reserve memory and initialize Q matrices to zero
		Qs.clear();
		Qs.reserve(numNodes);
		for (UInt i = 0; i < numNodes; ++i)
		{
			Qs.emplace_back();
			Qs[i] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
		}
			
		// Loop over all elements, for each triangle compute the 
		// related matrix K and add it to Q matrices of the vertices 
		// of the triangle
		for (UInt j = 0; j < numElems; ++j)
		{
			auto elem = this->oprtr->getPointerToMesh()->getElem(j);
			auto K = getKMatrix(j);
			Qs[elem[0]] += K;
			Qs[elem[1]] += K;
			Qs[elem[2]] += K;
		}
	}
	
	
	void DataGeo::updateQs(const UInt & newId)
	{
		// Extract the nodes connected to id
		auto nodes = this->oprtr->getPointerToConnectivity()
			->getNode2Node(newId).getConnected();
		
		// 
		// Re-build Q matrix for the collapsing point
		//
		
		// Set Q to zero
		Qs[newId] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
		
		// Loop over all elements sharing the collapsing point,
		// compute K and add it to Q
		auto id_elems = this->oprtr->getPointerToConnectivity()
			->getNode2Elem(newId).getConnected();
		for (auto elem : id_elems)
		{
			Qs[newId] += getKMatrix(elem);
		}
		
		// 
		// Re-build Q matrix for all nodes connected to
		// the collapsing point
		//
		
		for (auto node : nodes)
		{
			// Set Q to zero
			Qs[node] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
	
			// Loop over all elements sharing the node,
			// compute K and add it to Q
			auto node_elems = this->oprtr->getPointerToConnectivity()
				->getNode2Elem(node).getConnected();
			for (auto elem : node_elems)
				Qs[node] += getKMatrix(elem);
		}
	}
	
	
	void DataGeo::getOriginalDataPointsLocations()
	{
		assert(this->oprtr != nullptr);
		
		// Reserve memory
		dataOrigin.clear();
		dataOrigin.reserve(this->oprtr->getPointerToMesh()->getNumData());
		
		// Copy data points from the mesh
		for (UInt i = 0; i < this->oprtr->getPointerToMesh()->getNumData(); ++i)
			dataOrigin.emplace_back(this->oprtr->getPointerToMesh()->getData(i));
	}
	
	
	void DataGeo::buildQuantityOfInformation()
	{
		assert(this->oprtr != nullptr);
		
		// Set number of elements
		numElems = this->oprtr->getPointerToMesh()->getNumElems();
		
		// Reserve memory
		qoi.clear();
		qoi.reserve(numElems);
		
		// Get quantity of information for each element,
		// while computing the sum
		Real qoi_sum(0.);
		for (UInt i = 0; i < numElems; ++i)
		{
			Real qoi_elem(this->oprtr->getQuantityOfInformation(i));
			qoi.push_back(qoi_elem);
			qoi_sum += qoi_elem;
		}
		
		// Compute average quantity of information
		qoi_mean = qoi_sum/numElems;
	}
	
	
	void DataGeo::updateQuantityOfInformation(const UInt & newId, 
		const vector<UInt> & toRemove)
	{
		// Get sum of quantities of information
		// over the entire mesh before the collapse
		Real qoi_sum = qoi_mean * numElems;
		
		// Subtract from the sum the QOI's of the elements
		// to remove
		for (auto elem : toRemove)
			qoi_sum -= qoi[elem];
			
		// Extract extended patch for the collapsing node,
		// i.e. the elements whose QOI must be updated
		auto patch = this->oprtr->getExtendedNodePatch(newId);
		
		// Update QOI for each element in the extended patch
		for (auto elem : patch)
		{
			// Get new QOI
			Real qoi_new = this->oprtr->getQuantityOfInformation(elem);
			
			// For updating the sum of QOI's, split sum and difference
			// for (possibly) avoiding catastrophic cancellation
			qoi_sum -= qoi[elem];
			qoi_sum += qoi_new;
			
			// Update list of QOI's
			qoi[elem] = qoi_new;
		} 
		
		// Update number of elements and average quantity of information
		numElems = this->oprtr->getPointerToMesh()->getNumElems();
		qoi_mean = qoi_sum/numElems;
	}
	
	
	tuple<Real,Real,Real> DataGeo::getDecomposedCost(const UInt & id1, const UInt & id2,
		const point3d & p, const vector<UInt> & toKeep, const vector<UInt> & toMove)
	{
		//
		// Compute geometric cost function
		//
		
		// Extract the matrix Q associated to the edge
		auto Q = 0.5 * (Qs[id1] + Qs[id2]);
		
		// Compute the quadratic form
		Real geo = Q[0]*p[0]*p[0] + Q[4]*p[1]*p[1] + Q[7]*p[2]*p[2]
			+ 2*Q[1]*p[0]*p[1] + 2*Q[2]*p[0]*p[2] + 2*Q[5]*p[1]*p[2]
			+ 2*Q[3]*p[0] + 2*Q[6]*p[1] + 2*Q[8]*p[2] + Q[9];
			
		//
		// Compute data displacement cost function
		//
		// Get the maximum distance between the original data point
		// and its current position for involved data points
		
		Real disp(numeric_limits<Real>::lowest());
		for (auto datum : toMove)
		{
			point3d dataProjected(this->oprtr->getPointerToMesh()->getData(datum));
			Real dl = (dataProjected - dataOrigin[datum]).norm2();
			if (dl > disp)	
				disp = dl;
		}
		
		//
		// Compute data distribution cost function
		//
		
		Real equi(0.);
		for (auto elem : toKeep)
		{
			Real qoi_new(this->oprtr->getQuantityOfInformation(elem));
			equi += (qoi_new - qoi_mean)*(qoi_new - qoi_mean);
		}
		
		// Average over all involved elements
		equi /= toKeep.size();
		
		return make_tuple(geo, disp, equi);
	}
	
	
	void DataGeo::getMaximumCosts(const UInt & id1, const UInt & id2) 
	{
		//
		// Get potentially valid collapsing points
		//
		
		auto pointsList = getPointsList(id1, id2);
		
		if (pointsList.size() > 0)
		{
			Real geo, disp, equi;
			
			// 
			// Get elements and data involved in the collapse
			//
			
			auto invElems = this->oprtr->getElemsInvolvedInEdgeCollapsing(id1, id2);
			auto toRemove = this->oprtr->getElemsOnEdge(id1, id2);
			auto toKeep = this->oprtr->getElemsModifiedInEdgeCollapsing(id1, id2);
			auto toMove = this->oprtr->getDataModifiedInEdgeCollapsing(invElems);
			
			//
			// Update connections
			//
		
			// Store old id1 and make id2 inactive
			auto P(this->oprtr->getPointerToMesh()->getNode(id1));
			this->oprtr->getPointerToMesh()->setNodeInactive(id2);
		
			// Update node-node, node-element and element-node connections
			auto oldConnections = this->oprtr->getPointerToConnectivity()
				->applyEdgeCollapse(id2, id1, toRemove, toKeep);
			
			for (auto Q : pointsList)
			{
				//
				// Set collapsing point
				//
		
				// Change coordinates and boundary flag of id1
				this->oprtr->getPointerToMesh()->setNode(id1, Q);
			
				// Project data points and update data-element and 
				// element-data connections
				auto oldData = this->oprtr->project(toMove, toKeep);
				
				//
				// Update maximum values for each cost function
				//
				
				// Compute all cost functions
				tie(geo, disp, equi) = getDecomposedCost(id1, id2, Q, toKeep, toMove);
				
				// Possibly update maximum values
				if (geo > maxCost[0])	
					maxCost[0] = geo;
				if (disp > maxCost[1]) 
					maxCost[1] = disp;
				if (equi > maxCost[2])
					maxCost[2] = equi;
				
				//
				// Undo projections and restore data-element and
				// element-data connections
				//
				
				this->oprtr->undo(toMove, oldData);
			}
			
			//
			// Restoration
			//
		
			// Restore connections
			this->oprtr->getPointerToConnectivity() ->undoEdgeCollapse(id2, id1, 
				oldConnections.first, oldConnections.second, toRemove); 
			
			// Restore list of nodes
			this->oprtr->getPointerToMesh()->setNode(id1, P);
			this->oprtr->getPointerToMesh()->setNodeActive(id2);
		}
	}
	
	
	void DataGeo::getMaximumCosts()
	{
		assert(this->oprtr != nullptr);
		
		// Reset maximum (i.e. normalizing) values
		maxCost = {numeric_limits<Real>::lowest(),
			numeric_limits<Real>::lowest(), numeric_limits<Real>::lowest()};
			
		// Extract all edges
		auto edges = this->oprtr->getPointerToConnectivity()->getEdges();
		
		// Loop over all edges and for each one possibly update
		// the maximum values
		for (auto edge : edges)
			getMaximumCosts(edge[0], edge[1]);
	}
	
	
	void DataGeo::imp_update(const UInt & newId, const vector<UInt> & toRemove)
	{
		assert(this->oprtr != nullptr);
		
		// Update list of Q matrices
		updateQs(newId);
		
		// Update list of quantity of information for each element,
		// number of elements and average quantity of information
		updateQuantityOfInformation(newId, toRemove);
	}
	
	
	//
	// Set methods
	//
	
	void DataGeo::imp_setMeshOperation(bmeshOperation<Triangle, MeshType::DATA> * bmo)
	{
		// Set pointer to bmeshOperation object
		this->oprtr = bmo;
		
		// Build list of Q matrices
		buildQs();
		
		// Extract original data points locations
		getOriginalDataPointsLocations();
		
		// Compute quantity of information for each element
		buildQuantityOfInformation();
		
		// Get maximum for each cost function
		getMaximumCosts();
	}
	
	
	//
	// Get methods
	//
	
	pair<bool,point> DataGeo::getOptimumPoint(const UInt & id1, const UInt & id2) const
	{
		// The point (x,y,z) minimizing the geometric cost function
		// is given by the linear system 
		//
		// | Q(0,0) Q(0,1) Q(0,2) | | x |   | -Q(0,3) |
		// | Q(1,0) Q(1,1) Q(1,2) | | y | = | -Q(1,3) |
		// | Q(2,0) Q(2,1) Q(2,2) | | z |   | -Q(2,3) |
		//
		// where Q is the matrix associated with the edge.
		// To solve this system, we employ the Eigen library.
		
		//
		// Compute the matrix Q associated with the edge
		//
		// It is the average of the matrices associated 
		// with the end-points
		
		auto Q = 0.5 * (Qs[id1] + Qs[id2]);
		
		//
		// Construct the linear system
		//
		
		// System matrix
		Matrix3r A;
		A << Q[0], Q[1], Q[2], 
			Q[1], Q[4], Q[5],
			Q[2], Q[5], Q[7];
			
		// Right-hand side
		Vector3r b(-Q[3], -Q[6], -Q[8]);
		
		//
		// Solve the linear system
		//
		// Exploit QR decomposition with column pivoting
		// This choice should be a good compromise bewteen
		// performance and accuracy
		
		Vector3r x = A.colPivHouseholderQr().solve(b);
		
		//
		// Check if the solution exists and return
		//
		// As suggested on the Eigen wiki, to know if the
		// solution exists one may compute the relative
		// a posteriori error and check it is below an
		// user-defined tolerance
		
		auto err = (A*x - b).norm() / b.norm();
		point3d P1(this->oprtr->getPointerToMesh()->getNode(id1));
		point3d P2(this->oprtr->getPointerToMesh()->getNode(id2));
		if ((err < TOLL) && 
			(((P1[0] < x(0)) && (x(0) < P2[0])) || ((P2[0] < x(0)) && (x(0) < P1[0]))) &&
			(((P1[1] < x(1)) && (x(1) < P2[1])) || ((P2[1] < x(1)) && (x(1) < P1[1]))) &&
			(((P1[2] < x(2)) && (x(2) < P2[2])) || ((P2[2] < x(2)) && (x(2) < P1[2]))))
			return make_pair<bool,point>(true, {x(0), x(1), x(2)});
		return make_pair<bool,point>(false, {0.,0.,0.});
	}
	
	
	vector<point> DataGeo::imp_getPointsList(const UInt & id1, const UInt & id2) const
	{
		// The collapsing point for the edge (P,Q) is searched
		// among the following alternatives:
		//	- P
		//	- Q
		//	- (P+Q)/2
		//	- optimum point
		// However, the list may be smaller due to the non-existence 
		// of the optimum point and/or boundary flags of P and Q.
		// Indeed, points on the boundaries can be moved only along
		// the boundaries, while triple points cannot be move at all.
		// This to preserve the initial shape of the domain.
		
		// Extract the end-points and their boundary flags
		point P(this->oprtr->getPointerToMesh()->getNode(id1));
		auto bP = P.getBoundary();
		point Q(this->oprtr->getPointerToMesh()->getNode(id2));
		auto bQ = Q.getBoundary();
		
		//
		// Both points are internal or on boundary
		//
		// In this case, all alternatives are potentially valid
		
		if (((bP == 0) && (bQ == 0)) || ((bP == 1) && (bQ == 1)))
		{
			auto ans = getOptimumPoint(id1,id2);
			if (ans.first)
				return {P, Q, 0.5*(P+Q), ans.second};
			return {P, Q, 0.5*(P+Q)};
		}
		
		// 
		// Unique valid alternative is the first end-point
		//
		
		if (((bP == 1) && (bQ == 0)) || ((bP == 2) && (bQ != 2)))
			return {P};
			
		// 
		// Unique valid alternative is the second end-point
		//
		
		if (((bP == 0) && (bQ == 1)) || ((bP != 2) && (bQ == 2)))
			return {Q};
			
		//
		// Remaining scenario: both end-points are triple points
		//
		// In this case, no alternative is valid
		
		return {};
	}
	
	
	Real DataGeo::imp_getCost(const UInt & id1, const UInt & id2, const point3d & p,
		const vector<UInt> & toKeep, const vector<UInt> & toMove) const
	{
		//
		// Compute geometric cost function
		//
		
		// Extract the matrix Q associated to the edge
		auto Q = 0.5 * (Qs[id1] + Qs[id2]);
		
		// Compute the quadratic form
		Real geo = Q[0]*p[0]*p[0] + Q[4]*p[1]*p[1] + Q[7]*p[2]*p[2]
			+ 2*Q[1]*p[0]*p[1] + 2*Q[2]*p[0]*p[2] + 2*Q[5]*p[1]*p[2]
			+ 2*Q[3]*p[0] + 2*Q[6]*p[1] + 2*Q[8]*p[2] + Q[9];
			
		//
		// Compute data displacement cost function
		//
		// Get the maximum distance between the original data point
		// and its current position for involved data points
		
		Real disp(numeric_limits<Real>::lowest());
		for (auto datum : toMove)
		{
			point3d dataProjected(this->oprtr->getPointerToMesh()->getData(datum));
			Real dl = (dataProjected - dataOrigin[datum]).norm2();
			if (dl > disp)	
				disp = dl;
		}
		
		//
		// Compute data distribution cost function
		//
		
		Real equi(0.);
		for (auto elem : toKeep)
		{
			Real qoi_new(this->oprtr->getQuantityOfInformation(elem));
			equi += (qoi_new - qoi_mean)*(qoi_new - qoi_mean);
		}
		
		// Average over all involved elements
		equi /= toKeep.size();
		
		//
		// Final cost
		//
		
		return (weight[0] * geo / maxCost[0] + weight[1] * disp / maxCost[1]
			+ weight[2] * equi / maxCost[2]);
	}
}


