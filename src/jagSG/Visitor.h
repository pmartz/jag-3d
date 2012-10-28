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

#ifndef __JAGSG_VISITOR_H__
#define __JAGSG_VISITOR_H__ 1

//#include <jagSG/Export.h>
#include <jagSG/Node.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>


namespace jagSG {


/** \class VisitorBase Visitor.h <jagSG/Visitor.h>
\brief TBD
\details TBD
*/
class /*JAGSG_EXPORT*/ VisitorBase : protected jagBase::LogBase
{
public:
    VisitorBase( const std::string& logName )
      : jagBase::LogBase( "jag.sg.visit." + logName )
    {
    }

    VisitorBase( const std::string& logName, jagSG::Node& node )
      : jagBase::LogBase( "jag.sg.visit." + logName )
    {
        node.accept( *this );
    }

    VisitorBase( const VisitorBase& rhs )
      : jagBase::LogBase( rhs )
    {
    }

    virtual ~VisitorBase()
    {
    }


    /** \brief TBD
    \details TBD */
    virtual void visit( jagSG::Node& node )
    {
        node.traverse( *this );
        // or:
        // if( node.getTraverseCallback() != NULL )
        //     node.traverseCallback( *this );
        // else
        //     node.traverse( *this );
    }

protected:
};

typedef jagBase::ptr< jagSG::VisitorBase >::shared_ptr VisitorBasePtr;



/** \class Visitor Visitor.h <jagSG/Visitor.h>
\brief TBD
\details TBD
*/
class /*JAGSG_EXPORT*/ Visitor : protected VisitorBase
{
public:
    Visitor( const std::string& logName )
      : VisitorBase( logName )
    {
    }

    Visitor( const std::string& logName, jagSG::Node& node )
      : VisitorBase( logName, node )
    {
    }

    Visitor( const VisitorBase& rhs )
      : VisitorBase( rhs )
    {
    }

    virtual ~Visitor()
    {
    }


    /** \brief TBD
    \details TBD */
    virtual void visit( jagSG::Node& node )
    {
        if( node.getTraverseCallback() != NULL )
            (*node.getTraverseCallback())( *this );
        else
            node.traverse( *this );
    }

protected:
};

typedef jagBase::ptr< jagSG::Visitor >::shared_ptr VisitorPtr;


// jagSG
}


// __JAGSG_VISITOR_H__
#endif
