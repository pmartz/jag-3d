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

#ifndef __JAGDISK_OPTIONS_H__
#define __JAGDISK_OPTIONS_H__ 1


#include <jagDisk/Export.h>
#include <Poco/Path.h>

#include <jagBase/ptr.h>
#include <string>
#include <vector>


namespace jagDisk {


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/


/** \class Options Options.H <jagDisk/Options.H>
\brief
\details
*/
class JAGDISK_EXPORT Options
{
public:
    typedef enum {
        NO_PATHS                    = 0,
        USE_CURRENT_DIRECTORY       = ( 1 << 0x0 ),
        USE_JAG3D_DATA_PATH_ENV_VAR = ( 1 << 0x1 )
    } InitFlags;

    Options( const int initFlags=USE_JAG3D_DATA_PATH_ENV_VAR );
    Options( const Options& rhs );
    ~Options();

    void addPath( const std::string& path );
    void addPaths( const std::string& paths );
    void clearPaths();
    const Poco::Path::StringVec& getPaths() const;

protected:
    Poco::Path::StringVec _paths;
};

typedef jagBase::ptr< jagDisk::Options >::shared_ptr OptionsPtr;
typedef std::vector< OptionsPtr > OptionsList;


/**@}*/


// jagDisk
}


// __JAGDISK_OPTIONS_H__
#endif
