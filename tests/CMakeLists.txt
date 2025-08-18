enable_testing()

# Collect all test source files
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/**/test_*.cpp)

foreach(test_src ${TEST_SOURCES})
    # Get filename without extension
    get_filename_component(test_name ${test_src} NAME_WE)

    # Create test executable
    add_executable(${test_name} ${test_src})

    target_link_libraries(${test_name} 
    PRIVATE 
        Qt6::Test
        mixer_lib
    )

    target_include_directories(${test_name}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/Core
        ${CMAKE_SOURCE_DIR}/Renderer
        ${CMAKE_SOURCE_DIR}/UI
        ${CMAKE_SOURCE_DIR}/Application
)

    # Register with CTest
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
