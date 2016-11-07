/*! \file graphItem.hpp
	\brief A class representing the element of a generic graph. */
	
#ifndef HH_UNDIRECTEDGRAPHITEM_HH
#define HH_UNDIRECTEDGRAPHITEM_HH

#include <iostream>
#include <set>
#include <utility>
#include <algorithm>

#include "shapes.hpp"

namespace geometry
{
	using namespace std;
	
	// Forward declaration of graphItem
	class graphItem;
	
	/*! This class represents an element of a generic graph.
		The element is characterized by an Id and the Id's of the connected
		elements are stored in a STL set. In this way, the order in which
		the connected elements are inserted does not matter. 
		Connected Id's are ascendig-ordered.  
		Every operation is carefully performed not to give rise to 
		duplicated in connected Id's set.
		
		This class is useful for representing items whose connected elements
		may change throughout the code but whose order does not matter.
		If this is not the case, the user might prefer the class graphItem.  
		
		\sa graphItem */
		
	class undirectedGraphItem
	{
		private:
			/*! Element Id. */
			UInt id;
		
			/*! Id's of connected elements. */
			set<UInt> conn;
		
			/*! Flag
				<ol>
				<li> TRUE if this is an active node of the graph
				<li> FALSE if this is not an active node of the graph
				<\ol> */
			bool active;
						
		public:						
			//
			// Constructors
			//
									
			/*! (Default) constructor.
				\param ID	element Id */
			undirectedGraphItem(const UInt & ID = 0);
			
			/*! Constructor.
				\param ID	element Id 
				\param c	ID's of connected elements */
			undirectedGraphItem(const vector<UInt> & c, const UInt & ID = 0);
			
			/*! Constructor.
				\param ID	element Id 
				\param s	ID's of connected elements */
			undirectedGraphItem(const set<UInt> & c, const UInt & ID = 0);
			
			/*! Synthetic copy constructor.
				\param g	another graph item */
			undirectedGraphItem(const undirectedGraphItem & g) = default;
			
			/*! Copy construct from a graph item.
				\param g	a graph item */
			undirectedGraphItem(const graphItem & g);
			
			//
			// Operators
			//
			
			/*! Copy assignment operator.
				\param g	another undirected graph item
				\return		updated object */
			undirectedGraphItem & operator=(const undirectedGraphItem & g);
						
			/*! Less than operator. 
				\param g1	the first undirected graph item
				\param g2	the second undirected graph item
				\return		bool saying whether g1 is "less than" g2 or not */
			friend bool operator<(const undirectedGraphItem & g1, const undirectedGraphItem & g2);
			
			/*! The inequality operator. 
				\param g1	the first undirected graph item
				\param g2	the second undirected graph item
				\return		bool saying whether g1 is different from g2 or not */
			friend bool operator!=(const undirectedGraphItem & g1, const undirectedGraphItem & g2);
		
			/*! The equality operator. 
				\param g1	the first undirected graph item
				\param g2	the second undirected graph item
				\return		bool saying whether g1 is equal to g2 or not */
			friend bool operator==(const undirectedGraphItem & g1, const undirectedGraphItem & g2);
			
			/*! Output stream operator.
				\param out	output stream
				\param g	an undirected graph item
				\return		updated output stream */
			friend ostream & operator<<(ostream & out, const undirectedGraphItem & g);
			
			//
			// Get methods
			//
			
			/*! Get element Id.
				\return		element Id */
			inline UInt getId() const {return id;};
			
			/*! Get number of connected elements.
				\return 	number of connected elements */
			inline UInt size() const {return conn.size();};
			
			/*! Get a vector with the Id's of connected elements.
				\return 	Id's of connected elements */
			inline vector<UInt> getConnected() const 
			{
				return vector<UInt>(conn.cbegin(), conn.cend());
			};
			
			/*! Get active flag.
				\return		active flag */
			inline bool isActive() const {return active;};
			
			//
			// Set methods
			//
			
			/*! Set element Id.
				\param ID	the new element Id */
			inline void setId(const UInt & ID) {id = ID;};
						
			/*! Set all connected Id's.
				\param v	vector with connected Id's */
			void setConnected(const vector<UInt> & v);
			
			/*! Set active flag.
				\param flag	active flag */
			inline void setActive(const bool & flag) {active = flag;};
			
			//
			// Find, insert, replace and erase methods
			//
			
			/*! Find a connected Id.
				\param val	Id to search
				\return		TRUE if the Id has been found, FALSE otherwise */
			inline bool find(const UInt & val) const 
			{
				return std::find(conn.cbegin(), conn.cend(), val) != conn.cend();
			}
						
			/*! Find a connected Id.
				\param val	Id to search
				\return		pair where the first element is a bool saying
							whether the item has been found or not,
							while the second element is an iterator to the element */
			pair<set<UInt>::iterator,bool> find(const UInt & val);
						
			/*! Insert a new Id to the connected elements.
				\param val	value to append */
			inline void insert(const UInt & val) {conn.insert(val);};
			
			/*! Replace a connected.
				\param oldId	the old Id
				\param newId	the new Id
				
				Note: 	if oldId is not present in the list of connected Id's, 
						the method tries to insert newId anyway. */
			void replace(const UInt & oldId, const UInt & newId);
									
			/*! Erase a connected element.
				\param val	Id to erase 
				\return		number of removed elements */
			inline UInt erase(const UInt & val) {return conn.erase(val);};
			
			/*! Erase a connected element.
				\param it	iterator to the element to erase */
			inline void erase(set<UInt>::iterator it) {conn.erase(it);};
			
			/*! Clear the set with connected Id's. */
			inline void clear() {conn.clear();};
			
			//
			// Common and uncommon connected
			//
			
			/*! Find the connected Id's shared by two undirected graph items.
				\param g1	first undirected graph item
				\param g2	second undirected graph item
				\return		set of common Id's */
			friend set<UInt> set_intersection(const undirectedGraphItem & g1, const undirectedGraphItem & g2);
			
			/*! Find the connected Id's shared by at least one of two undirected graph items.
				\param g1	first undirected graph item
				\param g2	second undirected graph item
				\return		set of common and uncommon Id's */
			friend set<UInt> set_union(const undirectedGraphItem & g1, const undirectedGraphItem & g2);
			
			/*! Find the Id's connected to the first item but not to the second one.
				\param g1	first undirected graph item
				\param g2	second undirected graph item
				\return		set of desired Id's */
			friend set<UInt> set_difference(const undirectedGraphItem & g1, const undirectedGraphItem & g2);
			
			/*! Find the connected Id's not shared by two undirected graph items.
				\param g1	first undirected graph item
				\param g2	second undirected graph item
				\return		set of uncommon Id's */
			friend set<UInt> set_symmetric_difference(const undirectedGraphItem & g1, const undirectedGraphItem & g2);			
	};
}

#endif
