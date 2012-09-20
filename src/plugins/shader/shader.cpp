/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 *
 * Copyright 2012-2012 by Ames Laboratory
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.rb END do not edit this line> ***************/

#include <jagDisk/PluginManager.h>
#include <jagDisk/ReaderWriter.h>
#include <Poco/ClassLibrary.h>

#include <jagDraw/Shader.h>


using namespace jagDraw;
using namespace jagDisk;


class ShaderRW : public ReaderWriter
{
public:
    ShaderRW()
      : ReaderWriter( "shaderrw" )
    {}
    virtual ~ShaderRW()
    {}

    virtual bool supportsExtension( const std::string& extension )
    {
        const std::string supported( "vs gs fs vert geom frag" );
        return( !( extension.empty() ) &&
            ( supported.find( extension ) != std::string::npos ) );
    }


    virtual void* read( const std::string& fileName )
    {
        return( (Shader*)NULL );
    }

protected:
};

// Register the ShaderRW class with the PluginManager.
// This macro declares a static object initialized when the plugin is loaded.
REGISTER_READERWRITER(
    new ShaderRW(),   // Create an instance of MyMask.
    ShaderRW,         // Class name -- NOT a string.
    "ReaderWriter",   // Base class name as a string.
    "Test mask."      // Description text.
);


// Poco ClassLibrary manifest registration. Add a POCO_EXPORT_CLASS
// for each ReaderWriter class in the plugin.
POCO_BEGIN_MANIFEST( ReaderWriter )
    POCO_EXPORT_CLASS( ShaderRW )
POCO_END_MANIFEST
