
#
# _extensionsToOSGPlugins
#
# Specifically for the OSG-based plugins. Takes a list of extensions
# and returns a list of OSG plugin names.
#
#   _extList - Input, list of extensions (jpeg;jpg;flt;rgba;rgb;<etc>)
#   _outPlugins - Output, list of plugins (jpeg;OpenFlight;rgb;<etc>)
#
macro( _extensionsToOSGPlugins _outPlugins _extList )
    set( _processed )
    foreach( _plug ${${_extList}} )
        if( ${_plug} STREQUAL "jpg" OR ${_plug} STREQUAL "jpeg" )
            set( _item jpeg )
        elseif( ${_plug} STREQUAL "rgb" OR ${_plug} STREQUAL "rgba" )
            set( _item rgb )
        elseif( ${_plug} STREQUAL "tif" OR ${_plug} STREQUAL "tiff" )
            set( _item tiff )
        elseif( ${_plug} STREQUAL "flt" )
            set( _item OpenFlight )
        elseif( ${_plug} STREQUAL "osgt" OR ${_plug} STREQUAL "osgb" )
            set( _item osgt )
        else()
            set( _item ${_plug} )
        endif()
        if( _item )
            list( FIND _processed ${_item} _findResult )
            if( _findResult LESS 0 )
                list( APPEND _processed ${_item} )
                list( APPEND ${_outPlugins} ${_item} )
            endif()
        endif()
    endforeach()
endmacro()


#
# _createOSGPluginReferendeCode
#
# For use by the OSG-based plugins when generating the pluginSupport.h file.
# From a list of plugins, return a string of code that invokes plugin static
# initializers.
#
#   _pluginList - Input, list of plugins (jpeg;OpenFlight;rgb;<etc>)
#   _outCode - Output, USE_OSGPLUGIN(<ext>) for each plugin
#
macro( _createOSGPluginReferendeCode _outCode _pluginList )
    set( ${_outCode} "#ifdef OSG_LIBRARY_STATIC\n" )
    set( ${_outCode} "${${_outCode}}#include <osgDB/Registry>\n" )
    foreach( _plug ${${_pluginList}} )
        if( ${_plug} STREQUAL "osg" )
            set( ${_outCode} "${${_outCode}}USE_OSGPLUGIN(${_plug})\n" )
            set( ${_outCode} "${${_outCode}}USE_DOTOSGWRAPPER_LIBRARY(${_plug})" )
        elseif( ${_plug} STREQUAL "osgt" )
            set( ${_outCode} "${${_outCode}}USE_OSGPLUGIN(osg2)\n" )
            set( ${_outCode} "${${_outCode}}USE_SERIALIZER_WRAPPER_LIBRARY(osg)" )
        else()
            set( ${_outCode} "${${_outCode}}USE_OSGPLUGIN(${_plug})" )
        endif()
        set( ${_outCode} "${${_outCode}}\n" )
    endforeach()
    set( ${_outCode} "${${_outCode}}#endif" )
endmacro()


#
# _createOSGPluginLibraries
#
# From a list of plugins, generate a list of library names to link against.
# JAG executables that use the OSG-based plugins must link against these
# libraries for static builds.
#
#   _pluginList - Input, list of plugins (jpeg;OpenFlight;rgb;<etc>)
#   _outLibs - Output, list of libraries (osgdb_jpeg;osgdb_OpenFlight;osgdb_rgb;<etc>)
#
macro( _createOSGPluginLibraries _outLibs _pluginList )
    foreach( _plugOrig ${${_pluginList}} )
        string( TOLOWER ${_plugOrig} _plug )
        if( ${_plug} STREQUAL "osg" )
            list( APPEND ${_outLibs} osgdb_deprecated_${_plug} )
            list( APPEND ${_outLibs} osgdb_${_plug} )
        elseif( ${_plug} STREQUAL "osgt" )
            list( APPEND ${_outLibs} osgdb_serializers_osg )
        else()
            list( APPEND ${_outLibs} osgdb_${_plug} )
        endif()
    endforeach()
endmacro()


#
# _createExtensionSupportedCode
#
# From a list of extensions, generate a string of code to return true
# for an extension name match, and false otherwise. Used to generate the
# pluginSupport.h header file.
#
#   _pluginList - Input, list of extensions (jpg;flt;rgba;<etc>)
#   _outCode - Output, code to return true if 'ext' is in list, false otherwise.
#
macro( _createExtensionSupportedCode _outCode _extList )
    foreach( _plug ${${_extList}} )
        if( ${_outCode} )
            set( ${_outCode} "${${_outCode}}\n" )
        endif()
        set( ${_outCode} "${${_outCode}}    if( ext == std::string(\"${_plug}\") ) return( true );" )
        if( ${_plug} STREQUAL "osgt" )
            set( ${_outCode} "${${_outCode}}\n    if( ext == std::string(\"osgb\") ) return( true );" )
        endif()
    endforeach()
    set( ${_outCode} "${${_outCode}}\n    return( false );" )
