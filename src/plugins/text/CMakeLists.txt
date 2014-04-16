
set( _pluginName text )
set( _extensions txt text )

# Configure the jagp-<name>.jagpi file for this plugin.
_createPluginInfo( ${_pluginName}
    "Dump scene graph or draw graph information to text file."
    _extensions )


# Required for ModuleInstall.cmake
set( _libName jagp-${_pluginName} )

_addPlugin( ${_libName}
    text.cpp
    TextDumpDG.cpp
    TextDumpDG.h
    TextDumpSG.cpp
    TextDumpSG.h
    JAG_LIBRARIES
        jagDisk jagSG jagDraw jagBase
)
