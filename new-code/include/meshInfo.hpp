/*!	\file	meshInfo.hpp
	\brief	Class performing operations on and getting info from a mesh 
			without modifying its connections. */
			
#ifndef HH_MESHINFO_HH
#define HH_MESHINFO_HH

#include "bmeshInfo.hpp"

namespace geometry
{
	/*!	This class performs some operations on a mesh (e.g. set the
		geometric Id's) and retrieve some info on it just exploiting
		the geometry and the topology (i.e. the connectivity). 	
	
		This class is not allowed to modify the connections of a mesh.
		This privilege is exclusively given to doctor and, in some sense,
		the classes computing the edge cost function. 
		Hence the name of the class.
		
		This class inherits bmeshInfo, hence allowing to handle data
		distributed over the mesh.
		
		\sa bmeshInfo.hpp, doctor.hpp */
	template<typename SHAPE, MeshType MT = MeshType::GEO>
	class meshInfo final : public bmeshInfo<SHAPE,MT>
	{
	};
	
	/*!	Partial specialization for grids without distributed data. */
	template<typename SHAPE>
	class meshInfo<SHAPE, MeshType::GEO> : public bmeshInfo<SHAPE, MeshType::GEO>
	{
		public:
			//
			// Constructors
			//
			
			/*!	(Default) constructor.
				\param g	shared pointer to the grid */
			meshInfo(const shared_ptr<mesh<SHAPE, MeshType::GEO>> & g = nullptr);
			
			/*!	Constructor.
				\param bg	shared pointer to a bmesh
				
				\sa bmesh.hpp */
			meshInfo(const shared_ptr<bmesh<SHAPE>> & bg);
						
			/*!	Constructor.
				\param args	arguments to forward to mesh 
				
				\sa mesh.hpp */
			template<typename... Args>
			meshInfo(Args... args);
	};
	
	/*!	Partial specialization for grids with distributed data. */
	template<typename SHAPE>
	class meshInfo<SHAPE, MeshType::DATA> : public bmeshInfo<SHAPE, MeshType::DATA>
	{
		public:
			//
			// Constructors
			//
			
			/*!	(Default) constructor.
				\param g	shared pointer to the grid */
			meshInfo(const shared_ptr<mesh<SHAPE, MeshType::DATA>> & g = nullptr);
			
			/*!	Constructor.
				\param bg	shared pointer to a bmesh
				
				\sa bmesh.hpp */
			meshInfo(const shared_ptr<bmesh<SHAPE>> & bg);
						
			/*!	Constructor.
				\param args	arguments to forward to mesh 
				
				\sa mesh.hpp */
			template<typename... Args>
			meshInfo(Args... args);
	
			//
			// Get topological info
			//
			
			/*!	Get data involved in an edge collapsing, i.e. belonging to the elements
				involved in the edge collapsing, included the elements to remove.
				Debug mode.
				
				\param id1	first end-point of the edge
				\param id2	second end-point of the edge
				\return		Id's of the involved data */
			vector<UInt> getDataInvolvedInEdgeCollapsing(const UInt & id1, const UInt & id2) const;
			
			/*!	Get data involved in an edge collapsing, i.e. belonging to the elements
				involved in the edge collapsing, included the elements to remove.
				Release mode.
				
				\param invElems	elements involved in the edge collapsing
				\return			Id's of the involved data */	
			vector<UInt> getDataInvolvedInEdgeCollapsing(const vector<UInt> & invElems) const;
			
			/*!	Get data involved in an edge collapsing and 
				whose location may change after the collapsing itself.
				Debug mode.
				
				\param id1	first end-point of the edge
				\param id2	second end-point of the edge
				\return		Id's of the involved data */
			vector<UInt> getDataModifiedInEdgeCollapsing(const UInt & id1, const UInt & id2) const;
			
			/*!	Get data involved in an edge collapsing and 
				whose location may change after the collapsing itself.
				Release mode.
								
				\param invElems	elements involved in the collapsing
				\return			Id's of the involved data */
			vector<UInt> getDataModifiedInEdgeCollapsing(const vector<UInt> & invElems) const;
			
			/*!	Get data involved in an edge collapsing and 
				whose location may change after the collapsing itself.
				Release mode.
								
				\param invElems	elements involved in the collapsing
				\param invData	data involved in the collapsing
				\return			Id's of the involved data */
			vector<UInt> getDataModifiedInEdgeCollapsing(const vector<UInt> & invElems, 
				const vector<UInt> & invData) const;						
	};
}

/*!	Include definitions of class members. */
#include "implementation/imp_meshInfo.hpp"

#endif

