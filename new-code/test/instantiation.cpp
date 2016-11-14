/*! \file	instantiate.cpp
	\brief	Explicitly generate the code for all members of template classes.
			This is useful for debugging. */

#include "inc.hpp"
using namespace geometry;

#include "geoElement.hpp"
template class geoElement<Triangle>;

#include "bmesh.hpp"
template class bmesh<Triangle>;
template class bmesh<Quad>;

#include "mesh.hpp"
template class mesh<Triangle>;
template class mesh<Triangle, MeshType::DATA>;
template class mesh<Quad, MeshType::DATA>;

#include "bconnect.hpp"
template class bconnect<Triangle, MeshType::GEO>;
template class bconnect<Triangle, MeshType::DATA>;
template class bconnect<Quad, MeshType::DATA>;
