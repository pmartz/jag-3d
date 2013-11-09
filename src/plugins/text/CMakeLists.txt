# Required for ModuleInstall.cmake
set( _libName jagp-text )

_addPlugin( ${_libName}
    text.cpp
    TextDumpDG.cpp
    TextDumpDG.h
    TextDumpSG.cpp
    TextDumpSG.h
    JAG_LIBRARIES
        jagDisk jagSG jagDraw jagBase
)

_addPluginInfo( jagp-text.jagpi )
