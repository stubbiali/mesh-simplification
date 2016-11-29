/*!	\file	projection.cpp
	\brief	Implementations of members of class projection<Triangle>. */
	
#include <cmath>
#include <cstdlib>
#include <limits>

#include "gutility.hpp"
#include "projection.hpp"

namespace geometry
{
	//
	// Constructors
	//
	
	projection<Triangle>::projection(const bmesh<Triangle> & bg) :
		meshInfo<Triangle, MeshType::DATA>(bg)
	{
	}
		
		
	//
	// Static interface
	//
	
	pair<Real,point3d> projection<Triangle>::project(const point3d & P, 
		const point3d & A, const point3d & B, const point3d & C)
	{
		//
		// Preliminary computations
		//
		
		// Compute normal to the plane defined by the triangle
		// and (signed) distance from the origin
		auto N = ((B - A)^(C - B)).normalize();
		auto D = N*A;
		
		// Get the "xy"-plane
		auto z = N.getMaxCoor();
		auto x = (z+1) % 3;
		auto y = (z+2) % 3;
		
		//
		// Project onto the plane defined by the triangle
		//
		// Let $Q$ be the projected point, it belongs to the plane, i.e.
		// $N \cdot Q = D$. Then, introducing $t \in \mathcal{R}$ such that
		// $Q = P + t N$, we have $(P + t N) \cdot N = D$.
		// After few calculations, one gets $t = D - P \cdot N$, hence
		// $Q = P + (D - P \cdot N) N$.
		
		auto t = D - P*N;
		auto Q = P + t*N;
		
		//
		// Check if the projection falls within the triangle 
		//
		
		// Project all points onto the "xy"-plane
		point2d q(Q[x],Q[y]);
		point2d a(A[x],A[y]);
		point2d b(B[x],B[y]);
		point2d c(C[x],C[y]);
		
		// Check if q belongs to abc
		auto q_abc = gutility::inTri2d(q,a,b,c);
		if (q_abc == Point2Tri::EXTERN)
			return make_pair(-1,Q);
		else
			return make_pair(abs(t),Q);
	}
	
	
	pair<Real,point3d> projection<Triangle>::project(const point3d & P,
		const point2d & a, const point2d & b, const point2d & c,
		const point3d & N, const Real & D, const UInt & x, const UInt & y)
	{
		//
		// Project onto the plane defined by the triangle
		//
		// Let $Q$ be the projected point, it belongs to the plane, i.e.
		// $N \cdot Q = D$. Then, introducing $t \in \mathcal{R}$ such that
		// $Q = P + t N$, we have $(P + t N) \cdot N = D$.
		// After few calculations, one gets $t = D - P \cdot N$, hence
		// $Q = P + (D - P \cdot N) N$.
		
		auto t = D - P*N;
		auto Q = P + t*N;
		
		//
		// Check if the projection falls within the triangle 
		//
		
		// Project the projected point onto the "xy"-plane
		point2d q(Q[x],Q[y]);
				
		// Check if q belongs to abc
		auto q_abc = gutility::inTri2d(q,a,b,c);
		if (q_abc == Point2Tri::EXTERN)
			return make_pair(-1,Q);
		else
			return make_pair(abs(t),Q);
	}
	
	
	//
	// Auxiliary methods
	//
	
