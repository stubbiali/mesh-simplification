#ifndef MESH3DSEBE_HPP_
#define MESH3DSEBE_HPP_

#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <list>
#include <climits>
#include <stdexcept>
#include <cmath>

#include "../geometry/geoElement.hpp"
#include "../geometry/mesh0d.hpp"
#include "../geometry/mesh1d.hpp"
#include "../geometry/mesh2d.hpp"
#include "../utility/exceptions.hpp"

namespace geometry {

    inline int refinedIndex(const int & coarseIndex, const int & scale, const int & shift) {
        return coarseIndex * scale + shift;
    }

    inline int coarseIndex(const int & refinedIndex, const int & scale, const int & shift) {
        return (refinedIndex - shift) / scale;
    }

    //! Class containing the data structures for a 3d structured mesh, such as those used by SEBE.
    /*! This class contains the lists of elements, faces, edges and nodes of the mesh and how they are connected.
     * It does not, however, provide any methods to verify the conformity of the grid contained.
     * While it is templetized on a generic GEOSHAPE, it only works for elements with 6 faces and 8 nodes.
     * Elements and nodes are arrenged according to three indices (i,j,k)
     *
     * There are two ways to create a mesh:
     * - the first simply involves passing the arrays of coordinates using either the constructor:
     *
     *   - mesh3dSebe(const std::vector < std::vector< std::vector<double> > > &zCoor, const std::vector< std::vector<double> > &xCoor, const std::vector< std::vector<double> > &yCoor, const double & zNil)
     *
     *   or the analogue method:
     *
     *   - createFromCoords(const std::vector < std::vector< std::vector<double> > > &zCoor, const std::vector< std::vector<double> > &xCoor, const std::vector< std::vector<double> > &yCoor, const double & zNil)
     * - the second allows points to be added one by one, but requires connectivities (and j,j,k indices) to be set later:
     *   - add every point using either:
     *     - void insertNode(point p)
     *     or
     *     - void insertNode(std::vector<point> * _nodes);
     *   - createFromConnectivity(const std::vector< std::vector<int> > & elementNodeConnectivity,
     *      const std::vector< std::vector<int> > & elementEdgeConnectivity,
     *      const std::vector< std::vector<int> > & edgeNodeConnectivity,
     *      const std::vector<int> & elementIIndex,
     *      const std::vector<int> & elementJIndex,
     *      const std::vector<int> & elementKIndex,
     *      const std::vector<std::vector<double> > & xCoor,
     *      const std::vector<std::vector<double> > & yCoor
     *      const size_t & numLayers);
     *
     * If the list of faces (and their connectivity) is needed it can be built using:
     * - createFacesConn()
     */
    template<typename GEOSHAPE > class mesh3dSebe
    {
    public:
        //! Reference element
        typedef GEOSHAPE RefShape;

        //! Boundary mesh type
        typedef mesh2d<typename GEOSHAPE::BShape> BoundMesh;

    protected:
        //! Nodes list
        std::vector<point> nodes;

        //! Element to nodes connectivity
        std::vector<geoElement< GEOSHAPE> > elements;

        //! Edges to nodes connecivity
        std::vector<geoElement<typename GEOSHAPE::BShape::BShape> > edges;

        //! Element to edges connectivity
        std::vector<graphItem > elementEdgeConnectivity;

        //! Element to faces connectivity
        std::vector<std::vector <geoElement<typename GEOSHAPE::BShape> > > elementFaceConnectivity;

        //! Faces list
        std::vector< geoElement<typename GEOSHAPE::BShape> > faces;

        //! i,j,k node connectivity
        /*! points_ijk[k][j][i] will contain the id of the node with indices i,j,k */
        std::vector< std::vector< std::vector <int> > > points_ijk;

        //! i,j,k element connectivity
        /*! elements_ijk[k][j][i] will contain the id of the element with indices i,j,k */
        std::vector< std::vector< std::vector <int> > > elements_ijk;

        //! Number of layers
        UInt numLayers;

        //! List of edges along x
        /** The first dimension is in z, the second in y */
        std::vector< std::vector <int> > edges_i;

        //! List of edges along y
        /*! The first dimension is in z, the second in x */
        std::vector< std::vector <int> > edges_j;

        //! List of horizontal faces
        std::vector< std::vector< int > > faces_z;

        //! Vector containing the maximum dimension in I for each layer
        /*! If the layer has n elements in the i direction, the last element I will be n-1,
         * the last node I will be n and maxI will be n */
        UInt nodesMaxI;

        //! Vector containing the maximum dimension in J for each layer
        /*! If the layer has n elements in the j direction, the last element J will be n-1,
         * the last node J will be n and maxJ will be n */
        UInt nodesMaxJ;

        //! Coordinates along the x axis
        /*! X coordinates of the points */
        std::vector<std::vector<double> > coordsX;

        //! Coordinates along the y axis
        /*! Y coordinates of the points */
        std::vector<std::vector<double> > coordsY;

        //! \name Regular mesh info
        /*! This variables are only used for regular meshes */

        //! Is the mesh regular?
        /*! The mesh is regular if the x-y grid is rectilinear */
        bool regular;

        //! Rotation of the grid axis relative to the frame of reference
        double rotation;

        //! Distances between points in the I direction
        std::vector<double> deltaI;
        //! Distances between points in the J direction
        std::vector<double> deltaJ;

        //! Coordinates along the i axis
        /*! X coordinates of the points with respect to the frame of reference
         * centered in the lower left corner and aligned with the grid */
        std::vector<double> coordsI;

        //! Coordinates along the j axis
        /*! Y coordinates of the points with respect to the frame of reference
         * centered in the lower left corner and aligned with the grid */
        std::vector<double> coordsJ;

    public:
        //! \name Constructors
        ///\{

        //! Empty constructor
        mesh3dSebe();

        //! Creates a 3d mesh from a Sebe file containing the horizons
        mesh3dSebe(const std::vector < std::vector< std::vector<double> > > &zCoor, const std::vector< std::vector<double> > &xCoor, const std::vector< std::vector<double> > &yCoor, const double & zNil);

        //! Constructor
        /*!
            \param nV integer used to set the capacity of the node list
            \param nF integer used to set the capacity of the element list */
        mesh3dSebe(UInt _nV, UInt _nF);

        //! Constructor
        /*!
            \param _nZ number of layers
            \param _nF integer used to set the capacity of the element list
            \param nnodes integer used to set the capacity of the node list */
        mesh3dSebe(UInt _nZ, UInt _nF, UInt nnodes);

        //! Contructor using lists
        /*!
            \param _nodes    nodes list;
            \param _elements elements lists; */
        mesh3dSebe(std::vector<point> _nodes, std::vector<geoElement<GEOSHAPE> > _elements);

        //! Copy constructor
        /*!
         \param _mesh3dSebe  3d mesh to be copied */
        mesh3dSebe(const mesh3dSebe &_mesh3dSebe);

        /*! Assignment operator
         \param _mesh3dSebe  3d mesh to be copied */
        mesh3dSebe & operator=(const mesh3dSebe &_mesh3dSebe);
        ///\}

        //! \name Utilities
        ///\{

        //! Clear every list
        /*!
         * Empties every list (elements, nodes, edges, ...)
         * and resets the mesh to its default state (as if created with the empty constructor)
         */
        void clear();

        //! Print informations
        void print();
        ///\}

        //! \name Getters
        /*!
         * \note getNodePointer, getElementPointer and getNodePointer(UInt nodeId)
         * return pointers to the lists so that they can be better edited
         */
        ///\{

        //! Number of nodes
        UInt getNumNodes() const;

        //! Number of elements
        virtual UInt getNumElements() const;

        //! Number of edges
        UInt getNumEdges() const;

        //! Number of layers
        UInt getNumLayers() const {
            return numLayers;
        };

        //! Pointer to the list of nodes
        std::vector<point>* getNodePointer();

        //! Pointer to the list of elements
        std::vector<geoElement<GEOSHAPE> >* getElementPointer();

        //! Pointer to a specific node
        /*!
            \param nodeId node Id */
        point * getNodePointer(UInt nodeId);

        //! Pointer to a specific element
        /*!
            \param elemId element Id */
        geoElement<GEOSHAPE> * getElementPointer(UInt elemId);

        //! i-th node
        /*!
           \param i  node id*/
        const point& getNode(UInt i) const;

        //! i-th element
        /*!
           \param i element id */
        const geoElement<GEOSHAPE>& getElement(UInt i) const;

        //! i-th edge
        /*!
           \param i edge id */
        const geoElement<typename GEOSHAPE::BShape::BShape>& getEdge(UInt i) const;


        //! Returns the nodes connected to the i-th element
        /*!
           \param i element id
           \param[out] connectedNodes pointer to a std::vector that will contain the nodes */
        void getNodeOfElement(UInt i, std::vector<point> * connectedNodes);

        //! Returns a pointer to the list of edges
        std::vector<geoElement<typename GEOSHAPE::BShape::BShape> > * getEdgePointer() {
            return (&edges);
        };

        //! Returns a pointer to the list of faces
        std::vector< geoElement<typename GEOSHAPE::BShape> > * getFacesPointer() {
            return (faces);
        };

        std::vector< std::vector< int > > getFacesZPointer() {
            return faces_z;
        };

        //! Returns a pointer to the list of edges along the I direction
        /*! the first index refers to the z index */
        std::vector< std::vector <int> > getEdgeIPointer() {
            return edges_i;
        };

        //! Returns a pointer to the list of edges along the J direction
        /*! the first index refers to the z index */
        std::vector< std::vector <int> > getEdgeJPointer() {
            return edges_j;
        };

        //! Returns the maximum index I for the nodes
        /*! If the layer has n elements in the i direction, the last element I will be n-1,
         * the last node I will be n and nodesMaxI will be n */
        UInt getNodesMaxI() const {
            return nodesMaxI;
        }

        //!  Returns the maximum index J for the nodes
        /*! If the layer has n elements in the j direction, the last element J will be n-1,
         * the last node J will be n and nodesMaxJ will be n */
        UInt getNodesMaxJ() const {
            return nodesMaxJ;
        }

        //! Returns a reference to the three dimensional std::vector of points ids
        const std::vector < std::vector< std::vector <int> > >& getIjkPoints() const {
            return points_ijk;
        }

        //! Returns a const reference to the three dimensional std::vector of points ids
        std::vector < std::vector< std::vector <int> > >& getIjkPoints() {
            return points_ijk;
        }

        //! Returns a reference to the three dimensional std::vector of elements ids
        const std::vector < std::vector< std::vector <int> > >& getIjkElements() const {
            return elements_ijk;
        }

        //! Returns a const reference to the three dimensional std::vector of elements ids
        std::vector < std::vector< std::vector <int> > >& getIjkElements() {
            return elements_ijk;
        }

        //! Returns a const reference to the two dimensional array of x coordinates
        const std::vector<std::vector<double> > & getXCoordinates() const {return coordsX;};

        //! Returns a reference to the two dimensional array of x coordinates
        std::vector<std::vector<double> > & getXCoordinates() {return coordsX;};

        //! Returns a const reference to the two dimensional array of y coordinates
        const std::vector<std::vector<double> > & getYCoordinates() const {return coordsY;};

        //! Returns a reference to the two dimensional array of y coordinates
        std::vector<std::vector<double> > & getYCoordinates() {return coordsY;};

        virtual std::vector<double> getNodesXCoordinate();

        virtual std::vector<double> getNodesYCoordinate();

        virtual std::vector<double> getNodesZCoordinate();

        virtual std::vector< std::vector<UInt> > getElementConnectivity();

        virtual std::vector< std::vector<UInt> > getElementEdgeConnectivity();

        virtual std::vector< std::vector<UInt> > getEdgeConnectivity();

        //! Number of elements in each layer
        /*! \param[out] _elementLayer will be filled with the number of elements in each layer */
        void elementLayer(std::vector<UInt>& _elementLayer);

        //! Number of nodes in each layer
        /*! this includes both the nodes on the top of the layer and those on the bottom
         * \param[out] _nodeLayer will be filled with the number of nodes in each layer */
        void nodeLayer(std::vector<UInt>& _nodeLayer);

        ///\}


        //! \name Insert nodes elements, and edges
        ///\{

        //! Add a node to the list of nodes
        /*! \param p point */
        void insertNode(point p);

        //! Add a list of nodes
        /*! \param _nodes pointer to the list of nodes */
        void insertNode(std::vector<point> * _nodes);

        //! Add an element
        /*! \param element element to add */
        void insertElement(geoElement<GEOSHAPE> element);

        //! Add a std::vector of elements
        /*! \param _element pointer to the std::vector of elements to add */
        void insertElement(std::vector<geoElement<GEOSHAPE> > * _element);

        //! Add a set of elements
        /*! \param _element pointer to the set of elements to add */
        void insertElement(set<geoElement<GEOSHAPE> > * _element);

        //! Add an edge
        /*! \param edge edge to add */
        void insertEdge(geoElement<typename GEOSHAPE::BShape::BShape> edge);

        //! Create the elements-to-nodes connectivity
        /*! \param elementConnectivity std::vector containing the connectivity read from a sebe hdf5 file
         * \param elementIIndex
         * \param elementJIndex
         * \param elementKIndex */
        void createElementNodesConn(std::vector< std::vector<int> > elementConnectivity, std::vector<int> elementIIndex, std::vector<int> elementJIndex, std::vector<int> elementKIndex);

        //! Create the elements-to-edges connectivity
        /*! \param elementToEdgeConnectivity std::vector containing the connectivity read from a sebe hdf5 file */
        void createElementEdgesConn(std::vector< std::vector<int> > elementToEdgeConnectivity);

        //! Create the edges-to-nodes connectivity
        /*! \param edgesConnectivity std::vector containing the connectivity read from a sebe hdf5 file */
        void createEdgesNodesConn(std::vector< std::vector<int> > edgesConnectivity);

        //! Create the elements-to-faces connectivity
        void createFacesConn();

        //! This method actually creates most of the structures used to store the mesh
        /*! \param elementConnectivity std::vector containing the connectivity read from a sebe hdf5 file
         * \param ni number of nodes in i
         * \param nj number of nodes in j
         * \param nk number of nodes in k
         *
         * \warning this method requires nodes to be already added (e.g. using insertNode(...) ) and element connectivity to be set (using createElementNodesConn(...) ) */
        void setIjkPoints(const size_t & ni, const size_t & nj, const size_t & nk);

        //! Creates a 3d mesh from a 3D array of coordinates
        void createFromCoords(const std::vector < std::vector< std::vector<double> > > &zCoor, const std::vector< std::vector<double> > &xCoor, const std::vector< std::vector<double> > &yCoor, const double & zNil);

        //! Creates a 3d mesh from the connectivities of nodes and elements
        /*!
         * \warning points must have been previously inserted
         */
        void createFromConnectivity(const std::vector< std::vector<int> > & elementNodeConnectivity,
            const std::vector< std::vector<int> > & elementEdgeConnectivity,
            const std::vector< std::vector<int> > & edgeNodeConnectivity,
            const std::vector<int> & elementIIndex,
            const std::vector<int> & elementJIndex,
            const std::vector<int> & elementKIndex,
            const std::vector<std::vector<double> > & xCoor,
            const std::vector<std::vector<double> > & yCoor,
            const size_t & numLayers);

        //! Setup all parameters associated with a regular mesh
        /*! If the mesh is regular we can set a deltaX and deltaY
         * as well as the rotation of the wesh w.r.t. the x and y axis */
        void setupRegularMesh();

        ///\}

        //! \name Setters
        ///\{
        /*! Settaggio degli Id dei nodi e degli elementi in modo conforme all'ordinamento nei vettori nodes e elements*/
        void setUpIds();

        /*! Settaggio dei geoId con un valore fissato
            \param geo valore del geoId*/
        void setUpGeoId(UInt geo);

        /*! Settaggio dei geoId con una mappa
            \param oldToNew mappa che dice come cambiare i geoId */
        void setUpGeoId(map<UInt, UInt> oldToNew);
        ///\}

        //! \name Mesh generators
        ///\{

        /*! Metodo che si limita a creare la mesh1d di tutti i lati della mesh3d
            \param facce puntatore alla mesh id che conterrà la mesh2d */
        void createMesh2d(mesh2d<typename GEOSHAPE::BShape> * faces);

        /*! Metodo che si limita a creare la mesh1d di tutti i lati della mesh3d
            \param reticolo puntatore alla mesh id che conterrà la mesh1d */
        void createMesh1d(mesh1d<typename GEOSHAPE::BShape::BShape> * reticolo);

        /*! Metodo che si limita a creare la mesh0d di tutti i nodi della mesh
            \param cloud puntatore alla mesh id che conterrà la mesh0d */
        void createMesh0d(mesh0d<typename GEOSHAPE::BShape::BShape::BShape> * cloud);
        ///\}

        //! \name methods that perform simple analysis
        ///\{

        /*! Metodo che calcola il massimo e il minimo del bbox
            \param pMax punto che da il massimo
            \param pMin punto che da il minimo */
        void createBBox(point & pMax, point & pMin);

        /*! Metodo che calcola il massimo e il minimo del bbox di un elemento specifico
            \param elemId identificatore dell'elemento
            \param pMax punto che da il massimo
            \param pMin punto che da il minimo */
        void createBBox(UInt elemId, point & pMax, point & pMin);

        /*! Metodo che calcola l'edge più corto*/
        Real minH();

        /*! Metodo che calcola l'edge più lungo*/
        Real maxH();
        ///\}

        //! \name Interpolation methods
        ///\{

        void extractVertexSolution(std::vector< std::vector<double> >& ElementWiseVertexSolution, std::vector<double>& VertexSolution);

        std::vector< int > computeVertexIndexI() const;
        std::vector< int > computeVertexIndexJ() const;
        std::vector< int > computeVertexIndexK() const;

        std::vector< int > computeCellCenteredIndexI() const;
        std::vector< int > computeCellCenteredIndexJ() const;
        std::vector< int > computeCellCenteredIndexK() const;

        template<typename TYPE> std::vector< TYPE > interpolateCellCenteredSolution(const std::vector< TYPE > &coarseField, const int& scaleX, const int& scaleY, const int& shiftX, const int& shiftY, const std::vector<std::vector<std::vector<int > > > & coarsele_ijk) const;
        std::vector< double > interpolateCellCenteredSolution(std::vector< double > &field, const int& scaleX, const int& scaleY, const int& shiftX, const int& shiftY, std::vector<geoElement< GEOSHAPE> > coarsele, std::vector<point> coarsepoint, std::vector< std::vector< std::vector <int> > > coarse_ijk);

        std::vector< std::vector<Real> > interpolateElementWiseVertexSolution(std::vector < std::vector< double > >& field, const int& scaleX, const int& scaleY, const int& shiftX, const int& shiftY, std::vector<geoElement< GEOSHAPE> >& coarsele, std::vector<point>& coarsepoint, const std::vector < std::vector< std::vector <int> > >& coarsePointIJK);

        int getPointIJK(const std::vector< std::vector< std::vector<int > > > & pointsIJK, int i, int j, int k) const;
        double interpolateVertexUsingNeighbors(const UInt& vertexIndex, const std::vector< double > & interpolatedfield, const int& scaleX, const int& scaleY, const int& shiftX, const int& shiftY, const std::vector < std::vector< std::vector <int> > >& coarsePointIJK) const;
        std::vector< double > interpolateVertexSolution(std::vector< double > &field, const int& scaleX, const int& scaleY, const int& shiftX, const int& shiftY, std::vector<point>& coarsepoint, const std::vector<std::vector<std::vector<int> > > & coarsePointIJK);
        std::vector< UInt > pinchedPoints(const UInt coarsePointIndex, const std::vector<std::vector<std::vector<int> > > & coarsePointIJK, const UInt coarseIndexI, const UInt coarseIndexJ, const UInt coarseIndexK) const;

        std::vector< std::vector<double> > interpolateTwentyNodesHexSolution(std::vector< double > &field, const int& scaleX, const int& scaleY, const int& shiftX, const int& shiftY, std::vector<point>& coarsepoint, const std::vector<std::vector<std::vector<int> > > & coarsePointIJK);

        static void interpolateCellHorizonSolution(const std::vector<std::vector<double> > & coarseHorizonField, const int& scaleX, const int& scaleY, const int& shiftX, const int& shiftY, std::vector< std::vector<double > > & refinedHorizonField, const size_t refinedCellISize, const size_t refinedCellJSize);
        static void interpolateVertexHorizonSolution(const std::vector<std::vector<double> > & coarseCoordsX, const std::vector<std::vector<double> > & coarseCoordsY, const std::vector<std::vector<double> > & coarseField, const std::vector<std::vector<double> > & refinedCoordsX, const std::vector<std::vector<double> > & refinedCoordsY, const int& scaleX, const int& scaleY, const int& shiftX, const int& shiftY, std::vector< std::vector<double > > & refinedField);


        //! Creates a vector of points from their coordinates
        /*!
         * \param[in] xCoords x coordinates
         * \param[in] yCoords y coordinates
         * \param[out] points a single vector of points built from the input coordinates
         * \warning The dimensions of the two input vectors (and the vectors therein) has to match. This method doesn't check for this
         */
        static void getPointsVector(const std::vector<std::vector<double> > & xCoords, const std::vector<std::vector<double> > & yCoords, std::vector<point> & points);
        //! Creates a single vector of values from a vector of vectors
        /*!
         * \param[in] values two dimesional vector of values
         * \param[out] arrayValues a single vector of values
         * \warning The dimensions of the two input vectors (and the vectors therein) has to match. This method doesn't check for this
         */
        static void getValuesVector(const std::vector<std::vector<double> > &values, std::vector<double> &arrayValues);

        //! Interpolate a vector of values defined in different points
        /*!
         * \param _Coor the first point in this vector is the point we want to interpolate on, the others are the points associated to _value
         * \param _value values defined in _Coor
         */
        static Real interpolate(std::vector < point >& _Coor, std::vector<Real>& _value);

        ///\}


    };

