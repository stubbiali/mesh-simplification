/*! \file	imp_bmesh.hpp
	\brief	Definitions of members and friend functions of class bmesh. 
			For the specializations of members for reading the mesh
			from a .imp file: see bmesh.cpp. */
	
#ifndef HH_IMPBMESH_HH
#define HH_IMPBMESH_HH

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <istream>
#include <array>

#include "utility.hpp"

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
		// Extract file extension
		auto format = utility::getFileExtension(filename);
		
		// Switch the format
		if (format == "inp")
			read_inp(filename);
		else if (format == "vtk")
			read_vtk(filename);
		else
			throw runtime_error("Format " + format + " not known.");
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
	INLINE vector<point> bmesh<SHAPE>::getNodes() const
	{
		return nodes;
	}
	
	
	template<typename SHAPE>
	INLINE geoElement<SHAPE> bmesh<SHAPE>::getElem(const UInt & Id) const
	{
		return elems[Id];
	}
	
	
	template<typename SHAPE>
	INLINE vector<geoElement<SHAPE>> bmesh<SHAPE>::getElems() const
	{
		return elems;
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
		nodes[Id].setInactive();
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setIdx(const UInt & Id, const UInt & idx)
	{
		elems[Id].setIdx(idx);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setElemInactive(const UInt & Id)
	{
		elems[Id].setInactive();
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
		setUpNodesIds();
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::eraseElem(const UInt & Id)
	{
		// Erase
		auto it = elems.begin() + Id;
		elems.erase(it);
		
		// Update id's
		setUpElemsIds();
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::clear()
	{
		nodes.clear();
		elems.clear();
	}
	
	
	//
	// Read mesh from file
	//
	
	template<typename SHAPE>
	void bmesh<SHAPE>::read_inp(const string & filename)
	{
		ifstream file(filename);
		if (file.is_open())
		{
			string line;
						
			// Get number of nodes and elements
			UInt numNodes, numElems;
			getline(file,line);
			static_cast<stringstream>(line) >> numNodes >> numElems;
			
			// Assert
			assert(numNodes < MAX_NUM_NODES);
			assert(numElems < MAX_NUM_ELEMS);
			
			// Reserve memory
			nodes.reserve(numNodes);
			elems.reserve(numElems);
			
			// Insert nodes
			UInt Id;
			array<Real,3> coor; 
			for (UInt n = 0; n < numNodes && getline(file,line); n++)
			{
				// Extract coordinates
				static_cast<stringstream>(line)	>> Id 
												>> coor[0] 
												>> coor[1] 
												>> coor[2];
												
				// Insert at back
				nodes.emplace_back(coor,n);
			}
			
			// Insert elements
			UInt geoId;
			string foo; 
			array<UInt,NV> vert;
			for (UInt n = 0; n < numElems && getline(file,line); n++)
			{
				// Extract geometric Id
				stringstream ss(line);
				ss >> Id >> geoId >> foo;
				
				// Extract vertices Id's
				// They need to be made compliant with a zero-based indexing
				for (auto & v : vert)
				{
					ss >> v;
					v--;
				}
				
				// Insert at back							
				elems.emplace_back(vert, n, geoId);
			}
			
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::read_vtk(const string & filename)
	{
		// Note: we suppose the file does not include
		// any empty file, so in case they are present,
		// please remove them before calling this methods.
		
		ifstream file(filename);
		if (file.is_open())
		{
			string line, foo;
			
			// Disregard the first four lines
			for (UInt i = 0; i < 4; ++i)
				 getline(file,line);
				 
			//
			// Import nodes
			//
						
			// Get number of nodes
			UInt numNodes;
			getline(file,line);
			static_cast<stringstream>(line) >> foo >> numNodes;
						
			// Assert
			assert(numNodes < MAX_NUM_NODES);
			
			// Reserve memory
			nodes.reserve(numNodes);
			
			// Insert nodes
			// We take into account also the case of 
			// multiple points on the same line
			UInt id(0);
			array<Real,3> coor; 
			while ((id < numNodes) && getline(file,line))
			{
				stringstream ss(line);
													
				while (!(ss.eof()))
				{
					// Extract coordinates and insert at back
					ss >> coor[0] >> coor[1] >> coor[2];
					nodes.emplace_back(coor,id);
					++id;
					
					// To handle the case of a space before "\n"					
					char cfoo = ss.get();
					char pfoo = ss.peek();
				}
			}
			
			//
			// Import elements
			//
			
			// Get number of elements
			UInt numElems;
			getline(file,line);
			static_cast<stringstream>(line) >> foo >> numElems;
			
			// Assert
			assert(numElems < MAX_NUM_ELEMS);
			
			// Reserve memory
			elems.reserve(numElems);
			
			UInt geoId; 
			array<UInt,NV> vert;
			for (UInt id = 0; id < numElems && getline(file,line); ++id)
			{
				// Extract geometric Id
				stringstream ss(line);
				ss >> geoId;
				
				// Extract vertices Id's
				// They do not need to be made compliant 
				// with a zero-based indexing
				for (auto & v : vert)
					ss >> v;
				
				// Insert at back							
				elems.emplace_back(vert, id, geoId);
			}
			
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}
	
	
	//
	// Print
	//
	
	template<typename SHAPE>
	ostream & operator<<(ostream & out, const bmesh<SHAPE> & bm)
	{
		// Print list of nodes			
		out << "List of " << bm.getNumNodes() << " nodes:" << endl;
		for (auto node : bm.nodes)
			out << node << endl;
			
		// Print list of elements
		out << "List of " << bm.getNumElems() << " elements:" << endl;
		for (auto elem : bm.elems)
			out << elem << endl;
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::print(const string & filename) const
	{
		// Extract file extension
		auto format = utility::getFileExtension(filename);
		
		// Switch the format
		if (format == "inp")
			print_inp(filename);
		else if (format == "vtk")
			print_vtk(filename);
		else
			throw runtime_error("Format " + format + " not known.");
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
