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

#ifndef __TEXT_DUMP_S_G_H__
#define __TEXT_DUMP_S_G_H__ 1

#include <jagSG/Visitor.h>


/** \class TextDumpSG TextDumpSG.h
\brief Dump scene graph information to a text file.
\details TBD
*/
class TextDumpSG : public jagSG::Visitor
{
public:
    TextDumpSG( std::ostream& oStr );
    virtual ~TextDumpSG();

    /** \brief TBD
    \details TBD */
    virtual void visit( jagSG::Node& node );

protected:
    std::ostream& _oStr;
    int _indent;
};


// __TEXT_DUMP_S_G_H__
#endif
