qt_add_library(QJsonModel
STATIC
	QJsonModel.cpp
)

target_include_directories(QJsonModel
SYSTEM BEFORE
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(QJsonModel PROPERTIES
	AUTOMOC OFF
)

# Assume these MOC sources are generated later in the CMakeLists.txt
file(GLOB QJsonModel_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
qt_wrap_cpp(QJsonModel_MOC_SOURCES  ${QJsonModel_HEADER_FILES}
	TARGET QJsonModel
)

target_sources(QJsonModel PRIVATE ${QJsonModel_MOC_SOURCES})

# Create an Object library from the generated moc sources
add_library(QJsonModel_moc_objects OBJECT ${QJsonModel_MOC_SOURCES})

target_link_libraries(QJsonModel
PUBLIC
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
)
