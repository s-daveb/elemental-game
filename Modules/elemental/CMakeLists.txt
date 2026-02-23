add_library(elemental
    OBJECT
    LoopRegulator.cpp
    Observable.cpp
    SdlRenderer.cpp
    SdlEventSource.cpp
    Entity.cpp
    StateStack.cpp
    paths.cpp
    FontConfig.cpp
)

target_include_directories(elemental
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(elemental
    PUBLIC
    IOCore
    Threads::Threads
    ${SDL2_COMBINED_LINK_DEPS}
    ${STACKTRACE_DEP_LIBS}

    Fontconfig::Fontconfig
)

package_library_headers(elemental
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(elemental
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# vim: ts=4 sw=4 sts=4 et foldmethod=indent :
