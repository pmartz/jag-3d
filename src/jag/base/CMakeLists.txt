set( _libName jagBase )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGBASE_LIBRARY )
endif()

configure_file( "Version.h.in"
    "${PROJECT_SOURCE_DIR}/src/jag/base/Version.h" )

configure_file( "Config.h.in"
    "${PROJECT_SOURCE_DIR}/src/jag/base/Config.h" )


set( _jagBaseHeaderFiles
    Export.h
    Buffer.h
    Config.h
    gmtlSupport.h
    Log.h
    LogBase.h
    LogMacros.h
    MultiCallback.h
    Notifier.h
    Profile.h
    ptr.h
    Transform.h
    types.h
    Version.h
    UserDataOwner.h
)

set( _jagBaseSourceFiles
    Log.cpp
    Profile.cpp
    Version.cpp
    UserDataOwner.cpp
)

_addLibrary( ${_libName}
    ${_jagBaseHeaderFiles}
    ${_jagBaseSourceFiles}
)

install(
    DIRECTORY .
    DESTINATION include/jag/base
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN ".hg" EXCLUDE
    PATTERN ".txt" EXCLUDE
    PATTERN ".cpp" EXCLUDE
    PATTERN ".cxx" EXCLUDE
    PATTERN ".in" EXCLUDE
)
