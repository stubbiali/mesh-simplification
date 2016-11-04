/*! \file searchPoint.hpp
	\brief Class storing point information useful for structured data search. */
	
#ifndef HH_SEARCHPOINT_HH
#define HH_SEARCHPOINT_HH
	
#include "point.hpp"

namespace geometry 
{
	using namespace std;
	
	/*! Class inheriting point and storing the indices for structured data search. */
	class searchPoint final : public point
	{
		private:
			/*! Indices. */
			std::array<UInt,3> idx;
			
		public:
			//
			// Constructors
			//
			
			/*! Constructor.
				\param sw	South-West extreme of the mesh
				\param dl	array with cells sizes
				\param x	first coordinate
				\param y	second coordinate
				\param z	third coordinate
				\param ID	point id
				\param bond	boundary flag */
			searchPoint(const point & sw, const std::array<Real,3> & dl,
						const Real & x = 0.0, const Real & y = 0.0, const Real & z = 0.0, const UInt & ID = 0, const UInt & bond = 0);
			
			/*! Constructor.
				\param sw	South-West extreme of the mesh
				\param dl	array with cells sizes
				\param c	array with coordinates
				\param ID	point id
				\param bond	boundary flag */		
			searchPoint(const point & sw, const Real & dx, const Real & dy, const Real & dz,
						const array<Real,3> & c, const UInt & ID = 0, const UInt & bond = 0);
					
			//
			// Operators
			//
			
			/*! Output stream operator.
				\param out	output stream
				\param p	searchPoint to print
				\return		updated output stream */
			friend ostream & operator<<(ostream & out, const searchPoint & p);
					
			//
			// Get methods
			//
			
			/*! Get one index.
				\param i	component 
				\return		the index */
			inline UInt getIdx(UInt const & i) const {return idx[i];};
			
			/*! Get all indices.
				\return		array with the indices */
			inline std::array<UInt,3> getIdx() const {return idx;};
			
			//
			// Set methods
			//
			
			/*! Set one index.
				\param i	position
				\param val	value */
			inline void setIdx(const UInt & i, const UInt & val) {idx[i] = val;};
			
			/*! Set all indices.
				\param val	array with the indices */
			void setIdx(const std::array<UInt,3> & val);
			
		private:                         
			/*! Print to output the point data.
				\param out	the output string */
			void print(ostream & out) const;
	};
}

#endif
