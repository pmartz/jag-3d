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
#ifndef CHASKII_DB_PLUGIN_LOADER_DEF
#define CHASKII_DB_PLUGIN_LOADER_DEF 1

#include <string>
#include <map>
#include <Chaskii/Export.h>
#include <Chaskii/Mem/ptr.h>

namespace iiDB {

class II_EXPORT PluginLoader
{
    public:
        static PluginLoader *instance();
        bool loadPlugin( const std::string &ext );

    private:
        //friend class PluginLoader_ptr;
        PluginLoader();
        ~PluginLoader();

        std::map<std::string,std::string> _aliasMap;
};

typedef iiMem::ptr<PluginLoader>::shared_ptr PluginLoader_ptr;

}

#endif
