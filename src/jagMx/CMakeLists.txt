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


set( _dxHeaders )
set( _dxSources )
set( _dxLibrary )
if( DIRECTINPUT_FOUND )
	add_definitions(-DDIRECTINPUT_ENABLED)
    include_directories( ${DIRECTINPUT_INCLUDE_DIR} )
    set( _dxHeaders
        MxGamePadDX.h
    )
    set( _dxSources
        MxGamePadDX.cpp
    )
    set( _dxLibrary ${DIRECTINPUT_LIBRARIES} )
endif()

set( _jagMxHeaderFiles
    Export.h
    ${_dxHeaders}
    MxCore.h
    MxGamePad.h
    MxUtils.h
)
#    MxEventHandler.h
#)

set( _jagMxSourceFiles
    ${_dxSources}
    MxCore.cpp
    MxGamePad.cpp
    MxUtils.cpp
)
#    MxEventHandler.cpp
#)

_addLibrary( ${_libName}
    ${_jagMxHeaderFiles}
    ${_jagMxSourceFiles}
    JAG_LIBRARIES
        jagBase
        ${_dxLibrary}
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
