add_executable(phong
	main.cpp
	Phong.cpp
)

target_include_Directories(phong
PRIVATE
	${CMAKE_CURRENT_DIR}
)

target_link_libraries(phong PRIVATE
	elemental
	IOCore
	nlohmann_json
)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	set_target_properties(phong
	PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${Elemental_ARTIFACT_DIR}"
)
endif()

set_target_properties(phong
PROPERTIES
  MACOSX_BUNDLE True
  MACOSX_BUNDLE_GUI_IDENTIFIER "me.beniquez.sdavid.phong"
  MACOSX_BUNDLE_BUNDLE_NAME "${CUR_TARGET}"
  MACOSX_BUNDLE_BUNDLE_VERSION "0.1.0"
  MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
  MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/info.plist.in"
)

# Manually adds a predefined target as a dependency
#add_dependencies(phong copy_assets)

# vim: ts=2 sw=2 noet foldmethod=indent :
