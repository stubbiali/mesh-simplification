/*! \file 	inline_point.hpp
	\brief 	Definitions of inlined members and friend functions of class point. */
	
#ifndef HH_INLINEPOINT_HH
#define HH_INLINEPOINT_HH
	
namespace geometry
{
	//
	// Definitions of inlined members
	//

	INLINE Real & point::operator[](const UInt & i) 
	{
		return coor[i];
	}


	INLINE Real point::operator[](const UInt & i) const 
	{
		return coor[i];
	}


	INLINE Real point::norm2() const 
	{
		return sqrt(coor[0]*coor[0] + coor[1]*coor[1] + coor[2]*coor[2]);
	}


	INLINE UInt point::getId() const 
	{
		return Id;
	}


	INLINE UInt point::getBoundary() const 
	{
		return boundary;
	}


	INLINE void point::setId(const UInt & newId) 
	{
		Id = newId;
	}


	INLINE void point::setBoundary(const UInt & newBond) 
	{
		boundary = newBond;
	}


	//
	// Definitions of inlined friend functions
	//
	
	INLINE point operator+(const point & pA, const point & pB) 
	{
		return point(pA.coor[0]+pB.coor[0], pA.coor[1]+pB.coor[1], pA.coor[2]+pB.coor[2]);
	}


	INLINE point operator-(const point & pA, const point & pB) 
	{
		return point(pA.coor[0]-pB.coor[0], pA.coor[1]-pB.coor[1], pA.coor[2]-pB.coor[2]);
	}
	
	
	INLINE point operator^(const point & pA, const point & pB) 
	{
		return  point(  pA.coor[1]*pB.coor[2] - pB.coor[1]*pA.coor[2],
						pA.coor[2]*pB.coor[0] - pB.coor[2]*pA.coor[0],
						pA.coor[0]*pB.coor[1] - pB.coor[0]*pA.coor[1]);
	}


	INLINE point operator/(const point & p, const Real & a) 
	{
		return point(p.coor[0]/a, p.coor[1]/a, p.coor[2]/a);
	}


	INLINE point operator*(const point & p, const Real & a)
	{
		return point(p.coor[0]*a, p.coor[1]*a, p.coor[2]*a);
	}


	INLINE point operator*(const Real & a, const point & p)
	{
		return point(p.coor[0]*a, p.coor[1]*a, p.coor[2]*a);
	}


	INLINE Real operator*(const point & pA, const point & pB) 
	{
		return pA.coor[0]*pB.coor[0] + pA.coor[1]*pB.coor[1] + pA.coor[2]*pB.coor[2];
	}
}

#endif
