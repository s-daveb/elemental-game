# Define the executable 'test-runner'
add_executable(test-runner
	runtime.test.cpp
	Singleton.template.test.cpp
	IRenderer.test.cpp
	LoopRegulator.test.cpp
	sdl/SdlRenderer.test.cpp
	Observable.test.cpp
	sdl/SdlEventSource.test.cpp
	SDL_Memory.test.cpp
	JsonConfigFile.test.cpp
	paths.test.cpp

)

set_target_properties(test-runner
PROPERTIES
	EXCLUDE_FROM_ALL 1
)

target_compile_definitions(test-runner PRIVATE
	-DUNIT_TEST=1
	-DNO_GUI=1
)

target_include_directories(test-runner
	#PRIVATE ${Elemental_ARTIFACT_DIR}/include/elemental
	PRIVATE ${Elemental_CMAKE_SOURCE_DIR}/Tests
)

#set(Elemental_TEST_SUITE_LIBS
#IOCore
#${SDL2_COMBINED_LINK_DEPS}
#${STACKTRACE_DEP_LIBS}
#)
target_link_libraries(test-runner
PRIVATE
	elemental
	IOCore
	Catch2::Catch2WithMain
	FakeIt::FakeIt-catch
	Threads::Threads
)

# Set output directories for build targets
set_target_properties(test-runner PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Tests"
)

add_custom_target(test_bin)
add_dependencies(test_bin
	test-runner
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
	COMMAND ctest -c $configuration --test-dir ${CMAKE_BINARY_DIR}/Tests --allow-running-no-tests --output-on-failure
)
add_custom_target(copy_test_assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_BINARY_DIR}/tests/data
)
set_target_properties(ctest PROPERTIES EXCLUDE_FROM_ALL 1)
add_dependencies(ctest
	test_bin
	copy_test_assets
)

#add_subdirectory(sdl)

# vim: ts=2 sw=2 noet foldmethod=indent  :
