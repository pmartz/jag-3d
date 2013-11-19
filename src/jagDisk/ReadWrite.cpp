/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
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
