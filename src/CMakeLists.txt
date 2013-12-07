if( JAG3D_USE_GL3W )
    add_subdirectory( gl3w )
endif()

add_subdirectory( jag )

add_subdirectory( jagMx )
add_subdirectory( jagSG )
add_subdirectory( jagUtil )


if( JAG3D_BUILD_PLUGINS )
    add_subdirectory( plugins )
endif()

if( JAG3D_BUILD_TESTS )
    add_subdirectory( tests )
endif()

if( JAG3D_BUILD_UNITTESTS )
    add_subdirectory( unittests )
endif()

if( JAG3D_BUILD_APPS )
    add_subdirectory( apps )
endif()

if( JAG3D_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()
