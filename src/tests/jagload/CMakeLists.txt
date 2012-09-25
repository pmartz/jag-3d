if( NOT OSG_FOUND )
    return()
endif()

_addExecutable( Test jagload
    jagload.cpp
    osg2jag.cpp
    osg2jag.h
    ADDITIONAL_INCLUDES
        ${OSG_INCLUDE_DIRS}
    ADDITIONAL_LIBRARIES
        ${OSG_LIBRARIES}
)
