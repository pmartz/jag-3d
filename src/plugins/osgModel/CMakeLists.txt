if( NOT OSG_FOUND )
    return()
endif()

# Required for ModuleInstall.cmake
set( _libName jagp-osgModel )

_addPlugin( ${_libName}
    osgModel.cpp
    osg2jag.cpp
    osg2jag.h
    JAG_LIBRARIES
        jagSG jagDisk jagDraw jagBase
    ADDITIONAL_INCLUDES
        ${OSG_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${OSG_LIBRARIES}
)

_addPluginInfo( jagp-osgModel.jagpi )
