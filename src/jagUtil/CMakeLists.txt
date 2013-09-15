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

set( _libName jagUtil )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGUTIL_LIBRARY )
endif()


set( _jagUtilHeaderFiles
    Export.h
    BufferAggregationVisitor.h
    DrawGraphCountVisitor.h
    ResetBoundsVisitor.h
    Shapes.h
)

set( _jagUtilSourceFiles
    BufferAggregationVisitor.cpp
    ResetBoundsVisitor.cpp
    Shapes.cpp
)

_addLibrary( ${_libName}
    ${_jagUtilHeaderFiles}
    ${_jagUtilSourceFiles}
    JAG_LIBRARIES
        jagSG
        jagDraw
        jagBase
)

install(
    DIRECTORY .
    DESTINATION include/jagUtil
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN ".hg" EXCLUDE
    PATTERN ".txt" EXCLUDE
    PATTERN ".cpp" EXCLUDE
    PATTERN ".cxx" EXCLUDE
)
