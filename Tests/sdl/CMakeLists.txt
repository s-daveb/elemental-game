# Define the executable 'test-runner'
set(SDL_test-runner_SOURCES
	SdlMain.cpp
	SdlRenderer.test.cpp
	SdlEventSource.test.cpp
$<TARGET_OBJECTS:elemental>
)

add_executable(SDL_test-runner
	${SDL_test-runner_SOURCES}
)

target_include_directories(SDL_test-runner 
SYSTEM BEFORE
	PUBLIC ${SDL2_COMBINED_INCLUDE_DIRS}
	PUBLIC ${NLOHMANN_JSON_INCLUDE_DIRS}
)

set_target_properties(SDL_test-runner PROPERTIES EXCLUDE_FROM_ALL 0)

#target_compile_definitions(SDL_test-runner PRIVATE -DUNIT_TEST=1)

target_include_directories(SDL_test-runner PUBLIC
	Catch2::Catch2WithMain
	FakeIt::FakeIt-catch
	${TEST_LINK_DEPS}
	..
)

target_link_directories(SDL_test-runner PRIVATE ${CATCH2_LIBRARY_DIRS})

target_link_libraries(SDL_test-runner
PRIVATE
	${TESTS_LINK_DEPS}
)

# Set output directories for build targetsk
set_target_properties(SDL_test-runner PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${Elemental_ARTIFACT_DIR}/tests"
)

add_dependencies(test_bin
	SDL_test-runner
)
# vim: ts=2 sw=2 noet foldmethod=indent  :
