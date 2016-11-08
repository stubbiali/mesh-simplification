#include "undirectedGraphItem.hpp"
#include "graphItem.hpp"

namespace geometry
{
	//
	// Constructors
	//
	
	undirectedGraphItem::undirectedGraphItem(const UInt & ID) : id(ID), active(true) {}
	
	
	undirectedGraphItem::undirectedGraphItem(const vector<UInt> & c, const UInt & ID) :
		id(ID), conn(c.cbegin(), c.cend()), active(true) {}
		
	
	undirectedGraphItem::undirectedGraphItem(const set<UInt> & c, const UInt & ID) :
		id(ID), conn(c.cbegin(), c.cend()), active(true) {}
		
		
	undirectedGraphItem::undirectedGraphItem(const graphItem & g) :
		id(g.id), conn(g.conn.cbegin(), g.conn.cend()), active(g.active) {}
		
		
	//
	// Operators
	//
	
	undirectedGraphItem & undirectedGraphItem::operator=(const undirectedGraphItem & g)
	{
		// Copy Id
		id = g.id;
		
		// Copy connected elements
		conn.clear();
		copy(g.conn.cbegin(), g.conn.cend(), inserter(conn, conn.end()));
		
		// Copy active flag
		active = g.active;
		
		return *this;
	}
		
	
	ostream & operator<<(ostream & out, const undirectedGraphItem & g)
	{
		if (g.active)
		{
			out << "Element Id: " << g.id << endl;
			out << "Connected Id's: ";
			for (auto el : g.conn)
				out << el << " ";
			out << endl;
		}
		else
			out << "Element " << g.id << " is inactive." << endl;
			
		out << endl;
		return out;
	}
	
	
	//
	// Set methods
	//
	
	void undirectedGraphItem::setConnected(const vector<UInt> & v)
	{
		conn.clear();
		copy(v.cbegin(), v.cend(), inserter(conn, conn.end()));
	}
	
	
	//
	// Find, insert, replace and erase methods
	//
	
	pair<set<UInt>::iterator,bool> undirectedGraphItem::find(const UInt & val)
	{
		auto it = std::find(conn.cbegin(), conn.cend(), val);
		return make_pair(it, it != conn.cend());
	}
	
	
	void undirectedGraphItem::replace(const UInt & oldId, const UInt & newId)
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
	
	set<UInt> set_intersection(const undirectedGraphItem & g1, const undirectedGraphItem & g2)
	{
		set<UInt> res;
		set_intersection(g1.conn.cbegin(), g1.conn.cend(), g2.conn.cbegin(), g2.conn.cend(), inserter(res, res.end()));
		return res;
	}
	
	
	set<UInt> set_union(const undirectedGraphItem & g1, const undirectedGraphItem & g2)
	{
		set<UInt> res;
		set_union(g1.conn.cbegin(), g1.conn.cend(), g2.conn.cbegin(), g2.conn.cend(), inserter(res, res.end()));
		return res;
	}
	
	
	set<UInt> set_difference(const undirectedGraphItem & g1, const undirectedGraphItem & g2)
	{
		set<UInt> res;
		set_difference(g1.conn.cbegin(), g1.conn.cend(), g2.conn.cbegin(), g2.conn.cend(), inserter(res, res.end()));
		return res;
	}
	
	
	set<UInt> set_symmetric_difference(const undirectedGraphItem & g1, const undirectedGraphItem & g2)
	{
		set<UInt> res;
		set_symmetric_difference(g1.conn.cbegin(), g1.conn.cend(), g2.conn.cbegin(), g2.conn.cend(), inserter(res, res.end()));
		return res;
	}
}






