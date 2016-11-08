/*! \file  INLINE_undirectedGraphItem.hpp
	\brief Definitions of inlined members and friend functions of class undirectedGraphItem. */
	
#ifndef HH_INLINEUNDIRECTEDGRAPHITEM_HH
#define HH_INLINEUNDIRECTEDGRAPHITEM_HH

namespace geometry
{
	//
	// Definitions of inlined members
	//
	
	INLINE UInt undirectedGraphItem::getId() const 
	{
		return id;
	}
			
	
	INLINE UInt undirectedGraphItem::size() const 
	{
		return conn.size();
	}
	
	
	INLINE vector<UInt> undirectedGraphItem::getConnected() const 
	{
		return vector<UInt>(conn.cbegin(), conn.cend());
	}
	
	
	INLINE bool undirectedGraphItem::isActive() const 
	{
		return active;
	}
	
	
	INLINE void undirectedGraphItem::setId(const UInt & ID) 
	{
		id = ID;
	}
	
	
	INLINE void undirectedGraphItem::setActive(const bool & flag) 
	{
		active = flag;
	}
	
	
	INLINE bool undirectedGraphItem::find(const UInt & val) const 
	{
		return std::find(conn.cbegin(), conn.cend(), val) != conn.cend();
	}
	
	
	INLINE void undirectedGraphItem::insert(const UInt & val) 
	{
		conn.insert(val);
	}
	
	
	INLINE UInt undirectedGraphItem::erase(const UInt & val) 
	{
		return conn.erase(val);
	}
	
	
	INLINE void undirectedGraphItem::erase(set<UInt>::iterator it) 
	{
		conn.erase(it);
	}
	
	
	INLINE void undirectedGraphItem::clear() 
	{
		conn.clear();
	}
	
	
	//
	// Definitions of inlined friend functions
	//
	
	INLINE bool operator<(const undirectedGraphItem & g1, const undirectedGraphItem & g2)
	{
		return (g1.conn < g2.conn);
	}
	
	
	INLINE bool operator!=(const undirectedGraphItem & g1, const undirectedGraphItem & g2)
	{
		return (g1.conn != g2.conn);
	}
	
	
	INLINE bool operator==(const undirectedGraphItem & g1, const undirectedGraphItem & g2)
	{
		return (g1.conn == g2.conn);
	}
}

#endif
