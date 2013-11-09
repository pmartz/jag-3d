# Required for ModuleInstall.cmake
set( _libName jagp-shader )

_addPlugin( ${_libName}
    shader.cpp
    JAG_LIBRARIES
        jagDisk jagDraw jagBase
)

_addPluginInfo( jagp-shader.jagpi )
