# Finds GMTL
# This script defines the following:
#  GMTL_FOUND (if false or zero, the GMTL headers were not found)
#  GMTL_INCLUDE_DIR
#
# The GMTL_DIR environment variable can be set to
# the parent directory of gmtl/AABox.h


# Based on FindFreetype.cmake
# Created by Eric Wing.
# Modifications by Alexander Neundorf.
# GMTL Modifications by Paul Martz.

FIND_PATH( GMTL_INCLUDE_DIR gmtl/AABox.h 
  HINTS
    $ENV{GMTL_DIR}
    ${GMTL_DIR}
  PATHS
    /usr/local/X11R6/include
    /usr/local/X11/include
    /usr/X11/include
    /sw/include
    /opt/local/include
    /usr/freeware/include
)


# handle the QUIETLY and REQUIRED arguments and set
# GMTL_FOUND to TRUE as appropriate
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( GMTL DEFAULT_MSG GMTL_INCLUDE_DIR )

