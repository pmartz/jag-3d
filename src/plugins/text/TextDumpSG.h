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
