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

#ifndef __JAGDISK_READER_WRITER_H__
#define __JAGDISK_READER_WRITER_H__ 1


#include <jagDisk/Export.h>
#include <jagDisk/Options.h>
#include <jagBase/LogBase.h>
#include <Poco/Path.h>

#include <jagBase/ptr.h>
#include <boost/any.hpp>
#include <string>
#include <vector>
#include <iostream>


namespace jagDisk {


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/


/** \class ReadStatus ReaderWriter.H <jagDisk/ReaderWriter.H>
\brief Container for the result of a plugin read() operation.
\details Contains a boolean result and a boost::any. If the
result is true, boost::any holds a valid read result (usually
a jagBase::ptr::shared_ptr). */
class JAGDISK_EXPORT ReadStatus
{
public:
    ReadStatus()
        : _result( false )
    {}
    ReadStatus( boost::any& data )
        : _result( true ),
          _data( data )
    {}
    ~ReadStatus()
    {}

    void set( boost::any& data )
    {
        _result = true;
        _data = data;
    }

    bool operator()() const
    {
        return( _result );
    }

    boost::any operator*() const
    {
        return( _data );
    }

protected:
    bool _result;
    boost::any _data;
};



/** \class ReaderWriter ReaderWriter.H <jagDisk/ReaderWriter.H>
\brief
\details
*/
class JAGDISK_EXPORT ReaderWriter : protected jagBase::LogBase
{
public:
    ReaderWriter( const std::string& logNameSuffix, const std::string& logName=std::string( "" ) );
    ReaderWriter( const ReaderWriter& rhs );
    ~ReaderWriter();

    virtual bool supportsExtension( const std::string& /*extension*/ )
    {
        return( true );
    }
    virtual bool excludesExtension( const std::string& /*extension*/ )
    {
        return( false );
    }

    // Returns a boost::any containing a jagBase::ptr::shared_ptr.
    virtual ReadStatus read( const std::string& /*fileName*/, const Options* options=NULL ) const
    {
        return( ReadStatus() );
    }
    virtual bool write( const std::string& /*fileName*/, const void* /*data*/, const Options* options=NULL ) const
    {
        return( false );
    }

    virtual ReadStatus read( std::istream& /*iStr*/, const Options* options=NULL ) const
    {
        return( ReadStatus() );
    }
    virtual bool write( std::ostream& /*oStr*/, const void* /*data*/, const Options* options=NULL ) const
    {
        return( false );
    }
};

typedef jagBase::ptr< jagDisk::ReaderWriter >::shared_ptr ReaderWriterPtr;
typedef std::vector< ReaderWriterPtr > ReaderWriterVec;


/**@}*/


// jagDisk
}


// __JAGDISK_READER_WRITER_H__
#endif
