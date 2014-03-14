
# Required for ModuleInstall.cmake
set( _libName jagp-osgImage )


# Configure the osgSupport.h header file.
set( _pluginName osgImage )
set( _pluginOSGExtensions ${JAG3D_OSGIMAGE_PLUGIN_SUPPORT} )
_createOSGExtensionReferenceCode( _pluginOSGExtensionRefs _pluginOSGExtensions )
_createOSGExtensionSupportedCode( _pluginOSGExtensionSupported _pluginOSGExtensions )
configure_file( "${PROJECT_SOURCE_DIR}/src/plugins/osg/osgSupport.h.in"
    "${PROJECT_SOURCE_DIR}/src/plugins/osgImage/osgSupport.h" )


_addPlugin( ${_libName}
    osgImage.cpp
    osgSupport.h
    JAG_LIBRARIES
        jagDisk jagDraw jagBase
    ADDITIONAL_INCLUDES
        ${OSG_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${OSG_LIBRARIES}
)

_addPluginInfo( jagp-osgImage.jagpi )
