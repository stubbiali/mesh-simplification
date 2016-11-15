/*!	\file 	imp_connect.hpp
	\brief	Definitions of members of class connect. */
	
#ifndef HH_IMPCONNECT_HH
#define HH_IMPCONNECT_HH

namespace geometry
{
	//
	// Constructors (MeshType::GEO)
	//
	
	template<typename SHAPE>
	connect<SHAPE, MeshType::GEO>::connect(const shared_ptr<mesh<SHAPE, MeshType::GEO>> & g) :
		bconnect<SHAPE, MeshType::GEO>(g)
	{
	}
	
	
	template<typename SHAPE>
	template<typename... Args>
	connect<SHAPE, MeshType::GEO>::connect(Args... args) :
		bconnect<SHAPE, MeshType::GEO>(args...)
	{
	}
	
	
	//
	// Constructors (MeshType::DATA)
	//
	
	template<typename SHAPE>
	connect<SHAPE, MeshType::DATA>::connect(const shared_ptr<mesh<SHAPE, MeshType::DATA>> & g) :
		bconnect<SHAPE, MeshType::DATA>(g)
	{
		// Build data-element and element-data connections
		buildData2Elem();
		buildElem2Data();
	}
	
	
	template<typename SHAPE>
	template<typename... Args>
	connect<SHAPE, MeshType::DATA>::connect(Args... args) :
		bconnect<SHAPE, MeshType::DATA>(args...)
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
			copy(this->node2elem.begin(), this->node2elem.end(), back_inserter(data2elem));
		}
		else	throw runtime_error("Mesh pointer not set.");
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
		else	throw runtime_error("Mesh pointer not set.");
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
	
	
	//
	// Modify connections
	//
	
	template<typename SHAPE>
	void connect<SHAPE, MeshType::DATA>::eraseDataInElem2Data(const UInt & Id)
	{
		assert(Id < this->grid->getNumData());
		
		// Extract data-element connections
		auto elems = data2elem[Id].getConnected();
		
		// Remove datum
		for (auto elem : elems)
			elem2data[elem].erase(Id);
	}
	
	
	template<typename SHAPE>
	void connect<SHAPE, MeshType::DATA>::eraseDataInElem2Data(const vector<UInt> & ids)
	{
		for (auto id : ids)
			eraseDataInElem2Data(id);
	}
	
	
	template<typename SHAPE>
	void connect<SHAPE, MeshType::DATA>::insertDataInElem2Data(const UInt & Id)
	{
		assert(Id < this->grid->getNumData());
		
		// Extract data-element connections
		auto elems = data2elem[Id].getConnected();
		
		// Remove datum
		for (auto elem : elems)
			elem2data[elem].insert(Id);
	}
	
	
	template<typename SHAPE>
	void connect<SHAPE, MeshType::DATA>::insertDataInElem2Data(const vector<UInt> & ids)
	{
		for (auto id : ids)
			insertDataInElem2Data(id);
	}
	
	
	//
	// Get methods
	//
	
	template<typename SHAPE>
	INLINE graphItem connect<SHAPE, MeshType::DATA>::getData2Elem(const UInt & Id) const
	{
		assert(Id < this->grid->getNumData());
		return data2elem[Id];
	}
	
	
	template<typename SHAPE>
	INLINE vector<graphItem> connect<SHAPE, MeshType::DATA>::getData2Elem() const
	{
		return data2elem;
	}
	
	
	template<typename SHAPE>
	INLINE graphItem connect<SHAPE, MeshType::DATA>::getElem2Data(const UInt & Id) const
	{
		assert(Id < this->grid->getNumElems());
		return elem2data[Id];
	}
	
	
	template<typename SHAPE>
	INLINE vector<graphItem> connect<SHAPE, MeshType::DATA>::getElem2Data() const
	{
		return elem2data;
	}
	
	
	//
	// Set methods
	//
	
	template<typename SHAPE>
	vector<UInt> connect<SHAPE, MeshType::DATA>::setData2Elem(const UInt & Id, const set<UInt> & newConn)
	{
		assert(Id < this->grid->getNumData());
		
		// Extract old data-element connections
		auto oldConn = data2elem[Id].getConnected();
		
		// Erase old element-data connections
		eraseDataInElem2Data(Id);
		
		// Insert new data-element connections
		data2elem[Id].clear();
		data2elem[Id].insert(newConn);
		
		// Build new element-data connections
		insertDataInElem2Data(Id);
		
		// Return old data-element connections
		return oldConn;
	}
	
	
	template<typename SHAPE>
	vector<UInt> connect<SHAPE, MeshType::DATA>::setData2Elem(const UInt & Id, const vector<UInt> & newConn)
	{
		assert(Id < this->grid->getNumData());
		
		// Extract old data-element connections
		auto oldConn = data2elem[Id].getConnected();
		
		// Erase old element-data connections
		eraseDataInElem2Data(Id);
		
		// Insert new data-element connections
		data2elem[Id].clear();
		data2elem[Id].insert(newConn);
		
		// Build new element-data connections
		insertDataInElem2Data(Id);
		
		// Return old data-element connections
		return oldConn;
	}
	
	
	template<typename SHAPE>
	graphItem connect<SHAPE, MeshType::DATA>::setData2Elem(const graphItem & newData2Elem)
	{
		auto Id = newData2Elem.getId();
		assert(Id < this->grid->getNumData());
		
		// Extract old data-element connections
		auto oldConn = data2elem[Id];
		
		// Erase old element-data connections
		eraseDataInElem2Data(Id);
		
		// Insert new data-element connections
		data2elem[Id] = newData2Elem;
		
		// Build new element-data connections
		insertDataInElem2Data(Id);
		
		// Return old data-element connections
		return oldConn;
	}
	
	
	template<typename SHAPE>
	void connect<SHAPE, MeshType::DATA>::setData2Elem(const vector<graphItem> & newData2Elem)
	{
		// Clear (possibly) old data-element connections
		data2elem.clear();
		
		// Set new data-element connections
		data2elem.reserve(newData2Elem.size());
		copy(newData2Elem.cbegin(), newData2Elem.cend(), back_inserter(data2elem));
		
		// Build new element-data connections
		buildElem2Data();
	}
}

#endif
