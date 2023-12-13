
set(game_SOURCES
	main.cpp
	ElementalGame.cpp
)

add_executable(game
	${game_SOURCES}
)

#target_include_directories(app PRIVATE
#	${CMAKE_SOURCE_DIR}/src/
#)

target_link_libraries(game PRIVATE
	elemental
)

# Manually adds a predefined target as a dependency
add_dependencies(game copy_assets)

# vim: ts=2 sw=2 noet foldmethod=indent :
