# Copyright

# If you are using a static GLEW build, add the GLEW_STATIC entry
# in CMake and select it. Otherwise, glewInit() will come up as an
# undefined symbol.
if( GLEW_STATIC )
    add_definitions( -DGLEW_STATIC )
endif()


# Macro to force the FindGLEW.cmake script to
# search again for GLEW.
macro( unFindGLEW )
#    message( STATUS "In unFindGLEW" )

    unset( "GLEW_INCLUDE_DIR" CACHE )
    unset( "GLEW_LIBRARIES" CACHE )
    unset( "GLEW_FOUND" CACHE )
    unset( "GLEW_glew_LIBRARY" CACHE )
endmacro( unFindGLEW )


macro( GLEWMenuSetup )
    # What type of GLEW should we look for? This is a combo box
    # supporting three types of GLEW installations:
    #  * Default installation (in which the stock FindGLEW.cmake script does all the work)
    #  * Alternate Install Location (user must set the GLEWInstallLocation variable)
    #  * Source And Build Tree (user must supply both the GLEWSourceRoot and GLEWBuildRoot variables)
    set( GLEWInstallType "Default Installation" CACHE STRING "Type of GLEW install: 'Default Installation', 'Alternate Install Location', or 'Source And Build Tree'." )
    set_property( CACHE GLEWInstallType PROPERTY STRINGS "Default Installation" "Alternate Install Location" "Source And Build Tree" )

    # We need to detect when the user changes the GLEW install type
    # or any of the related directory variables, so that we'll know
    # to call unFindGLEW() and force the stock GLEW script to search
    # again. To do this, we save the last set value of these variables
    # in the CMake cache as internal (hidden) variables.
    if( NOT DEFINED _lastGLEWInstallType )
        set( _lastGLEWInstallType "empty" CACHE INTERNAL "" )
    endif()
    if( NOT DEFINED _lastGLEWInstallLocation )
        set( _lastGLEWInstallLocation "empty" CACHE INTERNAL "" )
    endif()
    if( NOT DEFINED _lastGLEWSourceRoot )
        set( _lastGLEWSourceRoot "empty" CACHE INTERNAL "" )
    endif()
    if( NOT DEFINED _lastGLEWBuildRoot )
        set( _lastGLEWBuildRoot "empty" CACHE INTERNAL "" )
    endif()

    if( NOT DEFINED GLEWInstallLocation )
        set( GLEWInstallLocation "Please specify" )
    endif()
    if( NOT DEFINED GLEWSourceRoot )
        set( GLEWSourceRoot "Please specify" )
    endif()
    if( NOT DEFINED GLEWBuildRoot )
        set( GLEWBuildRoot "Please specify" )
    endif()
endmacro()



macro( GLEWFinder )
    # If the user has changed the GLEW install type combo box
    # (or it's a clean cache), then set or unset our related
    # GLEW directory search variables.
    if( NOT ( ${GLEWInstallType} STREQUAL ${_lastGLEWInstallType} ) )
    #    message( STATUS "NOT ( ${GLEWInstallType} STREQUAL ${_lastGLEWInstallType} )" )

        if( GLEWInstallType STREQUAL "Default Installation" )
            # Remove our helper variables and tell the stock script to search again.
            unset( GLEWInstallLocation CACHE )
            unset( GLEWSourceRoot CACHE )
            unset( GLEWBuildRoot CACHE )
        elseif( GLEWInstallType STREQUAL "Alternate Install Location" )
            # Enable just the GLEWInstallLocation helper variable.
            set( GLEWInstallLocation "Please specify" CACHE PATH "Root directory where GLEW is installed" )
            unset( GLEWSourceRoot CACHE )
            unset( GLEWBuildRoot CACHE )
        elseif( GLEWInstallType STREQUAL "Source And Build Tree" )
            # Enable the GLEWSourceRoot and GLEWBuildRoot helper variables.
            unset( GLEWInstallLocation CACHE )
            set( GLEWSourceRoot "Please specify" CACHE PATH "Root directory of GLEW source tree" )
            set( GLEWBuildRoot "Please specify" CACHE PATH "Root directory of GLEW build tree" )
        endif()
    endif()

    # Look for conditions that require us to find GLEW again.
    set( _needToFindGLEW FALSE )
    if( GLEWInstallType STREQUAL "Default Installation" )
        if( NOT ( ${GLEWInstallType} STREQUAL ${_lastGLEWInstallType} ) )
    #        message( STATUS "Need to find: case A" )
            set( _needToFindGLEW TRUE )
        endif()
    elseif( GLEWInstallType STREQUAL "Alternate Install Location" )
        if( NOT ( "${GLEWInstallLocation}" STREQUAL "${_lastGLEWInstallLocation}" ) )
    #        message( STATUS "Need to find: case B" )
            set( _needToFindGLEW TRUE )
        endif()
    elseif( GLEWInstallType STREQUAL "Source And Build Tree" )
        if( ( NOT ( "${GLEWSourceRoot}" STREQUAL "${_lastGLEWSourceRoot}" ) ) OR
            ( NOT ( "${GLEWBuildRoot}" STREQUAL "${_lastGLEWBuildRoot}" ) ) )
    #        message( STATUS "Need to find: cade C" )
            set( _needToFindGLEW TRUE )
        endif()
    endif()
    if( _needToFindGLEW )
        unFindGLEW()
        set( _lastGLEWInstallType ${GLEWInstallType} CACHE INTERNAL "" FORCE )
        set( _lastGLEWInstallLocation ${GLEWInstallLocation} CACHE INTERNAL "" FORCE )
        set( _lastGLEWSourceRoot ${GLEWSourceRoot} CACHE INTERNAL "" FORCE )
        set( _lastGLEWBuildRoot ${GLEWBuildRoot} CACHE INTERNAL "" FORCE )
    endif()



    # Save internal variables for later restoration
    set( CMAKE_PREFIX_PATH_SAVE ${CMAKE_PREFIX_PATH} )
    set( CMAKE_LIBRARY_PATH_SAVE ${CMAKE_LIBRARY_PATH} )

    set( CMAKE_PREFIX_PATH
        ${GLEWInstallLocation}
        ${GLEWSourceRoot}
        ${GLEWBuildRoot} )
    set( CMAKE_LIBRARY_PATH
        ${GLEWInstallLocation}
        ${GLEWBuildRoot} )


    find_package( GLEW REQUIRED )


    # Restore internal variables
    set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH_SAVE} )
    set( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH_SAVE} )
endmacro()
