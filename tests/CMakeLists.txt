# Define the executable 'test-runner'
set(test-runner_SOURCES
	runtime.test.cpp
  any_ptr.test.cpp
	Exception.test.cpp
	Singleton.template.test.cpp
	Application.test.cpp
	IRenderer.test.cpp
	LoopRegulator.test.cpp
	sdl/SdlRenderer.test.cpp
	Observable.test.cpp
	sdl/SdlEventSource.test.cpp
	SDL_Memory.test.cpp
)

add_executable(test-runner
	${test-runner_SOURCES}
)

set_target_properties(test-runner PROPERTIES EXCLUDE_FROM_ALL 0)

target_compile_definitions(test-runner PRIVATE
	-DUNIT_TEST=1
	-DNO_GUI=1
)

set(TESTS_LINK_DEPS
	${SDL2_LINK_LIBRARIES}
	Catch2::Catch2WithMain
	FakeIt::FakeIt-catch
	elemental-testing
)

target_include_directories(test-runner SYSTEM BEFORE
	PRIVATE ${Catch2_INCLUDE_DIRS}
	PRIVATE ${FakeIt_INCLUDE_DIRS}
)

target_include_directories(test-runner
	PRIVATE	${CMAKE_SOURCE_DIR}/include
	PRIVATE ${CMAKE_SOURCE_DIR}/tests
)

# Assume your target is named "your_target_name"
#get_target_property(target_include_dirs test-runner INCLUDE_DIRECTORIES)
# Print the include directories
#message("Include Directories for target test-runner: ${target_include_dirs}")

target_link_directories(test-runner PRIVATE ${CATCH2_LIBRARY_DIRS})

target_link_libraries(test-runner
PRIVATE
	${TESTS_LINK_DEPS}
)

# Set output directories for build targets
set_target_properties(test-runner PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/tests"
)
# extras: catch2 ctest integration.
# add catch2's ctest cmake module and register the tests defined by mdml-tests
# using catch_discover_tests
include(CTest)
include(Catch)
catch_discover_tests(test-runner)

# bonus: add a custom ctest target to the build system, to run tests in the
#        correct directory
add_custom_target(ctest
	COMMAND ctest -c $configuration --test-dir . --allow-running-no-tests --output-on-failure
)
set_target_properties(ctest PROPERTIES EXCLUDE_FROM_ALL 1)
add_dependencies(ctest
	test-runner
	copy_assets
)

add_subdirectory(sdl)

# vim: ts=2 sw=2 noet foldmethod=indent foldlevel=99 :
