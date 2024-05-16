
add_subdirectory(third_party/IOCore)
add_subdirectory(third_party/QJsonModel)

# Add third_party modules to the C++ include search path
include_directories(third_party)

add_subdirectory(elemental)
add_subdirectory(EditorWidgets)

