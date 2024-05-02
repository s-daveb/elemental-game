qt_add_library(QJsonModel
OBJECT
	QJsonModel.cpp
)

# Since headers are in the include/ directory, going to handle MOC invocation
# manually.
set_target_properties(QJsonModel PROPERTIES
	AUTOMOC OFF
)

target_include_directories(QJsonModel
BEFORE
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(QJsonModel
SYSTEM BEFORE
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Manually call moc on all header files
file(GLOB QJsonModel_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
qt_wrap_cpp(QJsonModel_MOC_SOURCES  ${QJsonModel_HEADER_FILES}
	TARGET QJsonModel
)

# Append the MOC files to the source list
target_sources(QJsonModel PRIVATE ${QJsonModel_MOC_SOURCES})

# Create an Object library from the generated moc sources
add_library(QJsonModel_moc_objects OBJECT ${QJsonModel_MOC_SOURCES})

target_link_libraries(QJsonModel
PUBLIC
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
)
