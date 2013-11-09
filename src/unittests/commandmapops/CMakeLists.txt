set( _exeName commandmapops )

_addNonWindowedExecutable( UnitTest ${_exeName}
    ${_exeName}.cpp
)

add_test(
    NAME ${_exeName}
    COMMAND ${_exeName}
)
