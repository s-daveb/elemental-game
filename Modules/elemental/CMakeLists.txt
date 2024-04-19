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

#file(MAKE_DIRECTORY ${Elemental_ARTIFACT_DIR}/include/)
#add_custom_target(elemental_include_files
#	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/include ${Elemental_ARTIFACT_DIR}/include/elemental
#)
#add_dependencies(elemental elemental_include_files)

target_include_directories(elemental BEFORE
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(ELEMENTAL_LINK_DEPS
	IOCore::IOCoreStatic
	Threads::Threads
	${SDL2_COMBINED_LINK_DEPS}
	${STACKTRACE_DEP_LIBS}
	${NLOHMANN_JSON_LIBS}
)

# Link the library dependencies publicly so that that users of this library
# Also get the same dependencies.
target_link_libraries(elemental PUBLIC ${ELEMENTAL_LINK_DEPS})

# vim: ts=4 sw=4 noet foldmethod=indent :
