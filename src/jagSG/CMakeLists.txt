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

set( _libName jagSG )

if( BUILD_SHARED_LIBS )
    add_definitions( -DJAGSG_LIBRARY )
endif()


set( _jagSGHeaderFiles
    Export.h
    CollectionVisitor.h
    ExecuteVisitor.h
    Common.h
    Node.h
    SelectContainerCallback.h
    SmallFeatureCallback.h
    Visitor.h
)

set( _jagSGSourceFiles
    CollectionVisitor.cpp
    ExecuteVisitor.cpp
    SelectContainerCallback.cpp
    SmallFeatureCallback.cpp
    Node.cpp
)

_addLibrary( ${_libName}
    ${_jagSGHeaderFiles}
    ${_jagSGSourceFiles}
    JAG_LIBRARIES
        jagDraw jagBase
)

install(
    DIRECTORY .
    DESTINATION include/jagSG
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN ".hg" EXCLUDE
    PATTERN ".txt" EXCLUDE
    PATTERN ".cpp" EXCLUDE
    PATTERN ".cxx" EXCLUDE
)
