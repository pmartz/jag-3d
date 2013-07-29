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

#ifndef __JAGBASE_NOTIFIER_H__
#define __JAGBASE_NOTIFIER_H__ 1


#include <jagBase/ptr.h>
#include <jagBase/MultiCallback.h>
#include <jagBase/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagBase {


/** \class Notifier Notifier.h <jagBase/Notifier.h>
\brief Common notification system base class.
\details Currently, this is used by Drawable to notify jagSG::Node
when the Drawable bound is dirtied. This seems like a very heavy
weight solution. Need to consider leveraging Poco's notification
mechanism as a possible replacement. */
class Notifier
{
public:
    /** Constructor */
    Notifier()
    {
    }
    /** Copy constructor */
    Notifier( const Notifier& rhs )
      : _notifierCallbacks( rhs._notifierCallbacks )
    {
    }
    /** Destructor */
    ~Notifier()
    {
    }


    /** \struct NotifierInfo Notifier.h <jagBase/Notifier.h>
    \brief Base class containing information passed to Notifier::Callback objects. */
    struct NotifierInfo
    {
        NotifierInfo()
        {}
        NotifierInfo( const NotifierInfo& rhs )
        {}
        virtual ~NotifierInfo()
        {}

        // Derived classes add additional field / member functions as needed.
    };

    /** \struct Callback Notifier.h <jagBase/Notifier.h>
    \brief Notifies client code  */
    struct NotifierCallback
    {
        NotifierCallback()
        {}
        NotifierCallback( const NotifierCallback& rhs )
        {}
        virtual ~NotifierCallback()
        {}

        /** \brief TBD
        \details TBD */
        virtual void operator()( jagBase::Notifier* /* notifier */, const jagBase::Notifier::NotifierInfo& /* info */ )
        {
        }
    };
    typedef jagBase::ptr< NotifierCallback >::shared_ptr NotifierCallbackPtr;

    /** \brief Notify all attached callbacks.
    \details Invoke the operator function call on all attached callbacks
    and pass the specified \c info struct as a parameter. */
    void notify( const NotifierInfo& info )
    {
        BOOST_FOREACH( NotifierCallbackPtr& cb, _notifierCallbacks )
        {
            (*cb)( this, info );
        }
    }

    /** \brief TBD
    \details TBD */
    typedef jagBase::MultiCallback< NotifierCallbackPtr > NotifierCallbacks;

    /** \brief TBD
    \details TBD */
    NotifierCallbacks& getNotifierCallbacks()
    {
        return( _notifierCallbacks );
    }
    /** \brief TBD
    \details TBD */
    void removeNotifierCallback( NotifierCallbackPtr target )
    {
        JAG3D_CRITICAL_STATIC( "jag.base.notifier", "removeCallback()." );

        NotifierCallbacks::iterator it;
        for( it = _notifierCallbacks.begin(); it != _notifierCallbacks.end(); ++it )
        {
            if( (*it) == target )
            {
                it = _notifierCallbacks.erase( it );
            }
        }
    }

protected:
    /** Default value: _boundDirtyCallbacks = BoundDirtyCallbacks(); */
    NotifierCallbacks _notifierCallbacks;
};


// jagBase
}


// __JAGBASE_NOTIFIER_H__
#endif