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
#include <string>


namespace Poco {
    class FormattingChannel;
}


namespace jagBase
{


/** \addtogroup jagBaseLogging Message Logging Utilities */
/*@{*/


/** \class Log Log.h <jagBase/Log.h>
\brief Interface for setting message logging parameters.
\details Log is a singleton.
This class provides a single interface for managing message logging.
It maintains pointers to the console and log file channels, allows specifying
a log file name, and allows specifying Logger priority and channel destinations.
Under the hood, it provides uniform message formatting for all Jag Loggers.
*/
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


    /** \brief Sets the default log file name.
    \details By default, the default log file name is "jag3d.log". Change it by calling
    this function at init time prior to invocation of any other interaction with Jag.
    (Specifically, setLogFileName() must be called before creating a class derived from
    LogBase that uses LogFile as a destination.) */
    void setLogFileName( const std::string& logFileName );

    /** \brief TBD
    \details TBD
    \param prio Logging verbosity. 0 is silent, 8 is verbose. These values
    map directly to the Poco logging priorities in Poco/Message.h. */
    void setPriority( int prio, const std::string& logName="jag3d" );

    /** \brief TBD
    \details TBD
    */
    typedef enum {
        Console,
        LogFile
    } DestinationType;

    /** \brief TBD
    \details TBD
    \param prio Logging verbosity. 0 is silent, 8 is verbose. These values
    map directly to the Poco logging priorities in Poco/Message.h. */
    void setPriority( int prio, const DestinationType dest, const std::string& logName="jag3d" );

protected:
    Log();
    ~Log();

    // TBD Need singleton manager to cleanup/delete singletons.
    static Log* s_instance;;

    std::string _logFileName;

    // TBD should these be smart pointers?
    Poco::FormattingChannel* _console;
    Poco::FormattingChannel* _file;
};

/*@}*/


// jagBase
}


// __JAGBASE_LOG_H__
#endif
