
set( _pluginName shader )
set( _extensions vs gs fs vert geom frag )

# Configure the jagp-<name>.jagpi file for this plugin.
_createPluginInfo( ${_pluginName}
    "Load shader source from text files."
    _extensions )


# Required for ModuleInstall.cmake
set( _libName jagp-${_pluginName} )

_addPlugin( ${_libName}
    shader.cpp
    JAG_LIBRARIES
        jagDisk jagDraw jagBase
)
