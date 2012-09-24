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
#include <string>
#include <vector>
#include <iostream>


namespace jagDisk {


/** \addtogroup PluginSupport Plugin Support
*/
/**@{*/


/** \class ReaderWriter ReaderWriter.H <jagDisk/ReaderWriter.H>
\brief
\details
*/
class JAGDISK_EXPORT ReaderWriter : protected jagBase::LogBase
{
public:
    ReaderWriter( const std::string& logName=std::string( "rw" ) );
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

    virtual void* read( const std::string& /*fileName*/ ) const
    {
        return( NULL );
    }
    virtual bool write( const std::string& /*fileName*/, const void* /*data*/ ) const
    {
        return( false );
    }

    virtual void* read( std::istream& /*iStr*/ ) const
    {
        return( NULL );
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
