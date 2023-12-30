# Define the library 'engine'
add_library(elementalobj OBJECT
	debuginfo.cpp
	Exception.cpp
	Application.cpp
	SdlRenderer.cpp
	LoopRegulator.cpp
	Observable.cpp
	SdlEventEmitter.cpp
)

set_property(TARGET elementalobj PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(elementalobj SYSTEM BEFORE
	PUBLIC ${SDL2_COMBINED_INCLUDE_DIRS})


target_include_directories(elementalobj SYSTEM BEFORE
	PUBLIC NLOHMANN_JSON_INCLUDE_DIRS
)

# Add a target to copy this project's header files to the build dir
add_custom_target(include_files
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/include ${OUTPUT_DIR}/include
)
add_dependencies(elementalobj include_files)

set(ELEMENTAL_LINK_DEPS
	${SDL2_COMBINED_LINK_DEPS}
	${STACKTRACE_DEP_LIBS}
	Threads::Threads
	# other libraries here
)

# build three different variants of this library
add_library(elemental SHARED $<TARGET_OBJECTS:elementalobj>)
add_library(elemental-static STATIC $<TARGET_OBJECTS:elementalobj>)
if (ENABLE_TESTS)
	add_library(elemental-testing STATIC $<TARGET_OBJECTS:elementalobj>)
endif()


set_target_properties(elemental-static PROPERTIES
    CMAKE_POSITION_INDEPENDENT_CODE OFF
)

# Check if the system is Linux
if (USE_BOOST_STACKTRACE)
    # Add the -rdynamic linker option
    target_link_options(elemental PUBLIC -rdynamic)
    target_link_options(elemental-static PUBLIC -rdynamic)
endif()

# Link the library dependencies publicly so that that users of this library
# Also get the same dependencies.
target_link_libraries(elemental PUBLIC ${ELEMENTAL_LINK_DEPS} )
target_link_libraries(elemental-static PUBLIC ${ELEMENTAL_LINK_DEPS} )

if (ENABLE_TESTS)
	set_target_properties(elemental-testing PROPERTIES
    	CMAKE_POSITION_INDEPENDENT_CODE OFF
	)
	target_link_options(elemental-testing PUBLIC -rdynamic)
	target_link_libraries(elemental-testing PUBLIC ${ELEMENTAL_LINK_DEPS})

	# Define a preprocessor flag to enable helper code for unit-testing
	# This flag will propagate to programs that link to this library (PUBLIC)
	target_compile_definitions(elemental-testing PUBLIC -DUNIT_TEST=1)

endif()

# vim: ts=4 sw=4 noet foldmethod=indent :
