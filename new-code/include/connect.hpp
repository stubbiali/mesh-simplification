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
			
			void buildElem2Data();
			
			void buildData2Elem();
			
			virtual void refresh();
			
			//
			// Connections modifiers
			//
			
			
	};
}

#endif
