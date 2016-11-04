#include "searchPoint.hpp"

namespace geometry 
{
	using namespace std;
	
	//
	// Constructors
	//
	
	searchPoint::searchPoint(const point & sw, const array<Real,3> & dl,
							 const Real & x, const Real & y, const Real & z, const UInt & ID, const UInt & bond) :
							 point(x,y,z,ID,bond)
	{
		for (size_t i = 0; i < 3; i++)
			idx[i] = static_cast<UInt>((coor[i] - sw.coor[i]) / dl[i]);
	}
	
	//
	// Print
	//
	
	void searchPoint::print(ostream & out) const
	{
		out << "Point ID: " << id << endl;
		
		out << "Point coordinates: x = " << coor[0]
			<< "                   y = " << coor[1]
			<< "                   z = " << coor[2]
			<< endl;
			
		switch(boundary)
		{
			case 0:
				out << "Internal point" << endl;
				break;
			case 1:
				out << "Boundary point" << endl;
				break;
			case 2:
				out << "Triple point" << endl;
				break;
		}
		
		out << "Point indices: " << idx[0] << ", " << idx[1] << ", " << idx[2] << endl;
				
		out << endl;
	}
}
