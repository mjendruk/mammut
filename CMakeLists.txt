cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)


# PROJECT DESCRIPTION

set(META_PROJECT_NAME        "mammut")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "1")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_AUTHOR_ORGANIZATION "mammut-group")
set(META_AUTHOR_DOMAIN       "https://github.com/mjendruk/mammut")


# OPTIONS

option(OPTION_LIMIT_CONFIGS  "Generate limited configs (Release; Debug)" ON)
option(OPTION_LOCAL_INSTALL  "Install to a local directory instead of the system" OFF)

# CMAKE CONFIGURATION

set(MAMMUT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Include cmake modules from ./cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set configuration types
if(OPTION_LIMIT_CONFIGS)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)
endif()

# Project
project(${META_PROJECT_NAME} C CXX)

# Generate folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Include custom cmake functions
include(cmake/Custom.cmake)


# PLATFORM AND ARCHITECTURE

# Architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(X64 ON)
endif()

# Check for linux
if(UNIX AND NOT APPLE)
    set(LINUX 1)
endif()

# Setup platform specifics (compile flags, etc., ...)
if(MSVC)
    message(STATUS "Configuring for platform Windows/MSVC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsMSVC.cmake)
elseif(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Windows/GCC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsGCC.cmake)
elseif(LINUX AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Linux/GCC.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformLinuxGCC.cmake)
elseif(APPLE)
    message(STATUS "Configuring for platform MacOS.")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformMacOS.cmake)
else()
    # Unsupported system/compiler
    message(WARNING "Unsupported platform/compiler combination")
endif()


# Install locations

set(project ${META_PROJECT_NAME})
if(WIN32)
    set(INSTALL_BIN           ".")                      # C:\Programme\<project>
    set(INSTALL_SHARED        ".")                      # C:\Programme\<project>
    set(INSTALL_LIB           "lib")                    # C:\Programme\<project>\lib
    set(INSTALL_INCLUDE       "include")                # C:\Programme\<project>\include
    set(INSTALL_DOC           "doc")                    # C:\Programme\<project>\doc
    set(INSTALL_APPLICATIONS  "")                       # Not available under Windows
    set(INSTALL_ICONS         "")                       # Not available under Windows
    set(INSTALL_INIT          "")                       # Not available under Windows
    set(INSTALL_DATA          ".")                      # C:\Programme\<project>
    set(INSTALL_EXAMPLES      ".")                      # C:\Programme\<project>
elseif(LINUX OR APPLE)
    set(INSTALL_BIN           "bin")                    # /usr/[local]/bin
    set(INSTALL_SHARED        "lib")                    # /usr/[local]/lib
    set(INSTALL_LIB           "lib")                    # /usr/[local]/lib
    set(INSTALL_INCLUDE       "include")                # /usr/[local]/include
    set(INSTALL_DOC           "share/doc/${project}")   # /usr/[local]/share/doc/<project>
    set(INSTALL_APPLICATIONS  "share/applications")     # /usr/[local]/share/applications
    set(INSTALL_ICONS         "share/pixmaps")          # /usr/[local]/share/pixmaps
    set(INSTALL_INIT          "/etc/init")              # /etc/init (upstart init scripts)
    set(INSTALL_DATA          "share/${project}")       # /usr/[local]/share/<project>
    set(INSTALL_EXAMPLES      "share/${project}")       # /usr/[local]/share/<project>

    # Adjust target paths for Local installs
    if(OPTION_LOCAL_INSTALL)
        # Put binaries in root directory and keep data directory name
        set(INSTALL_BIN                       ".")           # /<INSTALL_PREFIX>
        set(INSTALL_DATA                      ".")           # /<INSTALL_PREFIX>

        # We have to change the RPATH of binaries to achieve a usable local install.
        # [TODO] For binaries, "$ORIGIN/lib" is right, so that libraries are found in ./lib.
        #        However, I have not yet tested what happens when libraries use other libraries.
        #        In that case, they might need the rpath $ORIGIN instead ...
        set(CMAKE_SKIP_BUILD_RPATH            FALSE)         # Use automatic rpath for build
        set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE)         # Use specific rpath for INSTALL
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)         # NO automatic rpath for INSTALL
        set(CMAKE_INSTALL_RPATH               "$ORIGIN/lib") # Libraries are relative to binary
    endif()
