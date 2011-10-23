/* @License 
 -------------------------------------------------------------------------------
 | Chaskii - Copyright (C) 2010  Don Burns, Andes Computer Engineering, LLC    |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
#ifndef CHASKII_DB_SCENE_LOADER_DEF
#define CHASKII_DB_SCENE_LOADER_DEF 1

#include <iostream>
#include <map>
#include <string>

#include <Chaskii/Export.h>
#include <Chaskii/SceneGraph/Node.h>

namespace iiDB {

class II_EXPORT SceneLoader
{
    public:
        SceneLoader() {}
        virtual ~SceneLoader() {}
        virtual iiSceneGraph::Node * load( const std::string &filename ){ return NULL; }
        virtual iiSceneGraph::Node * load( const std::istream & ) { return NULL; }
        virtual iiSceneGraph::Node * load( const char *inbuffer, size_t insize ) { return NULL; }
    private:
};

typedef iiMem::



class II_EXPORT SceneLoaderRegistry
{
    public:
        static SceneLoaderRegistry *instance();

        void registerSceneLoader( const std::string &extension, SceneLoader * );
        SceneLoader *findSceneLoaderForExtension( const std::string &extension );
        SceneLoader *findSceneLoaderForFile( const std::string &filename );

        void addAlias( const std::string &extension, const std::string &alias );
        const std::string &findAlias( const std::string &extension );

    private:
        SceneLoaderRegistry();
        virtual ~SceneLoaderRegistry();

        std::map<std::string, iiMem::ref_ptr<SceneLoader> > registry;
        std::map<std::string, std::string> aliases;
};

extern II_EXPORT iiSceneGraph::Node * LoadScene( const std::string &filename );
extern II_EXPORT iiSceneGraph::Node * LoadScene( const std::string &ext, const std::istream & );
extern II_EXPORT iiSceneGraph::Node * LoadScene( const std::string &ext, char *buffer, size_t insize );

}

#endif
