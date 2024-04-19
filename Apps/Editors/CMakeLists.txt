
qt_add_executable(ResourceEditor
	main.cpp
	MainWindow.ui
	MainWindow.cpp
)

target_link_libraries(ResourceEditor
PRIVATE
	IOCore::IOCoreStatic
	Qt6::Core
	Qt6::Widgets
	Qt6::Gui
)

set_target_properties(ResourceEditor
PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)
