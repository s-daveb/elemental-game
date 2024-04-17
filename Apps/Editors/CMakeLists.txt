

add_executable(ResourceEditor
	main.cpp
	MainWindow.ui
)

target_link_libraries(ResourceEditor
	PRIVATE
	Qt6::Widgets
)
