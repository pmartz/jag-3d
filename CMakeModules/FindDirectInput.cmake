# Locate directinput
# This module defines
# DIRECTINPUT_LIBRARIES
# DIRECTINPUT_FOUND, if false, do not try to link to directinput
# DIRECTINPUT_INCLUDE_DIR, where to find the headers
#
# $DIRECTINPUT_DIR is an environment variable that would
# point to the this path in the plateform devkit (Samples\Multimedia\DirectShow)
#
# Created by Cedric Pinson.
#

set( _dxArch )
if( DIRECTINPUT_ARCH_x64 )
    set( _dxArch "x64" )
else()
    set( _dxArch "x86" )
endif()


if( WIN32 )
#    FIND_PATH( DIRECTINPUT_ROOT_DIR Include/D3D10.h
#               PATHS
#               $ENV{PATH}
#               $ENV{PROGRAMFILES}
#     )

     FIND_PATH( DIRECTINPUT_INCLUDE_DIR dinput.h
               PATHS
               ${DIRECTINPUT_ROOT_DIR}/Include
    )
    
    FIND_LIBRARY( DIRECTINPUT_LIBRARY dinput7.lib dinput8.lib
                  PATHS
                  ${DIRECTINPUT_ROOT_DIR}/lib/${_dxArch}
    )
    
    FIND_LIBRARY( DIRECTINPUT_GUID_LIBRARY dxguid.lib
                  PATHS
                  ${DIRECTINPUT_ROOT_DIR}/lib/${_dxArch}
    )
    
    FIND_LIBRARY( DIRECTINPUT_ERR_LIBRARY dxerr.lib
                  PATHS
                  ${DIRECTINPUT_ROOT_DIR}/lib/${_dxArch}
    )
    
    set( DIRECTINPUT_LIBRARIES
         ${DIRECTINPUT_LIBRARY}
         ${DIRECTINPUT_GUID_LIBRARY}
         ${DIRECTINPUT_ERR_LIBRARY}
    )
    
    include( FindPackageHandleStandardArgs )
    set( DIRECTINPUT_FAIL_MESSAGE "Cound not find optional DirectInput dependency." )

    find_package_handle_standard_args( DIRECTINPUT
        REQUIRED_VARS DIRECTINPUT_INCLUDE_DIR DIRECTINPUT_LIBRARIES
        FAIL_MESSAGE ${DIRECTINPUT_FAIL_MESSAGE} )

    mark_as_advanced( DIRECTINPUT_ERR_LIBRARY )
    mark_as_advanced( DIRECTINPUT_GUID_LIBRARY )
    mark_as_advanced( DIRECTINPUT_INCLUDE_DIR )
    mark_as_advanced( DIRECTINPUT_LIBRARY )
endif()

MARK_AS_ADVANCED( DIRECTINPUT_FOUND )
