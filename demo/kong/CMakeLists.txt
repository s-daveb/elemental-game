
set(kong_SOURCES
	main.cpp
	Kong.cpp
)

add_executable(kong
	${kong_SOURCES}
)

#target_include_directories(app PRIVATE
#	${CMAKE_SOURCE_DIR}/src/
#)

target_link_libraries(kong PRIVATE
	elemental
)

# Manually adds a predefined target as a dependency
#add_dependencies(kong copy_assets)

# vim: ts=2 sw=2 noet foldmethod=indent :
