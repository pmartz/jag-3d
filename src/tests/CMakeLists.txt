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

add_subdirectory( drawable )
add_subdirectory( jagload )
add_subdirectory( rtt )
add_subdirectory( texture )


# Early turn-on tests, not compatible with
# core profile. Leave them out of the build.
# 
#add_subdirectory( draw )
#add_subdirectory( drawcommand )
#add_subdirectory( simple3x )
#add_subdirectory( uniform )

# Early turn-on test.
# 
#add_subdirectory( simplegmtl )
