/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License version 2.1 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*
*************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef __JAGBASE_VERSION_H__
#define __JAGBASE_VERSION_H__ 1

#include <jagBase/Export.h>
#include <string>


namespace jagBase {


/** \defgroup Version Version utilities */
/*@{*/

// Please keep in sync with top-level CMakeLists.txt JAG3D_VERSION variable.
#define JAG3D_MAJOR_VERSION 0
#define JAG3D_MINOR_VERSION 0
#define JAG3D_SUB_VERSION 0

/** \brief Jag3D version number as an integer

C preprocessor integrated version number
The form is Mmmss, where:
   \li M is the major version
   \li mm is the minor version (zero-padded)
   \li ss is the sub version (zero padded)

Use this in version-specific code, for example:
\code
   #if( JAG3D_VERSION < 10500 )
      ... code specific to releases before v1.05
   #endif
\endcode
*/
#define JAG3D_VERSION ( \
        ( JAG3D_MAJOR_VERSION * 10000 ) + \
        ( JAG3D_MINOR_VERSION * 100 ) + \
        JAG3D_SUB_VERSION )

/** \brief Run-time access to the Jag3D version number

Returns JAG3D_VERSION, the Jad3D version number as an integer
\see JAG3D_VERSION
*/
unsigned int JAGBASE_EXPORT getVersionNumber();

/** \brief Jag3D version number as a string

Example:
\code
Jag3D version 1.1.0 (10100)
\endcode
*/
std::string JAGBASE_EXPORT getVersionString();

/*@}*/


// namespace jagBase
}

// __JAGBASE_VERSION_H__
#endif
