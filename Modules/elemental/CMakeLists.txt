add_library(elemental
OBJECT
	JsonConfigFile.cpp
	LoopRegulator.cpp
	Observable.cpp
	SdlRenderer.cpp
	SdlEventSource.cpp
	paths.cpp)

target_include_directories(elemental
BEFORE
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(elemental PUBLIC
	IOCore
	Threads::Threads
	${SDL2_COMBINED_LINK_DEPS}
	${STACKTRACE_DEP_LIBS}
	${NLOHMANN_JSON_LIBS}
)

# vim: ts=4 sw=4 noet foldmethod=indent :
