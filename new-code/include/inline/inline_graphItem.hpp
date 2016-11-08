/*! \file  inline_graphItem.hpp
	\brief Definitions of inlined members of class graphItem. */
	
#ifndef HH_INLINEGRAPHITEM_HH
#define HH_INLINEGRAPHITEM_HH

namespace geometry
{
	INLINE UInt graphItem::operator[](const UInt & i) const 
	{
		return conn[i];
	}
	
	
	INLINE UInt & graphItem::operator[](const UInt & i) 
	{
		return conn[i];
	}
	
	
	INLINE graphItem::operator undirectedGraphItem() const 
	{
		return undirectedGraphItem(*this);
	}
	
	
	INLINE UInt graphItem::getId() const 
	{
		return id;
	}
			
	
	INLINE UInt graphItem::size() const 
	{
		return conn.size();
	}
	
	
	INLINE vector<UInt> graphItem::getConnected() const 
	{
		return conn;
	}
	
	
	INLINE bool graphItem::isActive() const 
	{
		return active;
	}


	INLINE void graphItem::setId(const UInt & ID) 
	{	
		id = ID;
	}
			
			
	INLINE void graphItem::resize(const UInt & N) 
	{
		conn.resize(N);
	}
	
	
	INLINE void graphItem::setActive(const bool & flag) 
	{
		active = flag;
	}	
	
	
	INLINE void graphItem::erase(vector<UInt>::iterator it) 
	{
		conn.erase(it);
	}
	
	
	INLINE void graphItem::clear() 
	{
		conn.clear();
	}		
}

#endif
