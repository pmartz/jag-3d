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

#include <boost/foreach.hpp>


namespace jagBase {


/** \class Notifier Notifier.h <jagBase/Notifier.h>
\brief Common notification system base class.
\details TBD
*/
class Notifier
{
public:
    /** Constructor */
    Notifier()
    {
    }
    /** Copy constructor */
    Notifier( const Notifier& rhs )
      : _callbacks( rhs._callbacks )
    {
    }
    /** Destructor */
    ~Notifier()
    {
    }


    /** \struct NotifyInfo Notifier.h <jagBase/Notifier.h>
    \brief Base class containing information passed to Notifier::Callback objects. */
    struct NotifyInfo
    {
        NotifyInfo()
        {
        }
        NotifyInfo( const NotifyInfo& rhs )
        {
        }
        virtual ~NotifyInfo()
        {
        }

        // Derived classes add additional field / member functions as needed.
    };
    typedef jagBase::ptr< NotifyInfo >::shared_ptr NotifyInfoPtr;

    /** \struct Callback Notifier.h <jagBase/Notifier.h>
    \brief Notifies client code  */
    struct Callback
    {
        Callback()
        {
        }
        Callback( const Callback& rhs )
        {
        }
        virtual ~Callback()
        {
        }

        /** \brief TBD
        \details TBD */
        virtual void operator()( Notifier* /* notifier */, NotifyInfoPtr /* info */ )
        {
        }
    };
    typedef jagBase::ptr< Callback >::shared_ptr CallbackPtr;

    /** \brief Notify all attached callbacks.
    \details Invoke the operator function call on all attached callbacks
    and pass the specified \c info struct as a parameter. */
    void notify( NotifyInfoPtr info=( NotifyInfoPtr( (NotifyInfo*)NULL ) ) )
    {
        BOOST_FOREACH( CallbackPtr& cb, _callbacks )
        {
            (*cb)( this, info );
        }
    }

    /** \brief TBD
    \details TBD */
    typedef jagBase::MultiCallback< CallbackPtr > Callbacks;

    /** \brief TBD
    \details TBD */
    Callbacks& getCallbacks()
    {
        return( _callbacks );
    }

protected:
    /** Default value: _boundDirtyCallbacks = BoundDirtyCallbacks(); */
    Callbacks _callbacks;
};


// jagBase
}


// __JAGBASE_NOTIFIER_H__
#endif
