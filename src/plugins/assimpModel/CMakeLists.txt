
# Required for ModuleInstall.cmake
set( _libName jagp-assimpModel )


_addPlugin( ${_libName}
    assimpModel.cpp
    assimp2jag.cpp
    assimp2jag.h
    JAG_LIBRARIES
        jagSG jagDisk jagDraw jagBase
    ADDITIONAL_INCLUDES
        ${ASSIMP_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${ASSIMP_LIBRARIES}
)

_addPluginInfo( jagp-assimpModel.jagpi )
