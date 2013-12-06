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

#ifndef __JAGDRAW_DRAW_NODE_H__
#define __JAGDRAW_DRAW_NODE_H__ 1

#include <jagDraw/Export.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/Drawable.h>
#include <jagBase/MultiCallback.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>
#include <jagBase/UserDataOwner.h>
#include <vector>


namespace jagDraw {


/** \addtogroup jagDrawDrawGraph Draw Graph Data Structure */
/*@{*/

/** \class DrawNode DrawNode.h <jagDraw/DrawNode.h>
\brief The draw graph node element.
\details The draw graph is a list of DrawNodeContainers, and a NodeContainer
is a list of DrawNodes.

DrawNode associates a CommandMap with a collection of Drawables.
The CommandMap must not be NULL. DrawNode also contains a list of Callbacks
that are executed prior to the CommandMap and Drawables.

DrawNode optionally supports rendering a wireframe box around the bound
extents of all attached Drawables. This is controlled by
DrawInfo::_controlFlags. Bound rendering requires a special Program.
See the example source jagModel.cpp.

\logname jag.draw.node */
class JAGDRAW_EXPORT DrawNode : protected jagBase::LogBase, public ObjectIDOwner, public jagBase::UserDataOwner
{
public:
    DrawNode( CommandMapPtr commands=CommandMapPtr( (CommandMap*)NULL ), const std::string& logName=std::string( "" ) );
    DrawNode( const std::string& logName );
    DrawNode( const DrawNode& rhs );
    ~DrawNode();


    /** \brief Remove drawables and execute callbacks */
    void reset()
    {
        _drawables.clear();
        _executeCallbacks.clear();
    }


    /** \brief Add a drawable prep command, such as a uniform or a shader program.
    \details All drawable prep commands are executed by the Drawale in order, and prior to
    issuing any of the vertex array commands (see addVertexArrayCommand()). */
    void setCommandMap( CommandMapPtr commands )
    {
        _commands = commands;
    }
    /** \brief TBD
    \details TBD */
    CommandMapPtr& getCommandMap()
    {
        return( _commands );
    }
    /** \overload */
    const CommandMapPtr& getCommandMap() const
    {
        return( _commands );
    }


    /** \brief Add a drawable.
    \details All drawables are executed after issuing all commands in \c _commands. */
    void addDrawable( DrawablePtr drawable )
    {
        _drawables.push_back( drawable );
    }
    /** \brief TBD
    \details TBD */
    DrawableVec& getDrawableVec()
    {
        return( _drawables );
    }
    /** \overload */
    const DrawableVec& getDrawableVec() const
    {
        return( _drawables );
    }
    /** \brief TBD
    \details TBD */
    unsigned int getNumDrawables() const
    {
        return( (unsigned int) _drawables.size() );
    }

    /** \brief Set the distance (absolute eye coordinate z value).
    \details Set by jagSG::CollectionVisitor during collection.
    Can be used to sort Nodes by depth. */
    void setDistance( const double distance )
    {
        _distance = distance;
    }
    /** \brief Get the disance (absolute eye coordinate z value). */
    double getDistance() const
    {
        return( _distance );
    }


    /** \brief Assignment operator.
    \details Required by std::sort(). */
    DrawNode& operator=( const DrawNode& rhs )
    {
        _matrix = rhs._matrix;
        _commands = rhs._commands;
        _drawables = rhs._drawables;
        _distance = rhs._distance;
        _executeCallbacks = rhs._executeCallbacks;
        return( *this );
    }


    /** \struct Callback
    \brief TBD
    \details TBD */
    struct Callback {
        /** \brief TBD
        \details TBD */
        virtual bool operator()( jagDraw::DrawNode& /* node */, jagDraw::DrawInfo& /* drawInfo */ )
        {
            return( false );
        }
    };
    typedef jagBase::ptr< Callback >::shared_ptr CallbackPtr;


    /** \brief Transform
    \details TBD */
    void setTransform( const gmtl::Matrix44d& matrix );
    /** \brief TBD
    \details TBD */
    gmtl::Matrix44d& getTransform();
    /** \overload */
    const gmtl::Matrix44d& getTransform() const;


