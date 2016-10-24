set(geo_HEADERS ${geo_HEADERS}
    geometry/connect1d.hpp
    geometry/connect2d.hpp
    geometry/connect3d.hpp
    geometry/geoElement.hpp
    geometry/geoElementSearch.h
    geometry/geoElementSize.hpp
    geometry/mesh0d.hpp
    geometry/mesh1d.hpp
    geometry/mesh2d.hpp
    geometry/mesh3d.hpp
    geometry/mesh3dSebe.hpp
    geometry/meshSearch.hpp
    geometry/meshSearchStructured.hpp
    geometry/tricky1d.h
    geometry/tricky2d.h
    geometry/tricky3d.h
    CACHE INTERNAL "")

set(geo_SOURCES ${geo_SOURCES}
    geometry/geoElementSearch.cpp
    geometry/tricky1d.cpp
    geometry/tricky2d.cpp
    geometry/tricky3d.cpp
    CACHE INTERNAL "")

