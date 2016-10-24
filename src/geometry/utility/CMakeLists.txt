set(geo_HEADERS ${geo_HEADERS}
    utility/barCoordinates.h
    utility/bin.hpp
    utility/bisection.hpp
    utility/dump.hpp
    utility/exceptions.hpp
    utility/inSegment.h
    utility/insidePolygon.h
    utility/insideVolume.h
    utility/inTriangle.h
    utility/inTetrahedron.h
    utility/newton.hpp
    utility/sortList.hpp
    utility/tree.hpp
    utility/triangleMapping.h
    utility/boostValidateVector.hpp
    CACHE INTERNAL "")

set(geo_SOURCES ${geo_SOURCES}
    utility/barCoordinates.cpp
    utility/inSegment.cpp
    utility/insidePolygon.cpp
    utility/insideVolume.cpp
    utility/inTriangle.cpp
    utility/inTetrahedron.cpp
    utility/triangleMapping.cpp
    CACHE INTERNAL "")


