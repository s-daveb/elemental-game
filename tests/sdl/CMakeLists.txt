# Define the executable 'test-runner'
set(SDL_test-runner_SOURCES
	SdlMain.cpp
	SdlRenderer.test.cpp
)

add_executable(SDL_test-runner
	${SDL_test-runner_SOURCES}
)

set_target_properties(SDL_test-runner PROPERTIES EXCLUDE_FROM_ALL 1)

target_compile_definitions(SDL_test-runner PRIVATE -DUNIT_TEST=1)

target_include_directories(SDL_test-runner PRIVATE
	Catch2::Catch2WithMain
	FakeIt::FakeIt-catch
	..
)

target_link_directories(SDL_test-runner PRIVATE ${CATCH2_LIBRARY_DIRS})

target_link_libraries(SDL_test-runner
PRIVATE
	${TESTS_LINK_DEPS}
)

# Set output directories for build targetsk
set_target_properties(SDL_test-runner PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/tests"
)

add_dependencies(ctest
	SDL_test-runner
)
# vim: ts=2 sw=2 noet foldmethod=indent foldlevel=99 :
