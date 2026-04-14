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
    GameScene.cpp
    ShapeDrawCommand.cpp
    ViewComponents.cpp
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

publish_library_headers(
    LIBRARY elemental
    HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    GLOB
)

# vim: ts=4 sw=4 sts=4 et foldmethod=indent :
