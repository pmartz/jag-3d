# Finds the OpenGL 3 header file.
# Looks for gl3.h or glcorearb.h
# 
# This script defines the following:
#  GL3_FOUND // Set to TRUE if gl3.h or glcorearb.h is found
#  GL3_INCLUDE_DIR // Parent directory of directory (gl, GL3, or OpenGL) containing the OpenGL 3 header.
#  GL3_GLCOREARB_HEADER // advanced
#  GL3_GL3_HEADER // advanced
#
# GL3_ROOT can be set as an environment variable or a CMake variable,
# to the parent directory of the gl, GL3, or OpenGL directory containing the OpenGL header.
#
# Note: glcorearb.h replaced gl3.h in mid-2012.
#  - If this script finds both glcorearb.h and gl3.h, it will display
#    a warning, but set GL3_FOUND to TRUE and set GL3_INCLUDE_DIR to
#    the parent of the directory where glcorearb.h was found.
#  - If it finds either gl3.h or glcorearb.h but not both, no warning
#    will be displayed (for backwards compatiblity).


FIND_PATH( GL3_GL3_HEADER 
    NAMES GL/gl3.h GL3/gl3.h OpenGL/gl3.h
    HINTS ${GL3_ROOT}
    PATHS ENV GL3_ROOT
)
FIND_PATH( GL3_GLCOREARB_HEADER
    NAMES GL/glcorearb.h GL3/glcorearb.h OpenGL/glcorearb.h gl/glcorearb.h
    HINTS ${GL3_ROOT}
    PATHS ENV GL3_ROOT
)

if( GL3_GLCOREARB_HEADER )
    if( GL3_GL3_HEADER )
        message( WARNING "Found both glcorearb.h and gl3.h. Using glcorearb.h." )
    endif()
    set( GL3_INCLUDE_DIR ${GL3_GLCOREARB_HEADER} )
elseif( GL3_GL3_HEADER )
    set( GL3_INCLUDE_DIR ${GL3_GL3_HEADER} )
else()
    set( GL3_INCLUDE_DIR )
endif()


# handle the QUIETLY and REQUIRED arguments and set
# GL3_FOUND to TRUE as appropriate
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( GL3
    "Set GL3_ROOT as the parent of the directory containing the OpenGL 3 header."
    GL3_INCLUDE_DIR )

MARK_AS_ADVANCED(
    GL3_INCLUDE_DIR
    GL3_GLCOREARB_HEADER
    GL3_GL3_HEADER
)