    //-------------------------------------------------------------------------------------------------------
    // IMPLEMENTATION
    //-------------------------------------------------------------------------------------------------------

    //
    // Constructors
    //

    template<typename GEOSHAPE> mesh3dSebe<GEOSHAPE>::mesh3dSebe() :
        nodes(0),
        elements(0),
        edges(0),
        elementEdgeConnectivity(0),
        elementFaceConnectivity(0),
        faces(0),
        points_ijk(0),
        elements_ijk(0),
        numLayers(0),
        edges_i(0),
        edges_j(0),
        faces_z(0),
        nodesMaxI(0),
        nodesMaxJ(0),
        coordsX(0),
        coordsY(0),
        regular(false),
        rotation(0),
        deltaI(0),
        deltaJ(0),
        coordsI(0),
        coordsJ(0)
    {
    }

    template<typename GEOSHAPE> mesh3dSebe<GEOSHAPE>::mesh3dSebe(UInt nV, UInt nF) :
        nodes(0),
        elements(0),
        edges(0),
        elementEdgeConnectivity(0),
        elementFaceConnectivity(0),
        faces(0),
        points_ijk(0),
        elements_ijk(0),
        numLayers(0),
        edges_i(0),
        edges_j(0),
        faces_z(0),
        nodesMaxI(0),
        nodesMaxJ(0),
        coordsX(0),
        coordsY(0),
        regular(false),
        rotation(0),
        deltaI(0),
        deltaJ(0),
        coordsI(0),
        coordsJ(0)
    {
        nodes.reserve(nV);
        elements.reserve(nF);
        elementEdgeConnectivity.reserve(nF);
        elementFaceConnectivity.reserve(nF);
    }

    template<typename GEOSHAPE> mesh3dSebe<GEOSHAPE>::mesh3dSebe(std::vector<point> _nodes, std::vector<geoElement<GEOSHAPE> > _elements) :
        nodes(_nodes),
        elements(_elements),
        edges(0),
        elementEdgeConnectivity(0),
        elementFaceConnectivity(0),
        faces(0),
        points_ijk(0),
        elements_ijk(0),
        numLayers(0),
        edges_i(0),
        edges_j(0),
        faces_z(0),
        nodesMaxI(0),
        nodesMaxJ(0),
        coordsX(0),
        coordsY(0),
        regular(false),
        rotation(0),
        deltaI(0),
        deltaJ(0),
        coordsI(0),
        coordsJ(0)
    {
        elementEdgeConnectivity.reserve(_elements.size());
        elementFaceConnectivity.reserve(_elements.size());
    }

    template<typename GEOSHAPE> mesh3dSebe<GEOSHAPE>::mesh3dSebe(const mesh3dSebe &_mesh3dSEBE) :
        nodes(_mesh3dSEBE.nodes),
        elements(_mesh3dSEBE.elements),
        edges(_mesh3dSEBE.edges),
        elementEdgeConnectivity(_mesh3dSEBE.elementEdgeConnectivity),
        elementFaceConnectivity(_mesh3dSEBE.elementFaceConnectivity),
        faces(_mesh3dSEBE.faces),
        points_ijk(_mesh3dSEBE.points_ijk),
        elements_ijk(_mesh3dSEBE.elements_ijk),
        numLayers(_mesh3dSEBE.numLayers),
        edges_i(_mesh3dSEBE.edges_i),
        edges_j(_mesh3dSEBE.edges_j),
        faces_z(_mesh3dSEBE.faces_z),
        nodesMaxI(_mesh3dSEBE.nodesMaxI),
        nodesMaxJ(_mesh3dSEBE.nodesMaxJ),
        coordsX(_mesh3dSEBE.coordsX),
        coordsY(_mesh3dSEBE.coordsY),
        regular(_mesh3dSEBE.regular),
        rotation(_mesh3dSEBE.rotation),
        deltaI(_mesh3dSEBE.deltaI),
        deltaJ(_mesh3dSEBE.deltaJ),
        coordsI(_mesh3dSEBE.coordsI),
        coordsJ(_mesh3dSEBE.coordsJ)
    {
    }

    template<typename GEOSHAPE> mesh3dSebe<GEOSHAPE>::mesh3dSebe(const std::vector < std::vector< std::vector<double> > > &zCoor,
        const std::vector< std::vector<double> > &xCoor,
        const std::vector< std::vector<double> > &yCoor,
        const double & zNil)
    {
        createFromCoords(zCoor, xCoor, yCoor, zNil);
    }

