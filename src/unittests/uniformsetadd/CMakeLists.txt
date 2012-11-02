set( _exeName uniformsetadd )

_addNonWindowedExecutable( UnitTest ${_exeName}
    uniformsetadd.cpp
)

add_test(
    NAME ${_exeName}
    COMMAND ${_exeName}
)
