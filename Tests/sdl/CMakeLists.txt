# Define the executable 'test-runner'
add_executable(SDL_test-runner
	SdlMain.cpp
	SdlRenderer.test.cpp
	SdlEventSource.test.cpp
)


set_target_properties(SDL_test-runner
PROPERTIES
EXCLUDE_FROM_ALL 1)

target_include_directories(SDL_test-runner
	PRIVATE
	${Elemental_CMAKE_SOURCE_DIR}/Tests
)
#target_compile_definitions(SDL_test-runner PRIVATE -DUNIT_TEST=1)

target_link_libraries(SDL_test-runner
PRIVATE
	elemental
	IOCore
	Catch2::Catch2WithMain
	FakeIt::FakeIt-catch
	Threads::Threads
)

# Set output directories for build targetsk
set_target_properties(SDL_test-runner PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${Elemental_ARTIFACT_DIR}/tests"
)

add_dependencies(test_bin
	SDL_test-runner
)
# vim: ts=2 sw=2 noet foldmethod=indent  :
