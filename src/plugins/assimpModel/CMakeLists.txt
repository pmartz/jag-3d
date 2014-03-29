
# Configure the pluginSupport.h header file.
set( _pluginOSGExtensionRefs )
set( _pluginName assimpModel )
set( _pluginExtensions ${JAG3D_ASSIMPMODEL_PLUGIN_SUPPORT} )
_createExtensionSupportedCode( _pluginExtensionSupported _pluginExtensions )
configure_file( ${PROJECT_SOURCE_DIR}/src/plugins/common/pluginSupport.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/pluginSupport.h )

# Configure the jagp-<name>.jagpi file for this plugin.
_createPluginInfo( ${_pluginName}
    "ASSIMP-based model loader."
    JAG3D_ASSIMPMODEL_PLUGIN_SUPPORT )


# Required for ModuleInstall.cmake
set( _libName jagp-${_pluginName} )

_addPlugin( ${_libName}
    assimpModel.cpp
    assimp2jag.cpp
    assimp2jag.h
    pluginSupport.h
    JAG_LIBRARIES
        jagSG jagDisk jagDraw jagBase
    ADDITIONAL_INCLUDES
        ${ASSIMP_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${ASSIMP_LIBRARIES}
)
