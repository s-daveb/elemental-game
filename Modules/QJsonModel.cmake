add_library(QJsonModel OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/QJsonModel/qjsonmodel.cpp)
target_include_directories(QJsonModel SYSTEM BEFORE
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/QJsonModel
)
target_link_libraries(QJsonModel
PRIVATE
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
)
