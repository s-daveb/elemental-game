
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
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
	IOCore
	EditorWidgets
	QJsonModel
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	set_target_properties(ResourceEditor
	PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${Elemental_ARTIFACT_DIR}"
)
endif()

set_target_properties(ResourceEditor
PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)