endmacro()


#
# _splitList
#
# Given an input token _tokenIn, return all the elements
# in ARGN preceding _tokenIn in _list0out, and all the elements
# following _tokenIn in _list1out. If _tokenIn is not present in _listIn,
# _list0out will be identical to ARGN, and _list1out will be empty.
#
macro( _splitList _tokenIn _list0out _list1out )
    set( ${_list0out} )
    set( ${_list1out} )
    set( tokenFound 0 )

    foreach( element ${ARGN} )
        if( tokenFound )
            list( APPEND ${_list1out} ${element} )
        elseif( ${element} STREQUAL ${_tokenIn} )
            set( tokenFound 1 )
        else()
            list( APPEND ${_list0out} ${element} )
        endif()
    endforeach()
endmacro()


#
# _subList
#
# Given two input token delimiters, _tokenStart and _tokenEnd, return
# all the elements in ARGN between the delimiters in _subListOut.
# If _tokenStart is the empty string (""), _subListOut will contain all
# elements from ARGN up to _tokenEnd. if _tokenEnd is the empty string,
# _subListOut will contain all elements from ARGN following _tokenStart.
# If both delimiters are empty, _subListOut will contain a copy of ARGN.
#
macro( _subList _tokenStart _tokenEnd _subListOut )
    string( LENGTH "${_tokenStart}" length )
    set( startFound FALSE )
    if( length EQUAL 0 )
        set( startFound TRUE )
    endif()

    unset( ${_subListOut} )
    foreach( element ${ARGN} )
        if( ${element} STREQUAL "${_tokenEnd}" )
            break()
        elseif( startFound )
            list( APPEND ${_subListOut} ${element} )
        elseif( ${element} STREQUAL ${_tokenStart} )
            set( startFound TRUE )
        endif()
    endforeach()
endmacro()


#
# _exeInstall
#
# Installs non-app executables into bin and share/Jag3D/bin directories.
# Category "App" goes into bin, and all other executables go into share/Jag3D/bin.
#
macro( _exeInstall _category _exeName )
    if( ${_category} STREQUAL "App" )
        set( _installDir bin )
    else()
        set( _installDir share/${CMAKE_PROJECT_NAME}/bin )
    endif()
    install(
        TARGETS ${_exeName}
        RUNTIME DESTINATION ${_installDir}
    )
endmacro()


#
# _addNonWindowedExecutable
# 
# add_executable() wrapper for all executables that do not require
# linking with a window system API.
#
macro( _addNonWindowedExecutable _category _exeName )
    add_executable( ${_exeName} ${ARGN} )
    # Append debug postfix to executables.
    set_property( TARGET ${_exeName} PROPERTY DEBUG_OUTPUT_NAME "${_exeName}${CMAKE_DEBUG_POSTFIX}" )

    include_directories(
        ${_projectIncludes}
        ${_requiredDependencyIncludes}
    )

    target_link_libraries( ${_exeName}
        ${_exeStaticLibs}
        ${_projectLibraries}
        ${_requiredDependencyLibraries}
    )

    _exeInstall( ${_category} ${_exeName} )

    set_target_properties( ${_exeName} PROPERTIES PROJECT_LABEL "${_category} ${_exeName}" )

    set_property( TARGET ${_exeName} PROPERTY FOLDER "${_category}")
endmacro()