endif()



# GET REPOSITORY VERSION

# Add a revision file containing the git-head tag for cpack and install
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

# Generate a shorter, googlelike variation for rev
string(SUBSTRING ${GIT_SHA1} 0 12 GIT_REV)
file(WRITE ${CMAKE_BINARY_DIR}/revision ${GIT_REV})

# [TODO] Really?? Do we want this deployed?
install(FILES ${CMAKE_BINARY_DIR}/revision DESTINATION .)

# [TODO] Deal with GIT-NOTFOUND

# Target
set(target mammut)

# Find externals
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)

find_package(GLOW REQUIRED)

set(BULLET_ROOT $ENV{BULLET_ROOT})

find_package(BULLET REQUIRED)
find_package(FMOD REQUIRED)

set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)

# Probably works in the next cmake release -> http://www.cmake.org/Bug/view.php?id=13788
# What we do not want is automocs beside the project -> http://www.cmake.org/Bug/view.php?id=13688

set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)

# try to find either qt4 or qt5, favoring qt5
# good resource: http://www.kdab.com/using-cmake-with-qt-5/

# http://qt-project.org/forums/viewthread/30006/

if (WIN32)
    cmake_policy(SET CMP0020 NEW)
endif()

find_package(Qt5Core    REQUIRED)
find_package(Qt5Gui     REQUIRED)
find_package(Qt5Widgets REQUIRED) # this is important in order to get uic working 
find_package(Qt5OpenGL  REQUIRED)

set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

# Include directories
include_directories(
    ${CMAKE_BINARY_DIR} # for Meta.h
    
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    
    ${GLOW_INCLUDE_DIR}

    ${BULLET_INCLUDE_DIRS}
    ${FMOD_INCLUDE_DIR}

    ${source_path}
)



# Generate meta-header
configure_file(${source_path}/mammut-version.h.in mammut-version.h)

