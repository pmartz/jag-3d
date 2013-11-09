if( NOT OSG_FOUND )
    return()
endif()

# Required for ModuleInstall.cmake
set( _libName jagp-image-osg )

_addPlugin( ${_libName}
    image-osg.cpp
    JAG_LIBRARIES
        jagDisk jagDraw jagBase
    ADDITIONAL_INCLUDES
        ${OSG_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${OSG_LIBRARIES}
)

_addPluginInfo( jagp-image-osg.jagpi )
