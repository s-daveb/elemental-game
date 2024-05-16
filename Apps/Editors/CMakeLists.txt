
qt_add_executable(ResourceEditor
	main.cpp
	MainWindow.ui
	MainWindow.cpp
)
set_target_properties(ResourceEditor
PROPERTIES
	AUTOMOC ON
	AUTOUIC ON
)
target_link_libraries(ResourceEditor
PRIVATE
	fmt::fmt-header-only
	${NLOHMANN_JSON_LIBS}
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
	IOCore
	EditorWidgets
	QJsonModel
)

set_target_properties(ResourceEditor
PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)
