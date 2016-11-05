#include "dataPoint.hpp"

namespace geometry
{
	using namespace std;
	
	//
	// Constructors
	//
	
	dataPoint::dataPoint(const array<Real,3> & c, const UInt & ID, const UInt & dat) : point(c,ID), datum(dat) {}
	
	
	dataPoint::dataPoint(const point & p, Real const & dat) : point(p), datum(dat) {}
	
	
	//
	// Operators
	//
	
	dataPoint & dataPoint::operator=(const dataPoint & p)
	{
		// Copy coordinates
		point::operator=(p);
		
		// Copy datum
		datum = p.datum;
		
		return *this;
	}
	
	
	//
	// Print
	//
	
	void dataPoint::print(ostream & out) const
	{
		out << "Point ID: " << id << endl;
		
		out << "Point coordinates: x = " << coor[0]
			<< "                   y = " << coor[1]
			<< "                   z = " << coor[2]
			<< endl;
			
		out << "Point datum: " << datum << endl;
				
		out << endl;
	}
}