set(sources
    ${source_path}/main.cpp

    ${source_path}/game_mechanics/ChunkGenerator.cpp
    ${source_path}/game_mechanics/ChunkGenerator.h
    ${source_path}/game_mechanics/Cuboid.cpp
    ${source_path}/game_mechanics/Cuboid.h
    ${source_path}/game_mechanics/CuboidChunk.cpp
    ${source_path}/game_mechanics/CuboidChunk.h
    ${source_path}/game_mechanics/GameCamera.cpp
    ${source_path}/game_mechanics/GameCamera.h
    ${source_path}/game_mechanics/GameMechanics.cpp
    ${source_path}/game_mechanics/GameMechanics.h
    ${source_path}/game_mechanics/GameObject.cpp
    ${source_path}/game_mechanics/GameObject.h
    ${source_path}/game_mechanics/Gravity.cpp
    ${source_path}/game_mechanics/Gravity.h
    ${source_path}/game_mechanics/Mammut.cpp
    ${source_path}/game_mechanics/Mammut.h
    ${source_path}/game_mechanics/MammutMotionState.cpp
    ${source_path}/game_mechanics/MammutMotionState.h

    ${source_path}/game_world_rendering/AbstractPainter.cpp
    ${source_path}/game_world_rendering/AbstractPainter.h
    ${source_path}/game_world_rendering/Canvas.cpp
    ${source_path}/game_world_rendering/Canvas.h
    ${source_path}/game_world_rendering/CaveDrawable.cpp
    ${source_path}/game_world_rendering/CaveDrawable.h
    ${source_path}/game_world_rendering/CavePainter.cpp
    ${source_path}/game_world_rendering/CavePainter.h
    ${source_path}/game_world_rendering/CharacterDrawable.cpp
    ${source_path}/game_world_rendering/CharacterDrawable.h
    ${source_path}/game_world_rendering/CuboidDrawable.cpp
    ${source_path}/game_world_rendering/CuboidDrawable.h
    ${source_path}/game_world_rendering/FileAssociatedAsset.cpp
    ${source_path}/game_world_rendering/FileAssociatedAsset.h
    ${source_path}/game_world_rendering/FileAssociatedShader.cpp
    ${source_path}/game_world_rendering/FileAssociatedShader.h
    ${source_path}/game_world_rendering/HUD.cpp
    ${source_path}/game_world_rendering/HUD.h
    ${source_path}/game_world_rendering/IDrawable.h
    ${source_path}/game_world_rendering/Painter.cpp
    ${source_path}/game_world_rendering/Painter.h
    ${source_path}/game_world_rendering/RawFile.cpp
    ${source_path}/game_world_rendering/RawFile.h
    ${source_path}/game_world_rendering/RenderCamera.cpp
    ${source_path}/game_world_rendering/RenderCamera.h
    ${source_path}/game_world_rendering/GameWorldRenderer.cpp
    ${source_path}/game_world_rendering/GameWorldRenderer.h
    ${source_path}/game_world_rendering/SSAO.cpp
    ${source_path}/game_world_rendering/SSAO.h
    ${source_path}/game_world_rendering/StringComposer.cpp
    ${source_path}/game_world_rendering/StringComposer.h
    ${source_path}/game_world_rendering/StringDrawer.cpp
    ${source_path}/game_world_rendering/StringDrawer.h
    
    ${source_path}/menu/Menu.cpp
    ${source_path}/menu/Menu.h
    ${source_path}/menu/MenuButton.cpp
    ${source_path}/menu/MenuButton.h
    ${source_path}/menu/MenuItem.cpp
    ${source_path}/menu/MenuItem.h
    ${source_path}/menu/MenuMechanics.cpp
    ${source_path}/menu/MenuMechanics.h
    ${source_path}/menu/MenuRenderer.cpp
    ${source_path}/menu/MenuRenderer.h
    ${source_path}/menu/StartMenu.cpp
    ${source_path}/menu/StartMenu.h

    ${source_path}/AbstractApplication.cpp
    ${source_path}/AbstractApplication.h
    ${source_path}/Conversions.cpp
    ${source_path}/Conversions.h
    ${source_path}/Game.cpp
    ${source_path}/Game.h
    ${source_path}/Mechanics.cpp
    ${source_path}/Mechanics.h
    ${source_path}/GlowExtensions.cpp
    ${source_path}/GlowExtensions.h
    ${source_path}/MessageHandler.cpp
    ${source_path}/MessageHandler.h
    ${source_path}/Renderer.cpp
    ${source_path}/Renderer.h
)

# Group source files
set(MAMMUT_SOURCE_GROUP "Source Files")
source_group_by_path(${source_path} "\\\\.h$|\\\\.hpp$|\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${MAMMUT_SOURCE_GROUP} ${sources})

# wrap ui files and group wrap headers
list_extract(uis "\\\\.ui$" ${sources})
qt5_wrap_ui(uihs ${uis})

source_group_by_path(${CMAKE_CURRENT_BINARY_DIR} "\\\\.h$" 
    ${MAMMUT_SOURCE_GROUP} ${uihs})


# Build library
add_executable(${target} ${sources} ${uihs})

qt5_use_modules(${target} OpenGL Gui Widgets)

target_link_libraries(${target} 
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}

    ${GLOW_LIBRARIES}
    ${GLOW_UTILS_LIBRARIES}

    ${BULLET_LIBRARIES}
    ${FMOD_LIBRARY}
)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")

add_definitions("-DGLM_SWIZZLE")
add_definitions("-D_USE_MATH_DEFINES")

# Deployment
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${INSTALL_DATA})

# Packaging
# add_subdirectory(packages)
