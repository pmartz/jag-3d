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

#ifndef __JAGDISK_READER_WRITER_H__
#define __JAGDISK_READER_WRITER_H__ 1


#include <jagDisk/Export.h>
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
    virtual ReadStatus read( const std::string& /*fileName*/ ) const
    {
        return( ReadStatus() );
    }
    virtual bool write( const std::string& /*fileName*/, const void* /*data*/ ) const
    {
        return( false );
    }

    virtual ReadStatus read( std::istream& /*iStr*/ ) const
    {
        return( ReadStatus() );
    }
    virtual bool write( std::ostream& /*oStr*/, const void* /*data*/ ) const
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
