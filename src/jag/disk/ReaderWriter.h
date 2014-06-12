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

#ifndef __JAG_DISK_READER_WRITER_H__
#define __JAG_DISK_READER_WRITER_H__ 1


#include <jag/disk/Export.h>
#include <jag/disk/Options.h>
#include <Poco/Path.h>

#include <jag/base/ptr.h>
#include <boost/any.hpp>
#include <string>
#include <vector>
#include <iostream>


namespace jag {
namespace disk {


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/


/** \class ReadStatus ReaderWriter.H <jag/disk/ReaderWriter.H>
\brief Container for the result of a plugin read() operation.
\details Contains a boolean result and a boost::any. If the
result is true, boost::any holds a valid read result (usually
a jag::base::ptr::shared_ptr). */
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



/** \class ReaderWriter ReaderWriter.H <jag/disk/ReaderWriter.H>
\brief
\details

Node: ReaderWriter can not derive from jag::base::LogBase due to the
indeterminate order of static initializers issue. Plugin classes
derived from ReaderWriter should use the *STATIC logging macros
(e.g. JAG3D_ERROR_STATIC) from jag/base/LogMacros.h.
*/
class JAGDISK_EXPORT ReaderWriter
{
public:
    ReaderWriter();
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

    // Returns a boost::any containing a jag::base::ptr::shared_ptr.
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

typedef jag::base::ptr< jag::disk::ReaderWriter >::shared_ptr ReaderWriterPtr;
typedef std::vector< ReaderWriterPtr > ReaderWriterVec;


/**@}*/


// namespace jag::disk::
}
}


// __JAG_DISK_READER_WRITER_H__
#endif
