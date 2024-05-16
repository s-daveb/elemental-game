add_library(elemental
OBJECT
	JsonConfigFile.cpp
	LoopRegulator.cpp
	Observable.cpp
	SdlRenderer.cpp
	SdlEventSource.cpp
	paths.cpp)

target_include_directories(elemental
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(elemental PUBLIC
	IOCore
	Threads::Threads
	${SDL2_COMBINED_LINK_DEPS}
	${STACKTRACE_DEP_LIBS}
	${NLOHMANN_JSON_LIBS}
)

#package_library_headers(elemental
#	GLOB ${Elemental_ARTIFACT_DIR}/*.hpp
#	${Elemental_ARTIFACT_DIR}/sys
#	${Elemental_ARTIFACT_DIR}/types
#	${Elemental_ARTIFACT_DIR}/details
#	${Elemental_ARTIFACT_DIR}/util
#)

target_include_directories(elemental
INTERFACE
	${CMAKE_BINARY_DIR}/include
)
#target_include_directories(elemental
#PUBLIC
#	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
#    $<INSTALL_INTERFACE:include>
#)
#install(TARGETS elemental
#	EXPORT elemental
#	ARCHIVE DESTINATION lib
#	LIBRARY DESTINATION lib
#	RUNTIME DESTINATION bin
#)
#install(DIRECTORY ${CMAKE_BINARY_DIR}/include DESTINATION include)
#
#install(EXPORT elemental 
#    NAMESPACE Elemental::
#    DESTINATION lib/cmake/elemental
#)

# vim: ts=4 sw=4 noet foldmethod=indent :
