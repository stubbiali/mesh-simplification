/*! \file	instantiate.cpp
	\brief	Explicitly generate the code for all members of template classes.
			This is useful for debugging. */
			
#include "geoElement.hpp"
#include "bmesh.hpp"
#include "mesh.hpp"

using namespace geometry;

template class geoElement<Triangle>;

template class bmesh<Triangle>;
template class bmesh<Quad>;

template class mesh<Triangle>;
template class mesh<Triangle, MeshType::DATA>;
