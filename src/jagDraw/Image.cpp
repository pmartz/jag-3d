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

#include <jagDraw/Image.h>


namespace jagDraw {


Image::Image()
  : jagBase::LogBase( "jag.draw.image" )
{
}
Image::Image( const Image& rhs )
  : LogBase( rhs )
{
}
Image::~Image()
{
}


void Image::set( const GLint level, const GLenum internalFormat,
    const GLsizei width, const GLsizei height, const GLsizei depth,
    const GLint border, const GLenum format, const GLenum type,
    jagBase::BufferPtr data )
{
    _level = level;
    _internalFormat = internalFormat;
    _width = width;
    _height = height;
    _depth = depth;
    _border = border;
    _format = format;
    _type = type;
    _data = data;
}
void Image::get( GLint& level, GLenum& internalFormat,
    GLsizei& width, GLsizei& height, GLsizei& depth,
    GLint& border, GLenum& format, GLenum& type,
    jagBase::BufferPtr data )
{
    level = _level;
    internalFormat = _internalFormat;
    width = _width;
    height = _height;
    depth = _depth;
    border = _border;
    format = _format;
    type = _type;
    data = _data;
}


// jagDraw
}
