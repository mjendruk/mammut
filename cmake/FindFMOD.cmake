
# FMOD_FOUND
# FMOD_INCLUDE_DIR
# FMOD_LIBRARY
# FMOD_BINARY

find_path(FMOD_INCLUDE_DIR fmod.hpp
    $ENV{FMOD_HOME}/include
    /usr/include/fmod
    /usr/local/include/fmod
    /sw/include/fmod
    /opt/local/include/fmod
    DOC "The directory where GL/glew.h resides")

find_library(FMOD_LIBRARY
    NAMES fmodex
    PATHS
    $ENV{FMOD_HOME}/lib
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    DOC "The FMOD library")

if(WIN32)

    find_file(FMOD_BINARY
        NAMES fmodex.dll
        PATHS
        $ENV{FMOD_HOME}/bin
        DOC "The FMOD binary")

endif()
    
if(FMOD_INCLUDE_DIR AND FMOD_LIBRARY)
    set(FMOD_FOUND 1 CACHE STRING "Set to 1 if FMOD is found, 0 otherwise")
else()
    set(FMOD_FOUND 0 CACHE STRING "Set to 1 if FMOD is found, 0 otherwise")
    message(WARNING "Note: an envvar like FMOD_HOME assists this script to locate glew.")
endif()

mark_as_advanced( FMOD_FOUND )
