/*! \file	connect.hpp
	\brief	A class creating the connections for a mesh. */
	
#ifndef HH_CONNECT_HH
#define HH_CONNECT_HH

#include "bconnect.hpp"

namespace geometry
{
	/*!	Forward declaration of class bmeshInfo. */
	template<typename SHAPE, MeshType MT>
	class bmeshInfo;
	
	/*!	Forward declaration of class meshInfo. */
	template<typename SHAPE, MeshType MT>
	class meshInfo;
		
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
	
	/*! Partial specialization for grids without distributed data. */
	template<typename SHAPE>
	class connect<SHAPE, MeshType::GEO> final : public bconnect<SHAPE, MeshType::GEO> 
	{
		public:
			/*! bmeshInfo is a friend class. */
			friend class bmeshInfo<SHAPE, MeshType::GEO>;
			
			/*! meshInfo is a friend class. */
			friend class meshInfo<SHAPE, MeshType::GEO>;
			
			//
			// Constructor
			//
			
			/*! (Default) constructor.
				\param g	shared pointer to the grid */
			connect(const shared_ptr<mesh<SHAPE, MeshType::GEO>> & g = nullptr);
			
			/*! Constructor.
				\param args	arguments to forward to mesh 
				
				\sa mesh.hpp */
			template<typename... Args>
			connect(Args... args);
	};
	
	/*! Partial specialization for grids with distributed data. */
	template<typename SHAPE>
	class connect<SHAPE, MeshType::DATA> final : public bconnect<SHAPE, MeshType::DATA> 
	{
		private:
			/*! Data-element connections. */
			vector<graphItem> data2elem;
			
			/*! Element-data connections. */
			vector<graphItem> elem2data;
						
		public:
			/*! bmeshInfo is a friend class. */
			friend class bmeshInfo<SHAPE, MeshType::DATA>;
			
			/*! meshInfo is a friend class. */
			friend class meshInfo<SHAPE, MeshType::DATA>;
			
			//
			// Constructor
			//
			
			/*! (Default) constructor.
				\param g	shared pointer to the grid */
			connect(const shared_ptr<mesh<SHAPE, MeshType::DATA>> & g = nullptr); 
			
			/*! Constructor.
				\param args	arguments to forward to mesh 
				
				\sa mesh.hpp */
			template<typename... Args>
			connect(Args... args);
			
			//
			// Initialize and clear connections
			//
			
			/*! Initialize data-element connections. 
				This method supposes the data points coincide with the nodes, 
				then data-element connections coincide with node-element connections.
				In case data points and nodes do not coincide, the user may manually
				set data-element connections through setData2Elem(). 
				
				\sa setData2Elem() */
			void buildData2Elem();
			
			/*! Initialize element-data connections. */
			void buildElem2Data();
					
			/*! Re-build all connections. */
			virtual void refresh();
			
			/*! Clear all connections and the set of edges. */
			virtual void clear();
			
			//
			// Modify connections
			//
			
			/*! Remove a datum from element-data connections.
				\param Id	Id of the datum to remove */
			void eraseDataInElem2Data(const UInt & Id);
			
			/*! Remove data from element-data connections.
				\param ids	vector with Id's of data to remove */
			void eraseDataInElem2Data(const vector<UInt> & ids);
			
			/*! Insert a datum in element-data connections.
				\param Id	Id of the datum to insert */
			void insertDataInElem2Data(const UInt & Id);
						
			/*! Insert data in element-data connections.
				\param ids	vector with Id's of data to insert */
			void insertDataInElem2Data(const vector<UInt> & ids);
			
			// 
			// Get methods
			//
			
			/*! Get data-element connections for a datum.
				\param Id	datum Id
				\return		the connections */
			graphItem getData2Elem(const UInt & Id) const;
			
			/*! Get data-element connections for all data.
				\return		vector of connections */
			vector<graphItem> getData2Elem() const;
			
			/*! Get element-data connections for an element.
				\param Id	element Id
				\return		the connections */
			graphItem getElem2Data(const UInt & Id) const;
			
			/*! Get element-data connections for all elements.
				\return		vector of connections */
			vector<graphItem> getElem2Data() const;
						
			//
			// Set methods
			//
			
			/*! Set data-element connections for a datum.
				Before setting the new connections, it calls eraseDataInElemToData().
				After having set the new connections, it calls insertDataInElemToData().
				It returns the old connections.
				
				\param Id			datum Id
				\param newConn		new connections 
				\return				old connections
				
				\sa eraseDataInElemToData(), insertDataInElemToData() */
			vector<UInt> setData2Elem(const UInt & Id, const set<UInt> & newConn);
			
			/*! Set data-element connections for a datum.
				Before setting the new connections, it calls eraseDataInElemToData().
				After having set the new connections, it calls insertDataInElemToData().
				It returns the old connections.
				
				\param Id			datum Id
				\param newConn		new connections 
				\return				old connections
				
				\sa eraseDataInElemToData(), insertDataInElemToData() */
			vector<UInt> setData2Elem(const UInt & Id, const vector<UInt> & newConn);
			
			/*! Set data-element connections for a datum.
				Before setting the new connections, it calls eraseDataInElemToData().
				After having set the new connections, it calls insertDataInElemToData().
				It returns the old connections.
				
				\param newData2Elem	new graph item 
				\return				old connections
				
				\sa eraseDataInElemToData(), insertDataInElemToData() */
			graphItem setData2Elem(const graphItem & newData2Elem);
			
			/*! Set data-element connections for all data.
				This is particularly useful if the data do not initially coincide with nodes.
				
				\param newData2Elem	vector of connections */
			void setData2Elem(const vector<graphItem> & newData2Elem);
	};
}

/*! Include implementations of class members. */
#include "implementation/imp_connect.hpp"

#endif