	vector<UInt> projection<Triangle>::getNewData2Elem(const point3d & Q, 
		const UInt & elemId)
	{
		// Extract the triangle and its vertices
		auto elem = this->getPointerToMesh()->getElem(elemId);
		point3d A(this->getPointerToMesh()->getNode(elem[0]));
		point3d B(this->getPointerToMesh()->getNode(elem[1]));
		point3d C(this->getPointerToMesh()->getNode(elem[2]));
		
		// Compute normal to the plane defined by the triangle
		// and the (signed) distance from the origin
		auto N = ((B - A)^(C - B)).normalize();
		auto D = N*A;
		
		// Get the "xy"-plane
		auto z = N.getMaxCoor();
		auto x = (z+1) % 3;
		auto y = (z+2) % 3;
		
		// Project all points onto the "xy"-plane
		point2d q(Q[x],Q[y]);
		point2d a(A[x],A[y]);
		point2d b(B[x],B[y]);
		point2d c(C[x],C[y]);
		
		// Compute area of the triangles qab, qbc and qca
		auto qab = gutility::getTriArea2d(q,a,b);
		auto qbc = gutility::getTriArea2d(q,b,c);
		auto qca = gutility::getTriArea2d(q,c,a);
		
		// The point is internal
		if (((qab > TOLL) && (qbc > TOLL) && (qca > TOLL)) ||
			((qab < -TOLL) && (qbc < -TOLL) && (qca < -TOLL)))
			return {elemId};
			
		// The point is on the edge AB...
		if ((-TOLL <= qab) && (qab <= TOLL)) 
		{
			// ... and the edge BC
			if ((-TOLL <= qbc) && (qbc <= TOLL)) 
				return this->connectivity.node2elem[elem[1]].getConnected();
				
			// ... and the edge CA
			if ((-TOLL <= qca) && (qca <= TOLL)) 
				return this->connectivity.node2elem[elem[0]].getConnected();
				
			return this->getElemsOnEdge(elem[0],elem[1]);
		}
		
		// The point is on the edge BC...
		if ((-TOLL <= qbc) && (qbc <= TOLL)) 
		{
			// ... and the edge CA
			if ((-TOLL <= qca) && (qca <= TOLL)) 
				return this->connectivity.node2elem[elem[2]].getConnected();
				
			return this->getElemsOnEdge(elem[1],elem[2]);
		}
		
		// Remaining scenario: the point is on the edge CA
		return this->getElemsOnEdge(elem[0],elem[2]);
	}
	
	
	vector<UInt> projection<Triangle>::getNewData2Elem(const point3d & Q, const UInt & elemId, 
		const point2d & a, const point2d & b, const point2d & c,
		const point3d & N, const Real & D, const UInt & x, const UInt & y)
	{
		// Extract the element
		auto elem = this->getPointerToMesh()->getElem(elemId);
		
		// Project the point onto the "xy"-plane
		point2d q(Q[x],Q[y]);
		
		// Compute area of the triangles pab, pbc and pca
		auto qab = gutility::getTriArea2d(q,a,b);
		auto qbc = gutility::getTriArea2d(q,b,c);
		auto qca = gutility::getTriArea2d(q,c,a);
		
		// The point is internal
		if (((qab > TOLL) && (qbc > TOLL) && (qca > TOLL)) ||
			((qab < -TOLL) && (qbc < -TOLL) && (qca < -TOLL)))
			return {elemId};
			
		// The point is on the edge AB...
		if ((-TOLL <= qab) && (qab <= TOLL)) 
		{
			// ... and the edge BC
			if ((-TOLL <= qbc) && (qbc <= TOLL)) 
				return this->connectivity.node2elem[elem[1]].getConnected();
				
			// ... and the edge CA
			if ((-TOLL <= qca) && (qca <= TOLL)) 
				return this->connectivity.node2elem[elem[0]].getConnected();
				
			return this->getElemsOnEdge(elem[0],elem[1]);
		}
		
		// The point is on the edge BC...
		if ((-TOLL <= qbc) && (qbc <= TOLL)) 
		{
			// ... and the edge CA
			if ((-TOLL <= qca) && (qca <= TOLL)) 
				return this->connectivity.node2elem[elem[2]].getConnected();
				
			return this->getElemsOnEdge(elem[1],elem[2]);
		}
		
		// Remaining scenario: the point is on the edge CA
		return this->getElemsOnEdge(elem[0],elem[2]);
	}
	
	
	//
	// Non-static interface
	//
	
