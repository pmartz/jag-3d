# Finds the gl.h header file.
# Looks for ES3/gl.h currently only for iOS sdk 7
# 
# This script defines the following:
#  GLES3_FOUND // Set to TRUE if gl.h is found
#  GLES3_INCLUDE_DIR // Parent directory of GL3 directory containing gl3.h.
#  GLES3_gl_LIBRARY //
#  EGL_INCLUDE_DIR // advanced
#  EGL_egl_LIBRARY // advanced
#
# GLES3_ROOT can be set as an environment variable or a CMake entry
# to the parent directory of the GL (or GL3 or OpenGL) directory containing gl.h.


FIND_PATH( GLES3_INCLUDE_DIR 
    NAMES ES3/gl.h
    HINTS ${GLES3_ROOT}
    PATHS ENV GLES3_ROOT
)

FIND_PATH( EGL_INCLUDE_DIR 
    NAMES EAGL.h
    HINTS ${GLES3_ROOT}
    PATHS ENV GLES3_ROOT
)
message( STATUS	"${EGL_INCLUDE_DIR}" )

set( GLES3_gl_LIBRARY "-framework OpenGLES" )

include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( GLES3 DEFAULT_MSG
    GLES3_INCLUDE_DIR
)
message( STATUS "${GLES3_INCLUDE_DIR}" )

mark_as_advanced(
    GLES3_INCLUDE_DIR
    EGL_INCLUDE_DIR
)
