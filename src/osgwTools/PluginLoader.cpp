/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * osgWorks is (C) Copyright 2009-2012 by Kenneth Mark Bryden
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

#include <osg/Notify>
#include <osgDB/Registry>
#include <osgwTools/Version.h>

#include <string>
#include <ostream>



// Works around a shortcoming in the osgDB::Registry. If there are two different
// plugins supporting two different formats that both use the same extension (such
// as ".skel") then one of them needs to be loaded explicitly in order for read
// operations to always succeed. This class explicitly loads a .skel plugin. We
// don't really care which one it finds and loads, as long as it succeeds. A
// static variable is declared below. The constructor should be invoked when the
// executable and shared library are loaded, before main() starts to
// execite.
//
/* \cond */
class PluginLoader
{
public:
    PluginLoader()
    {
        // Register the ".skel" slias for the skeleton plugin.
        osgDB::Registry::instance()->addFileExtensionAlias( "skel", "skeleton" );

        const std::string libName( osgDB::Registry::instance()->createLibraryNameForExtension( "skeleton" ) );
        std::ostream& ostr( osg::notify( osg::INFO ) );

#if( OSGWORKS_OSG_VERSION >= 20800 )
        osgDB::Registry::LoadStatus stat( osgDB::Registry::instance()->loadLibrary( libName ) );
        ostr << ".skeleton plugin lib name: \"" << libName << "\" ";
        switch( stat ) {
        case osgDB::Registry::NOT_LOADED:
            ostr << " NOT_LOADED" << std::endl;
            break;
        case osgDB::Registry::PREVIOUSLY_LOADED:
            ostr << " PREVIOUSLY_LOADED" << std::endl;
            break;
        case osgDB::Registry::LOADED:
            ostr << " LOADED" << std::endl;
            break;
        default:
            ostr << " Unknown load status" << std::endl;
            break;
        }
#else
        // No Registry::LoadStatus before OSG v2.8.
        bool stat( osgDB::Registry::instance()->loadLibrary( libName ) );
        ostr << ".skeleton plugin lib name: \"" << libName << "\" " <<
            (stat ? "Loaded" : "Failed to load") << std::endl;
#endif
    }
    ~PluginLoader()
    {
        //std::ostream& ostr( osg::notify( osg::INFO ) );
        //ostr << "~PluginLoader." << std::endl;
    }
};
/* \endcond */
static PluginLoader s_pluginLoader;
