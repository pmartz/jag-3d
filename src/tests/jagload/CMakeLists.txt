include( FindOSGHelper )

include_directories( 
    ${OSG_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include/jagDraw
)

_addExecutable( Test jagload
    jagload.cpp
    osg2jag.cpp
    osg2jag.h
    ADDITIONAL_LIBRARIES
    ${OSG_LIBRARIES}
)