#
# _addFreeglutExecutable
#
# add_executable() wrapper for executables that use FreeGlut.
#
macro( _addFreeglutExecutable _category _exeName )
    if( JAG3D_ENABLE_WINDOW_SYSTEM_SUFFIXES )
        set( _localExeName "${_exeName}-fg" )
    else()
        set( _localExeName "${_exeName}" )
    endif()

    unset( _sources )
    unset( _includesAndLibs )
    unset( _includes )
    unset( _libs )
    _splitList( ADDITIONAL_INCLUDES _sources _includesAndLibs ${ARGN} )
    if( "${_includesAndLibs}" STREQUAL "" )
        _splitList( ADDITIONAL_LIBRARIES _sources _libs ${ARGN} )
    else()
        _splitList( ADDITIONAL_LIBRARIES _includes _libs ${_includesAndLibs} )
    endif()

    add_executable( ${_localExeName}
        ${PROJECT_SOURCE_DIR}/src/demoSupport/freeglutSupport.cpp
        ${_sources}
    )
    # Append debug postfix to executables.
    set_property( TARGET ${_localExeName} PROPERTY DEBUG_OUTPUT_NAME "${_localExeName}${CMAKE_DEBUG_POSTFIX}" )

    unset( _allIncludes )
    set( _allIncludes
        ${_projectIncludes}
        ${Freeglut_INCLUDE_DIR}
        ${_includes}
        ${_requiredDependencyIncludes}
    )
    set_target_properties( ${_localExeName}
        PROPERTIES INCLUDE_DIRECTORIES "${_allIncludes}"
    )

    target_link_libraries( ${_localExeName}
        ${_libs}
        ${Freeglut_LIBRARIES}
        ${_exeStaticLibs}
        ${_projectLibraries}
        ${_requiredDependencyLibraries}
    )
    
    _exeInstall( ${_category} ${_localExeName} )

    set_target_properties( ${_localExeName} PROPERTIES PROJECT_LABEL "${_category} ${_localExeName}" )

    set_property( TARGET ${_localExeName} PROPERTY FOLDER "${_category}/FreeGlut")
endmacro()

#
# _addQtExecutable
#
# add_executable() wrapper for executables that use Qt.
#
macro( _addQtExecutable _category _exeName )
    if( JAG3D_ENABLE_WINDOW_SYSTEM_SUFFIXES )
        set( _localExeName "${_exeName}-qt" )
    else()
        set( _localExeName "${_exeName}" )
    endif()

    QT4_WRAP_CPP( _mocFiles
        ${PROJECT_SOURCE_DIR}/src/demoSupport/qtGlWidget.h
        OPTIONS "-f"
    )

    unset( _sources )
    unset( _includesAndLibs )
    unset( _includes )
    unset( _libs )
    _splitList( ADDITIONAL_INCLUDES _sources _includesAndLibs ${ARGN} )
    if( "${_includesAndLibs}" STREQUAL "" )
        _splitList( ADDITIONAL_LIBRARIES _sources _libs ${ARGN} )
    else()
        _splitList( ADDITIONAL_LIBRARIES _includes _libs ${_includesAndLibs} )
    endif()

    add_executable( ${_localExeName}
        ${PROJECT_SOURCE_DIR}/src/demoSupport/qtSupport.cpp
        ${_mocFiles}
        ${_sources}
    )
    # Append debug postfix to executables.
    set_property( TARGET ${_localExeName} PROPERTY DEBUG_OUTPUT_NAME "${_localExeName}${CMAKE_DEBUG_POSTFIX}" )

    unset( _allIncludes )
    set( _allIncludes
        ${_projectIncludes}
        ${QT_INCLUDE_DIR}
        ${QT_QTOPENGL_INCLUDE_DIR}
        ${QT_QTGUI_INCLUDE_DIR}
        ${QT_QTCORE_INCLUDE_DIR}
        ${_includes}
        ${_requiredDependencyIncludes}
    )
    set_target_properties( ${_localExeName}
        PROPERTIES INCLUDE_DIRECTORIES "${_allIncludes}"
    )

    target_link_libraries( ${_localExeName}
        ${_libs}
        ${QT_QTOPENGL_LIBRARY}
        ${QT_LIBRARIES}
        ${_exeStaticLibs}
        ${_projectLibraries}
        ${_requiredDependencyLibraries}
    )
    
    _exeInstall( ${_category} ${_localExeName} )

    set_target_properties( ${_localExeName} PROPERTIES PROJECT_LABEL "${_category} ${_localExeName}" )

    set_property( TARGET ${_localExeName} PROPERTY FOLDER "${_category}/Qt")
endmacro()


