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

#ifndef __JAG3D_DEMO_SUPPORT_DEMO_INTERFACE_H__
#define __JAG3D_DEMO_SUPPORT_DEMO_INTERFACE_H__ 1


#include <jag/draw/DrawInfo.h>
#include <jag/draw/Shader.h>
#include <jag/draw/Image.h>
#include <jagSG/Node.h>
#include <jagSG/CollectionVisitor.h>
#include <jagMx/MxCore.h>
#include <jag/disk/ReadWrite.h>
#include <jag/base/LogMacros.h>

#include <gmtl/gmtl.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/foreach.hpp>
#include <boost/any.hpp>


/** \class DemoInterface DemoInterface.h DemoInterface.h
\brief Facilitates building examples, tests, and demos with any
windowing system API, e.g., freeglut, Qt, etc.
*/
class DemoInterface
{
public:
    DemoInterface( const std::string& logName )
      : _logName( logName ),
        _startupCalled( false ),
        _continuousRedraw( false )
    {}
    virtual ~DemoInterface() {}

    static DemoInterface* create( boost::program_options::options_description& desc );
    virtual bool parseOptions( boost::program_options::variables_map& vm ) { return( true ); }

    /** Called before any contexts are created. */
    virtual bool startup( const unsigned int numContexts )
    {
        _startupCalled = true;

        _drawInfo._data.resize( numContexts );
        unsigned int id( 0 );
        BOOST_FOREACH( jag::draw::DrawInfo& drawInfo, _drawInfo._data )
        {
            drawInfo._id = id++;
        }

        return( true );
    }
    bool getStartupCalled() { return( _startupCalled ); }

    void setContinuousRedraw( bool redraw=true ) { _continuousRedraw = redraw; }
    bool getContinuousRedraw() { return( _continuousRedraw ); }

    /** Called for each context, after the context has been created. */
    virtual bool init() = 0;
    /** Called to render a frame. */
    virtual bool frame( const gmtl::Matrix44d& view=gmtl::MAT_IDENTITY44D, const gmtl::Matrix44d& proj=gmtl::MAT_IDENTITY44D ) = 0;
    /** Called on window resize. */
    virtual void reshape( const int w, const int h ) {}
    /** Called prior to exit. */
    virtual bool shutdown() { return( true ); }

    /** Simple interface for passing key press events. Return true to redraw. */
    virtual bool keyCommand( const int command ) { return( false ); }

    jag::draw::DrawInfo& getDrawInfo( const jag::draw::jagDrawContextID contextID )
    {
        return( _drawInfo._data[ contextID ] );
    }
    jagSG::CollectionVisitor& getCollectionVisitor()
    {
        return( _collectionVisitor );
    }

    jagMx::MxCorePtr getMxCore( const jag::draw::jagDrawContextID contextID )
    {
        if( _mxCore._data.empty() )
            return( jagMx::MxCorePtr( (jagMx::MxCore*)NULL ) );
        else
            return( _mxCore._data[ contextID ] );
    }

    /** Return a value to control base gamepad move rate in the scene. */
    virtual double getMoveRate() const
    {
        return( 1. );
    }

    /** \brief Default command line values.
    \details Demo code overrides these methods to change
    the default behavior for command line options that are
    used by all demo code. */
    virtual double defaultOpenGLVersion() const
    {
#if( POCO_OS == POCO_OS_MAC_OS_X )
        // In OSX 10.7/10.8, use GL 3.2 and GLSL 1.50
        return( 3.2 );
#else
        return( 4.0 );
#endif
    }
    virtual unsigned int defaultNumWindows() const
    {
        return( 1 );
    }
    typedef std::vector< int > WinSizeType;
    virtual WinSizeType defaultWinSize() const
    {
        return( WinSizeType( 2, 300 ) );
    }
    virtual bool defaultMultisample() const
    {
        return( true );
    }

    /** \brief Control window updates after mouse motion.
    \details If this method returns false, only the window
    receiving the mouse motion event will redraw after processing
    that event. Override this method and return true to cause
    all windows to redraw after the mouse motion event. */
    virtual bool mouseMotionRedrawsAllWindows() const
    {
        return( false );
    }

protected:
    std::string _logName;
    bool _startupCalled;

    bool _continuousRedraw;

    jag::draw::PerContextDrawInfo _drawInfo;
    jagSG::CollectionVisitor _collectionVisitor;

    typedef jag::draw::PerContextData< jagMx::MxCorePtr > PerContextMxCore;
    PerContextMxCore _mxCore;



#define __READ_UTIL( _RESULT, _TYPE, _NAME, _OPT ) \
    { \
        boost::any anyTemp( jag::disk::read( _NAME, _OPT ) ); \
        try { \
            _RESULT = boost::any_cast< _TYPE >( anyTemp ); \
        } \
        catch( boost::bad_any_cast bac ) \
        { \
            bac.what(); \
        } \
        if( _RESULT == NULL ) \
        { \
            JAG3D_FATAL_STATIC( _logName, std::string("Can't load \"") + std::string(_NAME) + std::string("\".") ); \
        } \
    }

    jagSG::NodePtr readSceneGraphNodeUtil( const std::string& fileName, const jag::disk::Options* options=NULL )
    {
        jagSG::NodePtr result;
        __READ_UTIL( result, jagSG::NodePtr, fileName, options );
        return( result );
    }
    jag::draw::ImagePtr readImageUtil( const std::string& fileName, const jag::disk::Options* options=NULL )
    {
        jag::draw::ImagePtr result;
        __READ_UTIL( result, jag::draw::ImagePtr, fileName, options );
        return( result );
    }
    jag::draw::ShaderPtr readShaderUtil( const std::string& fileName, const jag::disk::Options* options=NULL )
    {
        jag::draw::ShaderPtr result;
        __READ_UTIL( result, jag::draw::ShaderPtr, fileName, options );
        return( result );
    }
};


// __JAG3D_DEMO_SUPPORT_DEMO_INTERFACE_H__
#endif
