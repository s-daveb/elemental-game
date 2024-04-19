
qt_add_executable(ResourceEditor
	main.cpp
	MainWindow.ui
)

set_target_properties(ResourceEditor
PROPERTIES
	AUTOUIC OFF
 )

target_link_libraries(ResourceEditor
PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::Gui
)

set_target_properties(ResourceEditor
PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)
