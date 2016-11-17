/*!	\file	inline_geoPoint.hpp
	\brief	Specializations of some inlined friend functions of class geoPoint. */
	
#ifndef HH_INLINEGEOPOINT_HH
#define HH_INLINEGEOPOINT_HH

namespace geometry
{
	// Specialization for two-dimensional points 
	template<>
	INLINE geoPoint<2> operator+(const geoPoint<2> & gpA, const geoPoint<2> & gpB)
	{
		return geoPoint<2>(gpA.coor[0] + gpB.coor[0],
			gpA.coor[1] + gpB.coor[1]);
	}
	
	
	// Specialization for three-dimensional points 
	template<>
	INLINE geoPoint<3> operator+(const geoPoint<3> & gpA, const geoPoint<3> & gpB)
	{
		return geoPoint<3>(gpA.coor[0] + gpB.coor[0],
			gpA.coor[1] + gpB.coor[1], gpA.coor[2] + gpB.coor[2]);
	}
	
	
	// Specialization for two-dimensional points 
	template<>
	INLINE geoPoint<2> operator-(const geoPoint<2> & gpA, const geoPoint<2> & gpB)
	{
		return geoPoint<2>(gpA.coor[0] - gpB.coor[0],
			gpA.coor[1] - gpB.coor[1]);
	}
	
	
	// Specialization for three-dimensional points
	template<>
	INLINE geoPoint<3> operator-(const geoPoint<3> & gpA, const geoPoint<3> & gpB)
	{
		return geoPoint<3>(gpA.coor[0] - gpB.coor[0],
			gpA.coor[1] - gpB.coor[1], gpA.coor[2] - gpB.coor[2]);
	}
	
	
	// Specialization for two-dimensional points
	template<>
	INLINE geoPoint<3> operator^(const geoPoint<2> & gpA, const geoPoint<2> & gpB)
	{
		return geoPoint<3>(0., 0., gpA.coor[0]*gpB.coor[1] - gpB.coor[0]*gpA.coor[1]);
	}
	
	
	// Specialization for three-dimensional points
	template<>
	INLINE geoPoint<3> operator^(const geoPoint<3> & gpA, const geoPoint<3> & gpB)
	{
		return geoPoint<3>(gpA.coor[1]*gpB.coor[2] - gpB.coor[1]*gpA.coor[2],
			gpA.coor[2]*gpB.coor[0] - gpB.coor[2]*gpA.coor[0],
			gpA.coor[0]*gpB.coor[1] - gpB.coor[0]*gpA.coor[1]);
	}
	
	
	// Specialization for two-dimensional points (to avoid any for loop)
	template<>
	INLINE geoPoint<2> operator/(const geoPoint<2> & gp, const Real & a)
	{
		return geoPoint<2>(gp.coor[0]/a, gp.coor[1]/a);
	}
	
	
	// Specialization for three-dimensional points (to avoid any for loop)
	template<>
	INLINE geoPoint<3> operator/(const geoPoint<3> & gp, const Real & a)
	{
		return geoPoint<3>(gp.coor[0]/a, gp.coor[1]/a, gp.coor[1]/a);
	}
	
	
	// Specialization for two-dimensional points (to avoid any for loop)
	template<>
	INLINE geoPoint<2> operator*(const geoPoint<2> & gp, const Real & a)
	{
		return geoPoint<2>(gp.coor[0]*a, gp.coor[1]*a);
	}
	
	
	// Specialization for three-dimensional points (to avoid any for loop)
	template<>
	INLINE geoPoint<3> operator*(const geoPoint<3> & gp, const Real & a)
	{
		return geoPoint<3>(gp.coor[0]*a, gp.coor[1]*a, gp.coor[2]*a);
	}
}

#endif
