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

#include <boost/any.hpp>
#include <jagBase/ptr.h>
#include <string>
#include <vector>
#include <map>


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

    /** \brief Options class constructor
    \details Constructs an instance of the Options object.
    \param initFlags Specifies an initial list of directories to use when
    searching for data files to load. By default, Options loads its directory
    search path list \c _paths from the value of the JAG3D_DATA_PATH
    environment variable. If this variable is not set, the \c _paths will be
    empty and only the current working directory will be searched.

    To separate multiple directories in JAG3D_DATA_PATH, use the platform-
    specific path separator (";" on Windows, ":" on Linux, etc).
    \envvar JAG3D_DATA_PATH */
    Options( const int initFlags=USE_JAG3D_DATA_PATH_ENV_VAR );
    Options( const Options& rhs );
    ~Options();

    /** \brief Add an individual search path. */
    void addPath( const std::string& path );
    /** \brief Add multiple search paths separated by the platform-specific directory separator. */
    void addPaths( const std::string& paths );
    /** \brief Clear the list of directory search paths. */
    void clearPaths();
    /** \brief Get a list of all directory search paths. */
    const Poco::Path::StringVec& getPaths() const;

    /** \brief Search the directory path list for \c fileName and return the full path. */
    Poco::Path findFile( const std::string& fileName ) const;


    typedef std::map< std::string, boost::any > AnyMap;

    /** \class OptionsEasyInit Options.H <jagDisk/Options.H>
    */
    class OptionsEasyInit
    {
    public:
        OptionsEasyInit( AnyMap& anyMap )
          : _anyMap( anyMap )
        {}
        ~OptionsEasyInit() {}

        OptionsEasyInit& operator()( const std::string& name, const boost::any& value )
        {
            _anyMap[ name ] = value;
            return( *this );
        }

    protected:
        AnyMap& _anyMap;
    };

    OptionsEasyInit addOptions()
    {
        return( OptionsEasyInit( _anyMap ) );
    }
    bool hasOption( const std::string& name ) const
    {
        return( _anyMap.find( name ) != _anyMap.end() );
    }
    const boost::any getOption( const std::string& name ) const
    {
        AnyMap::const_iterator it( _anyMap.find( name ) );
        if( it != _anyMap.end() )
            return( *it );
        else
            return( boost::any() );
    }

protected:
    Poco::Path::StringVec _paths;

    AnyMap _anyMap;
};

typedef jagBase::ptr< jagDisk::Options >::shared_ptr OptionsPtr;
typedef jagBase::ptr< const jagDisk::Options >::shared_ptr ConstOptionsPtr;
typedef std::vector< OptionsPtr > OptionsVec;


/**@}*/


// jagDisk
}


// __JAGDISK_OPTIONS_H__
#endif
