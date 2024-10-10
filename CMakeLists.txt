cmake_minimum_required(VERSION 3.2)

project(playground LANGUAGES CXX)

option(check_memory "enable memory checking" OFF)

IF(MSVC)
    IF(NOT (MSVC_VERSION LESS 1900))
        # Multibyte character set has been deprecated since at least MSVC2015 (possibly earlier)
        ADD_DEFINITIONS( -DUNICODE -D_UNICODE )
    ENDIF()
ENDIF()

add_subdirectory(deps)
add_subdirectory(src)
