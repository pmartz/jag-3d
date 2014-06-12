set( _libName jagUtil )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGUTIL_LIBRARY )
endif()


set( _jagUtilHeaderFiles
    Export.h
    Blur.h
    BufferAggregationVisitor.h
    DrawGraphCountVisitor.h
    QuadNode.h
    ResetBoundsVisitor.h
    Shapes.h
)

set( _jagUtilSourceFiles
    Blur.cpp
    BufferAggregationVisitor.cpp
    QuadNode.cpp
    ResetBoundsVisitor.cpp
    Shapes.cpp
)

if(NOT APPLE)
    list(APPEND _jagUtilSourceFiles ABuffer.cpp)
    list(APPEND _jagUtilHeaderFiles ABuffer.h)
endif(NOT APPLE)

_addLibrary( ${_libName}
    ${_jagUtilHeaderFiles}
    ${_jagUtilSourceFiles}
    JAG_LIBRARIES
        jagDisk
        jagSG
        jagDraw
        jagBase
)

install(
    DIRECTORY .
    DESTINATION include/jag/util
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN ".hg" EXCLUDE
    PATTERN ".txt" EXCLUDE
    PATTERN ".cpp" EXCLUDE
    PATTERN ".cxx" EXCLUDE
)
