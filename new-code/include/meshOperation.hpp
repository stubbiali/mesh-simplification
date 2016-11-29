/*!	\file 	meshOperation.hpp
	\brief	Test class specialization for class meshOperation. */
	
#ifndef HH_MESHOPERATION_HH
#define HH_MESHOPERATION_HH

#include <type_traits>

#include "meshInfo.hpp"
#include "projection.hpp"

namespace geometry
{	
	/*!	constexpr function to convert a MeshType object in a bool. 
		\return		TRUE if the template argument is MeshType::GEO,
					FALSE otherwise */
	template<MeshType MT>
	constexpr bool MeshType2Bool();
		
	/*!	Specialization for MeshType::GEO. */
	template<>
	constexpr bool MeshType2Bool<MeshType::GEO>() {return true;};
	
	/*!	Specialization for MeshType::DATA. */
	template<>
	constexpr bool MeshType2Bool<MeshType::DATA>() {return false;};
	
	/*!	A template typedef exploiting type traits. */
	template<typename SHAPE, MeshType MT>
	using BaseClass = typename conditional<MeshType2Bool<MT>(), meshInfo<SHAPE,MT>, projection<SHAPE>>::type;
	
	/*!	The meshOperation class. */
	template<typename SHAPE, MeshType MT>
	class meshOperation : public BaseClass<SHAPE,MT>
	{
		public:
			meshOperation() = default;
	};
}

#endif
