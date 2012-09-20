add_subdirectory( jagBase )
add_subdirectory( jagDisk )
add_subdirectory( jagDraw )


if( JAG3D_BUILD_PLUGINS )
    add_subdirectory( plugins )
endif()

if( JAG3D_BUILD_TESTS )
    enable_testing()
    add_definitions( -DJAG3D_BUILD_TESTS )
    add_subdirectory( tests )
endif()

if( JAG3D_BUILD_APPS )
    add_subdirectory( apps )
endif()

if( JAG3D_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()
