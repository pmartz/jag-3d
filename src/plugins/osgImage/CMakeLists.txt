
# Required for ModuleInstall.cmake
set( _libName jagp-osgImage )


# Configure the pluginSupport.h header file.
set( _pluginName osgImage )
set( _pluginExtensions ${JAG3D_OSGIMAGE_PLUGIN_SUPPORT} )
_extensionsToOSGPlugins( _pluginList _pluginExtensions )
_createOSGPluginReferendeCode( _pluginOSGExtensionRefs _pluginList )
_createExtensionSupportedCode( _pluginExtensionSupported _pluginExtensions )
configure_file( "${PROJECT_SOURCE_DIR}/src/plugins/common/pluginSupport.h.in"
    "${PROJECT_SOURCE_DIR}/src/plugins/osgImage/pluginSupport.h" )


_addPlugin( ${_libName}
    osgImage.cpp
    pluginSupport.h
    JAG_LIBRARIES
        jagDisk jagDraw jagBase
    ADDITIONAL_INCLUDES
        ${OSG_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${OSG_LIBRARIES}
)

_addPluginInfo( jagp-osgImage.jagpi )
