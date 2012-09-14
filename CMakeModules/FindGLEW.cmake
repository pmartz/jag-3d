# Copyright

# Sets these:
#  GLEW_INCLUDE_DIR
#  GLEW_LIBRARIES
#  GLEW_FOUND, If false, do not try to use GLEW.
# Also defined, but not for general use are:
#  GLEW_glew_LIBRARY = the full path to the GLEW library.

find_path( GLEW_INCLUDE_DIR
    NAMES GL/glew.h
    PATHS
        ${GLEW_ROOT_PATH}/include
        /usr/include/GL
        /usr/openwin/share/include
        /usr/openwin/include
        /opt/graphics/OpenGL/include
        /opt/graphics/OpenGL/contrib/libglew
)
find_library( GLEW_glew_LIBRARY
    NAMES glew32mx glew32mxs
    PATHS
        ${OPENGL_LIBRARY_DIR}
        /usr/openwin/lib
        ${GLEW_ROOT_PATH}/Release
)

set( GLEW_FOUND "NO" )
if( GLEW_INCLUDE_DIR )
    if( GLEW_glew_LIBRARY )
        set( GLEW_LIBRARIES
          ${GLEW_glew_LIBRARY}
        )
        set( GLEW_FOUND "YES" )
    endif()
endif()

mark_as_advanced(
    GLEW_INCLUDE_DIR
    GLEW_glew_LIBRARY
)
