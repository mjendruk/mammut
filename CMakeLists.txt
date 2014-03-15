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
    ${source_path}/AbstractApplication.cpp
    ${source_path}/AbstractApplication.h
    ${source_path}/Game.cpp
    ${source_path}/Game.h
    ${source_path}/PerfCounter.cpp
    ${source_path}/PerfCounter.h
    ${source_path}/Util.cpp
    ${source_path}/Util.hpp
    ${source_path}/Util.h

    ${source_path}/logic/Mechanics.cpp
    ${source_path}/logic/Mechanics.h
    
    ${source_path}/3rdparty/tetgen/predicates.cxx
    ${source_path}/3rdparty/tetgen/tetgen.cxx
    ${source_path}/3rdparty/tetgen/tetgen.h

    ${source_path}/logic/highscore/HighscoreList.cpp
    ${source_path}/logic/highscore/HighscoreList.h
    ${source_path}/logic/highscore/HighscoreEntry.cpp
    ${source_path}/logic/highscore/HighscoreEntry.h

    ${source_path}/logic/menu/GameOverMenu.cpp
    ${source_path}/logic/menu/GameOverMenu.h
    ${source_path}/logic/menu/HighscoreMenu.cpp
    ${source_path}/logic/menu/HighscoreMenu.h
    ${source_path}/logic/menu/MainMenu.cpp
    ${source_path}/logic/menu/MainMenu.h
    ${source_path}/logic/menu/Menu.cpp
    ${source_path}/logic/menu/Menu.h
    ${source_path}/logic/menu/PauseMenu.cpp
    ${source_path}/logic/menu/PauseMenu.h
    ${source_path}/logic/menu/NewHighscoreMenu.cpp
    ${source_path}/logic/menu/NewHighscoreMenu.h
    
    ${source_path}/logic/menu/items/MenuItem.cpp
    ${source_path}/logic/menu/items/MenuItem.h
    ${source_path}/logic/menu/items/HighscoreListItem.cpp
    ${source_path}/logic/menu/items/HighscoreListItem.h
    ${source_path}/logic/menu/items/MenuButton.cpp
    ${source_path}/logic/menu/items/MenuButton.h    
    ${source_path}/logic/menu/items/MenuInput.cpp
    ${source_path}/logic/menu/items/MenuInput.h
    ${source_path}/logic/menu/items/MenuLogo.cpp
    ${source_path}/logic/menu/items/MenuLogo.h
    ${source_path}/logic/menu/items/MenuText.cpp
    ${source_path}/logic/menu/items/MenuText.h

    ${source_path}/logic/world/Cave.cpp
    ${source_path}/logic/world/Cave.h
    ${source_path}/logic/world/ChunkGenerator.cpp
    ${source_path}/logic/world/ChunkGenerator.h
    ${source_path}/logic/world/Cuboid.cpp
    ${source_path}/logic/world/Cuboid.h
    ${source_path}/logic/world/CuboidChunk.cpp
    ${source_path}/logic/world/CuboidChunk.h
    ${source_path}/logic/world/GameCamera.cpp
    ${source_path}/logic/world/GameCamera.h
    ${source_path}/logic/world/GameMechanics.cpp
    ${source_path}/logic/world/GameMechanics.h
    ${source_path}/logic/world/LevelStartChunkGenerator.cpp
    ${source_path}/logic/world/LevelStartChunkGenerator.h
    ${source_path}/logic/world/Mammut.cpp
    ${source_path}/logic/world/Mammut.h
    ${source_path}/logic/world/MammutPhysics.cpp
    ${source_path}/logic/world/MammutPhysics.h
    ${source_path}/logic/world/PhysicsObject.cpp
    ${source_path}/logic/world/PhysicsObject.h
    ${source_path}/logic/world/PhysicsWorld.cpp
    ${source_path}/logic/world/PhysicsWorld.h

    ${source_path}/logic/world/tets/TetGenerator.cpp
    ${source_path}/logic/world/tets/TetGenerator.h
    ${source_path}/logic/world/tets/Tet.cpp
    ${source_path}/logic/world/tets/Tet.h
    ${source_path}/logic/world/tets/BunchOfTets.cpp
    ${source_path}/logic/world/tets/BunchOfTets.h

    ${source_path}/rendering/Canvas.cpp
    ${source_path}/rendering/Canvas.h
    ${source_path}/rendering/RawFile.cpp
    ${source_path}/rendering/RawFile.h
    ${source_path}/rendering/Renderer.cpp
    ${source_path}/rendering/Renderer.h

    ${source_path}/rendering/menu/AbstractBackground.h
    ${source_path}/rendering/menu/BlankBackground.cpp
    ${source_path}/rendering/menu/BlankBackground.h
    ${source_path}/rendering/menu/MenuRenderer.cpp
    ${source_path}/rendering/menu/MenuRenderer.h
    ${source_path}/rendering/menu/ScreenshotBackground.cpp
    ${source_path}/rendering/menu/ScreenshotBackground.h

    ${source_path}/rendering/text/CharacterDrawable.cpp
    ${source_path}/rendering/text/CharacterDrawable.h
    ${source_path}/rendering/text/StringComposer.cpp
    ${source_path}/rendering/text/StringComposer.h
    ${source_path}/rendering/text/TextRenderer.cpp
    ${source_path}/rendering/text/TextRenderer.h
    
    ${source_path}/rendering/world/AbstractPainter.cpp
    ${source_path}/rendering/world/AbstractPainter.h
    ${source_path}/rendering/world/CaveDrawable.cpp
    ${source_path}/rendering/world/CaveDrawable.h
    ${source_path}/rendering/world/CavePainter.cpp
    ${source_path}/rendering/world/CavePainter.h
    ${source_path}/rendering/world/CuboidDrawable.cpp
    ${source_path}/rendering/world/CuboidDrawable.h
    ${source_path}/rendering/world/GameWorldRenderer.cpp
    ${source_path}/rendering/world/GameWorldRenderer.h
    ${source_path}/rendering/world/HUD.cpp
    ${source_path}/rendering/world/HUD.h
    ${source_path}/rendering/world/DrawableInterface.h
    ${source_path}/rendering/world/Painter.cpp
    ${source_path}/rendering/world/Painter.h
    ${source_path}/rendering/world/RenderCamera.cpp
    ${source_path}/rendering/world/RenderCamera.h

    ${source_path}/rendering/world/postprocessing/AbstractPostProcPass.h
    ${source_path}/rendering/world/postprocessing/MotionBlurPass.cpp
    ${source_path}/rendering/world/postprocessing/MotionBlurPass.h
    ${source_path}/rendering/world/postprocessing/SimplePostProcPass.cpp
    ${source_path}/rendering/world/postprocessing/SimplePostProcPass.hpp
    ${source_path}/rendering/world/postprocessing/SimplePostProcPass.h
    ${source_path}/rendering/world/postprocessing/SSAOPass.cpp
    ${source_path}/rendering/world/postprocessing/SSAOPass.h


    ${source_path}/sound/SoundManager.cpp
    ${source_path}/sound/SoundManager.h
    ${source_path}/sound/Sound.cpp
    ${source_path}/sound/Sound.h
)

