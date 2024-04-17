
set(phong_SOURCES
	main.cpp
	Phong.cpp
)

add_executable(phong
	${phong_SOURCES}
)

#target_include_directories(app PRIVATE
#	${CMAKE_SOURCE_DIR}/src/
#)

target_link_libraries(phong PRIVATE
	elemental
)

# Manually adds a predefined target as a dependency
#add_dependencies(phong copy_assets)

# vim: ts=2 sw=2 noet foldmethod=indent :