    /** \brief TBD
    \details TBD */
    typedef jagBase::MultiCallback< CallbackPtr > ExecuteCallbacks;
    /** \brief TBD
    \details TBD */
    ExecuteCallbacks& getExecuteCallbacks();
    /** \overload */
    const ExecuteCallbacks& getExecuteCallbacks() const;


    /** \brief Execute the attaches Callbacks, CommandMap and Drawables.
    \details Executes the contents of the jagDraw::DrawNode.

    If any attached callback returns false, execute() immediately returns
    without executing any subsequent callbacks, the CommandMap, or any of
    the Drawables.
    
    If DrawwInfo::_controlFlags has the DRAW_BOUND bit set, then
    the attahced CommandMap and Drawables are not executed. Rather, execute()
    specifies uniforms and a DrawCommand to render a wireframe representation
    of a box around the attached Drawables. Drawing the bound requies a
    special Program and VertexArrayObject be in effect, which is the application's
    responsibility. They can be set in application code, or in a NodeContainer
    callback, for example. */
    virtual void execute( DrawInfo& drawInfo );

    /** \brief Tell the contained ObjectID objects how many contexts to expect.
    \details Inherited from ObjectIDOwner. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Delete the ID for the given \c contextID.
    \details Inherited from ObjectIDOwner.
    OpenGL object ID cleanup is not yet implemented. TBD. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

protected:
    void setBoundUniforms( DrawInfo& drawInfo );

    gmtl::Matrix44d _matrix;
    CommandMapPtr _commands;
    DrawableVec _drawables;

    double _distance;

    ExecuteCallbacks _executeCallbacks;
};

typedef jagBase::ptr< jagDraw::DrawNode >::shared_ptr DrawNodePtr;
typedef std::vector< DrawNodePtr > DrawNodeVec;
typedef std::vector< DrawNode > DrawNodeSimpleVec;


class DrawNodeCommandSorter
{
public:
    DrawNodeCommandSorter()
    {}
    DrawNodeCommandSorter( const Command::CommandTypeVec& priorityVec ):
        _priorityVec( priorityVec )
    {}
    DrawNodeCommandSorter( const DrawNodeCommandSorter& rhs )
        : _priorityVec( rhs._priorityVec )
    {}
    ~DrawNodeCommandSorter()
    {}

    bool operator()( const DrawNodePtr& lhs, const DrawNodePtr& rhs ) const
    {
        const CommandMapPtr& lhsCommands( lhs->getCommandMap() );
        const CommandMapPtr& rhsCommands( rhs->getCommandMap() );

        for( Command::CommandTypeVec::const_iterator typeIter = _priorityVec.begin(); typeIter != _priorityVec.end(); ++typeIter )
        {
            switch( (int)( lhsCommands->_bits[ *typeIter ] ) | ( rhsCommands->_bits[ *typeIter ] << 1 ) )
            {
                case 3: 
                {
                    const DrawablePrepPtr a( lhsCommands->_data.find( *typeIter )->second );
                    const DrawablePrepPtr b( rhsCommands->_data.find( *typeIter )->second );
                    if( *a < *b )
                        return( true );
                    if( *a > *b )
                        return( false );
                    break;
                }
                case 0:
                    continue;
                case 1:
                    return( true );
                case 2:
                    return( false );
            }
        }
        return( false );
    }

protected:
    Command::CommandTypeVec _priorityVec;
};


class DrawNodeDistanceSorter
{
public:
    typedef enum {
        Ascending,
        Descending
    } SortOrder;

    DrawNodeDistanceSorter( const SortOrder order=Descending )
        : _order( order )
    {}
    DrawNodeDistanceSorter( const DrawNodeDistanceSorter& rhs )
        : _order( rhs._order )
    {}
    ~DrawNodeDistanceSorter()
    {}

    bool operator()( const DrawNodePtr& lhs, const DrawNodePtr& rhs ) const
    {
        const double lDist( lhs->getDistance() );
        const double rDist( rhs->getDistance() );
        if( lDist < rDist )
            return( _order == Ascending );
        else if( lDist > rDist )
            return( _order != Ascending );
        return( false );
    }

protected:
    SortOrder _order;
};


/*@}*/


// jagDraw
}


// __JAGDRAW_DRAW_NODE_H__
#endif
