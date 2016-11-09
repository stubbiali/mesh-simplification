/*! \file	imp_bmesh.hpp
	\brief	Definitions of members and friend functions of class bmesh. */
	
#ifndef HH_IMPBMESH_HH
#define HH_IMPBMESH_HH

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <istream>

namespace geometry
{
	//
	// Constructors
	//
	
	template<typename SHAPE>
	bmesh<SHAPE>::bmesh(const UInt & numNodes, const UInt & numElems) 
	{
		assert(numNodes < MAX_NUM_NODES);
		assert(numElems < MAX_NUM_ELEMS);
		
		nodes.reserve(numNodes);
		elems.reserve(numElems);
	}
	
	
	template<typename SHAPE>
	bmesh<SHAPE>::bmesh(const vector<point> & nds, const vector<geoElement<SHAPE>> & els) :
		nodes(nds.cbegin(), nds.cend()), elems(els.cbegin(), els.cend())
	{
	}
	
	
	template<typename SHAPE>
	bmesh<SHAPE>::bmesh(const string & filename)
	{
		// TODO
	}
	
	
	//
	// Get methods
	//
	
	template<typename SHAPE>
	INLINE point bmesh<SHAPE>::getNode(const UInt & Id) const
	{
		return nodes[Id];
	}
	
	
	template<typename SHAPE>
	INLINE geoElement<SHAPE> bmesh<SHAPE>::getElem(const UInt & Id) const
	{
		return elems[Id];
	}
	
	
	template<typename SHAPE>
	INLINE UInt bmesh<SHAPE>::getNumNodes() const
	{
		return nodes.size();
	}
	
	
	template<typename SHAPE>
	INLINE UInt bmesh<SHAPE>::getNumElems() const
	{
		return elems.size();
	}
	
	
	//
	// Set methods
	//
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setNode(const UInt & Id, const point & p)
	{
		nodes[Id] = p;
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setElem(const UInt & Id, const geoElement<SHAPE> & g)
	{
		elems[Id] = g;
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::resizeNodes(const UInt & numNodes)
	{
		nodes.resize(numNodes);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::reserveNodes(const UInt & numNodes)
	{
		nodes.reserve(numNodes);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::resizeElems(const UInt & numElems)
	{
		elems.resize(numElems);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::reserveElems(const UInt & numElems)
	{
		elems.reserve(numElems);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setBoundary(const UInt & Id, const UInt & bound)
	{
		nodes[Id].setBoundary(bound);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setNodeInactive(const UInt & Id)
	{
		nodes[Id].setActive(false);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setElemInactive(const UInt & Id)
	{
		elems[Id].setActive(false);
	}
	
	
	//
	// Insert and replace methods
	//
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::insertNode(const array<Real,3> & coor, const UInt & bound)
	{
		nodes.emplace_back(coor, nodes.size(), bound);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::insertElem(const array<UInt,NV> & vert, const UInt & geoId)
	{
		elems.emplace_back(vert, elems.size(), geoId);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::replaceVertex(const UInt & elemId, const UInt & oldId, const UInt & newId)
	{
		elems[elemId].replace(oldId,newId);
	}
	
	
	//
	// Erase and clear methods
	//
	
	template<typename SHAPE>
	void bmesh<SHAPE>::eraseNode(const UInt & Id)
	{
		// Erase
		auto it = nodes.begin() + Id;
		nodes.erase(it);
		
		// Update id's
		this->setUpNodesIds();
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::eraseElem(const UInt & Id)
	{
		// Erase
		auto it = elems.begin() + Id;
		elems.erase(it);
		
		// Update id's
		this->setUpElemsIds();
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::clear()
	{
		nodes.clear();
		elems.clear();
	}
	
	
	//
	// Print
	//
	
	template<typename SHAPE>
	ostream & operator<<(ostream & out, const bmesh<SHAPE> & bm)
	{
		out << "Number of nodes   : " << bm.nodes.size() << endl
			<< "Number of elements: " << bm.elems.size() << endl << endl;
			
		out << "List of nodes:" << endl;
		for (auto node : bm.nodes)
			out << node << endl;
			
		out << "List of elements:" << endl;
		for (auto elem : bm.elems)
			out << elem << endl;
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::print(const string & filename) const
	{
		// Reverse filename
		string aux(filename);
		reverse(aux.begin(), aux.end());
		
		// Extract the format
		string format;
		istringstream iss;
		iss.str(aux);
		if (!(getline(iss, format, '.')))
			reverse(format.begin(), format.end());
		else
			throw runtime_error("The provided file does not present an extension.");
		
		// Switch the format
		if (format == "inp")
			print_inp(filename);
		else if (format == "vtk")
			print_vtk(filename);
		else
			throw runtime_error("Format not allowed.");
	}
	
	
	template<>
	void bmesh<Triangle>::print_inp(const string & filename) const
	{
		// Open the file
		ofstream file(filename);
		
		if (file.is_open())
		{
			// Preamble
			file << nodes.size() << " "
				 << elems.size() << " " 
				 << "0 0 0" << endl;
				 
			// Print nodes
			for (auto node : nodes)
				file << node.getId()+1 	<< " "
					 << node[0] 		<< " "
					 << node[1] 		<< " "
					 << node[2] 		<< endl;
					 
			// Print elements
			for (auto elem : elems)
				file << elem.getId()+1 	<< " "
					 << elem.getGeoId() << "  "
					 << "tri" 			<< "  "
					 << elem[0] 		<< "  "
					 << elem[1] 		<< "  "
					 << elem[2] 		<< endl;
					 
			// Close the file
			file.close();
		}
		else
		{
			string err(filename + " can not be opened.");
			throw runtime_error(err);
		}
	}
	
	
	template<>
	void bmesh<Quad>::print_inp(const string & filename) const
	{
		// Open the file
		ofstream file(filename);
		
		if (file.is_open())
		{
			// Preamble
			file << nodes.size() << " "
				 << elems.size() << " " 
				 << "0 0 0" << endl;
				 
			// Print nodes
			for (auto node : nodes)
				file << node.getId()+1 	<< " "
					 << node[0] 		<< " "
					 << node[1] 		<< " "
					 << node[2] 		<< endl;
					 
			// Print elements
			for (auto elem : elems)
				file << elem.getId()+1 	<< " "
					 << elem.getGeoId() << "  "
					 << "quad" 			<< "  "
					 << elem[0] 		<< "  "
					 << elem[1] 		<< "  "
					 << elem[2] 		<< "  " 
					 << elem[3]			<< endl;
					 
			// Close the file
			file.close();
		}
		else
		{
			string err(filename + " can not be opened.");
			throw runtime_error(err);
		}
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::print_vtk(const string & filename) const
	{
		// TODO
	}
	
	
	//
	// Update Id's
	//
	
	template<typename SHAPE>
	void bmesh<SHAPE>::setUpNodesIds()
	{
		for (UInt i = 0; i < nodes.size(); i++)
			nodes[i].setId(i);
	}
	

	template<typename SHAPE>
	void bmesh<SHAPE>::setUpElemsIds()
	{
		for (UInt i = 0; i < elems.size(); i++)
			elems[i].setId(i);
	}
}

#endif
