#include "point.hpp"

// Include definitions of inlined members and friend functions. 
#ifndef INLINED
#include "inline/inline_point.hpp"
#endif

namespace geometry 
{
	//
	// Constructors
	//
	
	point::point(const Real & x, const Real & y, const Real & z, const UInt & ID, const UInt & bond) :
		Id(ID), coor{x, y, z}, boundary(bond), active(true)
	{
	}
	

	point::point(const array<Real,3> & c, const UInt & ID, const UInt & bond) :
		Id(ID), coor(c), boundary(bond), active(true)
	{
	}
	

	//
	// Operators
	//

	// For copy-assignment operator: just consider the coordinates and the boundary flag, but not the Id.
	// This will be useful when updating the mesh after an edge contraction.
	point & point::operator=(const point & p) 
	{
		// Copy the coordinates
		coor[0] = p.coor[0];
		coor[1] = p.coor[1];
		coor[2] = p.coor[2];
		
		// Copy the boundary flag
		boundary = p.boundary;
		
		return *this;
	}
		

	bool operator<(const point & pA, const point & pB) 
	{
		for (array<UInt,3>::size_type i = 0; i < 3; i++) 
		{
			if (pA.coor[i] < (pB.coor[i] - TOLL))
				return true;

			if (pA.coor[i] > (pB.coor[i] + TOLL))
				return false;
		}

		return false;
	}


	bool operator!=(const point & pA, const point & pB) 
	{
		for (array<UInt,3>::size_type i = 0; i < 3; i++)
		{
			if ((pA.coor[i] < (pB.coor[i] - TOLL)) || (pA.coor[i] > (pB.coor[i] + TOLL)))
				return true;
		}
		
		return false;
	}


	bool operator==(const point & pA, const point & pB) 
	{
		for (array<UInt,3>::size_type i = 0; i < 3; i++)
		{
			if ((pA.coor[i] < (pB.coor[i] - TOLL)) || (pA.coor[i] > (pB.coor[i] + TOLL)))
				return false;
		}
		
		return true;
	}
			
	
	ostream & operator<<(ostream & out, const point & p)
	{
		p.print(out);
		return out;
	}
	

	//
	// Replace methods
	//
	
	void point::replace(const point & V1, const point & V2, const Real & d)
	{
		coor[0] = d*(V1.coor[0] + V2.coor[0]);
		coor[1] = d*(V1.coor[1] + V2.coor[1]);
		coor[2] = d*(V1.coor[2] + V2.coor[2]);
	}
	
	
	void point::replace(const point & V1, const point & V2, const point & V3, const Real & d)
	{
		coor[0] = d*(V1.coor[0] + V2.coor[0] + V3.coor[0]);
		coor[1] = d*(V1.coor[1] + V2.coor[1] + V3.coor[1]);
		coor[2] = d*(V1.coor[2] + V2.coor[2] + V3.coor[2]);
	}
	
	
	void point::replace(const point & V1, const point & V2, const point & V3, const point & V4, const Real & d)
	{
		coor[0] = d*(V1.coor[0] + V2.coor[0] + V3.coor[0] + V4.coor[0]);
		coor[1] = d*(V1.coor[1] + V2.coor[1] + V3.coor[1] + V4.coor[1]);
		coor[2] = d*(V1.coor[2] + V2.coor[2] + V3.coor[2] + V4.coor[2]);
	}
	
	
	// 
	// Norm methods
	//
		
	void point::normalize()
	{
		// Compute the norm 
		Real len = norm2();
		
		// Check
		#ifndef NDEBUG
			if (len < TOLL)	
				cerr << "Warning: point is close to origin" << endl;
		#endif
		
		// Change coordinates
		coor[0] /= len;
		coor[1] /= len;
		coor[2] /= len;
	}
		
	
	//
	// Set methods
	//	

	void point::setCoor(const array<Real,3> & newCoor)
	{		
		coor[0] = newCoor[0];
		coor[1] = newCoor[1];
		coor[2] = newCoor[2];
	}
		
	
	//
	// Print
	//
	
	void point::print(ostream & out) const
	{
		if (active)
		{
			out << "Point ID: " << Id << endl;
		
			out << "Point coordinates: x = " << coor[0] << endl
				<< "                   y = " << coor[1] << endl
				<< "                   z = " << coor[2] << endl;
			
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
		}
		else
			out << "Point " << Id << " is inactive." << endl;
	}
}

