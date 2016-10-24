#include "shapes.hpp"

using namespace geometry;

UInt Line::edgeConn[Line::numEdges*Line::numVertices] = {0, 1};

UInt Triangle::edgeConn[Triangle::numEdges*Triangle::BShape::numVertices] = {0, 1, 
									     1, 2, 
									     2, 0};
									     
UInt Triangle::faceConn[Triangle::numFaces*Triangle::numVertices] = {0, 1, 2};

UInt Quad::edgeConn[Quad::numEdges*Quad::BShape::numVertices] = {0, 1, 
								 1, 2, 
								 2, 3, 
								 3, 0};

UInt Quad::faceConn[Quad::numFaces*Quad::numVertices] = {0, 1, 2, 3};

UInt Tetra::edgeConn[Tetra::numEdges*Tetra::numVerticesForEdges] = {0, 1,
								    0, 2, 
								    0, 3, 
								    1, 2, 
								    1, 3, 
								    2, 3};

UInt Tetra::faceConn[Tetra::numFaces*Tetra::BShape::numVertices] = {0, 1, 2,
								    3, 1, 0,
								    0, 2, 3,
								    3, 2, 1};

UInt Hexa::edgeConn[Hexa::numEdges*Hexa::numVerticesForEdges] = {0, 1,
								 1, 2, 
								 2, 3, 
								 3, 0, 
								 0, 4, 
								 1, 5, 
								 2, 6, 
								 3, 7, 
								 4, 5, 
								 5, 6, 
								 6, 7,
								 7, 4};

UInt Hexa::faceConn[Hexa::numFaces*Hexa::BShape::numVertices] = {0, 1, 2, 3,
								 0, 1, 5, 4,
								 0, 3, 7, 4,
								 1, 2, 6, 5,
								 2, 3, 7, 6,
								 4, 5, 6, 7};
		
								 
