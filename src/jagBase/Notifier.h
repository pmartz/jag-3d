/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
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
