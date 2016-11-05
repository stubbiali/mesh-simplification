/*! \file dataPoint.hpp
	\brief Declaration of a class representing a point with an associated datum. */
	
#ifndef HH_DATAPOINT_HH
#define HH_DATAPOINT_HH

#include "point.hpp"

namespace geometry
{
	using namespace std;
	
	/*! Class inheriting point and expanding it adding the datum associated to the point. */
	class dataPoint final : public point
	{
		private:
			/*! The datum. */
			Real datum;
			
		public:
			//
			// Constructors
			//
			
			/*! 
	} 
}

#endif
