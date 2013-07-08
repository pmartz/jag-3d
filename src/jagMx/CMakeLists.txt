#*************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# jag3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License version 2.1 as published by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
#************** <auto-copyright.pl END do not edit this line> ***************

set( _libName jagMx )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGMX_LIBRARY )
endif()


set( _directXHeaders )
set( _directXSource )
if( DIRECTINPUT_FOUND )
	add_definitions(-DDIRECTINPUT_ENABLED)
    include_directories( ${DIRECTINPUT_INCLUDE_DIR} )
    set( _directXHeaders
        MxGamePadDX.h
    )
    set( _directXSource
        MxGamePadDX.cpp
    )
endif()

set( _jagMxHeaderFiles
    Export.h
    ${_directXHeaders}
    MxCore.h
    MxUtils.h
)
#    MxEventHandler.h
#    MxGamePad.h
#)

set( _jagMxSourceFiles
    ${_directXSource}
    MxCore.cpp
)
#    MxEventHandler.cpp
#    MxGamePad.cpp
#    MxUtils.cpp
#)

_addLibrary( ${_libName}
    ${_jagMxHeaderFiles}
    ${_jagMxSourceFiles}
    JAG_LIBRARIES
        jagBase
)

install(
    DIRECTORY .
    DESTINATION include/jagMx
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN ".hg" EXCLUDE
    PATTERN ".txt" EXCLUDE
    PATTERN ".cpp" EXCLUDE
    PATTERN ".cxx" EXCLUDE
)
