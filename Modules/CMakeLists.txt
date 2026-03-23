
set(MODULES_EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External)

add_subdirectory(${MODULES_EXTERNAL_DIR}/IOCore EXCLUDE_FROM_ALL)
add_subdirectory(${MODULES_EXTERNAL_DIR}/QJsonModel)

# Add ${MODULES_EXTERNAL_DIR} modules to the C++ include search path
include_directories(BEFORE SYSTEM${MODULES_EXTERNAL_DIR})

add_subdirectory(elemental)
add_subdirectory(EditorWidgets)

