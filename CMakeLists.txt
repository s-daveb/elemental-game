cmake_minimum_required(VERSION 3.26)

# Additional paths to search for custom and third-party CMake modules
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

include(BuildProperties)

prevent_in_source_build()
disable_deprecated_features()

git_setup_submodules()

disable_tests_if_subproject()

project(Elemental
	VERSION 0.0.3
	LANGUAGES C CXX
	# Save this for later:
	# HOMEPAGE_URL <URL>
	DESCRIPTION "A simple top-down strategy game"
)

check_and_set_linker()

include(CPM)

SET(${PROJECT_NAME}_CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

option(CI_BUILD "Mark this as a CI/CD system build. Defines a C++ preprocessor macro CI_BUILD=1.")
if (CI_BUILD)
	add_compile_definitions(-DCI_BUILD=1)
endif()

# enable compile_commands.json generation for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

IF(NOT CMAKE_BUILD_TYPE)
	SET( CMAKE_BUILD_TYPE Debug )
ENDIF()

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# Disable GNU compiler extensions
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

if (BUILD_TESTING)
	CPMFindPackage(NAME Catch2
		GITHUB_REPOSITORY catchorg/Catch2
		VERSION 3.4.0
		OPTIONS
			"CATCH_DEVELOPMENT_BUILD OFF"
			"CATCH_BUILD_TESTING OFF"
	)
	CPMFindPackage(NAME FakeIt
		GITHUB_REPOSITORY eranpeer/FakeIt
		GIT_TAG 2.4.0
		OPTIONS
 		 		"BUILD_TESTINGING OFF"
	)

	if (TARGET Catch2)
		set_target_properties(Catch2 PROPERTIES
			CXX_STANDARD 20
		)
	endif()
	if (TARGET Catch2WithMain)
		set_target_properties(Catch2WithMain PROPERTIES
 			CXX_STANDARD 20
		)
	endif()

	if (Catch2_ADDED)
		list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
	else()
		if (Catch2_DIR)
			list(APPEND CMAKE_MODULE_PATH ${Catch2_DIR})
		endif()
	endif()
endif()


#CPMAddPackage(NAME IOCore
#	GIT_REPOSITORY "https://gitea.beniquez.me/sdaveb/IOCore.git"
#	GIT_TAG v0.2.21
#	OPTIONS
# 	 	"BUILD_SHARED_LIBS OFF"
#		"BUILD_TESTING OFF"
#)
#
#CPMAddPackage(NAME QJsonModel
#	GIT_REPOSITORY "https://gitea.beniquez.me/sdaveb/QJsonModel.git"
#	GIT_TAG v0.0.6
#	OPTIONS
# 		"USE_CCACHE ON"
#		"USE_MOLD ON"
#)

find_package(Qt6 COMPONENTS
	Core REQUIRED
	Widgets REQUIRED
	Gui REQUIRED
)
qt_standard_project_setup()

# Set output directories for build targets
set_artifact_dir(${CMAKE_BINARY_DIR}/out)

# Initialize FetchContent
include(FetchContent)
include(CheckIncludeFile)

# Initialize pkgconf
find_package(Threads)  # POSIX thread support
find_package(PkgConfig REQUIRED)

# Example: Find SDL2, SDL2_image, and SDL2_gfx using PkgConfig
# add IMPORTED_TARGET to enable fancy PkgConfig::SDL2 syntax
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_GFX REQUIRED IMPORTED_TARGET SDL2_gfx)

SET(SDL2_COMBINED_INCLUDE_DIRS "")
list(APPEND SDL2_COMBINED_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
list(APPEND SDL2_COMBINED_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIRS})
list(APPEND SDL2_COMBINED_INCLUDE_DIRS ${SDL2_GFX_INCLUDE_DIRS})
list(REMOVE_DUPLICATES SDL2_COMBINED_INCLUDE_DIRS)

include_directories(SYSTEM ${SDL2_COMBINED_INCLUDE_DIRS})

set(SDL2_COMBINED_LINK_DEPS
	PkgConfig::SDL2
	PkgConfig::SDL2_IMAGE
	PkgConfig::SDL2_GFX
)

pkg_check_modules(NLOHMANN_JSON "nlohmann_json >= 3.11.2" REQUIRED)
include_directories(SYSTEM ${NLOHMANN_JSON_INCLUDE_DIRS})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(-DDEBUG=1)
endif()

# Add subdirectories
add_subdirectory(Modules)
include_directories(BEFORE ${Elemental_CMAKE_SOURCE_DIR}/Modules)

if(BUILD_TESTING)
	add_subdirectory(Tests)
endif()

add_subdirectory(Apps)

# vim: ts=4 sw=4 noet foldmethod=indent :
