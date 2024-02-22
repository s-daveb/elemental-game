# Define the library 'engine'
set(ELEMENTAL_SOURCES
	debuginfo.cpp
	Exception.cpp
	Application.cpp
	SdlRenderer.cpp
	LoopRegulator.cpp
	Observable.cpp
	SdlEventSource.cpp
	FileResource.cpp
	JsonConfigFile.cpp
	paths.cpp
)

add_library(elemental OBJECT ${ELEMENTAL_SOURCES})

target_include_directories(elemental SYSTEM BEFORE
	PUBLIC ${SDL2_COMBINED_INCLUDE_DIRS}
	PUBLIC ${NLOHMANN_JSON_INCLUDE_DIRS}
)

# Add a target to copy this project's header files to the build dir
add_custom_target(include_files
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/include ${OUTPUT_DIR}/include
)
add_dependencies(elemental include_files)

set(ELEMENTAL_LINK_DEPS
	Threads::Threads
	${SDL2_COMBINED_LINK_DEPS}
	${STACKTRACE_DEP_LIBS}
)

# Add additional link options for boost::stacktrace
if (USE_BOOST_STACKTRACE)
	target_link_options(elemental PUBLIC -rdynamic)
endif()

# Link the library dependencies publicly so that that users of this library
# Also get the same dependencies.
target_link_libraries(elemental PUBLIC ${ELEMENTAL_LINK_DEPS})

#if (ENABLE_TESTS)
#	if (USE_BOOST_STACKTRACE)
#		target_link_options(elemental-testing PUBLIC -rdynamic)
#	endif()
#	target_link_libraries(elemental-testing PUBLIC ${ELEMENTAL_LINK_DEPS})
#
#	# Define a preprocessor flag to enable helper code for unit-testing
#	# This flag will propagate to programs that link to this library (PUBLIC)
#	#target_compile_definitions(elemental-testing PUBLIC -DUNIT_TEST=1)
#
#endif()

# vim: ts=4 sw=4 noet foldmethod=indent :
