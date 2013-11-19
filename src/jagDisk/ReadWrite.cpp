/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#include <jagDisk/ReadWrite.h>
#include <jagDisk/PluginManager.h>
#include <jagDisk/Options.h>
#include <jagBase/LogMacros.h>
#include <Poco/File.h>

#include <boost/any.hpp>
#include <boost/foreach.hpp>


namespace jagDisk {


boost::any read( const std::string& fileName, const Options* options )
{
    const std::string logStr( "jag.disk.rw" );
    JAG3D_TRACE_STATIC( logStr, "read() for: " + fileName );

    Options localOptions;
    const Options* opt( options != NULL ? options : &localOptions );
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
        ReadStatus readStatus( rw->read( fullName, opt ) );
        if( readStatus() )
        {
            JAG3D_TRACE_STATIC( logStr, "\tread(): Success." );
            return( *readStatus );
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
            ReadStatus readStatus( rw->read( fullName, opt ) );
            if( readStatus() )
                return( *readStatus );
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

    ConstOptionsPtr opt( options != NULL ? options : new Options() );

    // Try loaded ReaderWriters first.
    const ReaderWriterInfoVec& rwVec( pm->getLoadedReaderWriters() );
    BOOST_FOREACH( const ReaderWriterInfo& rwInfo, rwVec )
    {
        ReaderWriterPtr rw( rwInfo._rwInstance );
        if( !( rw->supportsExtension( extension ) ) )
            continue;

        JAG3D_TRACE_STATIC( logStr, "\tTrying loaded ReaderWriter subclass " + rwInfo._className );
        if( rw->write( fileName, data, opt.get() ) )
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
            if( rw->write( fileName, data, opt.get() ) )
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
