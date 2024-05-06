
qt_add_executable(ResourceEditor
	main.cpp
	MainWindow.ui
	MainWindow.cpp
	ExceptionDialog.hpp
	ExceptionDialog.ui
	JsonEditor.ui
	JsonEditor.cpp
)
set_target_properties(ResourceEditor PROPERTIES
	AUTOMOC ON
)

# Manually call moc on all header files
#file(GLOB_RECURSE ResourceEditor_HEADER_FILES
#	${CMAKE_CURRENT_SOURCE_DIR}/**.hpp)

qt_wrap_cpp(ResourceEditor_MOC_SOURCES	${ResourceEditor_HEADER_FILES}
	TARGET ResourceEditor
)
#target_sources(ResourceEditor PUBLIC ${ResourceEditor_MOC_SOURCES})

target_link_libraries(ResourceEditor
PRIVATE
	${NLOHMANN_JSON_LIBS}
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
	IOCore
	QJsonModel
	fmt::fmt-header-only
)

set_target_properties(ResourceEditor
PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)
