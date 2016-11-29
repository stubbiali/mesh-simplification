/*!	\file	main_meshOperation.cpp
	\brief	A small executable testing class meshOperation. */
	
#include "meshOperation.hpp"

int main()
{
	using namespace geometry;
	
	meshOperation<Triangle, MeshType::GEO> mog;
	meshOperation<Triangle, MeshType::DATA> mod;
}
