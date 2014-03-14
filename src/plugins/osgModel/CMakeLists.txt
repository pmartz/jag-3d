
# Required for ModuleInstall.cmake
set( _libName jagp-osgModel )


# Configure the osgSupport.h header file.
set( _pluginName osgModel )
set( _pluginOSGExtensions ${JAG3D_OSGMODEL_PLUGIN_SUPPORT} )
_createOSGExtensionReferenceCode( _pluginOSGExtensionRefs _pluginOSGExtensions )
_createOSGExtensionSupportedCode( _pluginOSGExtensionSupported _pluginOSGExtensions )
configure_file( "${PROJECT_SOURCE_DIR}/src/plugins/osg/osgSupport.h.in"
    "${PROJECT_SOURCE_DIR}/src/plugins/osgModel/osgSupport.h" )


_addPlugin( ${_libName}
    osgModel.cpp
    osg2jag.cpp
    osg2jag.h
    osgSupport.h
    JAG_LIBRARIES
        jagSG jagDisk jagDraw jagBase
    ADDITIONAL_INCLUDES
        ${OSG_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${OSG_LIBRARIES}
)

_addPluginInfo( jagp-osgModel.jagpi )
