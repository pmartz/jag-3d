
# Configure the pluginSupport.h header file.
set( _pluginName osgImage )

# Configure the pluginSupport.h header file.
_extensionsToOSGPlugins( _pluginList JAG3D_OSGIMAGE_PLUGIN_SUPPORT )
_createOSGPluginReferendeCode( OSGExtensionRefs _pluginList )
_createPluginSupportH( ${_pluginName}
    OSGExtensionRefs
    JAG3D_OSGIMAGE_PLUGIN_SUPPORT )

# Configure the jagp-<name>.jagpi file for this plugin.
_createPluginInfo( ${_pluginName}
    "OSG-based image read/write."
    JAG3D_OSGIMAGE_PLUGIN_SUPPORT )


# Required for ModuleInstall.cmake
set( _libName jagp-${_pluginName} )

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
