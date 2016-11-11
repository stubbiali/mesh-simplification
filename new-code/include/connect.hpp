/*! \file	connect.hpp
	\brief	A class creating the connections for a mesh. */
	
#ifndef HH_CONNECT_HH
#define HH_CONNECT_HH

#include "bconnect.hpp"

namespace geometry
{
	/*! This is a wrapper class for bconnect, which it inherits.
		It takes two template parameters:
		<ol>
		<li> SHAPE: shape of the elements;
		<li> MT: 	mesh type, i.e. with or without distributed data;
					default is GEO.
		<\ol>
		When MT is set to GEO (i.e. no data): the class just inherits bconnect
		without adding or overriding members.
		When MT is set to DATA (i.e. with data): the class inherits bconnect and 
		adds members for handling data distributed over the mesh. In particular,
		the connections data-element and element-data are built and maintained.
		
		\sa bconnect.hpp */
	template<typename SHAPE, MeshType MT = MeshType::GEO>
	class connect final : public bconnect<SHAPE,MT>
	{
	};
	
	/*! Partial specialization for grids with distributed data. */
	template<typename SHAPE>
	class connect<SHAPE, MeshType::DATA> final : public bconnect<SHAPE, MeshType::DATA> 
	{
		private:
			/*! Element-data connections. */
			vector<graphItem> elem2data;
			
			/*! Data-element connections. */
			vector<graphItem> data2elem;
			
		public:
			//
			// Constructor
			//
			
			/*! (Default) constructor.
				\param g	shared pointer to the grid */
			connect(smart_ptr<mesh<SHAPE,MT>> g = nullptr); 
			
			//
			// Connections initializers
			//
			
			/*! Initialize element-data connections. */
			void buildElem2Data();
			
			/*! Initialize data-element connections. */
			void buildData2Elem();
			
			/*! Re-build all connections. */
			virtual void refresh();
			
			//
			// Connections modifiers
			//
			
			/*! Remove data from element-data connections.
				\param ids	vector with Id's of data to remove */
			void eraseDataInElem2Data(const vector<UInt> & ids);
						
			/*! Insert data in element-data connections.
				\param ids	vector with Id's of data to insert */
			void insertDataInElem2Data(const vector<UInt> & ids);
			
			// 
			// Get methods
			//
			
			/*! Get element-data connections for an element.
				\param Id	element Id
				\return		the connections */
			graphItem getElem2Data(const UInt & Id) const;
			
			/*! Get element-data connections for all elements.
				\return		vector of connections */
			vector<graphItem> getElem2Data() const;
			
			/*! Get data-element connections for a datum.
				\param Id	datum Id
				\return		the connections */
			graphItem getData2Elem(const UInt & Id) const;
			
			/*! Get data-element connections for all data.
				\return		vector of connections */
			vector<graphItem> getData2Elem() const;
			
			//
			// Set methods
			//
			
			/*! Set data-element connections for a datum.
				\param Id			datum Id
				\param newConn		new connections */
			void setData2Elem(const UInt & Id, const set<UInt> & newConn);
	};
}

/*! Include implementations of class members. */
#include "implementation/imp_connect.hpp"

#endif
