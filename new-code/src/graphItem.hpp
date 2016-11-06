/*! \file graphItem.hpp
	\brief A class representing the element of a generic graph. */
	
#ifndef HH_GRAPHITEM_HH
#define HH_GRAPHITEM_HH

#include <iostream>
#include <vector>
#include <utility>

#include "shapes.hpp"

namespace geometry
{
	using namespace std;
	
	/*! This class represents an element of a generic graph.
		The element is characterized by an Id and the Id's of the connected
		elements are stored in a STL vector. In this way, the order in which
		the connected elements are inserted matters. 
		Every operation is carefully performed not to give rise to 
		duplicated in connected Id's vector.
		
		This class is useful for representing items whose connected elements
		may change throughout the code but whose number is fixed. 
		If this is not the case, the user might prefer the class undirectedGraphItem. 
		
		\sa undirectedGraphItem */
		
	class graphItem
	{
		private:
			/*! Element Id. */
			UInt id;
		
			/*! Id's of connected elements. */
			vector<UInt> conn;
		
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
			
			/*! Default constructor. */
			graphItem();
						
			/*! Constructor.
				\param N	number of connected elements 
				\param ID	element Id */
			graphItem(const UInt & N, const UInt & ID = 0);
			
			/*! Constructor.
				\param ID	element Id 
				\param c	ID's of connected elements */
			graphItem(const vector<UInt> & c, const UInt & ID = 0);
			
			/*! Synthetic copy constructor.
				\param g	another graph item */
			graphItem(const graphItem & g) = default;
			
			//
			// Operators
			//
			
			/*! Copy assignment operator.
				\param g	another graph item
				\return		updated object */
			graphItem & operator=(const graphItem & g);
			
			/*! Access operator (const version). It allows to access the connected elements.
				\param i	element position
				\return		i-th connected element Id */
			inline UInt operator[](const UInt & i) const {return conn[i];};
			
			/*! Access operator (non const version). It allows to access the connected elements.
				\param i	element position
				\return		i-th connected element Id */
			inline UInt & operator[](const UInt & i) {return conn[i];};
			
			/*! Less than operator. The connected id's are compared before comparison.
				\param g1	the first graph item
				\param g2	the second graph item
				\return		bool saying whether g1 is "less than" g2 or not */
			friend bool operator<(const graphItem & g1, const graphItem & g2);
			
			/*! The inequality operator. The connected id's are compared before comparison.
				\param g1	the first graph item
				\param g2	the second graph item
				\return		bool saying whether g1 is different from g2 or not */
			friend bool operator!=(const graphItem & g1, const graphItem & g2);
		
			/*! The equality operator. The connected id's are compared before comparison.
				\param g1	the first graph item
				\param g2	the second graph item
				\return		bool saying whether g1 is equal to g2 or not */
			friend bool operator==(const graphItem & g1, const graphItem & g2);
			
			/*! Output stream operator.
				\param out	output stream
				\param g	a graph item
				\return		updated output stream */
			friend ostream & operator<<(ostream & out, const graphItem & g);
			
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
			inline vector<UInt> getConnected() const {return conn;};
			
			/*! Get active flag.
				\return		active flag */
			inline bool isActive() const {return active;};
			
			//
			// Set methods
			//
			
			/*! Set element Id.
				\param ID	the new element Id */
			inline void setId(const UInt & ID) {id = ID;};
			
			/*! Set number of connected elements.
				\param N	number of connected elements */
			inline void resize(const UInt & N) {conn.resize(N);};
			
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
				\return		pair where the first element is a bool saying
							whether the item has been found or not,
							while the second element is an iterator to the element */
			pair<bool,vector<UInt>::iterator> find(const UInt & val);
						
			/*! Append a new Id to the connected elements.
				\param val	value to append
				\return		bool saying whether the Id has been added or not */
			bool push_back(const UInt & val);
			
			/*! Replace a connected.
				\param oldId	the old Id
				\param newId	the new Id
				\return 		bool saying whether the Id has been inserted or not */
			bool replace(const UInt & oldId, const UInt & newId);
									
			/*! Erase a connected element.
				\param val	Id to erase
				\return		bool saying whether the Id has been deleted or not */
			bool erase(const UInt & val);
			
			/*! Clear the vector with connected Id's. */
			inline void clear() {conn.clear();};
			
			//
			// Common and uncommon connected
			//
			
			/*! Find the connected Id's shared by two graph items.
				\param g1	first graph item
				\param g2	second graph item
				\return		vector of common Id's */
			friend vector<UInt> intersection(const graphItem & g1, const graphItem & g2);
			
			/*! Find the Id's connected to the first item but not to the second one.
				\param g1	first graph item
				\param g2	second graph item
				\return		vector of desired Id's */
			friend vector<UInt> difference(const graphItem & g1, const graphItem & g2);
			
			/*! Find the connected Id's not shared by two graph items.
				\param g1	first graph item
				\param g2	second graph item
				\return		vector of uncommon Id's */
			friend vector<UInt> symmetric_difference(const graphItem & g1, const graphItem & g2);			
	};
}

#endif
