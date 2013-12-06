set( _libName jagDisk )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGDISK_LIBRARY )
endif()


set( _jagDiskHeaderFiles
    Export.h
    Options.h
    PluginManager.h
    ReaderWriter.h
    ReadWrite.h
)

set( _jagDiskSourceFiles
    Options.cpp
    PluginManager.cpp
    ReaderWriter.cpp
    ReadWrite.cpp
)

_addLibrary( ${_libName}
    ${_jagDiskHeaderFiles}
    ${_jagDiskSourceFiles}
    JAG_LIBRARIES
        jagDraw
        jagBase
)

install(
    DIRECTORY .
    DESTINATION include/jagDisk
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN ".hg" EXCLUDE
    PATTERN ".txt" EXCLUDE
    PATTERN ".cpp" EXCLUDE
    PATTERN ".cxx" EXCLUDE
    PATTERN ".in" EXCLUDE
)