# Group source files
set(MAMMUT_SOURCE_GROUP "Source Files")
source_group_by_path(${source_path} "\\\\.h$|\\\\.hpp$|\\\\.cpp$|\\\\.cxx$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${MAMMUT_SOURCE_GROUP} ${sources})

# wrap ui files and group wrap headers
list_extract(uis "\\\\.ui$" ${sources})
qt5_wrap_ui(uihs ${uis})

source_group_by_path(${CMAKE_CURRENT_BINARY_DIR} "\\\\.h$" 
    ${MAMMUT_SOURCE_GROUP} ${uihs})

set(shader_path "${CMAKE_CURRENT_SOURCE_DIR}/data/shaders")
set(shaders
    ${shader_path}/cuboid.vert
    ${shader_path}/cuboid.frag
    ${shader_path}/motionBlur.frag
    ${shader_path}/motionBlurNM.frag
    ${shader_path}/motionBlurTM_horizontal.frag
    ${shader_path}/motionBlurTM_vertical.frag
    ${shader_path}/screenshot_background.frag
    ${shader_path}/ssao.vert
    ${shader_path}/ssao.frag
    ${shader_path}/string_drawer.vert
    ${shader_path}/string_drawer.frag
    ${shader_path}/horizontal_blur.frag
    ${shader_path}/vertical_blur.frag
)

set(MAMMUT_SHADER_GROUP "Shader Files")
source_group_by_path(${shader_path} "\\\\.vert$|\\\\.frag$" 
    ${MAMMUT_SHADER_GROUP} ${shaders})

# Build library
add_executable(${target} ${sources} ${shaders} ${uihs})

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
