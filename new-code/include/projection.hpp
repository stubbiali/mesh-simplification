/*!	\file	projection.hpp
	\brief	Class projecting a point on a set of elements. */
	
#ifndef HH_PROJECTION_HH
#define HH_PROJECTION_HH

namespace geometry
{
	/*!	This is a template class projecting a (data) point on a set of
		elements. The template parameter is the shape of the elements. 
		The class provides both a static and a non-static interface. 
		
		At the moment, only the specialization for triangles is
		implemented. Further specializations may be added in future. 
		
		Reference: 
		O’Rourke J. "Computational geometry in C". 
		Cambridge (UK), Cambridge University Press, 1998. */
	template<typename SHAPE = Triangle>
	class projection
	{
	};
	
	/*!	Specialization for triangles. */
	template<>
	class projection<Triangle>
	{
		private:
			/*!	Shared pointer to a mesh. */
			mesh<Triangle, MeshType::DATA> grid;
			
		public:
			//
			// Constructors
			//
			
			/*!	(Default) constructor.
				\param g	shared pointer to a mesh */
			projection(const shared_ptr<bmesh<Triangle>> & g = nullptr);
			
			/*!	Constructor.
				\param args	arguments which will be forwarded to mesh
				
				\sa mesh.hpp */
			template<typename... Args>
			projection(Args... args);
			
			//
			// Get methods
			//
			
			/*!	Get mesh pointer.
				\return		shared pointer to the mesh */
			shared_ptr<mesh<Triangle, MeshType::DATA>> getMesh();
			
			//
			// Interface
			//
			
			
	}
}

#endif