#
# _addVrjExecutable
#
# add_executable() wrapper for executables that use VR-Juggler.
#
macro( _addVrjExecutable _category _exeName )
    if( JAG3D_ENABLE_WINDOW_SYSTEM_SUFFIXES )
        set( _localExeName "${_exeName}-vrj" )
    else()
        set( _localExeName "${_exeName}" )
    endif()

    unset( _sources )
    unset( _includesAndLibs )
    unset( _includes )
    unset( _libs )
    _splitList( ADDITIONAL_INCLUDES _sources _includesAndLibs ${ARGN} )
    if( "${_includesAndLibs}" STREQUAL "" )
        _splitList( ADDITIONAL_LIBRARIES _sources _libs ${ARGN} )
    else()
        _splitList( ADDITIONAL_LIBRARIES _includes _libs ${_includesAndLibs} )
    endif()

    add_executable( ${_localExeName}
        ${PROJECT_SOURCE_DIR}/src/demoSupport/vrjSupport.cpp
        ${_sources}
    )
    # Append debug postfix to executables.
    set_property( TARGET ${_localExeName} PROPERTY DEBUG_OUTPUT_NAME "${_localExeName}${CMAKE_DEBUG_POSTFIX}" )

    unset( _allIncludes )
    set( _allIncludes
        ${_projectIncludes}
        ${VRJUGGLER_INCLUDE_DIRS}
        ${_includes}
        ${CPPDOM_INCLUDE_DIRS}
        ${_requiredDependencyIncludes}
    )
    set_target_properties( ${_localExeName}
        PROPERTIES INCLUDE_DIRECTORIES "${_allIncludes}"
    )

    target_link_libraries( ${_localExeName}
        ${_libs}
        ${VRJUGGLER_LIBRARIES}
        ${CPPDOM_LIBRARIES}
        ${_exeStaticLibs}
        ${_projectLibraries}
        ${_requiredDependencyLibraries}
    )
    
    _exeInstall( ${_category} ${_localExeName} )

    set_target_properties( ${_localExeName} PROPERTIES PROJECT_LABEL "${_category} ${_localExeName}" )

    set_property( TARGET ${_localExeName} PROPERTY FOLDER "${_category}/VRJ")
endmacro()


#
# _addExecutable
#
# For executables that use a windowing system API. Executable will
# be built for each enabled window system API. A window system is
# "enabled" if the JAG3D_USE_<name> CMake variable is ON, and the
# window system dependency is found by CMake.
#
macro( _addExecutable _category _exeName )
    if( Freeglut_FOUND AND JAG3D_USE_FREEGLUT )
        _addFreeglutExecutable( ${_category} ${_exeName} ${ARGN} )
    endif()
    if( QT4_FOUND AND JAG3D_USE_QT )
        _addQtExecutable( ${_category} ${_exeName} ${ARGN} )
    endif()
    if( VRJUGGLER_FOUND AND CPPDOM_FOUND AND JAG3D_USE_VRJ )
        _addVrjExecutable( ${_category} ${_exeName} ${ARGN} )
    endif()
endmacro()


#
# _addLibraryInternal
#
# Wrapper for add_library().
# CMake script common to both libraries and plugins.
#
macro( _addLibraryInternal _category _type _libName )
    _subList( "" JAG_LIBRARIES _sources ${ARGN} )
    _subList( JAG_LIBRARIES ADDITIONAL_INCLUDES _jagLibs ${ARGN} )
    _subList( ADDITIONAL_INCLUDES ADDITIONAL_LIBRARIES _includes ${ARGN} )
    _subList( ADDITIONAL_LIBRARIES "" _libs ${ARGN} )

    include_directories(
        ${_projectIncludes}
        ${_includes}
        ${_requiredDependencyIncludes}
    )

    if( BUILD_SHARED_LIBS )
        add_library( ${_libName} ${_type} ${_sources} )

        target_link_libraries( ${_libName}
            ${_jagLibs}
            ${_libs}
            ${_requiredDependencyLibraries}
        )
    else()
        add_library( ${_libName} STATIC ${_sources} )
    endif()


    set_target_properties( ${_libName} PROPERTIES PROJECT_LABEL "${_category} ${_libName}" )

    include( ModuleInstall REQUIRED )
endmacro()


#
# _addLibrary
#
# Add a core JAG library.
#
macro( _addLibrary _libName )
    _addLibraryInternal( Lib SHARED ${_libName} ${ARGN} )
    set_target_properties( ${_libName} PROPERTIES VERSION ${JAG3D_VERSION} )
    set_target_properties( ${_libName} PROPERTIES SOVERSION ${JAG3D_VERSION} )
endmacro()


#
# _addPlugin
#
# Add a JAG plugin library.
#
macro( _addPlugin _libName )
    _addLibraryInternal( Plugin MODULE ${_libName} ${ARGN} )

    set_property( TARGET ${_libName} PROPERTY FOLDER "Plugin")
endmacro()


#
# _addPluginInfo
#
# Supports jag3d plugin info (.jagpi) files, which must reside in the
# same directory as the plugin shared libraries / DLLs. This means
# they must be installed into the lib directory (bin on Windows), but also
# they must be copied into the CMake output build tree per config type.
#
macro( _addPluginInfo _name )
    if( BUILD_SHARED_LIBS )
        if( WIN32 )
            set( _pluginLoc bin )
        else()
            set( _pluginLoc lib )
        endif()

        # Copy .jagpi file to development lib (or bin) directories.
        # NOTE this is done during CMake config.
        foreach( _configDir ${CMAKE_CONFIGURATION_TYPES} )
            configure_file( ${_name}
                ${PROJECT_BINARY_DIR}/${_pluginLoc}/${_configDir}/${_name} COPYONLY )
        endforeach()

        # Install .jagpi file to destination directory.
        install( FILES ${_name}
            DESTINATION ${_pluginLoc}
        )
    endif()
