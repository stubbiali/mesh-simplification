#include "shapes.hpp"

namespace geometry {
	
	// Connections for Line
	std::array<UInt, Line::numEdges*Line::numVertices> Line::edgeConn = {0, 1};
	
	
	// Connections for Triangle
	std::array<UInt, Triangle::numEdges*Triangle::numVerticesPerEdge> Triangle::edgeConn = {0, 1, 
											 											  	1, 2, 
											 											  	2, 0};
											 
	std::array<UInt, Triangle::numFaces*Triangle::numVerticesPerFace> Triangle::faceConn = {0, 1, 2};

	
	// Connections for Quad
	std::array<UInt, Quad::numEdges*Quad::numVerticesPerEdge> Quad::edgeConn = {0, 1, 
											 									1, 2, 
											 									2, 3,
											 									3, 0};
											 
	std::array<UInt, Quad::numFaces*Quad::numVerticesPerFace> Quad::faceConn = {0, 1, 2, 3};


	// Connections for Tetra
	std::array<UInt, Tetra::numEdges*Tetra::numVerticesPerEdge> Tetra::edgeConn = {	0, 1, 
											 										1, 2, 
											 										2, 0,
											 										0, 3,
											 										1, 3,
											 										2, 3};
											 
	std::array<UInt, Tetra::numFaces*Tetra::numVerticesPerFace> Tetra::faceConn = {	0, 1, 2, 
																					3, 1, 2,
																					0, 3, 2,
																					0, 1, 3};
	
	
	// Connections for Hexa
	std::array<UInt, Hexa::numEdges*Hexa::numVerticesPerEdge> Hexa::edgeConn = {0, 1,
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

	std::array<UInt, Hexa::numFaces*Hexa::numVerticesPerFace> Hexa::faceConn = {0, 1, 2, 3,
																				0, 1, 5, 4,
																				0, 3, 7, 4,
																				1, 2, 6, 5,
																				2, 3, 7, 6,
																				4, 5, 6, 7};
									 
}	
								 
