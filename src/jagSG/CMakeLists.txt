set( _libName jagSG )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGSG_LIBRARY )
endif()


set( _jagSGHeaderFiles
    Export.h
    CollectionVisitor.h
    ExecuteVisitor.h
    FrustumCullCallback.h
    NodeMaskCullCallback.h
    Common.h
    Node.h
    SelectContainerCallback.h
    SmallFeatureCallback.h
    Visitor.h
    IntersectVisitor.h
)

set( _jagSGSourceFiles
    CollectionVisitor.cpp
    ExecuteVisitor.cpp
    FrustumCullCallback.cpp
    SelectContainerCallback.cpp
    SmallFeatureCallback.cpp
    NodeMaskCullCallback.cpp
    Node.cpp
    IntersectVisitor.cpp
)

_addLibrary( ${_libName}
    ${_jagSGHeaderFiles}
    ${_jagSGSourceFiles}
    JAG_LIBRARIES
        jagDraw jagBase
)

install(
    DIRECTORY .
    DESTINATION include/jagSG
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN ".hg" EXCLUDE
    PATTERN ".txt" EXCLUDE
    PATTERN ".cpp" EXCLUDE
    PATTERN ".cxx" EXCLUDE
)