endmacro()


#
# _findOSGPlugins
#
# The CMake stock "FindOSG.cmake" script does not find OSG plugins,
# which renders it inadequate for linking against static OSG. This
# macros finds the OSG plugins required by the OSG-based model and image
# loaders.
#
# Prerquisites:
# - OSG is already found (OSG_LIBRARY is set to valid path/filename).
# - JAG3D_OSGMODEL_PLUGIN_SUPPORT and JAG3D_OSGIMAGE_PLUGIN_SUPPORT
#   are set to CMake lists of extensions.
#
# Return:
# - OSG_PLUGIN_LIBRARIES list of plugin libraries and their dependencies.
#
macro( _findOSGPlugins )
    set( _pluginDir "osgPlugins-${OPENSCENEGRAPH_VERSION}" )
    get_filename_component( _path ${OSG_LIBRARY} PATH )
    set( OSG_PLUGIN_LIBRARIES )
    set( _allOSGExtensions ${JAG3D_OSGMODEL_PLUGIN_SUPPORT} ${JAG3D_OSGIMAGE_PLUGIN_SUPPORT} )
    _extensionsToOSGPlugins( _allPlugins _allOSGExtensions )
    _createOSGPluginLibraries( _osgPluginLibs _allPlugins )
    foreach( _lib ${_osgPluginLibs} )
        find_library( OSG_${_lib}_LIBRARY
            NAMES ${_lib}
            PATHS ${_path}
            PATH_SUFFIXES ${_pluginDir}
        )
        find_library( OSG_${_lib}_LIBRARY_DEBUG
            NAMES ${_lib}d
            PATHS ${_path}
            PATH_SUFFIXES ${_pluginDir}
        )

        if( NOT OSG_${_lib}_LIBRARY )
            message( WARNING "Could not find OSG plugin ${_lib}" )
        else()
            if( OSG_${_lib}_LIBRARY_DEBUG )
                list( APPEND OSG_PLUGIN_LIBRARIES "optimized" ${OSG_${_lib}_LIBRARY} )
                list( APPEND OSG_PLUGIN_LIBRARIES "debug" ${OSG_${_lib}_LIBRARY_DEBUG} )
            else()
                list( APPEND OSG_PLUGIN_LIBRARIES ${OSG_${_lib}_LIBRARY} )
            endif()
            mark_as_advanced( OSG_${_lib}_LIBRARY )
            mark_as_advanced( OSG_${_lib}_LIBRARY_DEBUG )
        endif()
    endforeach()

    # Now find any dependencies that the plugins require.
    foreach( _dep ${_allPlugins} )
        if( ${_dep} STREQUAL "gif" )
            find_package( GIF REQUIRED )
            if( GIF_FOUND )
                list( APPEND OSG_PLUGIN_LIBRARIES ${GIF_LIBRARY} )
            endif()
        endif()
        if( ${_dep} STREQUAL "jpeg" )
            find_package( JPEG REQUIRED )
            if( JPEG_FOUND )
                list( APPEND OSG_PLUGIN_LIBRARIES ${JPEG_LIBRARY} )
            endif()
        endif()
        if( ${_dep} STREQUAL "png" )
            find_package( PNG REQUIRED )
            if( PNG_FOUND )
                list( APPEND OSG_PLUGIN_LIBRARIES ${PNG_LIBRARY} )
            endif()
        endif()
        if( ${_dep} STREQUAL "tiff" )
            find_package( TIFF REQUIRED )
            if( TIFF_FOUND )
                list( APPEND OSG_PLUGIN_LIBRARIES ${TIFF_LIBRARY} )
            endif()
        endif()
    endforeach()
    find_package( ZLIB REQUIRED )
    if( ZLIB_FOUND )
        list( APPEND OSG_PLUGIN_LIBRARIES ${ZLIB_LIBRARY} )
    endif()
endmacro()


#
# _zeroPad
#
# Assumes the input string is a number. If necessary, the
# number is 0-padded to two digits.
#
macro( _zeroPad _input _output )
    if( ${${_input}} LESS 10 )
        set( ${_output} "0${${_input}}" )
    else()
        set( ${_output} "${${_input}}" )
    endif()
endmacro()
