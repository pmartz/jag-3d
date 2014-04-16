
set( _pluginName osgModel )

# Configure the pluginSupport.h header file.
_extensionsToOSGPlugins( _pluginList JAG3D_OSGMODEL_PLUGIN_SUPPORT )
_createOSGPluginReferendeCode( OSGExtensionRefs _pluginList )
_createPluginSupportH( ${_pluginName}
    OSGExtensionRefs
    JAG3D_OSGMODEL_PLUGIN_SUPPORT )

# Configure the jagp-<name>.jagpi file for this plugin.
_createPluginInfo( ${_pluginName}
    "OSG-based model read/write."
    JAG3D_OSGMODEL_PLUGIN_SUPPORT )


# Required for ModuleInstall.cmake
set( _libName jagp-${_pluginName} )

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
