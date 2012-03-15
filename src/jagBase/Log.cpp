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

#include <jagBase/Log.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FileChannel.h>


namespace jagBase {


Log* Log::s_instance( NULL );


Log::Log()
  : _console( NULL ),
    _file( NULL )
{
    _console = new Poco::FormattingChannel( new Poco::PatternFormatter("%s: %p: %t") );
	_console->setChannel( new Poco::ConsoleChannel );
	_console->open();

    _file = new Poco::FormattingChannel( new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t") );
    _file->setChannel( new Poco::FileChannel( "jag3d.log" ) );
    _file->open();
}
Log::~Log()
{
}


// jagBase
}
