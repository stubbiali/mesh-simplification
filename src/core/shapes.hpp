#ifndef SHAPES_HPP_
#define SHAPES_HPP_

namespace geometry
{

/*! definizione di p-greco */
#define PGRECO 3.141592653589793238462643383279502884197169399375105820974944592308

/*! Queste classi danno le definizione di base dei tipi delle variabili utilizzate nella libreria, inoltre offre anche la descrizione delle principali forme geometriche utilizzate dalle griglie */

/*! Definizioni delle variabili numeriche utilizzate */
typedef double   Real;
typedef unsigned int  UInt;

/*! Definizioni di enumerazioni che indetificano le varie forme geometriche */
enum ReferenceShapes {NONE, POINT, LINE, TRIANGLE, QUAD, HEXA, PRISM, TETRA};
enum ReferenceGeometry {NULLGEO = 0, VERTEX = 1, EDGE = 2, FACE = 3, VOLUME = 4};

/*! Definizione dei vari tipi di intersezione*/
enum intersectionType{NOTINTERSEC=0, INTERSEC=1, CONSECUTIVE=2, CONTENT=3, COINCIDE=4};

/*! Definizione dei vari tipi di posizione */
enum positionType{OUTSIDE=0, INSIDE=1, ONBOUNDARY=2};

/*! Classi delle principali forme geometriche */
class simplePoint
{
public:
    typedef simplePoint BShape;
    static const ReferenceShapes Shape = POINT; 
    static const ReferenceGeometry Geometry = VERTEX;
    static const UInt nDim = 0;   
    static const UInt numFaces = 0;
    static const UInt numEdges = 0;
    static const UInt numVertices = 1; 
};


class Line
{
public:
    typedef simplePoint BShape;
    static const ReferenceShapes Shape  = LINE;
    static const ReferenceGeometry Geometry = EDGE;
    static const UInt nDim = 1;
    static const UInt numFaces = 0;
    static const UInt numEdges = 1;
    static const UInt numVertices = 2;
    static const UInt numVerticesForEdges = 2;
    static UInt edgeConn[numEdges*numVerticesForEdges];
};


class Triangle
{
public:
    typedef Line BShape;
    static const ReferenceShapes Shape  = TRIANGLE;
    static const ReferenceGeometry Geometry = FACE;
    static const UInt nDim = 2;
    static const UInt numVertices = 3;
    static const UInt numFaces = 1;
    static const UInt numEdges = numVertices;
    static const UInt numVerticesForEdges = 2;
    static const UInt numVerticesForFaces = 3;
    static UInt edgeConn[numEdges*BShape::numVertices];
    static UInt faceConn[numFaces*numVertices];
};

class Quad
{
public:
    typedef Line BShape;
    static const ReferenceShapes Shape = QUAD;
    static const ReferenceGeometry Geometry = FACE;
    static const UInt nDim = 2;
    static const UInt numFaces = 1;
    static const UInt numVertices = 4;
    static const UInt numEdges = numVertices;
    static const UInt numVerticesForEdges = 2;
    static const UInt numVerticesForFaces = 4;
    static UInt edgeConn[numEdges*BShape::numVertices];
    static UInt faceConn[numFaces*numVertices];
};



class Tetra
{
public:
    typedef Triangle BShape;
    static const ReferenceShapes Shape = TETRA;
    static const ReferenceGeometry Geometry = VOLUME;
    static const UInt nDim = 3;
    static const UInt numVertices = 4;
    static const UInt numFaces = 4;
    static const UInt numEdges = numFaces + numVertices - 2;
    static const UInt numVerticesForEdges = 2;
    static const UInt numVerticesForFaces = 3;
    static const UInt numVerticesForVolume = 4;
    static UInt edgeConn[numEdges*Tetra::numVerticesForEdges];
    static UInt faceConn[numFaces*BShape::numVertices];
};


class Hexa
{
public:
    typedef Quad BShape;
    static const ReferenceShapes Shape = HEXA;
    static const ReferenceGeometry Geometry = VOLUME;
    static const UInt nDim = 3;
    static const UInt numFaces = 6;
    static const UInt numVertices = 8;
    static const UInt numVerticesForEdges = 2;
    static const UInt numVerticesForFaces = 4;
    static const UInt numVerticesForVolume = 8;
    static const UInt numEdges = numFaces + numVertices - 2;
    static UInt edgeConn[numEdges*Hexa::numVerticesForEdges];
    static UInt faceConn[numFaces*BShape::numVertices];
};


}

#endif /* SHAPES_HPP_ */
