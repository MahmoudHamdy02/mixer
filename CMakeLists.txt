cmake_minimum_required(VERSION 3.19)
project(mixer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGLWidgets Test)

qt_standard_project_setup()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(tests)

# PMP configuration
set(PMP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(PMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(PMP_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(PMP_BUILD_VIS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/pmp-library)

set(UI_SOURCES
    UI/mainwindow.cpp
    UI/mainwindow.h
    UI/Widgets/glwidget.cpp
    UI/Widgets/glwidget.h
    UI/Widgets/lefttoolbar.cpp
    UI/Widgets/lefttoolbar.h
    UI/Widgets/toptoolbar.cpp
    UI/Widgets/toptoolbar.h
    UI/Widgets/rightsidebar.cpp
    UI/Widgets/rightsidebar.h
    UI/Widgets/menubar.cpp
    UI/Widgets/menubar.h
)

set(RENDERER_SOURCES
    Renderer/renderer.cpp
    Renderer/renderer.h
    Renderer/meshgl.cpp
    Renderer/meshgl.h
    Renderer/shader.cpp
    Renderer/shader.h
    Renderer/camera.cpp
    Renderer/camera.h
    Renderer/grid.cpp
    Renderer/grid.h
    Renderer/selectionrectangle.cpp
    Renderer/selectionrectangle.h
)

set(CORE_SOURCES
    Core/scenecontroller.cpp
    Core/scenecontroller.h
    Core/mesh.cpp
    Core/mesh.h
    Core/Geometry/primitives.cpp
    Core/Geometry/primitives.h
    Core/Geometry/intersection.cpp
    Core/Geometry/intersection.h
    Core/ray.h
    Core/plane.h
)

set(APPLICATION_SOURCES
    Application/selectionmanager.cpp
    Application/selectionmanager.h
    Application/historymanager.cpp
    Application/historymanager.h
    Application/Commands/command.h
    Application/Commands/deletemeshcommand.h
    Application/Commands/deletemeshescommand.h
    Application/toolmanager.cpp
    Application/toolmanager.h
    Application/Tools/tool.h
    Application/Tools/cameratool.cpp
    Application/Tools/cameratool.h
    Application/Tools/selecttool.cpp
    Application/Tools/selecttool.h
)

qt_add_resources(RESOURCES resources.qrc)

# Compile into standalone binary so tests can link to it
add_library(mixer_lib
    ${UI_SOURCES}
    ${RENDERER_SOURCES}
    ${CORE_SOURCES}
    ${APPLICATION_SOURCES}
)
target_link_libraries(mixer_lib
    PUBLIC
        Qt::Core
        Qt::Widgets
        Qt::OpenGLWidgets
        pmp
)
if(WIN32)
    target_link_libraries(mixer_lib PUBLIC opengl32)
endif()
target_include_directories(mixer_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
        ${CMAKE_CURRENT_SOURCE_DIR}/UI
        ${CMAKE_CURRENT_SOURCE_DIR}/Application
)

qt_add_executable(mixer
    WIN32 MACOSX_BUNDLE
    main.cpp
    ${RESOURCES}
)

target_link_libraries(mixer
    PUBLIC
        mixer_lib
)

# Enable all warnings on GCC/Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mixer PUBLIC -Wall -Wextra -Wpedantic)
endif()

include(GNUInstallDirs)

install(TARGETS mixer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET mixer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
