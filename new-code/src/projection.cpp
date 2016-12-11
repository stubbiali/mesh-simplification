/*!	\file	projection.cpp
	\brief	Implementations of members of class projection<Triangle>. */
	
#include <cmath>
#include <cstdlib>
#include <limits>

#include "gutility.hpp"
#include "projection.hpp"

// Include implementations of inlined class members
#ifndef NDEBUG
#include "inline/inline_projection.hpp"
#endif

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
		UInt x = (z+1) % 3;
		UInt y = (z+2) % 3;
					
		//
		// Project the point onto the plane defined by the triangle
		//
		// Let $Q$ be the projected point, it belongs to the plane, i.e.
		// $N \cdot Q = D$. Then, introducing $t \in \mathcal{R}$ such that
		// $Q = P + t N$, we have $(P + t N) \cdot N = D$.
		// After few calculations, one gets $t = D - P \cdot N$, hence
		// $Q = P + (D - P \cdot N) N$.
		
		auto t = D - P*N;
		auto Q = P + t*N;
				
		//
		// Project all points onto the "xy"-plane
		//
		
		point2d q(Q[x],Q[y]);
		point2d a(A[x],A[y]);
		point2d b(B[x],B[y]);
		point2d c(C[x],C[y]);
		
		//
		// Check if the projection falls within the triangle
		//
		// If so, compute the square distance and return
		
		auto q_abc = gutility::inTri2d(q, a, b, c);
		if (q_abc != Point2Tri::EXTERN)
			return make_pair((P - Q)*(P - Q), Q);
			
		//
		// The projection is outside the triangle and the element of the
		// triangle closest to the projection is an edge
		//
		// For each edge, test if the orthogonal projection of the point
		// onto the line of edge falls within the edge itself.
		// To compute the projection $Q'$ of a point $Q$ onto the line
		// $A B$, first note that $Q'$ can be expressed as $Q' = A + t (B - A)$,
		// with $t \in \mathcal{R}$. Then, imposing the orthogonality between
		// $Q Q'$ and $A B$, one gets 
		// $t = \dfrac{(Q - A) \cdot (B - A)}{(B - A) \cdot (B - A)}$.
		// To check if $Q'$ actually falls between $A$ and $B$, check that $A$
		// is on the left (right) of $Q Q'$, while $B$ is on the right (left) of $Q Q'$. 
		
		Real opt_dist(numeric_limits<Real>::max());
		point3d opt_Qp;
		
		//
		// Edge AB
		//
		
		{
			// Project Q onto the line AB
			auto t = (Q - A) * (B - A) / ((B - A)*(B - A));
			auto Qp = A + t * (B - A);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within a and b...
			auto qqpa = gutility::getTriArea2d(q, qp, a);
			auto qqpb = gutility::getTriArea2d(q, qp, b);
			
			// ... and if so, update optimal variables
			if (qqpa*qqpb < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
				}
			}
		}
		
		//
		// Edge BC
		//
		
		{
			// Project Q onto the line BC
			auto t = (Q - B) * (C - B) / ((C - B)*(C - B));
			auto Qp = B + t * (C - B);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within b and c...
			auto qqpb = gutility::getTriArea2d(q, qp, b);
			auto qqpc = gutility::getTriArea2d(q, qp, c);
			
			// ... and if so, update optimal variables
			if (qqpb*qqpc < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
				}
			}
		}
		
		//
		// Edge CA
		//
		
		{
			// Project Q onto the line AB
			auto t = (Q - C) * (A - C) / ((A - C)*(A - C));
			auto Qp = C + t * (A - C);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within c and a...
			auto qqpc = gutility::getTriArea2d(q, qp, c);
			auto qqpa = gutility::getTriArea2d(q, qp, a);
			
			// ... and if so, update optimal variables
			if (qqpc*qqpa < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
				}
			}
		}
					
		//
		// The projection is outside the triangle and the point of the
		// triangle closest to the point is a vertex
		//
		
		// Vertex A
		Real qa = (q[0]-a[0])*(q[0]-a[0]) + (q[1]-a[1])*(q[1]-a[1]);
		if (qa < opt_dist)
		{
			opt_dist = qa;
			opt_Qp = A;
		}
		
		// Vertex B
		Real qb = (q[0]-b[0])*(q[0]-b[0]) + (q[1]-b[1])*(q[1]-b[1]);
		if (qb < opt_dist)
		{
			opt_dist = qb;
			opt_Qp = B;
		}
			
		// Vertex C
		Real qc = (q[0]-c[0])*(q[0]-c[0]) + (q[1]-c[1])*(q[1]-c[1]);
		if (qc < opt_dist)
		{
			opt_dist = qc;
			opt_Qp = C;
		}
		
		return make_pair(opt_dist, opt_Qp);
	}
	
	
	pair<Real,point3d> projection<Triangle>::project(const point3d & P, 
		const point3d & A, const point3d & B, const point3d & C,
		const point3d & N, const Real & D, const UInt & x, const UInt & y)
	{		
		//
		// Project the point onto the plane defined by the triangle
		//
		// Let $Q$ be the projected point, it belongs to the plane, i.e.
		// $N \cdot Q = D$. Then, introducing $t \in \mathcal{R}$ such that
		// $Q = P + t N$, we have $(P + t N) \cdot N = D$.
		// After few calculations, one gets $t = D - P \cdot N$, hence
		// $Q = P + (D - P \cdot N) N$.
		
		auto t = D - P*N;
		auto Q = P + t*N;
				
		//
		// Project the projection onto the "xy"-plane
		//
		
		point2d q(Q[x],Q[y]);
		point2d a(A[x],A[y]);
		point2d b(B[x],B[y]);
		point2d c(C[x],C[y]);
				
		//
		// Check if the projection falls within the triangle
		//
		// If so, compute the square distance and return
		
		auto q_abc = gutility::inTri2d(q, a, b, c);
		if (q_abc != Point2Tri::EXTERN)
			return make_pair((P - Q)*(P - Q), Q);
			
		//
		// The projection is outside the triangle and the element of the
		// triangle closest to the projection is an edge
		//
		// For each edge, test if the orthogonal projection of the point
		// onto the line of edge falls within the edge itself.
		// To compute the projection $Q'$ of a point $Q$ onto the line
		// $A B$, first note that $Q'$ can be expressed as $Q' = A + t (B - A)$,
		// with $t \in \mathcal{R}$. Then, imposing the orthogonality between
		// $Q Q'$ and $A B$, one gets 
		// $t = \dfrac{(Q - A) \cdot (B - A)}{(B - A) \cdot (B - A)}$.
		// To check if $Q'$ actually falls between $A$ and $B$, check that $A$
		// is on the left (right) of $Q Q'$, while $B$ is on the right (left) of $Q Q'$. 
		
		Real opt_dist(numeric_limits<Real>::max());
		point3d opt_Qp;
		
		//
		// Edge AB
		//
		
		{
			// Project Q onto the line AB
			auto t = (Q - A) * (B - A) / ((B - A)*(B - A));
			auto Qp = A + t * (B - A);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within a and b...
			auto qqpa = gutility::getTriArea2d(q, qp, a);
			auto qqpb = gutility::getTriArea2d(q, qp, b);
			
			// ... and if so, update optimal variables
			if (qqpa*qqpb < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
				}
			}
		}
		
		//
		// Edge BC
		//
		
		{
			// Project Q onto the line BC
			auto t = (Q - B) * (C - B) / ((C - B)*(C - B));
			auto Qp = B + t * (C - B);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within b and c...
			auto qqpb = gutility::getTriArea2d(q, qp, b);
			auto qqpc = gutility::getTriArea2d(q, qp, c);
			
			// ... and if so, update optimal variables
			if (qqpb*qqpc < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
				}
			}
		}
		
		//
		// Edge CA
		//
		
		{
			// Project Q onto the line AB
			auto t = (Q - C) * (A - C) / ((A - C)*(A - C));
			auto Qp = C + t * (A - C);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within c and a...
			auto qqpc = gutility::getTriArea2d(q, qp, c);
			auto qqpa = gutility::getTriArea2d(q, qp, a);
			
			// ... and if so, update optimal variables
			if (qqpc*qqpa < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
				}
			}
		}
			
		//
		// The projection is outside the triangle and the point of the
		// triangle closest to the point is a vertex
		//
		
		// Vertex A
		Real qa = (q[0]-a[0])*(q[0]-a[0]) + (q[1]-a[1])*(q[1]-a[1]);
		if (qa < opt_dist - TOLL)
		{
			opt_dist = qa;
			opt_Qp = A;
		}
		
		// Vertex B
		Real qb = (q[0]-b[0])*(q[0]-b[0]) + (q[1]-b[1])*(q[1]-b[1]);
		if (qb < opt_dist - TOLL)
		{
			opt_dist = qb;
			opt_Qp = B;
		}
			
		// Vertex C
		Real qc = (q[0]-c[0])*(q[0]-c[0]) + (q[1]-c[1])*(q[1]-c[1]);
		if (qc < opt_dist - TOLL)
		{
			opt_dist = qc;
			opt_Qp = C;
		}
		
		return make_pair(opt_dist, opt_Qp);
	}
		
	
	//
	// Auxiliary methods
	//
	
	vector<UInt> projection<Triangle>::getNewData2Elem(const point3d & Q, 
		const UInt & Id)
	{
		//
		// Preliminary computations
		//
		
		// Extract the triangle and its vertices
		auto elem = this->getCPointerToMesh()->getElem(Id);
		point3d A(this->getCPointerToMesh()->getNode(elem[0]));
		point3d B(this->getCPointerToMesh()->getNode(elem[1]));
		point3d C(this->getCPointerToMesh()->getNode(elem[2]));
		
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
		
		//
		// Compute area of the triangles qab, qbc and qca
		//
		
		auto qab = gutility::getTriArea2d(q,a,b);
		auto qbc = gutility::getTriArea2d(q,b,c);
		auto qca = gutility::getTriArea2d(q,c,a);
				
		//
		// The point is internal
		//
		// This happens if and only if the signed areas are concorde
		
		if (((qab > TOLL) && (qbc > TOLL) && (qca > TOLL)) ||
			((qab < -TOLL) && (qbc < -TOLL) && (qca < -TOLL)))
			return {Id};
			
		//
		// The point is on the boundary
		//
		// If only one signed area vanishes, then the point lies on the
		// related edge; if two areas vanish, the point coincides with
		// the vertex shared by the two edges.
			
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
		
		// The point is on the edge CA
		if ((-TOLL <= qca) && (qca <= TOLL)) 
			return this->getElemsOnEdge(elem[0],elem[2]);
		
		//
		// Floating point exception
		//
		// Due to floating point arithmetic, the point turns out to
		// be outside the triangle, i.e. all signed areas are not
		// vanishing and discorde. In this case, act as the point
		// was internal.
		
		return {Id};
	}
	
	
	vector<UInt> projection<Triangle>::getNewData2Elem(const point3d & Q, 
		const UInt & Id, const UInt & x, const UInt & y)
	{
		//
		// Preliminary computations
		//
		
		// Extract the triangle and its vertices
		auto elem = this->getCPointerToMesh()->getElem(Id);
		point3d A(this->getCPointerToMesh()->getNode(elem[0]));
		point3d B(this->getCPointerToMesh()->getNode(elem[1]));
		point3d C(this->getCPointerToMesh()->getNode(elem[2]));
				
		// Project all points onto the "xy"-plane
		point2d q(Q[x],Q[y]);
		point2d a(A[x],A[y]);
		point2d b(B[x],B[y]);
		point2d c(C[x],C[y]);
		
		//
		// Compute area of the triangles qab, qbc and qca
		//
		
		auto qab = gutility::getTriArea2d(q,a,b);
		auto qbc = gutility::getTriArea2d(q,b,c);
		auto qca = gutility::getTriArea2d(q,c,a);
				
		//
		// The point is internal
		//
		// This happens if and only if the signed areas are concorde
		
		if (((qab > TOLL) && (qbc > TOLL) && (qca > TOLL)) ||
			((qab < -TOLL) && (qbc < -TOLL) && (qca < -TOLL)))
			return {Id};
			
		//
		// The point is on the boundary
		//
		// If only one signed area vanishes, then the point lies on the
		// related edge; if two areas vanish, the point coincides with
		// the vertex shared by the two edges.
			
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
		
		// The point is on the edge CA
		if ((-TOLL <= qca) && (qca <= TOLL)) 
			return this->getElemsOnEdge(elem[0],elem[2]);
		
		//
		// Floating point exception
		//
		// Due to floating point arithmetic, the point turns out to
		// be outside the triangle, i.e. all signed areas are not
		// vanishing and discorde. In this case, act as the point
		// was internal.
		
		return {Id};
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
		
		point3d P(this->getCPointerToMesh()->getData(datum));
		
		//
		// Extract triangles
		//
		
		// Declare auxiliary vectors
		vector<point3d> A, B, C;
		#ifdef NDEBUG
			vector<point3d> N;
			vector<Real> D;
			vector<UInt> x, y;
		#endif
		
		// Reserve memory
		A.reserve(elems.size());
		B.reserve(elems.size());
		C.reserve(elems.size());
		#ifdef NDEBUG
			N.reserve(elems.size());
			D.reserve(elems.size());
			x.reserve(elems.size());
			y.reserve(elems.size());
		#endif
		
		// Get information on each triangle
		for (UInt i = 0; i < elems.size(); ++i)
		{
			auto elem = this->getCPointerToMesh()->getElem(elems[i]);
			
			// Extract vertices
			A.emplace_back(this->getCPointerToMesh()->getNode(elem[0]));
			B.emplace_back(this->getCPointerToMesh()->getNode(elem[1]));
			C.emplace_back(this->getCPointerToMesh()->getNode(elem[2]));
			#ifdef NDEBUG
				// Extract normal to the plane defined by the triangle
				// and (signed) distance from the origin
				N.emplace_back(((B[i] - A[i])^(C[i] - B[i])).normalize());
				D.emplace_back(N[i]*A[i]);
				
				// Get the "xy"-plane
				auto z = N[i].getMaxCoor();
				x.emplace_back((z+1) % 3);
				y.emplace_back((z+2) % 3);
			#endif
		}
		
		//
		// Project the point
		//
		
		Real dist, opt_dist(numeric_limits<Real>::max());
		point3d Q, opt_Q;
		UInt opt_id(MAX_NUM_ELEMS), opt_i;
		
		// Loop over all triangles
		for (UInt i = 0; i < elems.size(); ++i)
		{
			// Test projection
			#ifndef NDEBUG
				tie(dist,Q) = project(P, A[i], B[i], C[i]);
			#else
				tie(dist,Q) = project(P,  A[i], B[i], C[i], N[i], D[i], x[i], y[i]);
			#endif
			
			// If the projection falls within the triangle,
			// make sure it is the closest triangle to the point.
			// Performing the test $-TOLL < dist$ we take into
			// account the case where the point $P$ already
			// belongs to the plane defined by the i-th triangle
			if (dist < opt_dist - TOLL)
			{
				opt_dist = dist;
				opt_Q = Q;
				opt_id = elems[i];
				opt_i = i;
			}
		}
		
		// Test if the projection falls within a triangle
		// (only debug mode)
		assert(opt_id < MAX_NUM_ELEMS);
		
		//
		// Update connections
		//
		
		// Get new data-element connections
		#ifndef NDEBUG
			auto newData2Elem = getNewData2Elem(opt_Q, opt_id);
		#else
			auto newData2Elem = getNewData2Elem(opt_Q, opt_id, x[opt_i], y[opt_i]);
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
			P.emplace_back(this->getCPointerToMesh()->getData(datum));
		
		//
		// Extract triangles
		//
		
		// Declare auxiliary vectors
		vector<point3d> A, B, C;
		#ifdef NDEBUG
			vector<point3d> N;
			vector<Real> D;
			vector<UInt> x, y;
		#endif
		
		// Reserve memory
		A.reserve(elems.size());
		B.reserve(elems.size());
		C.reserve(elems.size());
		#ifdef NDEBUG
			N.reserve(elems.size());
			D.reserve(elems.size());
			x.reserve(elems.size());
			y.reserve(elems.size());
		#endif
		
		// Get information on each triangle
		for (UInt i = 0; i < elems.size(); ++i)
		{
			auto elem = this->getCPointerToMesh()->getElem(elems[i]);
			
			// Extract vertices
			A.emplace_back(this->getCPointerToMesh()->getNode(elem[0]));
			B.emplace_back(this->getCPointerToMesh()->getNode(elem[1]));
			C.emplace_back(this->getCPointerToMesh()->getNode(elem[2]));
			#ifdef NDEBUG
				// Extract normal to the plane defined by the triangle
				// and (signed) distance from the origin
				N.emplace_back(((B[i] - A[i])^(C[i] - B[i])).normalize());
				D.emplace_back(N[i]*A[i]);
				
				// Get the "xy"-plane
				auto z = N[i].getMaxCoor();
				x.emplace_back((z+1) % 3);
				y.emplace_back((z+2) % 3);
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
			UInt opt_id(MAX_NUM_ELEMS), opt_i;
			
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
					tie(dist,Q) = project(P[j], A[i], B[i], C[i], N[i], D[i], x[i], y[i]);
				#endif
			
				// If the projection falls within the triangle,
				// make sure it is the closest triangle to the point.
				// Performing the test $-TOLL < dist$ we take into
				// account the case where the point $P$ already
				// belongs to the plane defined by the i-th triangle
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Q = Q;
					opt_id = elems[i];
					opt_i = i;
				}
			}
		
			// Test if the projection falls within a triangle
			// (only debug mode)
			assert(opt_id < MAX_NUM_ELEMS);
		
			//
			// Update connections
			//
		
			#ifndef NDEBUG
				auto newData2Elem = getNewData2Elem(opt_Q, opt_id);
			#else
				auto newData2Elem = getNewData2Elem(opt_Q, opt_id, x[opt_i], y[opt_i]);
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
