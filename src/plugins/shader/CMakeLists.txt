# Required for ModuleInstall.cmake
set( _libName shader )

_addLibrary( ${_libName}
    shader.cpp
    JAG_LIBRARIES
        jagDisk jagDraw jagBase
)

_addPluginInfo( shader.jagpi )
