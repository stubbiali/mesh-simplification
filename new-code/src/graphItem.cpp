#include "graphItem.hpp"

namespace geometry
{
	//
	// Constructors
	//
	
	graphItem::graphItem(const UInt & ID) : 
		Id(ID), active(true) 
	{
	}
	
	
	graphItem::graphItem(const vector<UInt> & c, const UInt & ID) :
		Id(ID), conn(c.cbegin(), c.cend()), active(true) 
	{
	}
		
	
	graphItem::graphItem(const set<UInt> & c, const UInt & ID) :
		Id(ID), conn(c.cbegin(), c.cend()), active(true) 
	{
	}
		
		
	//
	// Operators
	//
	
	graphItem & graphItem::operator=(const graphItem & g)
	{
		// Copy Id
		Id = g.Id;
		
		// Copy connected elements
		conn.clear();
		copy(g.conn.cbegin(), g.conn.cend(), inserter(conn, conn.end()));
		
		// Copy active flag
		active = g.active;
		
		return *this;
	}
		
	
	ostream & operator<<(ostream & out, const graphItem & g)
	{
		if (g.active)
		{
			out << "Element Id: " << g.Id << endl;
			out << "Connected Id's: ";
			for (auto el : g.conn)
				out << el << " ";
			out << endl;
		}
		else
			out << "Element " << g.Id << " is inactive." << endl;
			
		out << endl;
		return out;
	}
	
	
	//
	// Set methods
	//
	
	void graphItem::setConnected(const vector<UInt> & v)
	{
		conn.clear();
		copy(v.cbegin(), v.cend(), inserter(conn, conn.end()));
	}
	
	
	//
	// Find, insert, replace and erase methods
	//
	
	pair<set<UInt>::iterator,bool> graphItem::find(const UInt & val)
	{
		auto it = std::find(conn.cbegin(), conn.cend(), val);
		return make_pair(it, it != conn.cend());
	}
	
	
	void graphItem::replace(const UInt & oldId, const UInt & newId)
	{
		#ifndef NDEBUG
		if (oldId == newId)
			cerr << "Warning: oldId (" << oldId << ") coincides with newId." << endl;
		#endif
		
		conn.erase(oldId);
		conn.insert(newId);
	}
	
	
	//
	// Common and uncommon connected
	//
	
	set<UInt> set_intersection(const graphItem & g1, const graphItem & g2)
	{
		set<UInt> res;
		set_intersection(g1.conn.cbegin(), g1.conn.cend(), g2.conn.cbegin(), g2.conn.cend(), inserter(res, res.end()));
		return res;
	}
	
	
	set<UInt> set_union(const graphItem & g1, const graphItem & g2)
	{
		set<UInt> res;
		set_union(g1.conn.cbegin(), g1.conn.cend(), g2.conn.cbegin(), g2.conn.cend(), inserter(res, res.end()));
		return res;
	}
	
	
	set<UInt> set_difference(const graphItem & g1, const graphItem & g2)
	{
		set<UInt> res;
		set_difference(g1.conn.cbegin(), g1.conn.cend(), g2.conn.cbegin(), g2.conn.cend(), inserter(res, res.end()));
		return res;
	}
	
	
	set<UInt> set_symmetric_difference(const graphItem & g1, const graphItem & g2)
	{
		set<UInt> res;
		set_symmetric_difference(g1.conn.cbegin(), g1.conn.cend(), g2.conn.cbegin(), g2.conn.cend(), inserter(res, res.end()));
		return res;
	}
}





