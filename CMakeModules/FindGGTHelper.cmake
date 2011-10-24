# Copyright

# Macro to force the FindGgt.cmake script to
# search again for Ggt.
macro( unFindGgt )
#    message( STATUS "In unFindGgt" )

    unset( "GGT_INCLUDE_DIR" CACHE )
    unset( "GGT_FOUND" CACHE )
endmacro( unFindGgt )


macro( GgtMenuSetup )
    # What type of Ggt should we look for? This is a combo box
    # supporting three types of Ggt installations:
    #  * Default installation (in which the stock FindGgt.cmake script does all the work)
    #  * Alternate Install Location (user must set the GgtInstallLocation variable)
    #  * Source And Build Tree (user must supply both the GgtSourceRoot and GgtBuildRoot variables)
    set( GgtInstallType "Default Installation" CACHE STRING "Type of Ggt install: 'Default Installation', 'Alternate Install Location', or 'Source And Build Tree'." )
    set_property( CACHE GgtInstallType PROPERTY STRINGS "Default Installation" "Alternate Install Location" "Source And Build Tree" )

    # We need to detect when the user changes the Ggt install type
    # or any of the related directory variables, so that we'll know
    # to call unFindGgt() and force the stock Ggt script to search
    # again. To do this, we save the last set value of these variables
    # in the CMake cache as internal (hidden) variables.
    if( NOT DEFINED _lastGgtInstallType )
        set( _lastGgtInstallType "empty" CACHE INTERNAL "" )
    endif()
    if( NOT DEFINED _lastGgtInstallLocation )
        set( _lastGgtInstallLocation "empty" CACHE INTERNAL "" )
    endif()
    if( NOT DEFINED _lastGgtSourceRoot )
        set( _lastGgtSourceRoot "empty" CACHE INTERNAL "" )
    endif()
    if( NOT DEFINED _lastGgtBuildRoot )
        set( _lastGgtBuildRoot "empty" CACHE INTERNAL "" )
    endif()

    if( NOT DEFINED GgtInstallLocation )
        set( GgtInstallLocation "Please specify" )
    endif()
    if( NOT DEFINED GgtSourceRoot )
        set( GgtSourceRoot "Please specify" )
    endif()
    if( NOT DEFINED GgtBuildRoot )
        set( GgtBuildRoot "Please specify" )
    endif()
endmacro()



macro( GgtFinder )
    # If the user has changed the Ggt install type combo box
    # (or it's a clean cache), then set or unset our related
    # Ggt directory search variables.
    if( NOT ( ${GgtInstallType} STREQUAL ${_lastGgtInstallType} ) )
    #    message( STATUS "NOT ( ${GgtInstallType} STREQUAL ${_lastGgtInstallType} )" )

        if( GgtInstallType STREQUAL "Default Installation" )
            # Remove our helper variables and tell the stock script to search again.
            unset( GgtInstallLocation CACHE )
            unset( GgtSourceRoot CACHE )
            unset( GgtBuildRoot CACHE )
        elseif( GgtInstallType STREQUAL "Alternate Install Location" )
            # Enable just the GgtInstallLocation helper variable.
            set( GgtInstallLocation "Please specify" CACHE PATH "Root directory where Ggt is installed" )
            unset( GgtSourceRoot CACHE )
            unset( GgtBuildRoot CACHE )
        elseif( GgtInstallType STREQUAL "Source And Build Tree" )
            # Enable the GgtSourceRoot and GgtBuildRoot helper variables.
            unset( GgtInstallLocation CACHE )
            set( GgtSourceRoot "Please specify" CACHE PATH "Root directory of Ggt source tree" )
            set( GgtBuildRoot "Please specify" CACHE PATH "Root directory of Ggt build tree" )
        endif()
    endif()

    # Look for conditions that require us to find Ggt again.
    set( _needToFindGgt FALSE )
    if( GgtInstallType STREQUAL "Default Installation" )
        if( NOT ( ${GgtInstallType} STREQUAL ${_lastGgtInstallType} ) )
    #        message( STATUS "Need to find: case A" )
            set( _needToFindGgt TRUE )
        endif()
    elseif( GgtInstallType STREQUAL "Alternate Install Location" )
        if( NOT ( "${GgtInstallLocation}" STREQUAL "${_lastGgtInstallLocation}" ) )
    #        message( STATUS "Need to find: case B" )
            set( _needToFindGgt TRUE )
        endif()
    elseif( GgtInstallType STREQUAL "Source And Build Tree" )
        if( ( NOT ( "${GgtSourceRoot}" STREQUAL "${_lastGgtSourceRoot}" ) ) OR
            ( NOT ( "${GgtBuildRoot}" STREQUAL "${_lastGgtBuildRoot}" ) ) )
    #        message( STATUS "Need to find: cade C" )
            set( _needToFindGgt TRUE )
        endif()
    endif()
    if( _needToFindGgt )
        unFindGgt()
        set( _lastGgtInstallType ${GgtInstallType} CACHE INTERNAL "" FORCE )
        set( _lastGgtInstallLocation ${GgtInstallLocation} CACHE INTERNAL "" FORCE )
        set( _lastGgtSourceRoot ${GgtSourceRoot} CACHE INTERNAL "" FORCE )
        set( _lastGgtBuildRoot ${GgtBuildRoot} CACHE INTERNAL "" FORCE )
    endif()



    # Save internal variables for later restoration
    set( CMAKE_PREFIX_PATH_SAVE ${CMAKE_PREFIX_PATH} )
    set( CMAKE_LIBRARY_PATH_SAVE ${CMAKE_LIBRARY_PATH} )

    set( CMAKE_PREFIX_PATH
        ${GgtInstallLocation}
        ${GgtSourceRoot}
        ${GgtBuildRoot} )
    set( CMAKE_LIBRARY_PATH
        ${GgtInstallLocation}
        ${GgtBuildRoot} )


    find_package( GGT )


    # Restore internal variables
    set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH_SAVE} )
    set( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH_SAVE} )
endmacro()
