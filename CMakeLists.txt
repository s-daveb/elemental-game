cmake_minimum_required(VERSION 3.26)

# Additional paths to search for custom and third-party CMake modules
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

include(DefaultConfig)


include(BuildOptions)
include(BuildProperties)

project(Elemental
    VERSION 0.0.6
    LANGUAGES C CXX
    # Save this for later:
    # HOMEPAGE_URL <URL>
    DESCRIPTION "A simple top-down strategy game"
)

include(CPM)

set(${PROJECT_NAME}_CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

OPTION(CI_BUILD
    "Mark this as a CI/CD system build."
)

if(CI_BUILD)
    ADD_COMPILE_DEFINITIONS(-DCI_BUILD=1)
endif()

# enable compile_commands.json generation for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Debug )
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# Disable GNU compiler extensions
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

CPMFindPackage(NAME span-lite
    GITHUB_REPOSITORY martinmoene/span-lite
    GIT_TAG v0.11.0
    OPTIONS
        "SPAN_LITE_OPT_BUILD_TESTS OFF"
        "SPAN_LITE_OPT_BUILD_EXAMPLES OFF"
)

FIND_PACKAGE(Fontconfig REQUIRED)

if(BUILD_TESTING)

    CPMFindPackage(NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        VERSION 3.6.0
        OPTIONS "CATCH_DEVELOPMENT_BUILD OFF" "CATCH_BUILD_TESTING OFF"
    )

    CPMFindPackage(NAME FakeIt
        GITHUB_REPOSITORY eranpeer/FakeIt
        GIT_TAG 2.4.1
        OPTIONS
        "BUILD_TESTING OFF"
    )

    if(TARGET Catch2)
        set_target_properties(Catch2 PROPERTIES
            CXX_STANDARD 20
        )
    endif()
    if(TARGET Catch2WithMain)
        set_target_properties(Catch2WithMain PROPERTIES
            CXX_STANDARD 20
        )
    endif()

    if(Catch2_ADDED)
        list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    else()
        if(Catch2_DIR)
            list(APPEND CMAKE_MODULE_PATH ${Catch2_DIR})
        endif()
    endif()
endif()

CPMFindPackage(
    NAME fmt
    URL https://github.com/fmtlib/fmt/archive/refs/tags/10.2.1.zip
    URL_HASH SHA256=d368f9c39a33a3aef800f5be372ec1df1c12ad57ada1f60adc62f24c0e348469
)
FIND_PACKAGE(Qt6 COMPONENTS
    Core REQUIRED
    Widgets REQUIRED
    Gui REQUIRED
)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)

qt_standard_project_setup()

# Set output directories for build targets
set_artifact_dir(${CMAKE_BINARY_DIR}/out)

# Initialize FetchContent
INCLUDE(FetchContent)
INCLUDE(CheckIncludeFile)

# Initialize pkgconf
FIND_PACKAGE(Threads)  # POSIX thread support
FIND_PACKAGE(PkgConfig REQUIRED)

# TODO: Switch this to CPM Package manager

# Example: Find SDL2, SDL2_image, and SDL2_gfx using PkgConfig
# add IMPORTED_TARGET to enable fancy PkgConfig::SDL2 syntax
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_GFX REQUIRED IMPORTED_TARGET SDL2_gfx)
pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)

set(SDL2_COMBINED_INCLUDE_DIRS "")
list(APPEND SDL2_COMBINED_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
list(APPEND SDL2_COMBINED_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIRS})
list(APPEND SDL2_COMBINED_INCLUDE_DIRS ${SDL2_GFX_INCLUDE_DIRS})
list(APPEND SDL2_COMBINED_INCLUDE_DIRS ${SDL2_TTF_INCLUDE_DIRS})
list(REMOVE_DUPLICATES SDL2_COMBINED_INCLUDE_DIRS)

include_directories(SYSTEM ${SDL2_COMBINED_INCLUDE_DIRS})

set(SDL2_COMBINED_LINK_DEPS
    PkgConfig::SDL2
    PkgConfig::SDL2_IMAGE
    PkgConfig::SDL2_GFX
    PkgConfig::SDL2_TTF
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_COMPILE_DEFINITIONS(-DDEBUG=1)
endif()

# Add subdirectories
add_subdirectory(Modules)
include_directories(BEFORE ${Elemental_CMAKE_SOURCE_DIR}/Modules)

if(BUILD_TESTING)
    add_subdirectory(Tests)
endif()

add_subdirectory(Apps)

# vim: ts=4 sw=4 et foldmethod=indent :
