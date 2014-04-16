if( ASSIMP_FOUND AND JAG3D_USE_ASSIMP )
    add_subdirectory( assimpModel )
endif()

if( OSG_FOUND AND
        ( NOT ( JAG3D_USE_OSG STREQUAL "none" ) ) )
    add_subdirectory( osgImage )
    add_subdirectory( osgModel )
endif()

add_subdirectory( shader )
add_subdirectory( text )
