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

#ifndef __JAGBASE_LOG_H__
#define __JAGBASE_LOG_H__ 1


#include <jagBase/Export.h>
#include <stdlib.h>


namespace Poco {
    class FormattingChannel;
}


namespace jagBase
{


class JAGBASE_EXPORT Log
{
public:
    static Log* instance()
    {
        if( s_instance == NULL )
        {
            s_instance = new Log;
        }
        return( s_instance );
    }

    Poco::FormattingChannel* getConsole()
    {
        return( _console );
    }
    Poco::FormattingChannel* getFile()
    {
        return( _file );
    }

protected:
    Log();
    ~Log();

    static Log* s_instance;;

    Poco::FormattingChannel* _console;
    Poco::FormattingChannel* _file;
};


// jagBase
}


// __JAGBASE_LOG_H__
#endif
