# Define the library 'engine'
set(ELEMENTAL_SOURCES
	JsonConfigFile.cpp
	LoopRegulator.cpp
	Observable.cpp
	SdlRenderer.cpp
	SdlEventSource.cpp
	paths.cpp
)

add_library(elemental OBJECT ${ELEMENTAL_SOURCES})

target_include_directories(elemental SYSTEM BEFORE
	PUBLIC ${SDL2_COMBINED_INCLUDE_DIRS}
	PUBLIC ${NLOHMANN_JSON_INCLUDE_DIRS}
)

target_include_directories(elemental BEFORE
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add a target to copy this project's header files to the build dir
add_custom_target(include_files
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/include ${Elemental_ARTIFACT_DIR}/include
)
add_dependencies(elemental include_files)

set(ELEMENTAL_LINK_DEPS
	IOCore::IOCoreStatic
	Threads::Threads
	${SDL2_COMBINED_LINK_DEPS}
	${STACKTRACE_DEP_LIBS}
	${NLOHMANN_JSON_LIBS}
)

# Add additional link options for boost::stacktrace
#if (USE_BOOST_STACKTRACE)
#	target_link_options(elemental PUBLIC -rdynamic)
#endif()

# Link the library dependencies publicly so that that users of this library
# Also get the same dependencies.
target_link_libraries(elemental PUBLIC ${ELEMENTAL_LINK_DEPS})

# vim: ts=4 sw=4 noet foldmethod=indent :
