/*!	\file	bcost.hpp
	\brief	Base class for classes computing the cost associated
			with an edge collapsing. CRTP paradigm is used. */
	
#ifndef HH_BCOST_HH
#define HH_BCOST_HH

#include <unordered_set>

#include "bmeshOperation.hpp"
#include "collapseInfo.hpp"

namespace geometry
{
	/*!	This is a base class for those classes which compute
		the cost associated with and edge collapsing.
		The Curiously Recurring Template Pattern (CRTP) is used.
		Then, this class defines the common interface
		for all its children, delegated to them the implementation
		of some of its members.
		Note that, as required by the CRTP technique, the
		derived class appears as a template parameter of the class. 
		
		All cost classes contain an ordered set of collapseInfo
		objects. Then, it should be fast to find an edge in the list
		based on its end-points. 
		Conversely, the class simplification will contain a set of 
		collapsingEdge objects ascending-ordered according to their 
		costs. Then, in simplification should be fast to find an edge 
		based on its cost.
		In this sense, bcost and simplification are complementary:
		when simplification needs to find an edge given its end-points,
		first it will ask bcost for the cost, then it will look for
		the cost in its list.
		
		\sa collapseInfo.hpp, collapsingInfo.hpp, simplification.hpp */
	template<typename SHAPE, MeshType MT, typename D>
	class bcost
	{
		protected:
			/*!	Pointer to a bmeshOperation object, i.e. the operator. 
				\sa bmeshOperation.hpp */
			bmeshOperation<SHAPE,MT> * oprtr;
			
			/*!	Store info on collapsing edges through
				an unordered set of collapseInfo objects. */
			unordered_set<collapseInfo> cInfoList;
			
		public:
			//
			// Constructors
			//
			
			/*!	(Default) constructor.
				\param bmo	pointer to a bmeshOperation object */
			bcost(bmeshOperation<SHAPE,MT> * bmo = nullptr);
			
			//
			// Set methods
			//
			
			/*!	Set pointer to bmeshOperation.
				The implementation is delegated to the derived class.
				
				\param bmo	pointer to a bmeshOperation object */
			void setMeshOperation(bmeshOperation<SHAPE,MT> * bmo);
			
			//
			// Get methods
			//
			
			/*!	Get list of valid collapsing points for an edge.
				Here, their validity is sanctioned according to 
				the boundary flags of the edge end-points.
				The implementation is delegated to the derived class.
				
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\return		vector of valid points */
			vector<point> getPointsList(const UInt & id1, const UInt & id2) const;
			
			/*!	Get cost for collapsing an edge in a point.
				The implementation is delegated to the derived class.
			
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\param p	collapsing point
				\return		the cost */
			Real getCost(const UInt & id1, const UInt & id2, const point3d & p) const;
			
			/*!	Get list of collapseInfo objects.
				\return		vector of collapseInfo objects */
			vector<collapseInfo> getCollapseInfoList() const;
			
			//
			// Update list
			//
			
			/*!	Add a collapseInfo object to the list.
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\param p	collapsing point
				\param val	collapsing cost */
			void addCollapseInfo(const UInt & id1, const UInt & id2, const point3d & p, 
				const Real & val);
			
			/*!	Erase a collapseInfo object from the list.
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\return		previously computed cost associated with the edge */
			Real eraseCollapseInfo(const UInt & id1, const UInt & id2); 
			
			/*!	Update after an edge collapse.
				This method should be called after having updated the mesh
				and all the connections.
				The implementation is delegated to the derived class.
				Actually, some derived cost classes may not require an update.
				In that case, the implementation would simply do nothing.
				
				\param id	Id of the collapsing point */
			template<typename... Args>
			void update(const UInt & id, Args... args);
	};
}

/*!	Include implementations of class members. */
#include "implementation/imp_bcost.hpp"

#endif
