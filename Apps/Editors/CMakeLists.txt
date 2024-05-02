
qt_add_executable(ResourceEditor
	main.cpp
	MainWindow.ui
	MainWindow.cpp
	EntityEditor.ui
	EntityEditor.cpp
)
target_link_libraries(ResourceEditor
PRIVATE
	${NLOHMANN_JSON_LIBS}
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
	IOCore::IOCoreStatic
	QJsonModel
)

set_target_properties(ResourceEditor
PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)
