# BuildProperties.cmake
# Copyright (c) 2024 Saul D Beniquez
# License: MIT
#
# This module defines a function prevent_in_source_build() that prevents in-source builds
# and sets a policy for CMake version 3.24.0 and above.

function(prevent_in_source_build)
	# Prevent in-source builds
	if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
		message(FATAL_ERROR "Source and build directories cannot be the same.")
	endif()
endfunction()


function(set_artifact_dir path)
    # Set local variable, not necessary to be parent scope since it's not used outside this function
    set(ARTIFACT_DIR "${path}")

    # Set project-specific artifact directory in parent scope
    set(${PROJECT_NAME}_ARTIFACT_DIR "${path}" PARENT_SCOPE)

    # Set output directories in parent scope using the provided path directly
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${path}/lib" PARENT_SCOPE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${path}/lib" PARENT_SCOPE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${path}/bin" PARENT_SCOPE)
endfunction()


function(disable_deprecated_features)
	# Use new timestamp behavior when extracting files archives
	if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
	endif()
endfunction()

function(disable_tests_if_subproject)
if (NOT DEFINED PROJECT_NAME)
	option(BUILD_TESTING "Build and run unit tests" ON)
else()
	option(BUILD_TESTING "Build and run unit tests" OFF)
endif()
endfunction()


function(git_setup_submodules)
	find_package(Git QUIET)
	if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    	option(GIT_SUBMODULE "Check submodules during build" ON)
    	if(GIT_SUBMODULE)
        	message(STATUS "Submodule update")
        	execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        	RESULT_VARIABLE GIT_SUBMOD_RESULT)
        	if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            	message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        	endif()
    	endif()
	endif()

	if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/repo/CMakeLists.txt")
    	message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
	endif()
endfunction()
# vim: ts=4 sts=4 sw=4 noet :
