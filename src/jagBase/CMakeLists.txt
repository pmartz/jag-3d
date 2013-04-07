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

set( _libName jagBase )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGBASE_LIBRARY )
endif()

configure_file( "Version.h.in"
    "${PROJECT_SOURCE_DIR}/src/jagBase/Version.h" @ONLY )

configure_file( "LogEnable.h.in"
    "${PROJECT_SOURCE_DIR}/src/jagBase/LogEnable.h" @ONLY )


set( _jagBaseHeaderFiles
    Export.h
    Buffer.h
    gmtlSupport.h
    Log.h
    LogBase.h
    LogEnable.h
    LogMacros.h
    MultiCallback.h
    Profile.h
    ptr.h
    Transform.h
    types.h
    Version.h
)

set( _jagBaseSourceFiles
    Log.cpp
    Profile.cpp
    Version.cpp
)

_addLibrary( ${_libName}
    ${_jagBaseHeaderFiles}
    ${_jagBaseSourceFiles}
)

install(
    DIRECTORY .
    DESTINATION include/jagBase
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN ".hg" EXCLUDE
    PATTERN ".txt" EXCLUDE
    PATTERN ".cpp" EXCLUDE
    PATTERN ".cxx" EXCLUDE
    PATTERN ".in" EXCLUDE
)
