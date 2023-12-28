cmake_minimum_required(VERSION 3.26)

# Prevent in-source builds
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same.")
endif()

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

# Check if this CMakeFile is being built as part of a FetchContent CMake
# dependency download.
if (NOT DEFINED PROJECT_NAME)
	# When this package is included as a subproject, save some time by skipping
 	# the unit test build and execution.
 	# To override this, set -DENABLE_TESTS=true when you call CMake
	option(ENABLE_TESTS "Build and run unit tests" ON)
else()
	option(ENABLE_TESTS "Build and run unit tests" OFF)
endif()
option(CI_BUILD "Defines a C preprocessor macro used to disable some code paths in CI servers" OFF)

project(Elemental
	VERSION 0.0.1
	LANGUAGES C CXX
	# Save this for later:
	# HOMEPAGE_URL <URL>
	DESCRIPTION "A simple top-down strategy game"
)

# Custom CMake Defines / Command-line options
option(USE_SYSTEM_CATCH2
	"Do not download & compile catch2 library for unit tests"
	ON
)

if (CI_BUILD)
	add_compile_definitions(-DCI_BUILD=1)
endif()

if( NOT CMAKE_BUILD_TYPE )
  SET( CMAKE_BUILD_TYPE Debug )
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

# Disable non-portable GNU compiler extensions
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# Check if ccache is installed and enable CMake integration
# if found. This will speed up repeated builds.
find_program(CCACHE_PATH ccache)
if(ccache_path)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PATH})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PATH})
endif(ccache_path)

if (USE_SYSTEM_CATCH2)
	message(CHECK_START "Detecting System Catch2 ")
	find_package(Catch2 3 QUIET)

	if(TARGET Catch2::Catch2)
		message(CHECK_PASS "found target Catch2::Catch2")
	else()
		message(CHECK_FAIL "not found")
		set(USE_SYSTEM_CATCH2 OFF)
		message(STATUS "USE_SYSTEM_CATCH2=OFF")
	endif()
endif()

if (UNIX AND NOT(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux"))
	option(USE_BOOST_STACKTRACE "Use Boost::stacktrace for stack traces" OFF)
	option(USE_EXECINFO_STACKTRACE "Use BSD/UNIX execinfo for stack traces" ON)
else()
	option(USE_BOOST_STACKTRACE "Use Boost::stacktrace for stack traces" ON)
	option(USE_EXECINFO_STACKTRACE "Use BSD/UNIX execinfo for stack traces" OFF)
endif()


# enable compile_commands.json generation for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

# Initialize FetchContent
include(FetchContent)
include(CheckIncludeFile)

# Initialize pkgconf
find_package(Threads)
find_package(PkgConfig REQUIRED)

# Example: Find SDL2, SDL2_image, and SDL2_gfx using PkgConfig
# add IMPORTED_TARGET to enable fancy PkgConfig::SDL2 syntax
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_GFX REQUIRED IMPORTED_TARGET SDL2_gfx)
pkg_check_modules(NLOHMANN_JSON "nlohmann_json >= 3.11.2" REQUIRED)

if (SDL2_FOUND)
	include_directories(${SDL2_INCLUDE_DIRS})
	link_directories(AFTER ${SDL2_IMG_LIBRARY_DIRS})
endif()
if (SDL2_IMG_FOUND)
	include_directories(${SDL2_IMG_INCLUDE_DIRS})
	link_directories(AFTER ${SDL2_LIBRARY_DIRS})
endif()
if (SDL2_GFX_FOUND)
	include_directories(${SDL2_GFX_INCLUDE_DIRS})
	link_directories(AFTER ${SDL2_GFX_LIBRARY_DIRS})
endif()
if (NLOHMANN_JSON_FOUND)
	include_directories(${NLOHMANN_JSON_INCLUDE_DIRS})
	#add_link_options(${NLOHMANN_JSON_LDFLAGS})
endif()

set(SDL2_LINK_DEPS
	PkgConfig::SDL2
	PkgConfig::SDL2_IMAGE
	PkgConfig::SDL2_GFX
)

if (USE_EXECINFO_STACKTRACE AND (NOT USE_BOOST_STACKTRACE))
	CHECK_INCLUDE_FILE("execinfo.h" HAVE_EXECINFO_H)

	if (HAVE_EXECINFO_H)
		add_definitions(-DHAVE_EXECINFO_H=1)
	endif()

	find_library(LIB_EXEC_INFO
		NAMES execinfo  # Specify the library name without the 'lib' prefix or file extension
		HINTS /usr/lib /usr/local/lib  # Optional hint for the library location
	)
	if (LIB_EXEC_INFO)
		message(STATUS "Found libexecinfo: ${LIB_EXEC_INFO}")
		set(STACKTRACE_DEP_LIBS ${LIB_EXEC_INFO})
	endif()
elseif((NOT USE_EXECINFO_STACKTRACE) AND USE_BOOST_STACKTRACE)
	set(Boost_USE_STATIC_LIBS OFF)
	set(Boost_USE_STATIC_RUNTIME OFF)  # Do not require static C++ runtime
	set(Boost_USE_MULTITHREADED ON)
	find_package(Boost 1.82.0 COMPONENTS system filesystem REQUIRED)


	if (Boost_FOUND)
		add_definitions(-DBOOST_STACKTRACER=1)
		add_definitions(-DBOOST_STACKTRACE_USE_ADDR2LINE=1)

		include_directories(${Boost_INCLUDE_DIRS})
	endif()
endif()

if (ENABLE_TESTS)
	if (NOT USE_SYSTEM_CATCH2)
		FetchContent_Declare(
    	  Catch2
	    	URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.4.0.zip
	    	URL_HASH MD5=c426e77d4ee0055410bc930182959ae5
		)
		FetchContent_MakeAvailable(Catch2)
	endif()

	FetchContent_Declare(
		FakeIt
		URL https://github.com/eranpeer/FakeIt/archive/refs/tags/2.4.0.zip
		URL_HASH MD5=72e4ce7f1c0de97074d2d5b517753286
		FIND_PACKAGE_ARGS
	)
	FetchContent_MakeAvailable(FakeIt)

	if (TARGET Catch2)
		set_target_properties(Catch2 PROPERTIES
			CXX_STANDARD 17
		)
	endif()

	if (TARGET Catch2WithMain)
		set_target_properties(Catch2WithMain PROPERTIES
 		       	CXX_STANDARD 17
		)
	endif()
	list(APPEND cmake_module_path ${Catch2_source_dir}/extras)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(-DDEBUG=1)
endif()

# Set output directories for build targets
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/bin)

# Include directories
#include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)


# Add a target to copy application resource files to the build dir
add_custom_target(copy_assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${OUTPUT_DIR}/share/data
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_BINARY_DIR}/tests/data
)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(apps)

if(ENABLE_TESTS)
	add_subdirectory(tests)
endif()

# vim: ts=2 sw=2 noet foldmethod=indent :
