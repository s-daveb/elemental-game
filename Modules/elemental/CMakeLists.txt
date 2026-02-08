ADD_LIBRARY(elemental
OBJECT
	LoopRegulator.cpp
	Observable.cpp
	SdlRenderer.cpp
	SdlEventSource.cpp
	Entity.cpp
	StateStack.cpp
	paths.cpp
	FontConfig.cpp
)

TARGET_INCLUDE_DIRECTORIES(elemental
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

TARGET_LINK_LIBRARIES(elemental PUBLIC
	IOCore
	Threads::Threads
	${SDL2_COMBINED_LINK_DEPS}
	${STACKTRACE_DEP_LIBS}

	Fontconfig::Fontconfig
)

package_library_headers(elemental
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

TARGET_INCLUDE_DIRECTORIES(elemental
INTERFACE
	${CMAKE_BINARY_DIR}/include
)

# vim: ts=4 sw=4 noet foldmethod=indent :
