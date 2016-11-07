#include "graphItem.hpp"

#include <algorithm>
#include <set>
#include <iterator>

namespace geometry
{
	using namespace std;
	
	//
	// Constructors
	//
	
	graphItem::graphItem() : id(0), conn(0), active(true) {}
	
	
	graphItem::graphItem(const UInt & N, const UInt & ID) : id(ID), conn(N), active(true) {}
	
	
	graphItem::graphItem(const vector<UInt> & c, const UInt & ID) : id(ID), conn(c), active(true) {}
	
	
	//
	// Operators
	//
	
	graphItem & graphItem::operator=(const graphItem & g)
	{
		// Copy id
		id = g.id;
		
		// Copy connected
		conn.resize(g.conn.size());
		copy(g.conn.cbegin(), g.conn.cend(), conn.begin());
		
		// Copy active flag
		active = g.active;
	}
	
	
	bool operator<(const graphItem & g1, const graphItem & g2)
	{
		// Sort connected Id's
		const set<UInt> s1(g1.conn.cbegin(), g1.conn.cend());
		const set<UInt> s2(g2.conn.cbegin(), g2.conn.cend());
		
		// Compare
		return (s1 < s2);
	}
	
	
	bool operator!=(const graphItem & g1, const graphItem & g2)
	{	
		// First, a check on sizes
		if (g1.size() != g2.size())	return true;
		
		// Sort connected Id's
		const set<UInt> s1(g1.conn.cbegin(), g1.conn.cend());
		const set<UInt> s2(g2.conn.cbegin(), g2.conn.cend());
		
		// Compare
		return (s1 != s2);
	}
	
	
	bool operator==(const graphItem & g1, const graphItem & g2)
	{	
		// First, a check on sizes
		if (g1.size() != g2.size())	return false;
		
		// Sort connected Id's
		const set<UInt> s1(g1.conn.cbegin(), g1.conn.cend());
		const set<UInt> s2(g2.conn.cbegin(), g2.conn.cend());
		
		// Compare
		return (s1 == s2);
	}
	
	
	ostream & operator<<(ostream & out, const graphItem & g)
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
	
	void graphItem::setConnected(const vector<UInt> & v)
	{
		conn.resize(v.size());
		copy(v.cbegin(), v.cend(), conn.begin());
	}
	
	
	//
	// Find, insert, replace and erase methods
	//
	
	pair<vector<UInt>::iterator,bool> graphItem::find(const UInt & val)
	{
		auto it = std::find(conn.begin(), conn.end(), val);
		return make_pair(it, it != conn.end());
	}
	
	
	bool graphItem::push_back(const UInt & val)
	{
		auto it = std::find(conn.begin(), conn.end(), val);
		if (it != conn.end())
		{
			conn.push_back(val);
			return true;
		}
		else	return false;
	}
	
	
	bool graphItem::replace(const UInt & oldId, const UInt & newId)
	{
		auto it = std::find(conn.begin(), conn.end(), oldId);
		if (it != conn.end())
		{
			*it = newId;
			return true;
		}
		else	return false;
	}
	
	
	bool graphItem::erase(const UInt & val)
	{
		auto it = std::find(conn.begin(), conn.end(), val);
		if (it != conn.end())
		{
			conn.erase(it);
			return true;
		}
		else	return false;
	}
	
	
	//
	// Common and uncommon connected
	//
	
	vector<UInt> intersection(const graphItem & g1, const graphItem & g2)
	{
		// Sort connected Id's
		const set<UInt> s1(g1.conn.cbegin(), g1.conn.cend());
		const set<UInt> s2(g2.conn.cbegin(), g2.conn.cend());
		
		// Intersec
		vector<UInt> res;
		set_intersection(s1.cbegin(), s1.cend(), s2.cbegin(), s2.cend(), back_inserter(res));
		
		return res;
	}
	
	
	vector<UInt> difference(const graphItem & g1, const graphItem & g2)
	{
		// Sort connected Id's
		const set<UInt> s1(g1.conn.cbegin(), g1.conn.cend());
		const set<UInt> s2(g2.conn.cbegin(), g2.conn.cend());
		
		// Make difference
		vector<UInt> res;
		set_difference(s1.cbegin(), s1.cend(), s2.cbegin(), s2.cend(), back_inserter(res));
		
		return res;
	}
	
	
	vector<UInt> symmetric_difference(const graphItem & g1, const graphItem & g2)
	{
		// Sort connected Id's
		const set<UInt> s1(g1.conn.cbegin(), g1.conn.cend());
		const set<UInt> s2(g2.conn.cbegin(), g2.conn.cend());
		
		// Make symmetric difference
		vector<UInt> res;
		set_symmetric_difference(s1.cbegin(), s1.cend(), s2.cbegin(), s2.cend(), back_inserter(res));
		
		return res;
	}
}


