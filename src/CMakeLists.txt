# Define the library 'engine'
add_library(elementalobj OBJECT
	library.cpp
)

# Add a target to copy this project's header files to the build dir
add_custom_target(include_files
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/include ${OUTPUT_DIR}/include
)
add_dependencies(elementalobj include_files)

set(ELEMENTAL_LINK_DEPS
	SDL2_LINK_TARGETS
	# other libraries here
)

# build three different variants of this library
add_library(elemental SHARED $<TARGET_OBJECTS:elementalobj>)
add_library(elemental-static STATIC $<TARGET_OBJECTS:elementalobj>)
add_library(elemental-testing $<TARGET_OBJECTS:elementalobj>)

# Define a preprocessor flag to enable helper code for unit-testing
# This flag will propagate to programs that link to this library (PUBLIC)
target_compile_definitions(elemental-testing PUBLIC -DUNIT_TEST=1)

# Link the library dependencies publicly so that that users of this library
# Also get the same dependencies.
target_link_libraries(elemental PUBLIC ${elemental_dep_libs} )
target_link_libraries(elemental-static PUBLIC ${elemental_dep_libs} )
target_link_libraries(elemental-testing PUBLIC ${elemental_dep_libs})

# vim: ts=2 sw=2 noet foldmethod=indent :