	pair<point3d, vector<UInt>> projection<Triangle>::project
		(const UInt & datum, const vector<UInt> & elems)
	{
		//
		// Extract data point
		// 
		
		point3d P(this->getPointerToMesh()->getData(datum));
		
		//
		// Extract triangles
		//
		
		// Declare auxiliary vectors
		#ifndef NDEBUG
			vector<point3d> A, B, C;
		#else
			vector<point3d> N;
			vector<Real> D;
			vector<UInt> x, y;
			vector<point2d> a, b, c;
		#endif
		
		// Reserve memory
		#ifndef NDEBUG
			A.reserve(elems.size());
			B.reserve(elems.size());
			C.reserve(elems.size());
		#else
			N.reserve(elems.size());
			D.reserve(elems.size());
			x.reserve(elems.size());
			y.reserve(elems.size());
			a.reserve(elems.size());
			b.reserve(elems.size());
			c.reserve(elems.size());
		#endif
		
		// Get information on each triangle
		for (UInt i = 0; i < elems.size(); ++i)
		{
			auto elem = this->getPointerToMesh()->getElem(elems[i]);
			
			#ifndef NDEBUG
				// Extract vertices
				A.emplace_back(this->getPointerToMesh()->getNode(elem[0]));
				B.emplace_back(this->getPointerToMesh()->getNode(elem[1]));
				C.emplace_back(this->getPointerToMesh()->getNode(elem[2]));
			#else
				// Extract vertices
				point3d A(this->getPointerToMesh()->getNode(elem[0]));
				point3d B(this->getPointerToMesh()->getNode(elem[1]));
				point3d C(this->getPointerToMesh()->getNode(elem[2]));
				
				// Extract normal to the plane defined by the triangle
				// and (signed) distance from the origin
				N.emplace_back(((B - A)^(C - B)).normalize());
				D.emplace_back(N[i]*A);
			
				// Get the "xy"-plane
				auto z = N[i].getMaxCoor();
				x.emplace_back((z+1) % 3);
				y.emplace_back((z+2) % 3);
			
				// Project vertices onto the "xy"-plane
				a.emplace_back(A[x[i]],A[y[i]]);
				b.emplace_back(B[x[i]],B[y[i]]);
				c.emplace_back(C[x[i]],C[y[i]]);
			#endif
		}
		
		//
		// Project the point
		//
		
		Real dist, opt_dist(numeric_limits<Real>::max());
		point3d Q, opt_Q;
		UInt opt_id(this->getPointerToMesh()->getNumElems()), opt_i;
		
		// Loop over all triangles
		for (UInt i = 0; i < elems.size(); ++i)
		{
			// Test projection
			#ifndef NDEBUG
				tie(dist,Q) = project(P, A[i], B[i], C[i]);
			#else
				tie(dist,Q) = project(P, a[i], b[i], c[i],
					N[i], D[i], x[i], y[i]);
			#endif
			
			// If the projection falls within the triangle,
			// make sure it is the closest triangle to the point.
			// Performing the test $-TOLL < dist$ we take into
			// account the case where the point $P$ already
			// belongs to the plane defined by the i-th triangle
			if ((-TOLL < dist) && (dist < opt_dist - TOLL))
			{
				opt_dist = dist;
				opt_Q = Q;
				opt_id = elems[i];
				opt_i = i;
			}
		}
		
		// Test if the projection falls within a triangle
		// (only debug mode)
		assert(opt_id < this->getPointerToMesh()->getNumElems());
		
		//
		// Update connections
		//
		
		// Get new data-element connections
		#ifndef NDEBUG
			auto newData2Elem = getNewData2Elem(opt_Q, opt_id);
		#else
			auto newData2Elem = getNewData2Elem(opt_Q, opt_id,
				a[opt_i], b[opt_i], c[opt_i], N[opt_i], D[opt_i], x[opt_i], y[opt_i]);
		#endif
		
		// Set new data-element connections
		auto oldData2Elem = this->connectivity.setData2Elem(datum, newData2Elem);
		
		// Update data point location
		this->getPointerToMesh()->setData(datum, opt_Q);
		
		return make_pair(P, oldData2Elem);
	}
	
	
	vector<pair<point3d, vector<UInt>>> projection<Triangle>::project
		(const vector<UInt> & data, const vector<UInt> & elems)
	{
		//
		// Extract data points
		// 
		
		vector<point3d> P;
		P.reserve(data.size());
		for (auto datum : data)
			P.emplace_back(this->getPointerToMesh()->getData(datum));
		
		//
		// Extract triangles
		//
		
		// Declare auxiliary vectors
		#ifndef NDEBUG
			vector<point3d> A, B, C;
		#else
			vector<point3d> N;
			vector<Real> D;
			vector<UInt> x, y;
			vector<point2d> a, b, c;
		#endif
		
		// Reserve memory
		#ifndef NDEBUG
			A.reserve(elems.size());
			B.reserve(elems.size());
			C.reserve(elems.size());
		#else
			N.reserve(elems.size());
			D.reserve(elems.size());
			x.reserve(elems.size());
			y.reserve(elems.size());
			a.reserve(elems.size());
			b.reserve(elems.size());
			c.reserve(elems.size());
		#endif
		
		// Get information on each triangle
		for (UInt i = 0; i < elems.size(); ++i)
		{
			auto elem = this->getPointerToMesh()->getElem(elems[i]);
			
			#ifndef NDEBUG
				// Extract vertices
				A.emplace_back(this->getPointerToMesh()->getNode(elem[0]));
				B.emplace_back(this->getPointerToMesh()->getNode(elem[1]));
				C.emplace_back(this->getPointerToMesh()->getNode(elem[2]));
			#else
				// Extract vertices
				point3d A(this->getPointerToMesh()->getNode(elem[0]));
				point3d B(this->getPointerToMesh()->getNode(elem[1]));
				point3d C(this->getPointerToMesh()->getNode(elem[2]));
				
				// Extract normal to the plane defined by the triangle
				// and (signed) distance from the origin
				N.emplace_back(((B - A)^(C - B)).normalize());
				D.emplace_back(N[i]*A);
			
				// Get the "xy"-plane
				auto z = N[i].getMaxCoor();
				x.emplace_back((z+1) % 3);
				y.emplace_back((z+2) % 3);
			
				// Project vertices onto the "xy"-plane
				a.emplace_back(A[x[i]],A[y[i]]);
				b.emplace_back(B[x[i]],B[y[i]]);
				c.emplace_back(C[x[i]],C[y[i]]);
			#endif
		}
		
		//
		// Project the points
		//
		
		vector<pair<point3d, vector<UInt>>> res;
		res.reserve(data.size());
		
		// Loop over all data points
		for (UInt j = 0; j < data.size(); j++)
		{
			Real dist, opt_dist(numeric_limits<Real>::max());
			point3d Q, opt_Q;
			UInt opt_id(this->getPointerToMesh()->getNumElems()), opt_i;
			
			//
			// Projection
			//
			// For each data point, loop over all triangles
			for (UInt i = 0; i < elems.size(); ++i)
			{
				// Test projection
				#ifndef NDEBUG
					tie(dist,Q) = project(P[j], A[i], B[i], C[i]);
				#else
					tie(dist,Q) = project(P[j], a[i], b[i], c[i],
						N[i], D[i], x[i], y[i]);
				#endif
			
				// If the projection falls within the triangle,
				// make sure it is the closest triangle to the point.
				// Performing the test $-TOLL < dist$ we take into
				// account the case where the point $P$ already
				// belongs to the plane defined by the i-th triangle
				if ((-TOLL < dist) && (dist < opt_dist - TOLL))
				{
					opt_dist = dist;
					opt_Q = Q;
					opt_id = elems[i];
					opt_i = i;
				}
			}
		
			// Test if the projection falls within a triangle
			// (only debug mode)
			assert(opt_id < this->getPointerToMesh()->getNumElems());
		
			//
			// Update connections
			//
		
			// Get new data-element connections
			#ifndef NDEBUG
				auto newData2Elem = getNewData2Elem(opt_Q, opt_id);
			#else
				auto newData2Elem = getNewData2Elem(opt_Q, opt_id,
					a[opt_i], b[opt_i], c[opt_i], N[opt_i], D[opt_i], x[opt_i], y[opt_i]);
			#endif
		
			// Set new data-element connections
			auto oldData2Elem = this->connectivity.setData2Elem(data[j], newData2Elem);
		
			// Update data point location
			this->getPointerToMesh()->setData(data[j], opt_Q);
			
			// Update output
			res.emplace_back(P[j], oldData2Elem);
		}
		
		return res;
	}
	
	
	void projection<Triangle>::undo(const vector<UInt> & ids, const vector<pair<point3d, vector<UInt>>> & oldData)
	{
		for (UInt i = 0; i < ids.size(); ++i)
		{
			// Set data point location
			this->getPointerToMesh()->setData(ids[i], oldData[i].first);
			
			// Set data-element connections
			this->connectivity.setData2Elem(ids[i], oldData[i].second);
		}
	}
}
