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
