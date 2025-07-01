cmake_minimum_required(VERSION 3.10)

# Set the project name
project(imgui-softrender)

option(BUILD_TESTAPP "Build test application" OFF)

find_package(imgui REQUIRED)

set(SOFTRENDER_HEADERS
    imgui/imgui_softrender.h
    imgui/softrender/barycentric.h
    imgui/softrender/color.h
    imgui/softrender/defines.h
    imgui/softrender/softrender.h
    imgui/softrender/texture.h
    imgui/softrender/utils.h
)

# Preserve the softrender subdirectory structure during installation
foreach(header IN LISTS SOFTRENDER_HEADERS)
    get_filename_component(header_dir ${header} DIRECTORY)
    install(FILES ${header}
        DESTINATION include/${header_dir}
    )
endforeach()

add_library(imgui-softrender SHARED
    imgui/imgui_softrender.cpp
    ${SOFTRENDER_HEADERS}
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
)
install(EXPORT imgui-softrender-targets
    FILE imgui-softrender-targets.cmake
    NAMESPACE imgui::
    DESTINATION lib/cmake/imgui-softrender
)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui-softrender-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/imgui-softrender-config.cmake"
    INSTALL_DESTINATION "lib/cmake/imgui-softrender"
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
    DESTINATION "lib/cmake/imgui-softrender")


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