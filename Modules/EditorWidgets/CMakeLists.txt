qt_add_library(EditorWidgets OBJECT
	ExceptionDialog.ui
	ExceptionDialog.hpp
	DocumentEditor.hpp
	DocumentEditor.cpp
	DocumentEditor.ui
	JsonEditor.ui
	JsonEditor.cpp
)

set(EditorWidgets_AUTOGEN_BUILD_DIR
	"${CMAKE_CURRENT_BINARY_DIR}/${NAME}"
)

set_target_properties(EditorWidgets
PROPERTIES
	AUTOMOC ON
	AUTOUIC ON
	LINKER_LANGUAGE CXX
	POSITION_INDEPENDENT_CODE ON
	AUTOGEN_BUILD_DIR "${EditorWidgets_AUTOGEN_BUILD_DIR}"
)

target_include_directories(EditorWidgets PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${EditorWidgets_AUTOGEN_BUILD_DIR}/include
)

## Append the MOC files to the source list
#target_sources(EditorWidgets PRIVATE ${EditorWidgets_MOC_SOURCES})
add_library(EditorWidgets::EditorWidgets ALIAS EditorWidgets)

target_link_libraries(EditorWidgets
PUBLIC
	IOCore
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
	QJsonModel
	fmt::fmt-header-only
)
target_include_directories(EditorWidgets INTERFACE
	${AUTOGEN_BUILD_DIR}/include
)

