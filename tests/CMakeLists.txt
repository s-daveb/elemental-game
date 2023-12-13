# Define the executable 'test-runner'
set(test-runner_SOURCES
	runtime-tests.cpp
)
add_executable(test-runner
	${test-runner_SOURCES}
)

target_compile_definitions(test-runner PRIVATE -DUNIT_TEST=1)

set(TESTS_LINK_DEPS
	Catch2::Catch2WithMain
	${SDL2_LINK_DEPS}
	elemental-testing
)

target_include_directories(test-runner PRIVATE
	${Catch2_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/src/tests
	${CMAKE_SOURCE_DIR}/src/tests/common
)

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
	COMMAND ctest -c $configuration --test-dir . --output-on-failure
)
add_dependencies(ctest
	test-runner
	copy_assets
)

# vim: ts=2 sw=2 noet foldmethod=indent :
