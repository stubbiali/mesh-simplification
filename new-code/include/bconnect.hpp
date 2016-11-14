/*!	\file	bconnect.hpp
	\brief	A class creating the connections for a mesh. */
	
#ifndef HH_BCONNECT_HH
#define HH_BCONNECT_HH

#include <memory>
#include <unordered_set>

#include "hash.hpp"
#include "graphItem.hpp"
#include "mesh.hpp"

namespace geometry
{
	/*!	This is a base class creating the following connections for
		a standard mesh (i.e. without distributed data):
		<ol>
		<li> node-node;
		<li> node-element;
		<li> element-element.
		<\ol>
		Moreover, the set of edges is created.
		For grids with distributed data, we delegate to the derived class
		connect the creation of the following two connections:
		<ol>
		<li> data-element;
		<li> element-data.
		<\ol>
		The class stores a shared pointer to the mesh. 
		When the class is required to modify some connections,
		it supposes the mesh has been already accordingly modified.
		This class does not directly modify the mesh in any way,
		other classes (e.g. doctor) will be in charge of this.
		To make the connections updated with the mesh, call refresh().
		
		\sa connect.hpp, doctor.hpp */
		
	template<typename SHAPE, MeshType MT>
	class bconnect
	{
		public:
			/*! Number of vertices for each element. */
			static constexpr UInt NV = SHAPE::numVertices;
			
			/*! Number of edges for each element. */
			static constexpr UInt NE = SHAPE::numEdges;
			
			/*! Number of vertices per edge times number of vertices. */
			static constexpr UInt N = NV*NE;
			
		protected:
			/*! Smart pointer to the mesh. */
			shared_ptr<mesh<SHAPE,MT>> grid;
			
			/*! Set of edges. */
			unordered_set<geoElement<Line>> edges;
			
			/*! Node-node connections. */
			vector<graphItem> node2node;
			
			/*! Node-element connections. */
			vector<graphItem> node2elem;
			
			/*! Element-element connections. */
			vector<graphItem> elem2elem;
			
		public:
			//
			// Constructor and destructor
			//
			
			/*! (Default) constructor. 
				\param g	shared pointer to the grid */
			bconnect(const shared_ptr<mesh<SHAPE,MT>> & g = nullptr);
			
			/*! Synthetic destructor. */
			virtual ~bconnect() = default;
			
			//
			// Initialize and clear connections
			//
			
			/*! Initialize node-node connections. It also fill the set of edges. */
			void buildNode2Node();
			
			/*! Initialize node-element connections. */
			void buildNode2Elem();
			
			/*! Build element-element connections for an element.
				\param Id	element Id */
			void buildElem2Elem(const UInt & Id);
			
			/*! Initialize element-element connections. */
			void buildElem2Elem();
			
			/*! Re-build all connections. */
			virtual void refresh();
			
			/*! Clear all connections and the set of edges. */
			virtual void clear();
			
			//
			// Modify connections
			//
			
			/*! Replace a node in node-node connections.
				Debug mode: the method finds itself the nodes connected to the old Id.
				
				\param oldId	the Id to substitute
				\param newId	the new Id */
			void replaceNodeInNode2Node(const UInt & oldId, const UInt & newId);
			
			/*! Replace a node in node-node connections.
				Release mode: the nodes connected to the old one are already provided.
				
				\param oldId	the Id to substitute
				\param newId	the new Id 
				\param involved	nodes involved in the changes */
			void replaceNodeInNode2Node(const UInt & oldId, const UInt & newId, const vector<UInt> & involved);
			
			/*! Replace a node in node-element connections.
				This method simply moves all connected elements 
				from the old Id to the new one.
				
				\param oldId	the Id to substitute
				\param newId	the new Id */
			void replaceNodeInNode2Elem(const UInt & oldId, const UInt & newId);
			
			/*! Remove elements from node-element connections.
				Debug mode: the method find itself the vertices of the elements to remove. 
				
				\param toRemove	vector with the Id's to remove */
			void eraseElemsInNode2Elem(const vector<UInt> & toRemove);
			
			/*! Remove two elements from node-element connections after an edge contraction.
				Release mode: the nodes to update are already provided.
				
				\param toRemove	Id's of the elements to remove 
				\param newId	Id of the node resulting from the collapse
				\param involved	Id's of nodes involved in the contraction */
			void eraseElemsInNode2Elem(const vector<UInt> & toRemove, const UInt & newId, const vector<UInt> & involved);
			
			/*! Remove elements from element-element connections.
				\param toRemove	Id's of elements to remove */
			void eraseElemsInElem2Elem(const vector<UInt> & toRemove);
			
			/*! Update connections after an edge contraction (debug mode).
				Note that the set of edges is not updated. 
				This method shows the order the various methods of the class should be used.
				
				\param oldId	Id of the node to remove
				\param newId	Id of the node to keep
				\param toRemove	Id's of the elements to remove */
			void applyEdgeCollapsing(const UInt & oldId, const UInt & newId, const vector<UInt> & toRemove);
			
			/*! Update connections after an edge contraction (release mode).
				Note that the set of edges is not updated. 
				This method shows the order the various methods of the class should be used.
				
				\param oldId	Id of the node to remove
				\param newId	Id of the node to keep
				\param toRemove	Id's of the elements to remove 
				\param involved	Id's of the nodes involved in the contraction */
			void applyEdgeCollapsing(const UInt & oldId, const UInt & newId, const vector<UInt> & toRemove,
				const vector<UInt> & involved);
						
			//
			// Get methods
			//
			
			/*! Get pointer to the mesh.
				\return		the mesh */
			shared_ptr<mesh<SHAPE,MT>> getMesh();
			
			/*! Get edges of the mesh.
				\return		vector of edges */
			// Note: we return a (possible huge) vector by value for Return Value Optimization (RVO).
			// Reference: https://web.archive.org/web/20130930101140/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value
			vector<geoElement<Line>> getEdges() const;
						
			/*! Get the node-node connections for a node.
				\param Id	node Id
				\return		the connections */
			graphItem getNode2Node(const UInt & Id) const;
			
			/*! Get node-node connections for all nodes.
				\return		vector of connections */			
			vector<graphItem> getNode2Node() const;
			
			/*! Get the node-element connections for a node.
				\param Id	node Id
				\return		the connections */
			graphItem getNode2Elem(const UInt & Id) const;
			
			/*! Get node-element connections for all nodes.
				\return		vector of connections */
			vector<graphItem> getNode2Elem() const;
			
			/*! Get the element-element connections for an element.
				\param Id	element Id
				\return		the connections */
			graphItem getElem2Elem(const UInt & Id) const;
			
			/*! Get element-element connections for all elements.
				\return		vector of connections */
			vector<graphItem> getElem2Elem() const;
			
			//
			// Set methods
			//
			
			/*! Set mesh pointer. After that, the connections are (re-)built.
				\param newGrid	the new mesh */
			void setMesh(const shared_ptr<mesh<SHAPE,MT>> & newGrid);
	};
}

/*! Include implementations of class members. */
#include "implementation/imp_bconnect.hpp"

#endif
