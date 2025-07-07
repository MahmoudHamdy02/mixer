cmake_minimum_required(VERSION 3.19)
project(mixer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGLWidgets)

qt_standard_project_setup()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
)

qt_add_executable(mixer
    WIN32 MACOSX_BUNDLE
    main.cpp
    ${UI_SOURCES}
)

target_link_libraries(mixer
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::OpenGLWidgets
        pmp
)

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
