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

#include <jagDisk/ReadWrite.h>
#include <jagDisk/PluginManager.h>
#include <jagBase/LogMacros.h>
#include <Poco/File.h>

#include <boost/foreach.hpp>


namespace jagDisk {


void* read( const std::string& fileName )
{
    PluginManager* pm( PluginManager::instance() );

    // Try loaded ReaderWriters first.
    const ReaderWriterInfoVec& rwVec( pm->getLoadedReaderWriters() );
    BOOST_FOREACH( const ReaderWriterInfo& rwInfo, rwVec )
    {
        ReaderWriterPtr rw( rwInfo._rwInstance );
        void* data( rw->read( fileName ) );
        if( data != NULL )
            return( data );
    }

    Poco::Path pathName( fileName );

    // Get a PluginInfo for each plugin that advertizes support for this extension.
    const std::string extension( pathName.getExtension() );
    PluginManager::PluginInfoPtrVec plugins( pm->getPluginsForExtension( extension ) );

    BOOST_FOREACH( PluginManager::PluginInfo* pi, plugins )
    {
        if( pi->_loaded )
            // We already tried the ReaderWriter(s) in this plugin.
            continue;

        if( !( pm->loadPlugin( pi ) ) )
            // Load failes.
            continue;

        BOOST_FOREACH( ReaderWriterPtr rw, pi->_readerWriters )
        {
            void* data( rw->read( fileName ) );
            if( data != NULL )
                return( data );
        }
    }

    const std::string logStr( "jag.disk.rw" );
    JAG3D_NOTICE_STATIC( logStr, "\tRead operation failed for:" );
    JAG3D_NOTICE_STATIC( logStr, "read( \"" + fileName + "\" )." );

    return( NULL );
}

bool write( const std::string& fileName, const void* data )
{
    return( false );
}


// jagDisk
}
