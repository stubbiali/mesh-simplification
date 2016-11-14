/*!	\file 	imp_connect.hpp
	\brief	Definitions of members of class connect. */
	
#ifndef HH_IMPCONNECT_HH
#define HH_IMPCONNECT_HH

namespace geometry
{
	//
	// Constructor (MeshType::GEO)
	//
	
	template<typename SHAPE>
	connect<SHAPE, MeshType::GEO>::connect(const shared_ptr<mesh<SHAPE, MeshType::GEO>> & g) :
		bconnect<SHAPE, MeshType::GEO>(g)
	{
	}
	
	
	//
	// Constructor (MeshType::DATA)
	//
	
	template<typename SHAPE>
	connect<SHAPE, MeshType::DATA>::connect(const shared_ptr<mesh<SHAPE, MeshType::DATA>> & g) :
		bconnect<SHAPE, MeshType::DATA>(g)
	{
		// Build data-element and element-data connections
		buildData2Elem();
		buildElem2Data();
	}
	
	
	//
	// Initialize and clear connections
	//
	
	template<typename SHAPE>
	void connect<SHAPE, MeshType::DATA>::buildData2Elem() 
	{
		if (this->grid != nullptr)
		{
			// First, build node-element connections
			if (this->node2elem.empty())
				this->buildNode2Elem();
				
			// Reserve memory
			data2elem.clear();
			data2elem.reserve(this->grid->getNumNodes());
			
			// Copy node-element connections into data-element connections
			copy(this->node2elem.begin(), this->node2elem.end(), back_inserter(data2elem.begin()));
		}
		else	throw runtime_error("Mesh pointer not set.")
	}
	
	
	template<typename SHAPE>
	void connect<SHAPE, MeshType::DATA>::buildElem2Data()
	{
		if (this->grid != nullptr)
		{
			// First, build data-element connections
			if (data2elem.empty())
				buildData2Elem();
				
			// Reserve memory
			elem2data.clear();
			elem2data.reserve(this->grid->getNumElems());
			
			// Set elements Id's
			for (UInt id = 0; id < this->grid->getNumElems(); id++)
				elem2data.emplace_back(id);
				
			// Loop over all elements
			for (UInt datumId = 0; datumId < this->grid->getNumData(); datumId++)
			{
				// Extract elements connected to the datum
				auto conn = data2elem[datumId].getConnected();
				
				// Add the datum to the elements
				for (auto elemId : conn)
					elem2data[elemId].insert(datumId);
			}
		}
		else	throw runtime_error("Mesh pointer not set.")
	}
	
	
	template<typename SHAPE>
	void connect<SHAPE, MeshType::DATA>::refresh()
	{
		// Clear all connections and the set of edges
		this->clear();
		
		// Re-build all connections and the set of edges
		this->buildNode2Node();
		this->buildNode2Elem();
		this->buildElem2Elem();
		buildData2Elem();
		buildElem2Data();
	}
	
	
	template<typename SHAPE>
	void connect<SHAPE, MeshType::DATA>::clear()
	{
		bconnect<SHAPE, MeshType::DATA>::clear();
		data2elem.clear();
		elem2data.clear();
	}
}

#endif
