if( NOT OSG_FOUND )
    return()
endif()

# Required for ModuleInstall.cmake
set( _libName jagp-osgImage )

_addPlugin( ${_libName}
    osgImage.cpp
    JAG_LIBRARIES
        jagDisk jagDraw jagBase
    ADDITIONAL_INCLUDES
        ${OSG_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${OSG_LIBRARIES}
)

_addPluginInfo( jagp-osgImage.jagpi )
