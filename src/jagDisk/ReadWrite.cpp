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
#include <jagDisk/Options.h>
#include <jagBase/LogMacros.h>
#include <Poco/File.h>

#include <boost/foreach.hpp>


namespace jagDisk {


void* read( const std::string& fileName, const Options* options )
{
    const std::string logStr( "jag.disk.rw" );
    JAG3D_TRACE_STATIC( logStr, "read() for: " + fileName );

    ConstOptionsPtr opt( options != NULL ? options : new Options() );
    Poco::Path pathName( opt->findFile( fileName ) );
    if( !( pathName.isFile() ) )
    {
        JAG3D_TRACE_STATIC( logStr, "\tFailed. File not found." );
        return( NULL );
    }
    const std::string fullName( pathName.toString() );
    JAG3D_TRACE_STATIC( logStr, "\tFound: \"" + fullName + "\"." );
    const std::string extension( pathName.getExtension() );

    PluginManager* pm( PluginManager::instance() );

    // Try loaded ReaderWriters first.
    const ReaderWriterInfoVec& rwVec( pm->getLoadedReaderWriters() );
    BOOST_FOREACH( const ReaderWriterInfo& rwInfo, rwVec )
    {
        ReaderWriterPtr rw( rwInfo._rwInstance );
        if( !( rw->supportsExtension( extension ) ) )
            continue;

        JAG3D_TRACE_STATIC( logStr, "\tTrying loaded ReaderWriter subclass " + rwInfo._className );
        void* data( rw->read( fullName ) );
        if( data != NULL )
        {
            JAG3D_TRACE_STATIC( logStr, "\tread(): Success." );
            return( data );
        }
    }

    // Get a PluginInfo for each plugin that advertizes support for this extension.
    PluginManager::PluginInfoPtrVec plugins( pm->getPluginsForExtension( extension ) );

    BOOST_FOREACH( PluginManager::PluginInfo* pi, plugins )
    {
        if( pi->_loaded )
            // We already tried the ReaderWriter(s) in this plugin.
            continue;

        JAG3D_TRACE_STATIC( logStr, "\tLoading new plugin: " + pi->_name );
        if( !( pm->loadPlugin( pi ) ) )
            // Load failes.
            continue;

        BOOST_FOREACH( ReaderWriterPtr rw, pi->_readerWriters )
        {
            JAG3D_TRACE_STATIC( logStr, "\tTrying new ReaderWriter." );
            void* data( rw->read( fullName ) );
            if( data != NULL )
                return( data );
        }
    }

    JAG3D_NOTICE_STATIC( logStr, "Read operation failed for:" );
    JAG3D_NOTICE_STATIC( logStr, "\tread( \"" + fullName + "\" )." );

    return( NULL );
}

bool write( const std::string& fileName, const void* data, const Options* options )
{
    PluginManager* pm( PluginManager::instance() );

    const std::string logStr( "jag.disk.rw" );
    JAG3D_TRACE_STATIC( logStr, "write() for: " + fileName );

    Poco::Path pathName( fileName );
    const std::string extension( pathName.getExtension() );

    // Try loaded ReaderWriters first.
    const ReaderWriterInfoVec& rwVec( pm->getLoadedReaderWriters() );
    BOOST_FOREACH( const ReaderWriterInfo& rwInfo, rwVec )
    {
        ReaderWriterPtr rw( rwInfo._rwInstance );
        if( !( rw->supportsExtension( extension ) ) )
            continue;

        JAG3D_TRACE_STATIC( logStr, "\tTrying loaded ReaderWriter subclass " + rwInfo._className );
        if( rw->write( fileName, data ) )
        {
            JAG3D_TRACE_STATIC( logStr, "\twrite(): Success." );
            return( true );
        }
    }

    // Get a PluginInfo for each plugin that advertizes support for this extension.
    PluginManager::PluginInfoPtrVec plugins( pm->getPluginsForExtension( extension ) );

    BOOST_FOREACH( PluginManager::PluginInfo* pi, plugins )
    {
        if( pi->_loaded )
            // We already tried the ReaderWriter(s) in this plugin.
            continue;

        JAG3D_TRACE_STATIC( logStr, "\tLoading new plugin: " + pi->_name );
        if( !( pm->loadPlugin( pi ) ) )
            // Load failes.
            continue;

        BOOST_FOREACH( ReaderWriterPtr rw, pi->_readerWriters )
        {
            JAG3D_TRACE_STATIC( logStr, "\tTrying new ReaderWriter." );
            if( rw->write( fileName, data ) )
            {
                JAG3D_TRACE_STATIC( logStr, "\twrite(): Success." );
                return( true );
            }
        }
    }

    JAG3D_NOTICE_STATIC( logStr, "Write operation failed for:" );
    JAG3D_NOTICE_STATIC( logStr, "\twrite( \"" + fileName + "\" )." );

    return( false );
}


// jagDisk
}