    template<typename GEOSHAPE>
    void mesh3dSebe<GEOSHAPE>::createFromCoords(const std::vector < std::vector< std::vector<double> > > &zCoor,
        const std::vector< std::vector<double> > &xCoor,
        const std::vector< std::vector<double> > &yCoor,
        const double & zNil)
    {
        using std::vector;

        // Store x and y coordinates so that we can easily look them up later
        coordsX = xCoor;
        coordsY = yCoor;

        // Every layer of the mesh is rectangular and has the same dimensions
        nodesMaxI = zCoor[0][0].size() - 1;
        nodesMaxJ = zCoor[0].size() - 1;

        // Reserve enough space for all the fectors
        // (This allocates memory even for elements/nodes/edges that don't exist,
        // but it speeds up the mesh creation a lot)
        elements.reserve( (zCoor.size() - 1) * (nodesMaxI) * (nodesMaxJ) );
        nodes.reserve( (zCoor.size()) * (nodesMaxI + 1) * (nodesMaxJ + 1) );
        edges.reserve( (zCoor.size() - 1) * (nodesMaxI + 1) * (nodesMaxJ + 1) + (zCoor.size()) * (nodesMaxI) * (nodesMaxJ + 1) + (zCoor.size()) * (nodesMaxI + 1) * (nodesMaxJ) );
        elementEdgeConnectivity.reserve( (zCoor.size() - 1) * (nodesMaxI) * (nodesMaxJ) );

        // allocating the right size of vectors
        points_ijk.resize(zCoor.size());
        elements_ijk.resize(zCoor.size()-1);

        for (UInt k = 0; k < zCoor.size(); ++k)
        {
            points_ijk[k].resize(zCoor[k].size());
            for (UInt j = 0; j < zCoor[k].size(); ++j)
            {
                points_ijk[k][j].resize(zCoor[k][j].size());
                for (UInt i = 0; i < zCoor[k][j].size(); ++i)
                {
                    points_ijk[k][j][i] = -1;
                }
            }
        }

        for (UInt k = 0; k < zCoor.size()-1; ++k)
        {
            elements_ijk[k].resize(zCoor[k].size()-1);
            for (UInt j = 0; j < zCoor[k].size()-1; ++j)
            {
                elements_ijk[k][j].resize(zCoor[k][j].size()-1);
                for (UInt i = 0; i < zCoor[k][j].size()-1; ++i)
                {
                    elements_ijk[k][j][i] = -1;
                }
            }
        }

        // First we check where element will be created, so that we can
        // later add only those points that are actually used
        std::vector<bool> isNodeUsed(nodes.size(), false);
        std::vector<std::vector<std::vector<bool> > > isPointUsed(zCoor.size());
        for (UInt k = 0; k < zCoor.size(); ++k) {
            isPointUsed[k].resize(zCoor[k].size());
            for (UInt j = 0; j < zCoor[k].size(); ++j) {
                isPointUsed[k][j].resize(zCoor[k][j].size());
                for (UInt i = 0; i < zCoor[k][j].size(); ++i) {
                    isPointUsed[k][j][i] = false;
                }
            }
        }

        for (UInt k = 0; k < zCoor.size() - 1; ++k)
        {
            for (UInt j = 0; j < zCoor[0].size() - 1; ++j)
            {
                for (UInt i = 0; i < zCoor[0][0].size() - 1; ++i)
                {
                    const double& z1 = zCoor[k][j][i];
                    const double& z2 = zCoor[k][j][i + 1];
                    const double& z3 = zCoor[k][j + 1][i + 1];
                    const double& z4 = zCoor[k][j + 1][i];

                    const double& z5 = zCoor[k + 1][j][i];
                    const double& z6 = zCoor[k + 1][j][i + 1];
                    const double& z7 = zCoor[k + 1][j + 1][i + 1];
                    const double& z8 = zCoor[k + 1][j + 1][i];

                    if (z1 != zNil && z2 != zNil && z3 != zNil && z4 != zNil && z5 != zNil && z6 != zNil && z7 != zNil && z8 != zNil)
                    {
                        isPointUsed[k][j][i] = true;
                        isPointUsed[k][j][i+1] = true;
                        isPointUsed[k][j+1][i+1] = true;
                        isPointUsed[k][j+1][i] = true;
                        isPointUsed[k+1][j][i] = true;
                        isPointUsed[k+1][j][i+1] = true;
                        isPointUsed[k+1][j+1][i+1] = true;
                        isPointUsed[k+1][j+1][i] = true;
                    }
                }
            }
        }

        // Create the list of points and their connectivity matrix
        numLayers = zCoor.size() - 1;
        UInt counter(0);
        for (UInt k = 0; k < zCoor.size(); ++k)
        {
            for (UInt j = 0; j < zCoor[k].size(); ++j)
            {
                for (UInt i = 0; i < zCoor[k][j].size(); ++i)
                {
                    if (zCoor[k][j][i] != zNil)
                    {
                        point p1(xCoor[j][i], yCoor[j][i], zCoor[k][j][i]);
                        p1.setId(counter);
                        p1.setI(i);
                        p1.setJ(j);
                        p1.setK(k);
                        // If this point is pinched out we'll use the same node as the one below
                        if (k>0 && zCoor[k][j][i] == zCoor[k-1][j][i])
                        {
                            points_ijk[k][j][i] = points_ijk[k-1][j][i];
                        }
                        else if (isPointUsed[k][j][i])
                        {
                            nodes.push_back(p1);
                            points_ijk[k][j][i] = p1.getId();
                            counter++;
                        }
                    }
                }
            }
        }

        // We store the edges along the i,j,k directions so that we can easily know if an edge has already been added
        std::vector<std::vector<std::vector<int> > > edgesI(zCoor.size(), std::vector<std::vector<int> >(zCoor[0].size(), std::vector<int>(zCoor[0][0].size()-1, -1) ) );
        std::vector<std::vector<std::vector<int> > > edgesJ(zCoor.size(), std::vector<std::vector<int> >(zCoor[0].size()-1, std::vector<int>(zCoor[0][0].size(), -1) ) );
        std::vector<std::vector<std::vector<int> > > edgesK(zCoor.size()-1, std::vector<std::vector<int> >(zCoor[0].size(), std::vector<int>(zCoor[0][0].size(), -1) ) );

        // Create the list of elements and their connectivity matrix
        counter = 0;
        UInt edgeCounter(0);
        for (UInt k = 0; k < zCoor.size()-1; ++k)
        {
            for (UInt j = 0; j < zCoor[k].size()-1; ++j)
            {
                for (UInt i = 0; i < zCoor[k][j].size()-1; ++i)
                {
                    int p[8];
                    p[0] = points_ijk[k][j][i];
                    p[1] = points_ijk[k][j][i + 1];
                    p[2] = points_ijk[k][j + 1][i + 1];
                    p[3] = points_ijk[k][j + 1][i];

                    p[4] = points_ijk[k + 1][j][i];
                    p[5] = points_ijk[k + 1][j][i + 1];
                    p[6] = points_ijk[k + 1][j + 1][i + 1];
                    p[7] = points_ijk[k + 1][j + 1][i];

                    if (p[0] >= 0 && p[1] >= 0 && p[2] >= 0 && p[3] >= 0 && p[4] >= 0 && p[5] >= 0 && p[6] >= 0 && p[7] >= 0)
                    {
                        if (p[0] != p[4] || p[1] != p[5] || p[2] != p[6] || p[3] != p[7])
                        {
                            geoElement<GEOSHAPE> element;
                            element.setConnectedId(0, p[0]);
                            element.setConnectedId(1, p[1]);
                            element.setConnectedId(2, p[2]);
                            element.setConnectedId(3, p[3]);
                            element.setConnectedId(4, p[4]);
                            element.setConnectedId(5, p[5]);
                            element.setConnectedId(6, p[6]);
                            element.setConnectedId(7, p[7]);
                            element.setId(counter);
                            element.setI(i);
                            element.setJ(j);
                            element.setK(k);
                            elements.push_back(element);
                            ++counter;
                            elements_ijk[k][j][i] = element.getId();
                        }

                        // We need to update edgesI, edgesJ and edgesK even if
                        // the element is pinched-out

                        geoElement<Hexa::BShape::BShape> edge;
                        graphItem newElementEdgeConnectivity(12);
                        std::vector<std::vector<std::vector<int> > >* edgeListPtr;
                        int edgeNodeConnectivity[12][2] = { {0, 1},
                                                        {1, 2},
                                                        {2, 3},
                                                        {3, 0},
                                                        {4, 5},
                                                        {5, 6},
                                                        {6, 7},
                                                        {7, 4},
                                                        {0, 4},
                                                        {1, 5},
                                                        {2, 6},
                                                        {3, 7} };

                        for (int e(0); e<12; ++e)
                        {
                            // We want to know if the edge lies along the i, j or k direction
                            if (e<8 && e%2==0)
                            {
                                edgeListPtr = &edgesI;
                            }
                            else if (e<8 && e%2==1)
                            {
                                edgeListPtr = &edgesJ;
                            }
                            else
                            {
                                edgeListPtr = &edgesK;
                            }
                            // We want to know if we have to look inside edgeListPtr
                            // for the edge in i or in i+1 (and the same for j and k)
                            int ii = (e==1 || e==5 || e==9 || e==10) ? 1 : 0;
                            int jj = (e==2 || e==6 || e==10 || e==11) ? 1 : 0;
                            int kk = (e==4 || e==5 || e==6 || e==7) ? 1 : 0;

                            // If this edge hasn't already been added
                            if ((*edgeListPtr)[k+kk][j+jj][i+ii] < 0)
                            {
                                // We want to find whether there's a pinch-out (unless k+kk==0)
                                if ( k+kk == 0 ||
                                    ( elements_ijk[k][j][i] >= 0 && // if there's no element there's definately a pinch-out
                                    ( (*edgeListPtr)[k+kk-1][j+jj][i+ii] < 0 ||
                                    ( (*edgeListPtr)[k+kk-1][j+jj][i+ii] >= 0 && // check if there's an edge below this one
                                      // and check if it has the same nodes as the one we are about to add
                                      // (considering that connected nodes are not oredered could be swapped)
                                      ( int(edges[(*edgeListPtr)[k+kk-1][j+jj][i+ii]].getConnectedId(0)) != p[edgeNodeConnectivity[e][0]] ||
                                        int(edges[(*edgeListPtr)[k+kk-1][j+jj][i+ii]].getConnectedId(1)) != p[edgeNodeConnectivity[e][1]] ) &&
                                      ( int(edges[(*edgeListPtr)[k+kk-1][j+jj][i+ii]].getConnectedId(0)) != p[edgeNodeConnectivity[e][1]] ||
                                        int(edges[(*edgeListPtr)[k+kk-1][j+jj][i+ii]].getConnectedId(1)) != p[edgeNodeConnectivity[e][0]] ) ) ) )
                                    )

                                {
                                    edge.setConnectedId(0, p[edgeNodeConnectivity[e][0]]);
                                    edge.setConnectedId(1, p[edgeNodeConnectivity[e][1]]);
                                    edge.setId(edgeCounter);
                                    edges.push_back(edge);
                                    (*edgeListPtr)[k+kk][j+jj][i+ii] = edgeCounter;
                                    ++edgeCounter;
                                }
                                else // There is a pinch-out
                                {
                                    (*edgeListPtr)[k+kk][j+jj][i+ii] = (*edgeListPtr)[k+kk-1][j+jj][i+ii];
                                }
                            }
                            newElementEdgeConnectivity.setConnectedId(e, (*edgeListPtr)[k+kk][j+jj][i+ii]);
                        }
                        if (elements_ijk[k][j][i] >= 0)
                        {
                            elementEdgeConnectivity.push_back(newElementEdgeConnectivity);
                        }

                    }
                }
            }
        }

        setupRegularMesh();

    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::createFromConnectivity(const std::vector< std::vector<int> > & elementNodeConnectivity,
        const std::vector< std::vector<int> > & elementEdgeConnectivity,
        const std::vector< std::vector<int> > & edgeNodeConnectivity,
        const std::vector<int> & elementIIndex,
        const std::vector<int> & elementJIndex,
        const std::vector<int> & elementKIndex,
        const std::vector<std::vector<double> > & xCoor,
        const std::vector<std::vector<double> > & yCoor,
        const size_t & numLayers)
    {
        coordsX = xCoor;
        coordsY = yCoor;
        createElementNodesConn(elementNodeConnectivity, elementIIndex, elementJIndex, elementKIndex);
        setIjkPoints(xCoor[0].size(), xCoor.size(), numLayers + 1);
        createElementEdgesConn(elementEdgeConnectivity);
        createEdgesNodesConn(edgeNodeConnectivity);
        setupRegularMesh();
    }

    template<typename GEOSHAPE> mesh3dSebe<GEOSHAPE> & mesh3dSebe<GEOSHAPE>::operator=(const mesh3dSebe &_mesh3dSEBE)
    {
        nodes = _mesh3dSEBE.nodes;
        elements = _mesh3dSEBE.elements;
        edges = _mesh3dSEBE.edges;
        elementEdgeConnectivity = _mesh3dSEBE.elementEdgeConnectivity;
        elementFaceConnectivity = _mesh3dSEBE.elementFaceConnectivity;
        faces = _mesh3dSEBE.faces;
        edges_i = _mesh3dSEBE.edges_i;
        edges_j = _mesh3dSEBE.edges_j;
        points_ijk = _mesh3dSEBE.points_ijk;
        elements_ijk = _mesh3dSEBE.elements_ijk;
        numLayers = _mesh3dSEBE.numLayers;
        faces_z = _mesh3dSEBE.faces_z;
        nodesMaxI = _mesh3dSEBE.nodesMaxI;
        nodesMaxJ = _mesh3dSEBE.nodesMaxJ;
        coordsX = _mesh3dSEBE.coordsX;
        coordsY = _mesh3dSEBE.coordsY;
        rotation = _mesh3dSEBE.rotation;
        regular = _mesh3dSEBE.regular;
        coordsI = _mesh3dSEBE.coordsI;
        coordsJ = _mesh3dSEBE.coordsJ;
        deltaI = _mesh3dSEBE.deltaI;
        deltaJ = _mesh3dSEBE.deltaJ;
        return *this;
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::clear()
    {
        nodes.clear();
        elements.clear();
        edges.clear();
        elementEdgeConnectivity.clear();
        elementFaceConnectivity.clear();
        faces.clear();
        edges_i.clear();
        edges_j.clear();
        points_ijk.clear();
        elements_ijk.clear();
        numLayers=0;
        faces_z.clear();
        nodesMaxI=0;
        nodesMaxJ=0;
        elements.clear();
        coordsX.clear();
        coordsY.clear();
        rotation=0;
        regular=false;
        coordsI.clear();
        coordsJ.clear();
        deltaI.clear();
        deltaJ.clear();
    }

    //
    // Get generic information
    //

    template<typename GEOSHAPE> inline UInt mesh3dSebe<GEOSHAPE>::getNumNodes() const {
        return (nodes.size());
    }

    template<typename GEOSHAPE> inline UInt mesh3dSebe<GEOSHAPE>::getNumElements() const {
        return (elements.size());
    }

    template<typename GEOSHAPE> inline UInt mesh3dSebe<GEOSHAPE>::getNumEdges() const {
        return (edges.size());
    }


    //
    // Get node, element ecc
    //

    template<typename GEOSHAPE> inline std::vector<point>* mesh3dSebe<GEOSHAPE>::getNodePointer() {
        return &nodes;
    }

    template<typename GEOSHAPE> inline std::vector<geoElement<GEOSHAPE> >* mesh3dSebe<GEOSHAPE>::getElementPointer() {
        return &elements;
    }

    template<typename GEOSHAPE> inline point * mesh3dSebe<GEOSHAPE>::getNodePointer(UInt nodeId) {
        assert(nodeId < nodes.size());
        return (&nodes[nodeId]);
    }

    template<typename GEOSHAPE> inline geoElement<GEOSHAPE> * mesh3dSebe<GEOSHAPE>::getElementPointer(UInt elemId) {
        assert(elemId < elements.size());
        return (&elements[elemId]);
    }

    template<typename GEOSHAPE> inline const point& mesh3dSebe<GEOSHAPE>::getNode(UInt i) const {
        assert(i < nodes.size());
        return (nodes[i]);
    }

    template<typename GEOSHAPE> inline const geoElement<GEOSHAPE>& mesh3dSebe<GEOSHAPE>::getElement(UInt i) const {
        assert(i < elements.size());
        return (elements[i]);
    }

    template<typename GEOSHAPE> inline const geoElement<typename GEOSHAPE::BShape::BShape>& mesh3dSebe<GEOSHAPE>::getEdge(UInt i) const {
        assert(i < edges.size());
        return (edges[i]);
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::getNodeOfElement(UInt i, std::vector<point> * connessi) {
        assert(i < elements.size());

        // resize connessi
        connessi->clear();
        connessi->resize(GEOSHAPE::numVertices);

        for (UInt j = 0; j < GEOSHAPE::numVertices; ++j) connessi->at(j) = nodes[elements[i].getConnectedId(j)];
    }

    //
    // Insert node e elements
    //

    template<typename GEOSHAPE> inline void mesh3dSebe<GEOSHAPE>::insertNode(point p) {
        p.setId(nodes.size());
        nodes.push_back(p);
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::insertNode(std::vector<point> * _nodes) {
        // clear the list of nodes
        nodes.clear();

        // resize the vector
        nodes.resize(_nodes->size());

        // copy new nodes into the vector
        copy(_nodes->begin(), _nodes->end(), nodes.begin());

        // set IDs
        for (UInt i = 0; i < nodes.size(); ++i) nodes[i].setId(i);
    }

    template<typename GEOSHAPE> inline void mesh3dSebe<GEOSHAPE>::insertElement(geoElement<GEOSHAPE> element) {
        element.setId(elements.size());
        elements.push_back(element);
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::insertElement(std::vector<geoElement<GEOSHAPE> > * _elements) {
        // clear the list of elements
        elements.clear();

        // resize the vector
        elements.resize(_elements->size());

        // copy new elements into the vector
        copy(_elements->begin(), _elements->end(), elements.begin());

        // set IDs
        for (UInt i = 0; i < elements.size(); ++i) elements[i].setId(i);
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::insertElement(set<geoElement<GEOSHAPE> > * _elements) {
        // clear the list of elements
        elements.clear();

        // resize the vector
        elements.resize(_elements->size());

        // copy new elements into the vector
        copy(_elements->begin(), _elements->end(), elements.begin());

        // set IDs
        for (UInt i = 0; i < elements.size(); ++i) elements[i].setId(i);
    }

    //
    // Settings
    //

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::setUpIds() {
        // set nodes and elements ids so that they reflect their position inside the vector
        for (UInt i = 0; i < nodes.size(); ++i) nodes[i].setId(i);
        for (UInt i = 0; i < elements.size(); ++i) elements[i].setId(i);
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::setUpGeoId(UInt geo) {
        for (UInt i = 0; i < elements.size(); ++i) elements[i].setGeoId(geo);
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::setUpGeoId(map<UInt, UInt> oldToNew) {
        // used variable
        UInt geo;

        for (UInt i = 0; i < elements.size(); ++i) {
            // take its old geoId
            geo = elements[i].getGeoId();

            // swap it
            elements[i].setGeoId(oldToNew[geo]);
        }
    }

    //
    // methods to perform simple analysis
    //

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::createBBox(point & pMax, point & pMin) {
        // check wether the mesh contains any pooints
        assert(nodes.size() != 0);

        // set variables
        pMax = nodes[0];
        pMin = nodes[0];

        // find min and max
        for (UInt i = 1; i < nodes.size(); ++i) {
            // set max
            pMax.setX(std::max(nodes[i].getX(), pMax.getX()));
            pMax.setY(std::max(nodes[i].getY(), pMax.getY()));
            pMax.setZ(std::max(nodes[i].getZ(), pMax.getZ()));

            // set min
            pMin.setX(std::min(nodes[i].getX(), pMin.getX()));
            pMin.setY(std::min(nodes[i].getY(), pMin.getY()));
            pMin.setZ(std::min(nodes[i].getZ(), pMin.getZ()));
        }
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::createBBox(UInt elemId, point & pMax, point & pMin) {
        // assert to be sure
        assert(elemId < elements.size());

        // set variables
        Real MIN[3], MAX[3];

        // initialize them
        MIN[0] = nodes[elements[elemId].getConnectedId(0)].getI(0);
        MIN[1] = nodes[elements[elemId].getConnectedId(0)].getI(1);
        MIN[2] = nodes[elements[elemId].getConnectedId(0)].getI(2);

        MAX[0] = nodes[elements[elemId].getConnectedId(0)].getI(0);
        MAX[1] = nodes[elements[elemId].getConnectedId(0)].getI(1);
        MAX[2] = nodes[elements[elemId].getConnectedId(0)].getI(2);

        // loop on coordinates
        for (UInt j = 0; j < 3; ++j) {
            // loop on elements
            for (UInt i = 1; i < RefShape::numVertices; ++i) {
                MIN[j] = std::min(MIN[j], nodes[elements[elemId].getConnectedId(i)].getI(j));
                MAX[j] = std::max(MAX[j], nodes[elements[elemId].getConnectedId(i)].getI(j));
            }
        }

        // set max and min
        for (UInt j = 0; j < 3; ++j) {
            pMax.setI(j, MAX[j]);
            pMin.setI(j, MIN[j]);
        }
    }

    template<typename GEOSHAPE> Real mesh3dSebe<GEOSHAPE>::minH() {
        // temp variable
        mesh1d<typename GEOSHAPE::BShape::BShape> reticolo;

        // create the mesh
        createMesh1d(&reticolo);

        // ritorno il valore
        return (reticolo.minH());
    }

    template<typename GEOSHAPE> Real mesh3dSebe<GEOSHAPE>::maxH() {
        // temp variable
        mesh1d<typename GEOSHAPE::BShape::BShape> reticolo;

        // create the mesh
        createMesh1d(&reticolo);

        // return
        return (reticolo.maxH());
    }

    //
    // method to generate a mesh1d
    //

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::createMesh2d(mesh2d<typename GEOSHAPE::BShape> * faces) {
        // used variable
        set < geoElement<typename GEOSHAPE::BShape> > listaTmp;
        geoElement<typename GEOSHAPE::BShape> face;

        // check how many edges my element has
        UInt num = GEOSHAPE::numFaces;
        UInt numConn = GEOSHAPE::numVerticesForFaces;

        // loop on elements to find the edges of the triangle
        for (UInt i = 0; i < elements.size(); ++i) {
            for (UInt j = 0; j < num * numConn; j = j + numConn) {
                // fill the line
                for (UInt k = 0; k < numConn; ++k) face.setConnectedId(k, elements[i].getConnectedId(GEOSHAPE::faceConn[j + k]));

                listaTmp.insert(face);
                face.setId(0);
            }
        }

        // insert nodes in a grid
        faces->insertNode(&nodes);

        // fill the new vector of elements
        faces->insertElement(&listaTmp);

        // adjust ids
        faces->setUpIds();
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::createMesh1d(mesh1d<typename GEOSHAPE::BShape::BShape> * reticolo) {
        // used variable
        set < geoElement<typename GEOSHAPE::BShape::BShape> > listaTmp;
        geoElement<typename GEOSHAPE::BShape::BShape> lin;

        // check how many edges my element has
        UInt num = GEOSHAPE::numEdges;
        UInt numConn = GEOSHAPE::numVerticesForEdges;

        // loop on elements to find the edges of the triangle
        for (UInt i = 0; i < elements.size(); ++i) {
            for (UInt j = 0; j < num * numConn; j = j + numConn) {
                // fill the line
                for (UInt k = 0; k < numConn; ++k) lin.setConnectedId(k, elements[i].getConnectedId(GEOSHAPE::edgeConn[j + k]));

                listaTmp.insert(lin);
                lin.setId(0);
            }
        }

        // insert nodes in a grid
        reticolo->insertNode(&nodes);

        // fill the new vector of elements
        reticolo->insertElement(&listaTmp);

        // adjust ids
        reticolo->setUpIds();
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::createMesh0d(mesh0d<typename GEOSHAPE::BShape::BShape::BShape> * cloud) {
        // used variable
        UInt geoTmp;
        std::vector<UInt> listaGeoId(nodes.size(), 0);
        std::vector < geoElement<typename GEOSHAPE::BShape::BShape::BShape> > lista;
        geoElement<typename GEOSHAPE::BShape::BShape::BShape> elem;

        // NOTE ON HOW THE METHOD WAS IMPLEMENTED:
        // as a convention we assign to nodes the max geoId of the elements to whom they belong

        // clear the input
        cloud->clear();

        // create the list of ids
        for (UInt i = 0; i < elements.size(); ++i) {
            // read the geoID
            geoTmp = elements[i].getGeoId();

            // assign it to every node of the element
            for (UInt j = 0; j < RefShape::numVertices; ++j) {
                listaGeoId[elements[i].getConnectedId(j)] = std::max(geoTmp, listaGeoId[elements[i].getConnectedId(j)]);
            }
        }

        // resize
        cloud->getElementPointer()->resize(nodes.size());

        // insert the nodes
        cloud->insertNode(&nodes);

        // create the elements
        for (UInt i = 0; i < nodes.size(); ++i) {
            // create the element
            elem.setConnectedId(0, i);
            elem.setGeoId(listaGeoId[i]);

            // insert the elemento
            cloud->getElementPointer()->at(i) = elem;
        }

        // adjust ids
        cloud->setUpIds();
    }

    //
    // Print
    //

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::print() {
        cout << "Mesh3d info" << endl;
        cout << "Number of nodes:       " << this->getNumNodes() << endl;
        cout << "Number of elements:   " << this->getNumElements() << endl;
        cout << endl;
        cout << endl;

        // print nodes
        if (this->getNumNodes() != 0) {
            cout << "List of nodes" << endl;
            for (UInt i = 0; i < nodes.size(); ++i) {
                cout << this->getNode(i).getId() << "  ";
                this->getNode(i).print();
            }
            cout << endl;
        }

        // print elements
        if (this->getNumElements() != 0) {
            cout << "List of elements" << endl;
            cout << endl;
            for (UInt i = 0; i < elements.size(); ++i) {
                this->getElement(i).print();
                cout << endl;
            }
            cout << endl;
        }
    }

    // Points and elements are numbered starting from zero
    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::createElementNodesConn(std::vector< std::vector<int> > elementConnectivity,
        std::vector<int> elementIIndex, std::vector<int> elementJIndex, std::vector<int> elementKIndex)
    {

        UInt idxi(0), idxj(0), idxk(0);

        elements.resize(elementConnectivity.size());
        for (UInt i = 0; i < elements.size(); ++i) {
            for (UInt k = 0; k < GEOSHAPE::numVertices; ++k) {
                elements[i].setConnectedId(k, elementConnectivity[i][k] - 1);
                elements[i].setId(i);
            }
            idxi = elementIIndex[i] - 1;
            idxj = elementJIndex[i] - 1;
            idxk = elementKIndex[i] - 1;
            elements[i].setI(idxi);
            elements[i].setJ(idxj);
            elements[i].setK(idxk);
        }

    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::setIjkPoints(const size_t & ni, const size_t & nj, const size_t & nk) {

        UInt idx_i(0), idx_j(0), idx_k(0);

        // Count x and y dimensions of each layer
        nodesMaxI = ni - 1;
        nodesMaxJ = nj - 1;
        numLayers = nk - 1;
        for (UInt i = 0; i < elements.size(); ++i) {

            idx_i = elements[i].getI();
            idx_j = elements[i].getJ();
            idx_k = elements[i].getK();
            if (nodesMaxI < idx_i + 1) nodesMaxI = idx_i + 1;
            if (nodesMaxJ < idx_j + 1) nodesMaxJ = idx_j + 1;
            if (numLayers < idx_k + 1) numLayers = idx_k + 1;
        }

        // Allocate the matrix cointaining points ijk connectivity
        points_ijk.resize(numLayers + 1);
        for (UInt k = 0; k < points_ijk.size(); ++k)
        {
            points_ijk[k].resize(nodesMaxJ + 1);
            for (UInt j = 0; j < points_ijk[k].size(); ++j)
            {
                points_ijk[k][j].resize(nodesMaxI + 1);
                for (UInt i = 0; i < points_ijk[k][j].size(); ++i)
                {
                    points_ijk[k][j][i] = -1;
                }
            }
        }

        // Allocate the matrix cointaining elements ijk connectivity
        elements_ijk.resize(numLayers);
        for (UInt k = 0; k < elements_ijk.size(); ++k)
        {
            elements_ijk[k].resize(nodesMaxJ);
            for (UInt j = 0; j < elements_ijk[k].size(); ++j)
            {
                elements_ijk[k][j].resize(nodesMaxI);
                for (UInt i = 0; i < elements_ijk[k][j].size(); ++i)
                {
                    elements_ijk[k][j][i] = -1;
                }
            }
        }

        // Assign i,j,k to points and insert them in the connectivity matrix
        for (UInt i = 0; i < elements.size(); ++i) {

            idx_i = elements[i].getI();
            idx_j = elements[i].getJ();
            idx_k = elements[i].getK();

            elements_ijk[idx_k][idx_j][idx_i] = elements[i].getId();

            for (UInt j = 0; j < (GEOSHAPE::numVertices) / (GEOSHAPE::numVerticesForFaces); ++j) {

                nodes[elements[i].getConnectedId(0 + j * GEOSHAPE::numVerticesForFaces)].setI(idx_i);
                nodes[elements[i].getConnectedId(0 + j * GEOSHAPE::numVerticesForFaces)].setJ(idx_j);
                nodes[elements[i].getConnectedId(0 + j * GEOSHAPE::numVerticesForFaces)].setK(idx_k);
                points_ijk[idx_k][idx_j][idx_i] = nodes[elements[i].getConnectedId(0 + j * GEOSHAPE::numVerticesForFaces)].getId();

                nodes[elements[i].getConnectedId(1 + j * GEOSHAPE::numVerticesForFaces)].setI(idx_i + 1);
                nodes[elements[i].getConnectedId(1 + j * GEOSHAPE::numVerticesForFaces)].setJ(idx_j);
                nodes[elements[i].getConnectedId(1 + j * GEOSHAPE::numVerticesForFaces)].setK(idx_k);
                points_ijk[idx_k][idx_j][idx_i + 1] = nodes[elements[i].getConnectedId(1 + j * GEOSHAPE::numVerticesForFaces)].getId();

                nodes[elements[i].getConnectedId(2 + j * GEOSHAPE::numVerticesForFaces)].setI(idx_i + 1);
                nodes[elements[i].getConnectedId(2 + j * GEOSHAPE::numVerticesForFaces)].setJ(idx_j + 1);
                nodes[elements[i].getConnectedId(2 + j * GEOSHAPE::numVerticesForFaces)].setK(idx_k);
                points_ijk[idx_k][idx_j + 1][idx_i + 1] = nodes[elements[i].getConnectedId(2 + j * GEOSHAPE::numVerticesForFaces)].getId();

                nodes[elements[i].getConnectedId(3 + j * GEOSHAPE::numVerticesForFaces)].setI(idx_i);
                nodes[elements[i].getConnectedId(3 + j * GEOSHAPE::numVerticesForFaces)].setJ(idx_j + 1);
                nodes[elements[i].getConnectedId(3 + j * GEOSHAPE::numVerticesForFaces)].setK(idx_k);
                points_ijk[idx_k][idx_j + 1][idx_i] = nodes[elements[i].getConnectedId(3 + j * GEOSHAPE::numVerticesForFaces)].getId();

                idx_k += 1;

            }
        }

        // We need to set points_ijk for pinched out points
        for (size_t j = 0; j < nodesMaxJ + 1; ++j)
        {
            for (size_t i = 0; i < nodesMaxI + 1; ++i)
            {
                int firstNonPinchedK = -1;
                for (int k = 0; k < numLayers + 1; ++k)
                {
                    if (points_ijk[k][j][i] >= 0)
                    {
                        firstNonPinchedK = (firstNonPinchedK == -1) ? k : firstNonPinchedK;
                    }
                    else if (k > 0)
                    {
                        points_ijk[k][j][i] = points_ijk[k - 1][j][i];
                    }
                    // Else, this is a piched out point at the bottom and in order
                    // to set its value we need to know which is the firstNonPinchedK
                }
                // Pinched out points at the bottom
                for (int k = 0; k < firstNonPinchedK; ++k)
                {
                    points_ijk[k][j][i] = points_ijk[firstNonPinchedK][j][i];
                }
            }
        }

        edges_j.resize(numLayers + 1);
        edges_i.resize(numLayers + 1);

        for (UInt k = 0; k < numLayers+1; ++k)
        {
            edges_j[k].reserve(nodesMaxJ + 1);
        }

        for (UInt k = 0; k < numLayers+1; ++k)
        {
            edges_i[k].reserve(nodesMaxI + 1);
        }

        // Allocate space for the list of faces in each layer
        faces_z.resize(numLayers + 1);

    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::createElementEdgesConn(std::vector< std::vector<int> > elementToEdgeConnectivity) {

        UInt numEdgesPerElement(GEOSHAPE::numEdges);
        elementEdgeConnectivity.resize(elementToEdgeConnectivity.size());
        for (UInt i = 0; i < elements.size(); ++i) {
            elementEdgeConnectivity[i].connectedResize(numEdgesPerElement);
            for (UInt k = 0; k < GEOSHAPE::numEdges; ++k) {
                elementEdgeConnectivity[i].setConnectedId(k, elementToEdgeConnectivity[i][k] - 1);
                elementEdgeConnectivity[i].setId(i);
            }
        }

    }

    // Edges are numbered starting from zero
    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::createEdgesNodesConn(std::vector< std::vector<int> > edgeConnectivity) {


        UInt numEdges(edgeConnectivity.size());
        edges.resize(numEdges);

        std::vector< list <int> > edges_itmp;
        std::vector< list <int> > edges_jtmp;

        edges_jtmp.resize(numLayers + 1);
        edges_itmp.resize(numLayers + 1);


        UInt idxi(0), idxj(0), idxk(0);

        UInt idxconn(0);

        // Copy the connectivity of each node
        for (UInt i = 0; i < numEdges; ++i) {
            for (UInt k = 0; k < GEOSHAPE::numVerticesForEdges; ++k) {
                idxconn = edgeConnectivity[i][k] - 1;
                edges[i].setConnectedId(k, idxconn);
                edges[i].setId(i);
            }
        }

        // Assign i,j,k to the edges and crete a list of edges along i and j
        for (UInt i = 0; i < elements.size(); ++i) {

            idxi = elements[i].getI();
            idxj = elements[i].getJ();
            idxk = elements[i].getK();


            UInt edgeidx(0);
            UInt idx(0);


            idx = elementEdgeConnectivity[i].getConnectedId(0);
            edges[idx].setI(idxi);
            edges[idx].setJ(idxj);
            edges[idx].setK(idxk);
            edgeidx = edges[idx].getId();
            edges_jtmp[idxk].push_back(edgeidx);

            idx = elementEdgeConnectivity[i].getConnectedId(2);
            edges[idx].setI(idxi);
            edges[idx].setJ(idxj + 1);
            edges[idx].setK(idxk);
            edgeidx = edges[idx].getId();
            edges_jtmp[idxk].push_back(edgeidx);


            idx = elementEdgeConnectivity[i].getConnectedId(4);
            edges[idx].setI(idxi);
            edges[idx].setJ(idxj);
            edges[idx].setK(idxk + 1);
            edgeidx = edges[idx].getId();
            edges_jtmp[idxk + 1].push_back(edgeidx);

            idx = elementEdgeConnectivity[i].getConnectedId(6);
            edges[idx].setI(idxi);
            edges[idx].setJ(idxj + 1);
            edges[idx].setK(idxk + 1);
            edgeidx = edges[idx].getId();
            edges_jtmp[idxk + 1].push_back(edgeidx);

            idx = elementEdgeConnectivity[i].getConnectedId(1);
            edges[idx].setI(idxi + 1);
            edges[idx].setJ(idxj);
            edges[idx].setK(idxk);
            edgeidx = edges[idx].getId();
            edges_itmp[idxk].push_back(edgeidx);

            idx = elementEdgeConnectivity[i].getConnectedId(3);
            edges[idx].setI(idxi);
            edges[idx].setJ(idxj);
            edges[idx].setK(idxk);
            edgeidx = edges[idx].getId();
            edges_itmp[idxk].push_back(edgeidx);

            idx = elementEdgeConnectivity[i].getConnectedId(5);
            edges[idx].setI(idxi + 1);
            edges[idx].setJ(idxj);
            edges[idx].setK(idxk + 1);
            edgeidx = edges[idx].getId();
            edges_itmp[idxk + 1].push_back(edgeidx);

            idx = elementEdgeConnectivity[i].getConnectedId(7);
            edges[idx].setI(idxi);
            edges[idx].setJ(idxj);
            edges[idx].setK(idxk + 1);
            edgeidx = edges[idx].getId();
            edges_itmp[idxk + 1].push_back(edgeidx);

        }

        for (UInt k = 0; k < numLayers + 1; ++k) {
            edges_itmp[k].unique();
            edges_jtmp[k].unique();
        }

        list<int>::iterator it;

        for (UInt k = 0; k < numLayers + 1; ++k) {
            edges_i[k].resize(edges_itmp[k].size());
            it = edges_itmp[k].begin();
            for (UInt i = 0; i < edges_i[k].size(); ++i) {
                edges_i[k][i] = *it;
                ++it;
            }
        }

        for (UInt k = 0; k < numLayers + 1; ++k) {
            edges_j[k].resize(edges_jtmp[k].size());
            it = edges_jtmp[k].begin();
            for (UInt i = 0; i < edges_j[k].size(); ++i) {
                edges_j[k][i] = *it;
                ++it;
            }
        }
    }

    // Faces are numbered starting from one
    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::createFacesConn() {

        elementFaceConnectivity.resize(elements.size());
        for (UInt i = 0; i < elementFaceConnectivity.size(); ++i) {
            elementFaceConnectivity[i].resize(GEOSHAPE::numFaces);
        }

        // List of element->faces
        UInt nodesfaces(GEOSHAPE::numVerticesForFaces);
        for (UInt i = 0; i < elements.size(); ++i) {
            for (unsigned  int k = 0; k < GEOSHAPE::numFaces; ++k) {
                for (UInt j = 0; j < GEOSHAPE::numVerticesForFaces; ++j) {
                    elementFaceConnectivity[i][k].setConnectedId(j, elements[i].getConnectedId(elements[i].faceConn[j + nodesfaces * k]));
                }
            }
        }

        // Create the list of faces
        std::set< geometry::geoElement<geometry::Hexa::BShape> > facesSet;
        std::pair < std::set<geometry::geoElement<geometry::Hexa::BShape> >::iterator, bool> ret;
        UInt count_faces(1);

        for (UInt i = 0; i < elements.size(); ++i) {
            for (UInt k = 0; k < GEOSHAPE::numFaces; ++k) {
                ret = facesSet.insert(elementFaceConnectivity[i][k]);
                if (ret.second) {
                    (const_cast<geometry::geoElement<geometry::Hexa::BShape> *> (&(*(ret.first))))->setId(count_faces);
                    //               ret.first->setId(count_faces);
                    ++count_faces;
                }
            }
        }

        // Copy the set of faces in a vector
        faces.reserve(facesSet.size());
        std::set< geometry::geoElement<geometry::Hexa::BShape> >::iterator p2;
        UInt face_counter(1);
        for (p2 = facesSet.begin(); p2 != facesSet.end(); ++p2) {
            (const_cast<geometry::geoElement<geometry::Hexa::BShape> *> (&(*(p2))))->setId(face_counter);
            //       p2->setId(face_counter);
            faces.push_back(*p2);
            ++face_counter;
        }

        // Set faces ids in the list element->faces
        std::set< geometry::geoElement<geometry::Hexa::BShape> >::iterator p;
        for (UInt i = 0; i < elements.size(); ++i) {
            for (UInt k = 0; k < GEOSHAPE::numFaces; ++k) {
                p = facesSet.find(elementFaceConnectivity[i][k]);
                elementFaceConnectivity[i][k].setId(p->getId());
            }
        }

        UInt idxk(0);

        // Assign k-ids to the faces and insert them in the list of horizontal faces
        for (UInt i = 0; i < elements.size(); ++i) {
            idxk = elements[i].getK();
            elementFaceConnectivity[i][0].setK(idxk);
            elementFaceConnectivity[i][5].setK(idxk + 1);
            faces_z[idxk].push_back(elementFaceConnectivity[i][0].getId());
            faces_z[idxk + 1].push_back(elementFaceConnectivity[i][5].getId());
        }
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::extractVertexSolution(std::vector< std::vector<double> >& ElementWiseVertexSolution, std::vector<double>& VertexSolution) {
        UInt idx(0);
        for (UInt i = 0; i < elements.size(); ++i) {
            for (UInt k = 0; k < GEOSHAPE::numVertices; ++k) {
                idx = elements[i].getConnectedId(k);
                VertexSolution[idx] = ElementWiseVertexSolution[i][k];
            }
        }
    }

    template<typename GEOSHAPE> std::vector<double> mesh3dSebe<GEOSHAPE>::getNodesXCoordinate() {

        std::vector<double> xCoord(nodes.size(), 0.0);

        for (UInt i = 0; i < nodes.size(); ++i) {
            xCoord[i] = nodes[i].getX();
        }

        return xCoord;
    }

    template<typename GEOSHAPE> std::vector<double> mesh3dSebe<GEOSHAPE>::getNodesYCoordinate() {

        std::vector<double> yCoord(nodes.size(), 0.0);

        for (UInt i = 0; i < nodes.size(); ++i) {
            yCoord[i] = nodes[i].getY();
        }

        return yCoord;
    }

    template<typename GEOSHAPE> std::vector<double> mesh3dSebe<GEOSHAPE>::getNodesZCoordinate() {

        std::vector<double> zCoord(nodes.size(), 0.0);

        for (UInt i = 0; i < nodes.size(); ++i) {
            zCoord[i] = nodes[i].getZ();
        }

        return zCoord;
    }

    template<typename GEOSHAPE> std::vector< std::vector<UInt> > mesh3dSebe<GEOSHAPE>::getElementConnectivity() {

        int numElements(elements.size());
        std::vector< std::vector<UInt> > eleconn(numElements);

        for (int i = 0; i < numElements; ++i) {
            eleconn[i] = elements[i].getConnectedIds();
        }

        return eleconn;
    }

    template<typename GEOSHAPE> std::vector< std::vector<UInt> > mesh3dSebe<GEOSHAPE>::getElementEdgeConnectivity() {

        int numElements(elements.size());
        std::vector< std::vector<UInt> > eleedgeconn(numElements);

        for (int i = 0; i < numElements; ++i) {
            eleedgeconn[i] = elementEdgeConnectivity[i].getConnectedIds();
        }

        return eleedgeconn;
    }

    template<typename GEOSHAPE> std::vector< std::vector<UInt> > mesh3dSebe<GEOSHAPE>::getEdgeConnectivity() {

        int numEdges(edges.size());
        std::vector< std::vector<UInt> > edgeconn(numEdges);

        for (int i = 0; i < numEdges; ++i) {
            edgeconn[i] = edges[i].getConnectedIds();
        }

        return edgeconn;
    }

    template<typename GEOSHAPE> std::vector< int > mesh3dSebe<GEOSHAPE>::computeVertexIndexI() const {
        std::vector<int> indexesI(nodes.size(), 0);
        for (UInt nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex) {
            indexesI[nodeIndex] = nodes[nodeIndex].getI();
        }
        return indexesI;
    }

    template<typename GEOSHAPE> std::vector< int > mesh3dSebe<GEOSHAPE>::computeVertexIndexJ() const {
        std::vector<int> indexesJ(nodes.size(), 0);
        for (UInt nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex) {
            indexesJ[nodeIndex] = nodes[nodeIndex].getJ();
        }
        return indexesJ;
    }

    template<typename GEOSHAPE> std::vector< int > mesh3dSebe<GEOSHAPE>::computeVertexIndexK() const {
        std::vector<int> indexesK(nodes.size(), 0);
        for (UInt nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex) {
            indexesK[nodeIndex] = nodes[nodeIndex].getK();
        }
        return indexesK;
    }

    template<typename GEOSHAPE> std::vector< int > mesh3dSebe<GEOSHAPE>::computeCellCenteredIndexI() const {
        std::vector<int> indexesI(elements.size(), 0);
        for (int el = 0; el < int(elements.size()); ++el) {
            indexesI[el] = elements[el].getI() + 1;
        }
        return indexesI;
    }

    template<typename GEOSHAPE> std::vector< int > mesh3dSebe<GEOSHAPE>::computeCellCenteredIndexJ() const {
        std::vector<int> indexesJ(elements.size(), 0);
        for (int el = 0; el < int(elements.size()); ++el) {
            indexesJ[el] = elements[el].getJ() + 1;
        }
        return indexesJ;
    }

    template<typename GEOSHAPE> std::vector< int > mesh3dSebe<GEOSHAPE>::computeCellCenteredIndexK() const {
        std::vector<int> indexesK(elements.size(), 0);
        for (int el = 0; el < int(elements.size()); ++el) {
            indexesK[el] = elements[el].getK() + 1;
        }
        return indexesK;
    }

    template<typename GEOSHAPE> template<typename TYPE> std::vector< TYPE > mesh3dSebe<GEOSHAPE>::interpolateCellCenteredSolution(
        const std::vector< TYPE > &coarseField,
        const int& scaleX, const int& scaleY,
        const int& shiftX, const int& shiftY,
        const std::vector<std::vector<std::vector<int > > > & coarsele_ijk) const
    {
        std::vector<TYPE> field_refined(elements.size());
        for (UInt refinedElementIndex = 0; refinedElementIndex < elements.size(); ++refinedElementIndex) {
            int i = elements[refinedElementIndex].getI();
            int j = elements[refinedElementIndex].getJ();
            int k = elements[refinedElementIndex].getK();
            // it could be that we reside outside a plausible coarseElement, we gotta look for it
            int coarseI = coarseIndex(i, scaleX, shiftX);
            int coarseJ = coarseIndex(j, scaleY, shiftY);

            // If the cell is outside the bouds of the coarse mesh
            // we consider the nearest cell
            coarseJ = std::min(coarseJ, int(coarsele_ijk[k].size()-1) );
            coarseI = std::min(coarseI, int(coarsele_ijk[k][coarseJ].size()-1) );

            int coarseElementIndex = coarsele_ijk[k][coarseJ][coarseI];
            if (coarseElementIndex < 0) {
                // ouch, in here the research will be expensive

                // we try with more distance, increasingly until we reached scaleX and scaleY
                // if at scaleX,scaleY we still can't reach a valid point there is something wrong and we terminate
                int maxInfDistanceI = 1;
                int maxInfDistanceJ = 1;
                while (maxInfDistanceI <= scaleX && maxInfDistanceJ <= scaleY && coarseElementIndex < 0)
                {
                    maxInfDistanceI = std::min(maxInfDistanceI, scaleX);
                    maxInfDistanceJ = std::min(maxInfDistanceJ, scaleY);
                    // we try finding nodes on the square with this maxInfDistance
                    for (int iOffset = -maxInfDistanceI; iOffset <= maxInfDistanceI && coarseElementIndex < 0; iOffset++) {
                        for (int jOffset = -maxInfDistanceJ; jOffset <= maxInfDistanceJ && coarseElementIndex < 0; jOffset++) {
                            if (abs(iOffset) == maxInfDistanceI || abs(jOffset) == maxInfDistanceJ) {
                                if (0 < coarseJ + jOffset && coarseJ + jOffset < int(coarsele_ijk[0].size())
                                        && 0 < coarseI + iOffset && coarseI + iOffset < int(coarsele_ijk[0][0].size())) {
                                    coarseElementIndex = coarsele_ijk[k][coarseJ + jOffset][coarseI + iOffset];
                                }
                            }
                        }
                    }
                    maxInfDistanceI++;
                    maxInfDistanceJ++;
                }
            }
            if (coarseElementIndex < 0) {
                throw StringException("In interpolateCellCenteredSolution(): impossible to find a valid value for interpolation");
            }

            field_refined[refinedElementIndex] = coarseField[coarseElementIndex];
        }
        return field_refined;
    }

    template<typename GEOSHAPE > std::vector< double > mesh3dSebe<GEOSHAPE>::interpolateCellCenteredSolution(
        std::vector< double > &field,
        const int& scaleX, const int& scaleY,
        const int& shiftX, const int& shiftY,
        std::vector<geoElement< GEOSHAPE> > /*coarsele*/,
        std::vector<point> coarsepoint,
        std::vector< std::vector< std::vector <int> > > coarse_ijk)
    {

        UInt Ilocalmax(0), Jlocalmax(0);

        Ilocalmax = points_ijk[0].size() - 1;
        Jlocalmax = points_ijk[0][0].size() - 1;
        std::vector<double> field_refined(elements.size(), 0.07);
        UInt idxk(0);

        // to remove, do it once for all
        UInt Icoarsemin(INT_MAX), Jcoarsemin(INT_MAX);
        UInt idxi(0), idxj(0);
        for (UInt i = 0; i < coarsepoint.size(); ++i) {
            idxi = refinedIndex(coarsepoint[i].getI(), scaleX, shiftX);
            idxj = refinedIndex(coarsepoint[i].getJ(), scaleY, shiftY);
            if (idxi < Icoarsemin) Icoarsemin = idxi;
            if (idxj < Jcoarsemin) Jcoarsemin = idxj;
            if (idxi > Ilocalmax) idxi = Ilocalmax;
            if (idxj > Jlocalmax) idxj = Jlocalmax;
        }

        UInt idxicoarse(0), idxjcoarse(0);
        for (UInt el = 0; el < elements.size(); ++el) {
            idxi = elements[el].getI();
            idxj = elements[el].getJ();
            idxk = elements[el].getK();
            if (idxi >= Icoarsemin && idxj >= Jcoarsemin) {
                idxicoarse = coarseIndex(idxi, scaleX, shiftX);
                idxjcoarse = coarseIndex(idxj, scaleY, shiftY);

                // If the cell is outside the bouds of the coarse mesh
                // we consider the nearest cell
                idxjcoarse = std::min(idxjcoarse, UInt(coarse_ijk[idxk].size()-1) );
                idxicoarse = std::min(idxicoarse, UInt(coarse_ijk[idxk][idxjcoarse].size()-1) );

                if (coarse_ijk[idxk][idxjcoarse][idxicoarse] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][idxjcoarse][idxicoarse]);
                } else if (idxicoarse + 1 < coarse_ijk[idxk][idxjcoarse].size() && coarse_ijk[idxk][idxjcoarse][idxicoarse + 1] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][idxjcoarse][idxicoarse + 1]);
                } else if (idxjcoarse + 1 < coarse_ijk[idxk].size() && coarse_ijk[idxk][idxjcoarse + 1][idxicoarse] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][idxjcoarse + 1][idxicoarse]);
                } else if (idxjcoarse + 1 < coarse_ijk[idxk].size() &&
                        idxicoarse + 1 < coarse_ijk[idxk][idxjcoarse + 1].size() &&
                        coarse_ijk[idxk][idxjcoarse + 1][idxicoarse + 1] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][idxjcoarse + 1][idxicoarse + 1]);
                } else if (idxjcoarse > 0 && idxicoarse > 0 && coarse_ijk[idxk][idxjcoarse - 1][idxicoarse - 1] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][idxjcoarse - 1][idxicoarse - 1]);
                } else if (idxicoarse > 0 && coarse_ijk[idxk][idxjcoarse][idxicoarse - 1] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][idxjcoarse][idxicoarse - 1]);
                } else if (idxicoarse > 0 && idxjcoarse + 1 < coarse_ijk[idxk].size() && coarse_ijk[idxk][idxjcoarse + 1][idxicoarse - 1] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][idxjcoarse + 1][idxicoarse - 1]);
                } else if (idxjcoarse > 0 && coarse_ijk[idxk][idxjcoarse - 1][idxicoarse] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][idxjcoarse - 1][idxicoarse]);
                } else if (idxjcoarse > 0 && idxicoarse + 1 < coarse_ijk[idxk][idxjcoarse - 1].size() && coarse_ijk[idxk][idxjcoarse - 1][idxicoarse + 1] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][idxjcoarse - 1][idxicoarse + 1]);
                }
            } else if (idxi < Icoarsemin && idxj < Jcoarsemin) {
                field_refined.at(el) = field.at(coarse_ijk[idxk][Jcoarsemin][Icoarsemin]);
            } else if (idxi < Icoarsemin && idxj >= Jcoarsemin) {
                idxjcoarse = coarseIndex(idxj, scaleY, shiftY);
                if (coarse_ijk[idxk][idxjcoarse][Icoarsemin] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][idxjcoarse][Icoarsemin]);
                }
            } else if (idxi >= Icoarsemin && idxj < Jcoarsemin) {
                idxicoarse = coarseIndex(idxi, scaleX, shiftY);
                if (coarse_ijk[idxk][Jcoarsemin][idxicoarse] >= 0) {
                    field_refined.at(el) = field.at(coarse_ijk[idxk][Jcoarsemin][idxicoarse]);
                }
            }
        }

        return field_refined;
    }

    template<typename GEOSHAPE> int mesh3dSebe<GEOSHAPE>::getPointIJK(const std::vector< std::vector< std::vector<int > > > & pointsIJK, int i, int j, int k) const {
        try {
            return pointsIJK.at(k).at(j).at(i);
        } catch (out_of_range& ex) {
            return -1;
        }
    }

    template<typename GEOSHAPE> double mesh3dSebe<GEOSHAPE>::interpolateVertexUsingNeighbors(
        const UInt& vertexIndex,
        const std::vector< double > & interpolatedfield,
        const int& scaleX, const int& scaleY,
        const int& shiftX, const int& shiftY,
        const std::vector < std::vector< std::vector <int> > >& coarsePointIJK) const
    {
        // current vertex I J K in refined mesh
        int vertexK = nodes[vertexIndex].getK();
        int vertexJ = nodes[vertexIndex].getJ();
        int vertexI = nodes[vertexIndex].getI();

        // current vertex bounding I J K in coarse mesh
        int coarseVertexSubI = coarseIndex(vertexI, scaleX, shiftX);
        int coarseVertexSuperI = coarseIndex(vertexI, scaleX, shiftX) + 1;
        int coarseVertexSubJ = coarseIndex(vertexJ, scaleY, shiftY);
        int coarseVertexSuperJ = coarseIndex(vertexJ, scaleY, shiftY) + 1;

        // current vertex bounding I J K in coarse mesh, but in refined mesh indexes
        // TODO do we have to use shift or Icoarsemin Jcoarsemin ?!
        int coarseVertexRefinedIndexSubI = refinedIndex(coarseVertexSubI, scaleX, shiftX);
        int coarseVertexRefinedIndexSuperI = refinedIndex(coarseVertexSuperI, scaleX, shiftX);
        int coarseVertexRefinedIndexSubJ = refinedIndex(coarseVertexSubJ, scaleY, shiftY);
        int coarseVertexRefinedIndexSuperJ = refinedIndex(coarseVertexSuperJ, scaleY, shiftY);

        int coarsePointIJKSubISubJ = getPointIJK(coarsePointIJK, coarseVertexSubI, coarseVertexSubJ, vertexK);
        int coarsePointIJKSuperISubJ = getPointIJK(coarsePointIJK, coarseVertexSuperI, coarseVertexSubJ, vertexK);
        int coarsePointIJKSubISuperJ = getPointIJK(coarsePointIJK, coarseVertexSubI, coarseVertexSuperJ, vertexK);
        int coarsePointIJKSuperISuperJ = getPointIJK(coarsePointIJK, coarseVertexSuperI, coarseVertexSuperJ, vertexK);

        int subIsuperJ = getPointIJK(points_ijk, coarseVertexRefinedIndexSubI, coarseVertexRefinedIndexSuperJ, vertexK);
        int subIsubJ = getPointIJK(points_ijk, coarseVertexRefinedIndexSubI, coarseVertexRefinedIndexSubJ, vertexK);
        int superIsuperJ = getPointIJK(points_ijk, coarseVertexRefinedIndexSuperI, coarseVertexRefinedIndexSuperJ, vertexK);
        int superIsubJ = getPointIJK(points_ijk, coarseVertexRefinedIndexSuperI, coarseVertexRefinedIndexSubJ, vertexK);
        // now we understand in which case we are

        // check if we are on an edge
        bool onEdgeSubI = (coarseVertexRefinedIndexSubI == vertexI && coarsePointIJKSubISubJ >= 0 && coarsePointIJKSubISuperJ >= 0 && (subIsubJ >= 0 || subIsuperJ >= 0));
        bool onEdgeSuperI = (coarseVertexRefinedIndexSuperI == vertexI && coarsePointIJKSuperISubJ >= 0 && coarsePointIJKSuperISuperJ >= 0 && (superIsubJ >= 0 || superIsuperJ >= 0));
        bool onEdgeSubJ = (coarseVertexRefinedIndexSubJ == vertexJ && coarsePointIJKSubISubJ >= 0 && coarsePointIJKSuperISubJ >= 0 && (subIsubJ >= 0 || superIsubJ >= 0));
        bool onEdgeSuperJ = (coarseVertexRefinedIndexSuperJ == vertexJ && coarsePointIJKSubISuperJ >= 0 && coarsePointIJKSuperISuperJ >= 0 && (subIsuperJ >= 0 || superIsuperJ >= 0));
        bool onEdge = (onEdgeSubI || onEdgeSuperI || onEdgeSubJ || onEdgeSuperJ);

        // check if we are in an internal point
        bool isInternal = !onEdge && (coarsePointIJKSubISubJ >= 0
                && coarsePointIJKSuperISubJ >= 0
                && coarsePointIJKSubISuperJ >= 0
                && coarsePointIJKSuperISuperJ >= 0)
                && (subIsubJ >= 0 && subIsuperJ >= 0 && superIsubJ >= 0 && superIsuperJ >= 0);

        // now we know if it is either on an edge, internal or external, we can manage the cases separately

        std::vector<point> Coor;
        // add the interpolation coordinates of the desired point in the first place
        Coor.push_back(nodes[vertexIndex]);

        std::vector<Real> Value;

        /* Interpolation for vertices on edge */
        if (onEdge) {

            if (onEdgeSubI) {
                // add the subI vertices
                if (subIsuperJ >= 0) {
                    Coor.push_back(nodes[subIsuperJ]);
                    Value.push_back(interpolatedfield[subIsuperJ]);
                }

                if (subIsubJ >= 0) {
                    Coor.push_back(nodes[subIsubJ]);
                    Value.push_back(interpolatedfield[subIsubJ]);
                }
            } else if (onEdgeSuperI) {
                // add the superI vertices
                if (superIsuperJ >= 0) {
                    Coor.push_back(nodes[superIsuperJ]);
                    Value.push_back(interpolatedfield[superIsuperJ]);
                }
                if (superIsubJ >= 0) {
                    Coor.push_back(nodes[superIsubJ]);
                    Value.push_back(interpolatedfield[superIsubJ]);
                }
            } else if (onEdgeSubJ) {
                // add the subJ vertices
                if (superIsubJ >= 0) {
                    Coor.push_back(nodes[superIsubJ]);
                    Value.push_back(interpolatedfield[superIsubJ]);
                }
                if (subIsubJ) {
                    Coor.push_back(nodes[subIsubJ]);
                    Value.push_back(interpolatedfield[subIsubJ]);
                }
            } else if (onEdgeSuperJ) {
                // add the superJ vertices
                if (superIsuperJ) {
                    Coor.push_back(nodes[superIsuperJ]);
                    Value.push_back(interpolatedfield[superIsuperJ]);
                }
                if (subIsuperJ) {
                    Coor.push_back(nodes[subIsuperJ]);
                    Value.push_back(interpolatedfield[subIsuperJ]);
                }
            }
        } else if (isInternal) {
            /* this case is slightly easier, we just have to take all of the corners */
            Coor.push_back(nodes[subIsuperJ]);
            Value.push_back(interpolatedfield[subIsuperJ]);

            Coor.push_back(nodes[subIsubJ]);
            Value.push_back(interpolatedfield[subIsubJ]);

            Coor.push_back(nodes[superIsuperJ]);
            Value.push_back(interpolatedfield[superIsuperJ]);

            Coor.push_back(nodes[superIsubJ]);
            Value.push_back(interpolatedfield[superIsubJ]);
        } else {
            /* Yeah, I know, external ones sucks, what's you're gonna do about it... */

            // add those that exist, and if you added nothing we have to find at least one freakin value
            if (coarsePointIJKSubISuperJ >= 0 && subIsuperJ >= 0) {
                Coor.push_back(nodes[subIsuperJ]);
                Value.push_back(interpolatedfield[subIsuperJ]);
            }
            if (coarsePointIJKSubISubJ >= 0 && subIsubJ >= 0) {
                Coor.push_back(nodes[subIsubJ]);
                Value.push_back(interpolatedfield[subIsubJ]);
            }
            if (coarsePointIJKSuperISuperJ >= 0 && superIsuperJ >= 0) {
                Coor.push_back(nodes[superIsuperJ]);
                Value.push_back(interpolatedfield[superIsuperJ]);
            }
            if (coarsePointIJKSuperISubJ >= 0 && superIsubJ >= 0) {
                Coor.push_back(nodes[superIsubJ]);
                Value.push_back(interpolatedfield[superIsubJ]);
            }

            // check if we added at least one value
            if (Value.empty()) {
                // ouch, in here the research will be expensive

                // we try with more distance, increasingly until we reached scaleX and scaleY
                // if at scaleX,scaleY we still can't reach a valid point there is something wrong and we terminate
                int maxInfDistanceI = 1;
                int maxInfDistanceJ = 1;
                while (maxInfDistanceI <= scaleX && maxInfDistanceJ <= scaleY && Value.empty())
                {
                    maxInfDistanceI = std::min(maxInfDistanceI, scaleX);
                    maxInfDistanceJ = std::min(maxInfDistanceJ, scaleY);
                //for (int maxInfDistance = 2; maxInfDistance <= kFactor && Value.empty(); maxInfDistance++) {
                    // we try finding nodes on the square with this maxInfDistance
                    for (int iOffset = -maxInfDistanceI; iOffset <= maxInfDistanceI && Value.empty(); iOffset++) {
                        for (int jOffset = -maxInfDistanceJ; jOffset <= maxInfDistanceJ && Value.empty(); jOffset++) {
                            if (abs(iOffset) == maxInfDistanceI || abs(jOffset) == maxInfDistanceJ) {
                                // we are only interested in elements on this exact maxInfDistance
                                //                                int coarsePointIJKSubISubJ = getPointIJK(coarsePointIJK, coarseVertexSubI, coarseVertexSubJ, vertexK);
                                int searchCoarseVertexI; // we take the sub for negative offsets, and super for positive offsets
                                if (iOffset < 0) {
                                    searchCoarseVertexI = coarseVertexSubI + iOffset;
                                } else {
                                    searchCoarseVertexI = coarseVertexSuperI + iOffset;
                                }

                                int searchCoarseVertexJ;
                                if (jOffset < 0) {
                                    searchCoarseVertexJ = coarseVertexSubJ + jOffset;
                                } else {
                                    searchCoarseVertexJ = coarseVertexSuperJ + jOffset;
                                }
                                int searchCoarseIndex = getPointIJK(coarsePointIJK, searchCoarseVertexI, searchCoarseVertexJ, vertexK);
                                if (searchCoarseIndex >= 0) {
                                    // this one exists in the coarse mesh
                                    int searchCoarseVertexRefinedIndexI = refinedIndex(searchCoarseVertexI, scaleX, shiftX);
                                    int searchCoarseVertexRefinedIndexJ = refinedIndex(searchCoarseVertexJ, scaleY, shiftY);

                                    int searchIndex = getPointIJK(points_ijk, searchCoarseVertexRefinedIndexI, searchCoarseVertexRefinedIndexJ, vertexK);

                                    if (searchIndex >= 0) {
                                        // it exists in the refined mesh too
                                        Coor.push_back(nodes[searchIndex]);
                                        Value.push_back(interpolatedfield[searchIndex]);
                                        // we will exit now, since Value is not empty anymore
                                    }
                                }

                            }
                        }
                    }
                    maxInfDistanceI++;
                    maxInfDistanceJ++;
                }
            }
        }

        // now we can interpolate
        return this->interpolate(Coor, Value);
    }

    template<typename GEOSHAPE > std::vector< std::vector<Real> > mesh3dSebe<GEOSHAPE>::interpolateElementWiseVertexSolution(
        std::vector < std::vector< double > >& field,
        const int& scaleX, const int& scaleY,
        const int& shiftX, const int& shiftY,
        std::vector<geoElement< GEOSHAPE> >& coarsele,
        std::vector<point>& coarsepoint,
        const std::vector < std::vector< std::vector <int> > >& coarsePointIJK)
    {

        std::vector <double> interpolatedfield(nodes.size(), 0.0);

        std::vector<bool> isNodeOnCoarse(nodes.size(), false);

        std::vector< std::vector<Real> > refined_field;
        refined_field.resize(elements.size());

        for (UInt i = 0; i < refined_field.size(); ++i) {
            refined_field[i].resize(GEOSHAPE::numVertices);
        }

        // copy the solution on the vertices that are shared with the coarse mesh
        for (UInt elementIndex = 0; elementIndex < coarsele.size(); ++elementIndex) {
            for (UInt localVertexIndex = 0; localVertexIndex < GEOSHAPE::numVertices; ++localVertexIndex) {
                UInt coarseVertexIndex = coarsele[elementIndex].getConnectedId(localVertexIndex);

                // just for check
                UInt coarseIndexI = coarsepoint[coarseVertexIndex].getI();
                UInt coarseIndexJ = coarsepoint[coarseVertexIndex].getJ();
                UInt coarseIndexK = coarsepoint[coarseVertexIndex].getK();

                // if this point is protagonist of a pinch out we have to set the value of all of the refined mesh points associated
                std::vector<UInt> pinchedKs = pinchedPoints(coarseVertexIndex, coarsePointIJK, coarseIndexI, coarseIndexJ, coarseIndexK);

                for (UInt pinchedKIndex = 0; pinchedKIndex < pinchedKs.size(); pinchedKIndex++) {
                    UInt vertexK = pinchedKs[pinchedKIndex];

                    // refined mesh indexes corresponding to this coarse vertex
                    UInt refinedIndexI = refinedIndex(coarseIndexI, scaleX, shiftX);
                    UInt refinedIndexJ = refinedIndex(coarseIndexJ, scaleY, shiftY);

                    int refinedVertexIndex = getPointIJK(points_ijk, refinedIndexI, refinedIndexJ, vertexK);

                    if (refinedVertexIndex != -1) {
                        isNodeOnCoarse[refinedVertexIndex] = true;
                        interpolatedfield[refinedVertexIndex] = field[elementIndex][localVertexIndex];
                    }
                }
            }
        }

        for (UInt elementIndex = 0; elementIndex < elements.size(); ++elementIndex) {
            for (UInt localVertexIndex = 0; localVertexIndex < GEOSHAPE::numVertices; ++localVertexIndex) {
                UInt vertexIndex = elements[elementIndex].getConnectedId(localVertexIndex);

                if (!isNodeOnCoarse[vertexIndex]) {

                    interpolatedfield[vertexIndex] = interpolateVertexUsingNeighbors(vertexIndex, interpolatedfield, scaleX, scaleY, shiftX, shiftY, coarsePointIJK);
                    isNodeOnCoarse[vertexIndex] = true;
                }
            }
        }


        for (UInt i = 0; i < elements.size(); ++i) {
            for (UInt k = 0; k < GEOSHAPE::numVertices; ++k) {
                UInt vertexIndex = elements[i].getConnectedId(k);
                refined_field[i][k] = interpolatedfield[vertexIndex];
            }
        }


        return refined_field;
    }

    template<typename GEOSHAPE > std::vector< UInt > mesh3dSebe<GEOSHAPE>::pinchedPoints(
        const UInt coarsePointIndex, const std::vector<std::vector<std::vector<int> > > & coarsePointIJK,
        const UInt coarseIndexI, const UInt coarseIndexJ, const UInt coarseIndexK) const
    {
        std::vector<UInt> pinchedKs(0);
        pinchedKs.push_back(coarseIndexK);

        // TODO this shouldn't be necessary, because we should have only one output F3 point if the original was pinched out
        for (UInt pinchedK = coarseIndexK + 1;
                (
                int(coarsePointIndex) == getPointIJK(coarsePointIJK, coarseIndexI, coarseIndexJ, pinchedK)
                || -1 == getPointIJK(coarsePointIJK, coarseIndexI, coarseIndexJ, pinchedK)
                )
                && pinchedK < coarsePointIJK.size();
                pinchedK++) {
            if (0 <= getPointIJK(coarsePointIJK, coarseIndexI, coarseIndexJ, pinchedK)) {
                pinchedKs.push_back(pinchedK);
            }
        }
        for (int pinchedK = coarseIndexK - 1;
                (
                int(coarsePointIndex) == getPointIJK(coarsePointIJK, coarseIndexI, coarseIndexJ, pinchedK)
                || -1 == getPointIJK(coarsePointIJK, coarseIndexI, coarseIndexJ, pinchedK)
                )
                && pinchedK >= 0;
                pinchedK--) {
            if (0 <= getPointIJK(coarsePointIJK, coarseIndexI, coarseIndexJ, pinchedK)) {
                pinchedKs.push_back(pinchedK);
            }
        }
        return pinchedKs;
    }

    template<typename GEOSHAPE > std::vector< double > mesh3dSebe<GEOSHAPE>::interpolateVertexSolution(
        std::vector< double > &field,
        const int& scaleX, const int& scaleY,
        const int& shiftX, const int& shiftY,
        std::vector<point>& coarsepoint,
        const std::vector<std::vector<std::vector<int> > > & coarsePointIJK)
    {

        std::vector <double> interpolatedfield(nodes.size(), NAN);

        std::vector<bool> isNodeOnCoarse(nodes.size(), false);

        for (UInt coarsePointIndex = 0; coarsePointIndex < coarsepoint.size(); ++coarsePointIndex) {

            UInt coarseIndexI = coarsepoint[coarsePointIndex].getI();
            UInt coarseIndexJ = coarsepoint[coarsePointIndex].getJ();
            UInt coarseIndexK = coarsepoint[coarsePointIndex].getK();

            std::vector<UInt> pinchedKs = pinchedPoints(coarsePointIndex, coarsePointIJK, coarseIndexI, coarseIndexJ, coarseIndexK);

            for (UInt pinchedKIndex = 0; pinchedKIndex < pinchedKs.size(); pinchedKIndex++) {
                UInt vertexK = pinchedKs[pinchedKIndex];

                UInt refinedIndexI = refinedIndex(coarseIndexI, scaleX, shiftX);
                UInt refinedIndexJ = refinedIndex(coarseIndexJ, scaleY, shiftY);

                int refinedVertexIndex = getPointIJK(points_ijk, refinedIndexI, refinedIndexJ, vertexK);
                if (refinedVertexIndex != -1) {
                    isNodeOnCoarse[refinedVertexIndex] = true;
                    interpolatedfield[refinedVertexIndex] = field[coarsePointIndex];
                }
            }
        }

        for (UInt pointIndex = 0; pointIndex < nodes.size(); ++pointIndex) {

            if (!isNodeOnCoarse[pointIndex]) {
                interpolatedfield[pointIndex] = interpolateVertexUsingNeighbors(pointIndex, interpolatedfield, scaleX, scaleY, shiftX, shiftY, coarsePointIJK);
                isNodeOnCoarse[pointIndex] = true;
            }
        }

        return interpolatedfield;
    }

    template<typename GEOSHAPE > std::vector< std::vector<double> > mesh3dSebe<GEOSHAPE>::interpolateTwentyNodesHexSolution(
        std::vector< double > &field,
        const int& scaleX, const int& scaleY,
        const int& shiftX, const int& shiftY,
        std::vector<point>& coarsepoint,
        const std::vector<std::vector<std::vector<int> > > & coarsePointIJK)
    {

        std::vector< std::vector<double> > nodesrefsolution(2);

        nodesrefsolution[0].resize(nodes.size());

        std::vector<bool> isNodeOnCoarse(nodes.size(), false);


        nodesrefsolution[0] = interpolateVertexSolution(field, scaleX, scaleY, shiftX, shiftY, coarsepoint, coarsePointIJK);
        nodesrefsolution[1].resize(edges.size());

        UInt idx1(0), idx2(0);

        std::vector <point> Coor;
        Coor.resize(3);
        std::vector<Real> value(2, 0.0);


        for (UInt i = 0; i < edges.size(); ++i) {
            idx1 = edges[i].getConnectedId(0);
            idx2 = edges[i].getConnectedId(1);
            double interpolation;
            if (idx1 == idx2) {
                interpolation = nodesrefsolution[0][idx1];
            } else {
                Coor[1] = nodes[idx1];
                Coor[2] = nodes[idx2];
                Coor[0] = (Coor[1] + Coor[2]) / 2;
                value[0] = nodesrefsolution[0][idx1];
                value[1] = nodesrefsolution[0][idx2];
                interpolation = this->interpolate(Coor, value);
            }

            if (isnan(interpolation)) {
                throw StringException("In interpolateTwentyNodesHexSolution(): a NaN has occurred");
            }
            nodesrefsolution[1][i] = interpolation;

        }

        return nodesrefsolution;
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::interpolateCellHorizonSolution(
        const std::vector<std::vector<double> > & coarseHorizonField,
        const int& scaleX, const int& scaleY,
        const int& shiftX, const int& shiftY,
        std::vector< std::vector<double > > & refinedHorizonField,
        const size_t refinedCellISize, const size_t refinedCellJSize)
    {
        // allocate the right size for the refined matrix
        refinedHorizonField.resize(refinedCellISize);
        for (UInt i = 0; i < refinedHorizonField.size(); ++i) {
            refinedHorizonField[i].resize(refinedCellJSize);
        }

        for (UInt refinedI = 0; refinedI < refinedCellISize; ++refinedI) {
            for (UInt refinedJ = 0; refinedJ < refinedCellJSize; ++refinedJ) {
                int coarseI = coarseIndex(refinedI, scaleX, shiftX);
                int coarseJ = coarseIndex(refinedJ, scaleY, shiftY);
                refinedHorizonField[refinedI][refinedJ] = coarseHorizonField[coarseI][coarseJ];
            }
        }

    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::interpolateVertexHorizonSolution(
        const std::vector<std::vector<double> > & coarseCoordsX,
        const std::vector<std::vector<double> > & coarseCoordsY,
        const std::vector<std::vector<double> > & coarseField,
        const std::vector<std::vector<double> > & refinedCoordsX,
        const std::vector<std::vector<double> > & refinedCoordsY,
        const int& scaleX, const int& scaleY,
        const int& shiftX, const int& shiftY,
        std::vector< std::vector<double > > & refinedField)
    {
        int coarseElementsSizeI = coarseCoordsX.size() - 1;
        int coarseElementsSizeJ = 0;
        if (!coarseCoordsX.empty()) {
            coarseElementsSizeJ = coarseCoordsX[0].size() - 1;
        }

        refinedField.clear();
        refinedField.resize(coarseElementsSizeI * scaleX + 1);
        for (int i = 0; i < int(refinedField.size()); ++i) {
            refinedField[i].resize(coarseElementsSizeJ * scaleY + 1);
        }

        for (int refinedI = 0; refinedI < int(refinedField.size()); ++refinedI) {
            for (int refinedJ = 0; refinedJ < int(refinedField[0].size()); ++refinedJ) {
                int coarseSubI = coarseIndex(refinedI, scaleX, shiftX);
                int coarseSuperI = coarseIndex(refinedI, scaleX, shiftX) + 1;
                int coarseSubJ = coarseIndex(refinedJ, scaleY, shiftY);
                int coarseSuperJ = coarseIndex(refinedJ, scaleY, shiftY) + 1;

                int coarseRefinedSubI = refinedIndex(coarseSubI, scaleX, shiftX);
                int coarseRefinedSuperI = refinedIndex(coarseSuperI, scaleX, shiftX);
                int coarseRefinedSubJ = refinedIndex(coarseSubJ, scaleY, shiftY);
                int coarseRefinedSuperJ = refinedIndex(coarseSuperJ, scaleY, shiftY);

                // check if we are on an edge
                bool onEdgeSubI = (coarseRefinedSubI == refinedI);
                bool onEdgeSuperI = (coarseRefinedSuperI == refinedI);
                bool onEdgeSubJ = (coarseRefinedSubJ == refinedJ);
                bool onEdgeSuperJ = (coarseRefinedSuperJ == refinedJ);
                bool onEdge = (onEdgeSubI || onEdgeSuperI || onEdgeSubJ || onEdgeSuperJ);
                bool onCorner = (onEdgeSubI && onEdgeSubJ);

                double value;
                if (onCorner) {
                    value = coarseField[coarseSubI][coarseSubJ];
                } else if (onEdge) {
                    std::vector<point> points;
                    points.push_back(point(refinedCoordsX[refinedI][refinedJ], refinedCoordsY[refinedI][refinedJ], 0.0L));
                    std::vector<double> values;
                    if (onEdgeSubI) {
                        points.push_back(point(coarseCoordsX[coarseSubI][coarseSubJ], coarseCoordsY[coarseSubI][coarseSubJ], 0.0L));
                        values.push_back(coarseField[coarseSubI][coarseSubJ]);

                        points.push_back(point(coarseCoordsX[coarseSubI][coarseSuperJ], coarseCoordsY[coarseSubI][coarseSuperJ], 0.0L));
                        values.push_back(coarseField[coarseSubI][coarseSuperJ]);

                    } else if (onEdgeSuperI) {
                        points.push_back(point(coarseCoordsX[coarseSuperI][coarseSubJ], coarseCoordsY[coarseSuperI][coarseSubJ], 0.0L));
                        values.push_back(coarseField[coarseSuperI][coarseSubJ]);

                        points.push_back(point(coarseCoordsX[coarseSuperI][coarseSuperJ], coarseCoordsY[coarseSuperI][coarseSuperJ], 0.0L));
                        values.push_back(coarseField[coarseSuperI][coarseSuperJ]);

                    } else if (onEdgeSubJ) {
                        points.push_back(point(coarseCoordsX[coarseSubI][coarseSubJ], coarseCoordsY[coarseSubI][coarseSubJ], 0.0L));
                        values.push_back(coarseField[coarseSubI][coarseSubJ]);

                        points.push_back(point(coarseCoordsX[coarseSuperI][coarseSubJ], coarseCoordsY[coarseSuperI][coarseSubJ], 0.0L));
                        values.push_back(coarseField[coarseSuperI][coarseSubJ]);

                    } else if (onEdgeSuperJ) {
                        points.push_back(point(coarseCoordsX[coarseSubI][coarseSuperJ], coarseCoordsY[coarseSubI][coarseSuperJ], 0.0L));
                        values.push_back(coarseField[coarseSubI][coarseSuperJ]);

                        points.push_back(point(coarseCoordsX[coarseSuperI][coarseSuperJ], coarseCoordsY[coarseSuperI][coarseSuperJ], 0.0L));
                        values.push_back(coarseField[coarseSuperI][coarseSuperJ]);
                    }
                    value = mesh3dSebe::interpolate(points, values);

                } else {
                    std::vector<point> points;
                    points.push_back(point(refinedCoordsX[refinedI][refinedJ], refinedCoordsY[refinedI][refinedJ], 0.0L));
                    std::vector<double> values;

                    points.push_back(point(coarseCoordsX[coarseSubI][coarseSubJ], coarseCoordsY[coarseSubI][coarseSubJ], 0.0L));
                    values.push_back(coarseField[coarseSubI][coarseSubJ]);

                    points.push_back(point(coarseCoordsX[coarseSubI][coarseSuperJ], coarseCoordsY[coarseSubI][coarseSuperJ], 0.0L));
                    values.push_back(coarseField[coarseSubI][coarseSuperJ]);

                    points.push_back(point(coarseCoordsX[coarseSuperI][coarseSubJ], coarseCoordsY[coarseSuperI][coarseSubJ], 0.0L));
                    values.push_back(coarseField[coarseSuperI][coarseSubJ]);

                    points.push_back(point(coarseCoordsX[coarseSuperI][coarseSuperJ], coarseCoordsY[coarseSuperI][coarseSuperJ], 0.0L));
                    values.push_back(coarseField[coarseSuperI][coarseSuperJ]);

                    value = mesh3dSebe::interpolate(points, values);
                }
                refinedField[refinedI][refinedJ] = value;
            }
        }
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::getPointsVector(const std::vector<std::vector<double> > & xCoords, const std::vector<std::vector<double> > & yCoords, std::vector<point> & points) {
        points.clear();
        for (int i = 0; i < xCoords.size(); ++i) {
            for (int j = 0; j < xCoords[0].size(); ++j) {
                points.push_back(geometry::point(xCoords[i][j], yCoords[i][j], 0.0));
            }
        }
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::getValuesVector(const std::vector<std::vector<double> > &values, std::vector<double> &arrayValues) {
        arrayValues.clear();
        for (int i = 0; i < values.size(); ++i) {
            for (int j = 0; j < values[0].size(); ++j) {
                arrayValues.push_back(values[i][j]);
            }
        }
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::elementLayer(std::vector<UInt>& _elementLayer) {

        for (UInt i = 0; i < elements.size(); ++i) {
            _elementLayer[elements[i].getK()] = _elementLayer[elements[i].getK()] + 1;
        }
    }

    template<typename GEOSHAPE> void mesh3dSebe<GEOSHAPE>::nodeLayer(std::vector<UInt>& _nodeLayer) {

        for (unsigned int k = 0; k<points_ijk.size()-1; ++k)
        {
            for (unsigned int j = 0; j<points_ijk[k].size(); ++j)
            {
                for (unsigned int i = 0; i<points_ijk[k][j].size(); ++i)
                {
                    if (points_ijk[k][j][i] < 0)
                        continue;

                    // If any of the four elements (in this layer) connected to
                    // this node is valid
                    if ((j<elements_ijk[k].size() && i<elements_ijk[k][j].size() && elements_ijk[k][j][i] >=0 ) ||
                        (i>0 && j<elements_ijk[k].size() && elements_ijk[k][j][i-1] >=0) ||
                        (j>0 && i<elements_ijk[k][j-1].size() && elements_ijk[k][j-1][i] >=0) ||
                        (i>0 && j>0 && elements_ijk[k][j-1][i-1] >=0) )
                    {
                        // If the node is pinched we count it once
                        if (points_ijk[k][j][i] != points_ijk[k+1][j][i])
                        {
                            _nodeLayer[k]+=2;
                        }
                        else
                        {
                            _nodeLayer[k]+=1;
                        }
                    }
                }
            }
        }
    }

    template<typename GEOSHAPE > Real mesh3dSebe<GEOSHAPE>::interpolate(std::vector < point >& _Coor, std::vector<Real>& _value) {

        if (!_value.empty()) {
            Real firstValue = _value[0];
            bool unchanged = true;
            for (UInt i = 1; i < _value.size() && unchanged; ++i) {
                if (_value[i] != firstValue) {
                    unchanged = false;
                }
            }
            if (unchanged) {
                // all points have the same value, return that
                return firstValue;
            }
        }

        Real sumDist(0.0);
        point p(_Coor[0]);

        Real valueInt(0.0);

        for (UInt i = 1; i < _Coor.size(); ++i) {
            point tmp;
            tmp = p - _Coor[i];
            Real distance = tmp.norm2();
            if (distance == 0) {
                // we are interpolating using a point which is coincident with this one.
                throw StringException("In interpolate(): interpolating using a point which is coincident with the one we wish to interpolate on");
            }
            sumDist += 1.0 / distance;
        }

        for (UInt i = 1; i < _Coor.size(); ++i) {
            point tmp;
            tmp = p - _Coor[i];
            Real neighborValue = _value[i - 1];
            if (isnan(neighborValue)) {
                throw StringException("In interpolate(): interpolating using a point with NaN value");
            }
            valueInt += (neighborValue * (1 / tmp.norm2())) / sumDist;
        }


        if (isnan(valueInt) != 0) {
            throw StringException("In interpolate(): interpolation gave a NaN value");
        }
        return valueInt;

    }

    template<typename GEOSHAPE >
    void mesh3dSebe<GEOSHAPE>::setupRegularMesh()
    {
        regular = true;
        rotation = std::atan( (coordsY[0][1]-coordsY[0][0])/
                                (coordsX[0][1]-coordsX[0][0]) );

        int numI = coordsX[0].size();
        int numJ = coordsX.size();
        deltaI.resize(numI-1);
        deltaJ.resize(numJ-1);
        coordsI.resize(numI);
        coordsJ.resize(numJ);
        for (UInt j=0; j<deltaJ.size(); ++j)
        {
            double dx = coordsX[j+1][0] - coordsX[j][0];
            double dy = coordsY[j+1][0] - coordsY[j][0];
            deltaJ[j] = std::sqrt(dx*dx + dy*dy);
        }
        for (UInt i=0; i<deltaI.size(); ++i)
        {
            double dx = coordsX[0][i+1] - coordsX[0][i];
            double dy = coordsY[0][i+1] - coordsY[0][i];
            deltaI[i] = std::sqrt(dx*dx + dy*dy);
        }

        coordsJ[0] = 0.0;
        for (UInt j=1; j<deltaJ.size()+1; ++j)
        {
            double dx = coordsX[j][0] - coordsX[0][0];
            double dy = coordsY[j][0] - coordsY[0][0];
            coordsJ[j] = std::sqrt(dx*dx + dy*dy);
        }
        coordsI[0] = 0.0;
        for (UInt i=1; i<deltaI.size()+1; ++i)
        {
            double dx = coordsX[0][i] - coordsX[0][0];
            double dy = coordsY[0][i] - coordsY[0][0];
            coordsI[i] = std::sqrt(dx*dx + dy*dy);
        }
    }

}


#endif
