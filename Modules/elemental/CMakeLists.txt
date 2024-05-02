add_library(elemental
OBJECT
	JsonConfigFile.cpp
	LoopRegulator.cpp
	Observable.cpp
	SdlRenderer.cpp
	SdlEventSource.cpp
	paths.cpp)

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
