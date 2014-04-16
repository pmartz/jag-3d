
set( _pluginName assimpModel )

# Configure the pluginSupport.h header file.
_createPluginSupportH( ${_pluginName}
    ""
    JAG3D_ASSIMPMODEL_PLUGIN_SUPPORT )

# Configure the jagp-<name>.jagpi file for this plugin.
_createPluginInfo( ${_pluginName}
    "ASSIMP-based model loader."
    JAG3D_ASSIMPMODEL_PLUGIN_SUPPORT )


# Required for ModuleInstall.cmake
set( _libName jagp-${_pluginName} )

_addPlugin( ${_libName}
    assimpModel.cpp
    assimp2jag.cpp
    assimp2jag.h
    pluginSupport.h
    JAG_LIBRARIES
        jagSG jagDisk jagDraw jagBase
    ADDITIONAL_INCLUDES
        ${ASSIMP_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${ASSIMP_LIBRARIES}
)
