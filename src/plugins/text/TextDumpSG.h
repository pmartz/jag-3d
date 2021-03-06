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

#ifndef __TEXT_DUMP_S_G_H__
#define __TEXT_DUMP_S_G_H__ 1

#include <jag/sg/Visitor.h>


/** \class TextDumpSG TextDumpSG.h
\brief Dump scene graph information to a text file.
\details TBD
*/
class TextDumpSG : public jag::sg::Visitor
{
public:
    TextDumpSG( std::ostream& oStr );
    virtual ~TextDumpSG();

    /** \brief TBD
    \details TBD */
    virtual void visit( jag::sg::Node& node );

protected:
    std::ostream& _oStr;
    int _indent;
};


// __TEXT_DUMP_S_G_H__
#endif
