
# Required for ModuleInstall.cmake
set( _libName jagp-osgModel )


# Configure the pluginSupport.h header file.
set( _pluginName osgModel )
set( _pluginExtensions ${JAG3D_OSGMODEL_PLUGIN_SUPPORT} )
_extensionsToOSGPlugins( _pluginList _pluginExtensions )
_createOSGPluginReferendeCode( _pluginOSGExtensionRefs _pluginList )
_createExtensionSupportedCode( _pluginExtensionSupported _pluginExtensions )
configure_file( ${PROJECT_SOURCE_DIR}/src/plugins/common/pluginSupport.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/pluginSupport.h )


_addPlugin( ${_libName}
    osgModel.cpp
    osg2jag.cpp
    osg2jag.h
    pluginSupport.h
    JAG_LIBRARIES
        jagSG jagDisk jagDraw jagBase
    ADDITIONAL_INCLUDES
        ${OSG_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${OSG_LIBRARIES}
)

_addPluginInfo( jagp-osgModel.jagpi )
