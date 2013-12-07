set( _libName jagUtil )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGUTIL_LIBRARY )
endif()


set( _jagUtilHeaderFiles
    Export.h
    ABuffer.h
    Blur.h
    BufferAggregationVisitor.h
    DrawGraphCountVisitor.h
    QuadNode.h
    ResetBoundsVisitor.h
    Shapes.h
)

set( _jagUtilSourceFiles
    ABuffer.cpp
    Blur.cpp
    BufferAggregationVisitor.cpp
    QuadNode.cpp
    ResetBoundsVisitor.cpp
    Shapes.cpp
)

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
