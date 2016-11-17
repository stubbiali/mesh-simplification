/*!	\file	intersect.hpp
	\brief	A class to test if two equally-shaped elements intersect. */
	
#ifndef HH_INTERSECT_HH
#define HH_INTERSECT_HH

#include <memory>
#include <tuple>

#include "point.hpp"
#include "mesh.hpp"

namespace geometry
{
	/*! Relative position of a point with respect to a segment. */
	enum class Point2Seg {EXTERN, INTERN, ONVERTEX};
	
	/*! Relative position of a point with respect to a triangle. */
	enum class Point2Tri {EXTERN, INTERN, ONEDGE, ONVERTEX};
	
	/*! Relative position of a straight line with respect to a plane. */
	enum class Line2Plane {PARALLEL, COMPLANAR, INCIDENT};
	
	/*!	Type of intersection:
		<ol>
		<li> NONE:		the elements do not intersect;
		<li> VALID:		the elements intersect in a conformal way,
						i.e. they share a vertex or an entire edge;
		<li> UNVALID:	the elements intersect in a non-conformal way.
		<\ol> */
	enum class IntersectionType {NONE, VALID, INVALID};
			
	/*!	This is a template class testing whether two equally-shaped
		elements intersect or not. The template parameter is the 
		shape of the elements. The class provides both a static and
		a non-static interface. 
		
		At the moment, only the specialization for triangles is
		implemented. Further specializations may be added in future. 
		
		Reference: 
		O’Rourke J. "Computational geometry in C". 
		Cambridge (UK), Cambridge University Press, 1998. */
	template<typename SHAPE = Triangle>
	class intersect
	{
	};
	
	/*!	Specialization for triangles. */
	template<>
	class intersect<Triangle>
	{
		private:
			/*!	Shared pointer to a mesh. 
				We use bmesh class in the declaration so to 
				exploit polymorphism with the class mesh. */
			shared_ptr<bmesh<Triangle>> grid;
			
		public:
			/*!	A couple of typedef's to ease notation. */
			using point2d = geoPoint<2>;
			using point3d = geoPoint<3>;
			
			//
			// Constructors
			//
			
			/*!	(Default) constructor.
				\param g	shared pointer to a mesh */
			intersect(const shared_ptr<bmesh<Triangle>> & g = nullptr);
			
			/*!	Constructor.
				\param args	arguments which will be forwarded to mesh
				
				\sa mesh.hpp */
			template<typename... Args>
			intersect(Args... args);
			
			//
			// Interface
			//
			
			/*!	Check the intersection between two triangular elements.
				\param id1	Id of first triangle
				\param id2	Id of second triangle
				\return		FALSE if the triangles do not intersect or
							intersect in a conformal way, TRUE otherwise */
			bool doIntersect(const UInt & id1, const UInt & id2) const;
			
			/*!	Check the intersection between two triangles.
				Note that this is a static interface.
				
				\param A	first vertex of first triangle
				\param B	second vertex of first triangle
				\param C	third verted of first triangle
				\param D	first vertex of second triangle
				\param E	second vertex of second triangle
				\param F	third vertex of second triangle */
			static bool doIntersect(const point3d & A, const point3d & B, const point3d & C, 
				const point3d & D, const point3d & E, const point3d & F);
				
			//
			// Auxiliary methods
			//
			
			/*!	Test if a segment intersect a triangle (debug mode).
				\param Q	querying end-point of the segment
				\param R	ray end-point of the segment
				\param A	first vertex of the triangle
				\param B	second vertex of the triangle
				\param C	third vertex of the triangle 
				\return		intersection type */ 
			static IntersectionType intSegTri(const point3d & Q, const point3d & R,
				const point3d & A, const point3d & B, const point3d & C);
				
			/*!	Test if a segment intersect a triangle (release mode).
				\param Q	querying end-point of the segment
				\param R	ray end-point of the segment
				\param a	first vertex of the triangle 
							projected onto the "xy"-plane
				\param b	second vertex of the triangle
							projected onto the "xy"-plane
				\param c	third vertex of the triangle 
							projected onto the "xy"-plane
				\param N	unit normal to the triangle 
				\param D	RHS term in the equation of the plane
							determined by the triangle 
				\param x	"x"-coordinate
				\param y	"y"-coordinate
				\return		intersection type */
			static IntersectionType intSegTri(const point3d & Q, const point3d & R,
				const point2d & a, const point2d & b, const point2d & c, 
				const point3d & N, const Real & D, const UInt & x, const UInt & y);
			
			/*!	Test if a segment intersect a plane.
				\param Q	querying end-point of the segment
				\param R	ray end-point of the segment 
				\param N	unit normal to the triangle 
				\param D	RHS term in the equation of the plane
							determined by the triangle
				\return		relative position line-plane
				\return		position of the intersection point \$ p \$
							(if any) in the segment
				\return		\$ t \$ s.t. \$ p = q + t \cdot (r - q) \$ */
			static tuple<Line2Plane, Point2Seg, Real> intSegPlane(const point3d & Q, 
				const point3d & R, const point3d & N, const Real & D); 
				
			/*!	Test if two segments in the plane intersect one each other. 
				\param q1	querying end-point of first segment
				\param r1	ray end-point of first segment
				\param q2	querying end-point of second segment
				\param r2	ray end-point of second segment 
				\return		intersection type */
			static IntersectionType intSegSeg2d(const point2d & q1, const point2d & r1,
				const point2d & q2, const point2d & r2);
				
			/*!	Test if a two-dimensional point lays within a triangle. 
				\param p	the point
				\param a	first vertex of the triangle
				\param b	second vertex of the triangle
				\param c	third vertex of the triangle
				\return		relative position of the point w.r.t. the triangle */
			static Point2Tri inTri2d(const point2d & p, const point2d & a,
				const point2d & b, const point2d & c);
				
			/*!	Get the signed area of a two-dimensional triangle.
				\param a	first vertex of the triangle
				\param b	second vertex of the triangle
				\param c	third vertex of the triangle
				\return		the signed area */
			static Real getTriArea2d(const point2d & a, const point2d & b, const point2d & c);
	};
}

#endif
