cmake_minimum_required(VERSION 3.10)

# Set the project name
project(imgui-softrender)

option(BUILD_TESTAPP "Build test application" OFF)

find_package(imgui REQUIRED)

add_library(imgui-softrender SHARED
    imgui/imgui_softrender.cpp
    imgui/imgui_softrender.h
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(imgui-softrender
    PRIVATE
        cxx_std_17
)
target_compile_options(imgui-softrender
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)
target_link_libraries(imgui-softrender
    PUBLIC
        imgui::imgui
)
set_target_properties(imgui-softrender PROPERTIES
    VERSION 1.0
    SOVERSION 1
    PUBLIC_HEADER "imgui/imgui_softrender.h"
)
set_target_properties(imgui-softrender PROPERTIES
    OUTPUT_NAME "imgui-softrender"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
install(TARGETS imgui-softrender
    EXPORT imgui-softrender-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/imgui
)
install(EXPORT imgui-softrender-targets
    FILE imgui-softrender-targets.cmake
    NAMESPACE imgui::
    DESTINATION lib/cmake/imgui
)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui-softrender-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/imgui-softrender-config.cmake"
    INSTALL_DESTINATION "lib/cmake/imgui"
    PATH_VARS CMAKE_INSTALL_PREFIX
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/imgui-softrender-config-version.cmake"
    VERSION 1.0
    COMPATIBILITY AnyNewerVersion
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/imgui-softrender-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/imgui-softrender-config-version.cmake"
    DESTINATION "lib/cmake/imgui")


if (BUILD_TESTAPP)
    find_package(OpenCV REQUIRED)
    
    add_executable(testrender
        testrender.cpp
    )

    target_compile_features(testrender
        PRIVATE
            cxx_std_17
    )
    target_compile_options(testrender
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
    )
    target_link_libraries(testrender
        PRIVATE
            imgui-softrender
            ${OpenCV_LIBS}
            imgui::imgui
    )
    install(TARGETS testrender
        RUNTIME DESTINATION bin
    )

    # let's also run the test app if we are not cross-compiling
    if (NOT CMAKE_CROSSCOMPILING)
        add_custom_command(TARGET testrender
            POST_BUILD
            COMMAND testrender
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    endif()
endif()